// PPFS.h automatically generated by bin/add_new_algorithm.pl
// in Tue Oct 24 19:26:08 2017.

//
// PPFS.h -- definition of the class "PPFS".
//
//    This file is part of the featsel program
//    Copyright (C) 2016  Marcelo S. Reis
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

#ifndef PPFS_H_
#define PPFS_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"
#include "../PFSNode.h"
#include "../ForestOBDD.h"


class PPFS : public Solver
{

protected:

  // A threshold to keep the list of minima small
  //
  float bound;  

  // Branches on the lower forest
  //
  PFSNode * lower_forest_branch (map<string, PFSNode *> *,
   map<string, PFSNode *> *);

  // Branches on the upper forest
  // 
  PFSNode * upper_forest_branch (map<string, PFSNode *> *, 
    map<string, PFSNode *> *);

  // Given an ElementSubset that is going to be removed from the lower
  // forest, finds its child and, if the children is not supposed to be
  // pruned, adds it to the Forest
  //
  void search_upper_children (map<string, PFSNode *> *, 
    PFSNode *, ElementSubset *, ElementSubset *);

  // Given an ElementSubset that is going to be removed from the upper
  // forest, finds its child and, if the children is not supposed to be
  // pruned, adds it to the Forest
  //
  void search_lower_children (map<string, PFSNode *> *, 
    PFSNode *, ElementSubset *, ElementSubset *);

  // Given an ElementSubset X that is being removed from the upper
  // forest, updates the forest creating roots with elements that
  // contains X until we find a root that contains X or we reach S
  // ElementSubset.
  //
  void search_upper_root (map<string, PFSNode *> *, ElementSubset *);

  // Given an ElementSubset X that is being removed from the lower
  // forest, updates the forest creating roots with elements that are
  // contained by X until we find a root that is contained by X or we
  // reach the empty set ElementSubset.
  //
  void search_lower_root (map<string, PFSNode *> *, ElementSubset *);

  // Updates the upper forest after branching on the lower forest.
  //
  void upper_forest_pruning (map<string, PFSNode *> *, PFSNode *);

  // Updates the lower forest after branching on the upper forest
  //
  void lower_forest_pruning (map<string, PFSNode *> *, PFSNode *);

  // Calculates the cost of a PFSNode, trying to use the dual forest
  // to fetch the already calculated cost if possible.
  //
  void calculate_node_cost (PFSNode *, map<string, PFSNode *> *);


public:

  // Default constructor.
  //
  PPFS ();

  // Default destructor.
  //
  virtual ~PPFS ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* PPFS_H_ */