// UBB_PFS.h automatically generated by bin/add_new_algorithm.pl
// in Mon Nov 13 08:04:37 2017.

//
// UBB_PFS.h -- definition of the class "UBB_PFS".
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

#ifndef UBB_PFS_H_
#define UBB_PFS_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"
#include "../PartitionNode.h"
#include "../Partition.h"
#include "../Collection.h"



class UBB_PFS : public Solver
{

protected:

  // Solves with PFS sub-instances of the U-Curve problem. The list
  // contains roots of trees of the search space; for each root we
  // create an auxiliary U-Curve problem that finds a solution for 
  // that tree specifically
  //
  void parallel_solve (list<ElementSubset *> *, list<unsigned int> *);

  void solve_part (Solver *, PartitionNode *);

  void update_visited_subsets (Collection *, PartitionNode *);

  Partition * set_partition (unsigned int);

public:

  unsigned int max_size_of_minima_list;

  // Default constructor.
  //
  UBB_PFS ();

  // Default destructor.
  //
  virtual ~UBB_PFS ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* UBB_PFS_H_ */