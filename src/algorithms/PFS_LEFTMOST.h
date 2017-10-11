// PFS_LEFTMOST.h automatically generated by bin/add_new_algorithm.pl
// in Mon Oct  9 16:07:02 2017.

//
// PFS_LEFTMOST.h -- definition of the class "PFS_LEFTMOST".
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

#ifndef PFS_LEFTMOST_H_
#define PFS_LEFTMOST_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"
#include "../PFSNode.h"


class PFS_LEFTMOST : public Solver
{

protected:

  struct root_comp {
    bool operator () (string a, string b) 
    {
      string::iterator it_a = --a.end ();
      string::iterator it_b = --b.end ();
      while (it_a != a.begin ())
      {
        if (*it_a < *it_b) 
          return true;
        else if (*it_a > *it_b)
          return false;
        it_a--;
        it_b--;
      }
      return false;
    }
  };

  // a threshold to keep the list of minima small
  float bound;  

  PFSNode * lower_forest_branch (map<string, PFSNode *, root_comp> *, map<string, PFSNode *, root_comp> *);

  PFSNode * upper_forest_branch (map<string, PFSNode *, root_comp> *, map<string, PFSNode *, root_comp> *);

  void search_upper_children
       (map<string, PFSNode *, root_comp> *, PFSNode *, ElementSubset *, ElementSubset *);

  void search_lower_children
       (map<string, PFSNode *, root_comp> *, PFSNode *, ElementSubset *, ElementSubset *);

  void search_upper_root (map<string, PFSNode *, root_comp> *, ElementSubset *);

  void search_lower_root (map<string, PFSNode *, root_comp> *, ElementSubset *);

  void upper_forest_pruning (map<string, PFSNode *, root_comp> *, PFSNode *);

  void lower_forest_pruning (map<string, PFSNode *, root_comp> *, PFSNode *);


public:

  // Default constructor.
  //
  PFS_LEFTMOST ();

  // Default destructor.
  //
  virtual ~PFS_LEFTMOST ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* PFS_LEFTMOST_H_ */
