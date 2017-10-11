
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
    Vertex * next_u = u->get_parents ().front ();
    if (next_u != NULL)
      next_u->set_child (lo, u == next_u->get_child (true));
    else
    {
      root = lo;
      next_u = root;
    }
    u->set_child (NULL, true);
    u->set_child (NULL, false);
    delete hi;
    delete u;
    cardinality -= 2;

    u = next_u;
    lo = u->get_child (false);
    hi = u->get_child (true);    
  }
}


void ForestOBDD::add_node (PFSNode * node) 
{
  ElementSubset * subset = node->vertex;
  OBDD::change_subset_value (subset, true);
  Vertex * v = get_subset_leaf (subset);
  v->set_node (node);

  print ();
}


void ForestOBDD::remove_node (PFSNode * node) 
{
  ElementSubset * subset = node->vertex;
  OBDD::change_subset_value (subset, false);
  Vertex * v = get_subset_leaf (subset);
  reduce_from_vertex (v);
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