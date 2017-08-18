//
// ExpandableOBDD.cpp -- implementation of the class "ExpandableOBDD".
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

#include "ExpandableOBDD.h"

ExpandableOBDD::ExpandableOBDD (ElementSet * set, ROBDD * R)
{
  elm_set = set;
  obdd = R;
  current_node = R->get_root ();
  current_subset = NULL;
}


ExpandableOBDD::~ExpandableOBDD ()
{
  return;
}


ElementSubset * ExpandableOBDD::next_subset ()
{
  if (obdd->is_full ())
    return NULL;

  expand ();
  return current_subset;
}


void ExpandableOBDD::expand ()
{
  unsigned int set_card = elm_set->get_set_cardinality ();
  unsigned int elm_idx = current_node->get_index () - 1;
  while (elm_idx != set_card - 1) 
  {
    Vertex * lo = current_node->get_child (false);
    Vertex * hi = current_node->get_child (true);
    Vertex * next;
    if (lo->get_value () != 1)
    {
      next = lo;
      if (lo->get_index () != current_node->get_index () + 1)
      {
        lo = new Vertex (elm_set->get_element (elm_idx - 1), elm_idx);
        obdd->insert_vertex (current_node, v, false);
      }
      current_node = lo;
    }
    else if (hi->get_value () != 1)
    {
      next = hi;
      if (hi->get_index () != current_node->get_index () + 1)
      {
        hi = new Vertex (elm_set->get_element (elm_idx - 1), elm_idx);
        obdd->insert_vertex (current_node, v, true);
      }
      current_subset->add_element (current_node->get_index () - 1)
      current_node = hi;
    }
    else
    {
      next = current_node->get_parents ()[0];
      bool side = next->get_child (true) == current_node;
      robdd->simplify (current_node);
      current_node = next;
      if (side)
        current_subset->remove_element (current_node->get_index () - 1);
    }
    elm_idx = current_node->get_index () - 1;
  }
}


// void ExpandableOBDD::reduce () 
// {
//   return;
// }