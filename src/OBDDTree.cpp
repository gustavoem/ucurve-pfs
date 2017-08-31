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
}


OBDDTree::~OBDDTree ()
{
  delete current_subset;
  delete building_subset;
}


ElementSubset * OBDDTree::next_subset ()
{
  ElementSubset * answer_subset = NULL;
  expand ();
  if (current_subset != NULL)
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
  
  if (!lo->get_value () == 0)
    building_subset->add_element (set_card - 1);
  update_current_subset ();
  obdd->add_subset (building_subset);

  // Makes OBDD reduction if necessary
  while (hi != NULL && hi->get_value () == 1)
  {
    restrict_node ();
    lo = current_node->get_child (false);
    hi = current_node->get_child (true);
  }
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
    // when OBDD is full
    current_node = obdd->get_root ();
    delete current_subset;
    current_subset = NULL;
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

  cout << "\nExpanding tree: " << endl;
  obdd->print ();

  while (elm_idx != set_card - 1)
  {
    Vertex * next_node;
    bool next_side;

    cout << "\n\nExpansion iteration "<< endl;
    cout << "tree = " << endl;
    obdd->print ();
    cout << "    current node = " << current_node << endl;

    // unsignedsing the reduction we removed the case where both lo and hi
    // evaluates to one
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
    if (next_node->get_index () != current_node->get_index () + 1)
    {
      next_node = new Vertex (elm_set->get_element (elm_idx + 1),
       elm_idx + 2);
      obdd->insert_vertex (current_node, next_node, next_side);
    }
    cout << "Next node = " << next_node << endl;

    current_node = next_node;
    lo = current_node->get_child (false);
    hi = current_node->get_child (true);
    elm_idx = current_node->get_index () - 1;
  }

  // if (!current_node->is_terminal ())
  set_current_subset ();

  cout << "\nResulting tree: " << endl;
  obdd->print ();
}


void OBDDTree::restrict_branch ()
{
  unsigned int leftmost, elm_idx;
  unsigned int set_card = elm_set->get_set_cardinality ();

  // cout << "Restricting subset " << current_subset->print_subset () << endl;
  // obdd->print ();

  leftmost = 0;
  for (unsigned int i = 0; i < set_card; i++)
    if (current_subset->has_element (i))
      leftmost = i;

  elm_idx = current_node->get_index () - 1;
  while (elm_idx != leftmost)
  {
    Vertex * next_node = current_node->get_parents ().front ();
    restrict_node ();
    current_node = next_node;
    elm_idx = current_node->get_index () - 1;
  }

  // cout << "Resulting in: " << endl;
  // obdd->print ();
}