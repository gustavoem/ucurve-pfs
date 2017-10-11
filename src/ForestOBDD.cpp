
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


ForestOBDD::ForestOBDD (ElementSet * set, Vertex * root,
 int card) 
  : OBDD (set, root, card)
{
  return;
}


ForestOBDD::ForestOBDD (ElementSet * set, ElementSubset * subset) 
  : OBDD (set, subset)
{
  return;
}

ForestOBDD::~ForestOBDD ()
{
  return;
}


void ForestOBDD::fill_vlist (Vertex * v, 
  list<Vertex *> ** vlists)
{   
  if (v == NULL || v->mark)
    return;
  unsigned int i = v->get_index ();
  vlists[i]->push_back (v);   
  v->mark = true;

  fill_vlist (v->get_child (false), vlists);
  fill_vlist (v->get_child (true), vlists);
  return;
}


Vertex * ForestOBDD::get_subset_node (ElementSubset * X)
{
  Vertex * v = root;
  while (!v->is_terminal ())
  {
    unsigned int elm_idx = v->get_index () - 1;
    if (X->has_element (elm_idx))
      v = v->get_child (true);
    else
      v = v->get_child (false);
  }
  return v;
}


void ForestOBDD::reduce_from_subset ()
{

}


void ForestOBDD::add_node (ElementSubset * subset) 
{
  change_subset_value (subset, true);
}


void ForestOBDD::remove_node (ElementSubset * subset) 
{
  change_subset_value (subset, false);
}