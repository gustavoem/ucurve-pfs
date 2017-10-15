//
// ForestOBDD.h -- definition of the namespace "ForestOBDD".
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

#ifndef FOREST_OBDD_H_
#define FOREST_OBDD_H_

#include "global.h"
#include "ElementSet.h"
#include "ForestOBDDVertex.h"
#include "ElementSubset.h"
#include "CostFunction.h"
#include "OBDD.h"
#include "PFSNode.h"

class ForestOBDD : public OBDD
{

protected:

  // Updates a subset value. 
  // Returns the address of the new leaf that represents the value
  // of the subset that has been changed
  //
  Vertex * change_subset_value (ElementSubset *, bool);

  // Finds the vertex that represents the subset in the OBDD
  //
  Vertex * get_subset_leaf (ElementSubset *);

  // Reduces the tree. This is only done after removing a node
  // from the OBDD
  //
  void reduce_from_vertex (Vertex *);

  // Overrides super method by returning NULL always. This is done
  // to avoid leaf sharing between subsets.
  //
  Vertex * get_leaf (bool);

  // Stores the number of nodes added to the Forest.
  //
  unsigned int forest_size;

public:

  // Builds a ForestOBDD representing the function "0"
  //
  ForestOBDD (ElementSet *);

  // Creates a ForestOBDD represented by the tree with root root.
  //
  ForestOBDD (ElementSet * set, ForestOBDDVertex * root, int card);

  // Builds a ForestOBDD representing the function "0" using the ordering in 
  // unsigned int *
  //
  ForestOBDD (ElementSet *, unsigned int *);

  // Builds an ForestOBDD where the only path valued "1", is the path representing
  // the subset
  //
  ForestOBDD (ElementSet *, ElementSubset *);

  // Default destructor.
  //
  virtual ~ForestOBDD ();  

  // Adds to the ForestOBDD a subset. Does nothing if subset already in the
  // ForestOBDD
  //
  void add_node (PFSNode *);

  // Removes a subset from the ForestOBDD. Does nothing if subset not in the
  // ForestOBDD.
  //
  void remove_node (PFSNode *);

  // Returns the PFSNode of a subset
  //
  PFSNode * get_node (string);

  // Returns the number of nodes contained by the OBDD
  //
  unsigned int size ();
};

#endif /* FOREST_OBDD_H_ */
