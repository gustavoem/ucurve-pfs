// PFS_RAND.h automatically generated by bin/add_new_algorithm.pl
// in Mon Oct  9 15:15:14 2017.

//
// PFS_RAND.h -- definition of the class "PFS_RAND".
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

#ifndef PFS_RAND_H_
#define PFS_RAND_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"
#include "../PFSNode.h"


class PFS_RAND : public Solver
{

protected:

  // a threshold to keep the list of minima small
  float bound;  

  PFSNode * lower_forest_branch (map<string, PFSNode *> *, map<string, PFSNode *> *);

  PFSNode * upper_forest_branch (map<string, PFSNode *> *, map<string, PFSNode *> *);

  void search_upper_children
       (map<string, PFSNode *> *, PFSNode *, ElementSubset *, ElementSubset *);

  void search_lower_children
       (map<string, PFSNode *> *, PFSNode *, ElementSubset *, ElementSubset *);

  void search_upper_root (map<string, PFSNode *> *, ElementSubset *);

  void search_lower_root (map<string, PFSNode *> *, ElementSubset *);

  void upper_forest_pruning (map<string, PFSNode *> *, PFSNode *);

  void lower_forest_pruning (map<string, PFSNode *> *, PFSNode *);


public:

  // Default constructor.
  //
  PFS_RAND ();

  // Default destructor.
  //
  virtual ~PFS_RAND ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* PFS_RAND_H_ */
