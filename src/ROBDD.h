//
// ROBDD.h -- definition of the namespace "ROBDD".
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

#ifndef ROBDD_H_
#define ROBDD_H_

#include "global.h"
#include "ElementSet.h"
#include "Vertex.h"
#include "ElementSubset.h"
#include "CostFunction.h"
#include "OBDD.h"

class ROBDD : public OBDD
{

protected:

  // Auxiliar function to ROBDD::reduce ()
  //
  void fill_vlist (Vertex *, list<Vertex *> **);

  // Updates a subset value
  //
  void change_subset_value (ElementSubset *, bool);

public:

  // Builds a ROBDD representing the function "0"
  //
  ROBDD (ElementSet *);

  // Creates a ROBDD represented by the tree with root root.
  //
  ROBDD (ElementSet * set, Vertex * root, int card);

  // Builds a ROBDD representing the function "0" using the ordering in 
  // unsigned int *
  //
  ROBDD (ElementSet *, unsigned int *);

  // Builds an ROBDD where the only path valued "1", is the path representing
  // the subset
  //
  ROBDD (ElementSet *, ElementSubset *);

  // Default destructor.
  //
  virtual ~ROBDD ();  

  // Reduces the OBDD
  //
  void reduce ();

  // Makes the union of this ROBDD and the one passed by argument
  //
  void union_to (Vertex *);

  // Adds to the ROBDD a subset. Does nothing if subset already in the
  // ROBDD
  //
  void add_subset (ElementSubset *);

  // Removes a subset from the ROBDD. Does nothing if subset not in the
  // ROBDD.
  //
  void remove_subset (ElementSubset *);

  // Inserts a vertex V as a child of a ROBDD vertex U. This method will
  // replace the current child W of U by V. V will have as child W and a 
  // copy of the subtree started in W. This method may make the ROBDD 
  // not reduced
  //
  void insert_vertex (Vertex *, Vertex *, bool);

  // Adds to the ROBDD all the subsets covered by (if bool false) or 
  // that covers (if bool true) ElementSubset *
  //  
  void add_interval (ElementSubset *, bool);

};

#endif /* ROBDD_H_ */
