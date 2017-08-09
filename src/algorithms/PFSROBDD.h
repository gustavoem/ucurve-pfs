// PFSROBDD.h automatically generated by bin/add_new_algorithm.pl
// in Tue Jul 25 13:25:58 2017.

//
// PFSROBDD.h -- definition of the class "PFSROBDD".
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

#ifndef PFSROBDD_H_
#define PFSROBDD_H_

#include "../global.h"
#include "../Solver.h"
#include "../ElementSubset.h"


class PFSROBDD : public Solver
{

protected:
	typedef struct MyNode
  {
    ElementSubset * vertex,
                  * adjacent;
    unsigned int leftmost;
    float cost;
  } Node;

  // a threshold to keep the list of minima small
  float bound;  

  Node * lower_forest_branch (map<string, Node *> *, map<string, Node *> *);

  Node * upper_forest_branch (map<string, Node *> *, map<string, Node *> *);

  void search_upper_children
       (map<string, Node *> *, Node *, ElementSubset *, ElementSubset *);

  void search_lower_children
       (map<string, Node *> *, Node *, ElementSubset *, ElementSubset *);

  void search_upper_root (map<string, Node *> *, ElementSubset *);

  void search_lower_root (map<string, Node *> *, ElementSubset *);

  void upper_forest_pruning (map<string, Node *> *, Node *);

  void lower_forest_pruning (map<string, Node *> *, Node *);

public:

  // Default constructor.
  //
  PFSROBDD ();

  // Default destructor.
  //
  virtual ~PFSROBDD ();

  // Run the algorithm, getting up to 'max_size_of_minima_list' minima.
  //
  void get_minima_list (unsigned int);

};

#endif /* PFSROBDD_H_ */
