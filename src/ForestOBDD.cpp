
//
// ForestOBDD.cpp -- implementation of the class "ForestOBDD".
//
//    This file is part of the featsel program
//    Copyright (C) 2015  Marcelo S. Reis, Gustavo Estrela
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


#include "ForestOBDD.h"
#include <set>

ForestOBDD::ForestOBDD (ElementSet * set) : OBDD (set)
{
  forest_size = 0;
  return;
}


ForestOBDD::~ForestOBDD ()
{
  return;
}

// By doing this we avoid leaf sharing between subsets
Vertex * ForestOBDD::get_leaf (bool value)
{
  return NULL;
}


Vertex * ForestOBDD::get_subset_leaf (ElementSubset * subset)
{
  Vertex * v = root;
  while (!v->is_terminal ())
  {
    unsigned int elm_idx = v->get_index () - 1;
    if (subset->has_element (elm_idx))
      v = v->get_child (true);
    else
      v = v->get_child (false);
  }
  return v;
}


void ForestOBDD::reduce_from_vertex (Vertex * v)
{
  Vertex * u = v->get_parents ().front ();
  Vertex * lo = u->get_child (false);
  Vertex * hi = u->get_child (true);
    
  while (lo != NULL && lo->get_value () == 0 && hi->get_value () == 0)
  {
    Vertex * next_u;
    if (u->get_parents ().empty ())
    {
      root = lo;
      next_u = root;
    }
    else
    {
      next_u = u->get_parents ().front ();
      next_u->set_child (lo, u == next_u->get_child (true));
    }

    // cout << "next_u = " << next_u << endl;
    u->set_child (NULL, true);
    u->set_child (NULL, false);
    delete hi;
    delete u;
    cardinality -= 2;

    // cout << endl <<  "After reducing step: " << endl;
    // this->print ();

    u = next_u;
    lo = u->get_child (false);
    hi = u->get_child (true);    
    // cout << "lo = " << lo << "; hi = " << hi << "; u = " << u << endl;
  }
}


Vertex * ForestOBDD::change_subset_value (ElementSubset * subset, 
  bool new_value) 
{
  bool current_edge, last_edge;
  unsigned int set_card = elm_set->get_set_cardinality ();
  unsigned int idx = 0;
  Vertex * current = root;
  Vertex * last = NULL;
  Vertex * new_value_leaf, * old_value_leaf;

  if ((contains (subset) && new_value == true) ||
      (!contains (subset) && new_value == false))
    return NULL;

  new_value_leaf = new Vertex (new_value, set_card + 1);
  cardinality++;

  while (idx < set_card)
  {
    current_edge = subset->has_element (idx);
    if ((current->get_index () - 1) != idx)
    {
      if (last == NULL) 
      {
        Vertex * new_root;
        new_root = new Vertex (elm_set->get_element (idx), idx + 1);
        cardinality++;
        new_root->set_child (root, current_edge);
        new_root->set_child (copy_subtree (root), !current_edge);
        current = root = new_root;
      }
      else
      {
        last_edge = subset->has_element (idx - 1);
        current = new Vertex (elm_set->get_element (idx), idx + 1);
        insert_vertex (last, current, last_edge);
      }
    }

    // Actual change of values
    if (idx == set_card - 1) 
    {
      old_value_leaf = current->get_child (current_edge);
      current->set_child (new_value_leaf, current_edge);
      cardinality--;
      delete old_value_leaf;
    }

    last = current;
    current = current->get_child (current_edge);
    idx++;
  }
  return new_value_leaf;
}


void ForestOBDD::add_node (PFSNode * node) 
{
  ElementSubset * subset = node->vertex;
  Vertex * v = change_subset_value (subset, true);
  if (v == NULL)
    return;
  else
  {
    v->set_node (node);
    forest_size++;
  }
}


void ForestOBDD::remove_node (PFSNode * node) 
{
  ElementSubset * subset = node->vertex;
  Vertex * v = change_subset_value (subset, false);
  if (v == NULL)
    return;
  else
  {
    reduce_from_vertex (v);
    forest_size--;
  }
}


PFSNode * ForestOBDD::get_node (string s)
{
  ElementSubset * subset = new ElementSubset ("", elm_set);
  string::iterator it = s.begin ();
  unsigned int idx = 0;
  PFSNode * node;
  Vertex * v;
  while (it != s.end ())
  {
    if (*it == '1')
    {
      subset->add_element (idx);
      idx++;
    }
    else if (*it == '0')
    {
      idx++;
    }
    it++;
  }

  v = get_subset_leaf (subset);
  node = v->get_node ();
  delete subset;
  return node;
}


PFSNode * ForestOBDD::get_best_pruning_potential_node1 ()
{
  Vertex * v = root;
  while (!v->is_terminal ())
  {
    Vertex * next_v;
    if (v->get_child (true)->is_terminal () && 
        !v->get_child (true)->get_value ())
      next_v = v->get_child (false);
    else if (v->get_child (false)->is_terminal () &&
             v->get_child (!false)->get_value ())
      next_v = v->get_child (true);
    else
      next_v = v->get_child ((int) rand() % 2);
    v = next_v;
  }
  return v->get_node ();
}


unsigned int ForestOBDD::size ()
{
  return forest_size;
}
