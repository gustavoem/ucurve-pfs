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

ExpandableOBDD::ExpandableOBDD (ElementSet * elm_set) :
   ROBDD (elm_set) 
{
  current_node = root;
  current_subset = NULL;
}


ElementSubset * ExpandableOBDD::next_subset ()
{
  if (this->is_full ())
    return NULL;

  expand ();
  return current_subset;
}


void ExpandableOBDD::expand ()
{
  return;
}


ExpandableOBDD::reduce () 
{
  return;
}