//
// OBDDTree.h -- definition of the namespace "OBDDTree".
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

#ifndef OBDDTREE_H_
#define OBDDTREE_H_

#include "global.h"
#include "ElementSet.h"
#include "Vertex.h"
#include "ElementSubset.h"
#include "CostFunction.h"
#include "ROBDD.h"

class OBDDTree
{
 protected:

  // Stores the current node of the OBDD expansion
  //
  Vertex * current_node;

  // Stores the last returned subset on expansion
  //
  ElementSubset * current_subset;

  // Stores the OBDD being expanded
  //
  OBDD * obdd;

  // Stores the element set
  //
  ElementSet * elm_set;

  // Does the first step on expansion
  //
  void start_expansion ();

  // Completes an expansion by setting the current subset
  //
  void set_current_subset ();

  // Reduces an OBDD node that is redundant and has value 1, i.e. 
  // is in the OBDD
  //
  void reduce_node ();

  // Expands the OBDD to the next subset
  //
  void expand ();



 public:

  // Default constructor
  //
  OBDDTree (ElementSet *, OBDD *);


  // Default destructor
  //
  virtual ~OBDDTree ();


  // Expandes the OBDD and returns the next unrestricted
  // element subset
  //
  ElementSubset * next_subset ();


  // Restricts all nodes that are reachable from the current node on
  // the Tree
  // TODO: rewrite this
  //
  void restrict_branch ();


};

#endif /* OBDDTREE_H_ */