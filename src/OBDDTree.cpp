//
// OBDDTree.cpp -- implementation of the class "OBDDTree".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Marcelo S. Reis, Gustavo Estrela
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "OBDDTree.h"

OBDDTree::OBDDTree (ElementSet * set, OBDD * R)
{
  elm_set = set;
  obdd = R;
  current_node = obdd->get_root ();
  current_subset = new ElementSubset ("", elm_set);
  building_subset = new ElementSubset ("", elm_set);
  orientation = UP;
}


OBDDTree::OBDDTree (ElementSet * set, OBDD * R, bool orientation)
{
  elm_set = set;
  obdd = R;
  current_node = obdd->get_root ();
  current_subset = new ElementSubset ("", elm_set);
  building_subset = new ElementSubset ("", elm_set);
  this->orientation = orientation;
}


OBDDTree::~OBDDTree ()
{
  delete current_subset;
  delete building_subset;
}


ElementSubset * OBDDTree::next_subset ()
{
  if (obdd->is_full ())
    return NULL;

  ElementSubset * answer_subset;
  expand ();
  answer_subset = new ElementSubset (current_subset);
  return answer_subset;
}


void OBDDTree::start_expansion ()
{
  Vertex * new_root = new Vertex (elm_set->get_element (0), 1);
  obdd->stack_root (new_root);
  current_node = obdd->get_root ();
}

void OBDDTree::update_current_subset ()
{
  delete current_subset;
  current_subset = new ElementSubset (building_subset);
}

void OBDDTree::set_current_subset ()
{
  unsigned int set_card = elm_set->get_set_cardinality ();
  Vertex * lo = current_node->get_child (false);
  Vertex * hi = current_node->get_child (true);
  
  // cout << "\n\n\nSetting current subset of ..." << endl;
  // obdd->print ();
  // cout << "curent node = " << current_node << endl;

  if (orientation == UP)
  {
    if (!lo->get_value () == 0)
      building_subset->add_element (set_card - 1);
  }
  else
  {
    if (hi->get_value () == 0)
      building_subset->add_element (set_card - 1);
    else
      building_subset->remove_element (set_card - 1);
  }

  update_current_subset ();
  obdd->add_subset (building_subset);

  // Makes OBDD reduction
  reduce_tree ();

  // cout << "After setting current subset..."<< endl;
  // obdd->print ();
}


void OBDDTree::restrict_node ()
{
  // cout << "Restricting Node: " << current_node << endl;
  // cout << "Current tree = " << endl;
  // obdd->print ();

  Vertex * next = current_node->get_parents ().front ();
  
  // todo: maybe this is not necessary
  building_subset->remove_element (current_node->get_index () - 1);
  obdd->restrict_subtree (current_node);
  if (next != NULL)
  {
    current_node = next;
    bool side = next->get_child (true) == current_node;
    if (side)
      building_subset->remove_element (current_node->get_index () - 1);
  }
  else 
  {
    current_node = obdd->get_root ();
  }
}


void OBDDTree::expand ()
{
  unsigned int set_card = elm_set->get_set_cardinality ();
  unsigned int elm_idx;
  Vertex * lo, * hi;
  

  if (current_node->is_terminal ())
  {
    if (current_node->get_value ())
      return;
    else
      start_expansion ();
  }

  lo = current_node->get_child (false);
  hi = current_node->get_child (true);
  elm_idx = current_node->get_index () - 1;

  // cout << "\nExpanding tree: " << endl;
  // obdd->print ();

  while (elm_idx != set_card - 1)
  {
    Vertex * next_node;
    bool next_side;

    // cout << "\n\nExpansion iteration "<< endl;
    // cout << "tree = " << endl;
    // obdd->print ();
    // cout << "    current node = " << current_node << endl;

    // using the reduction we removed the case where both lo and hi
    // evaluates to one
    if (orientation)
    {
      if (lo->get_value () != 1)
      {
        next_node = lo;
        next_side = false;
      }
      else
      {
        next_node = hi;
        next_side = true;
        building_subset->add_element (current_node->get_index () - 1);
      }
    }
    else
    {
      if (hi->get_value () != 1)
      {
        next_node = hi;
        next_side = true;
        building_subset->add_element (current_node->get_index () - 1);
      }
      else
      {
        next_node = lo;
        next_side = false;
        building_subset->remove_element (current_node->get_index () - 1);
      } 
    }
    if (next_node->get_index () != current_node->get_index () + 1)
    {
      next_node = new Vertex (elm_set->get_element (elm_idx + 1),
       elm_idx + 2);
      obdd->insert_vertex (current_node, next_node, next_side);
    }
    // cout << "Next node = " << next_node << endl;

    current_node = next_node;
    lo = current_node->get_child (false);
    hi = current_node->get_child (true);
    elm_idx = current_node->get_index () - 1;
  }

  // if (!current_node->is_terminal ())
  set_current_subset ();

  // cout << "\nResulting tree: " << endl;
  // obdd->print ();
}


void OBDDTree::restrict_branch ()
{
  unsigned int leftmost, elm_idx;
  unsigned int set_card = elm_set->get_set_cardinality ();

  // cout << "Restricting subset " << current_subset->print_subset () << endl;
  // obdd->print ();

  if (current_node->is_terminal ()) 
  {
    restrict_node ();
    return;
  }

  leftmost = 0;
  for (unsigned int i = 0; i < set_card; i++)
    if ((current_subset->has_element (i) && orientation == UP) ||
        (!current_subset->has_element (i) && orientation == DOWN))
      leftmost = i;

  elm_idx = current_node->get_index () - 1;
  while (elm_idx > leftmost)
  {
    restrict_node ();
    elm_idx = current_node->get_index () - 1;
  }
  
  if (is_redundant (current_node))
    restrict_node ();

  // cout << "Resulting in: " << endl;
  // obdd->print ();
}


bool OBDDTree::is_redundant (Vertex * v) 
{
  Vertex * lo = v->get_child (false);
  Vertex * hi = v->get_child (true);

  if (lo == NULL || hi == NULL)
    return false;

  if (lo->get_value () == hi->get_value () && lo->is_terminal ())
    return true;
  else
    return false;
}

void OBDDTree::reduce_tree ()
{
  Vertex * lo = current_node->get_child (false);
  Vertex * hi = current_node->get_child (true);

  while (hi != NULL && hi->get_value () && lo->get_value ())
  {
    restrict_node ();
    // cout << "Current node = " << current_node << endl;
    lo = current_node->get_child (false);
    hi = current_node->get_child (true);
  }
}