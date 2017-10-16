// RUBB2.cpp automatically generated by bin/add_new_algorithm.pl
// in Mon Oct  9 12:19:49 2017.

//
// RUBB2.cpp -- implementation of the class "RUBB2".
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

#include "RUBB2.h"


RUBB2::RUBB2 ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
  obdd = NULL;
  obdd_tree = NULL;
}


RUBB2::~RUBB2 ()
{
  if (list_of_visited_subsets != NULL)
  delete list_of_visited_subsets;
  delete obdd;
  delete obdd_tree;
}


void RUBB2::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);
  list<ElementSubset *> Q;
  ElementSubset * X, * Y;

  obdd = new OBDD (set);
  obdd_tree = new OBDDTree (set, obdd, DOWN);

  X = obdd_tree->next_subset ();
  if (store_visited_subsets)
    list_of_visited_subsets->add_subset (X);
  X->cost = cost_function->cost (X);
  Q.push_back (X);

  while (!obdd->is_full ())
  {
    X = Q.back ();
    Y = obdd_tree->next_subset ();

    // cout << "\n\nRUBB Iteration: " << endl;
    // cout << "X = " << X->print_subset () << " Y = " << Y->print_subset () << endl;

    if (store_visited_subsets)
      list_of_visited_subsets->add_subset (Y);
    double costX, costY;

    // cout << "Queue before: " << endl;
    // list<ElementSubset *>::iterator it = Q.begin ();
    // while (it != Q.end ())
    // {
    //   cout << "  " << (*it)->print_subset () << endl;
    //   it++;
    // }


    while (Q.size () > 0 && !X->contains (Y))
    {
      list_of_minima.push_back (X);
      Q.pop_back ();
      X = Q.back ();
    }

    // cout << "Queue after: " << endl;
    // it = Q.begin ();
    // while (it != Q.end ())
    // {
    //   cout << "  " << (*it)->print_subset () << endl;
    //   it++;
    // }

    costX = X->cost;
    costY = cost_function->cost (Y);
    Y->cost = costY;

    // cout << "c (X) = " << costX << ", c (Y) = "  << costY << endl;

    if (costY > costX)
    {
      obdd_tree->restrict_branch ();
      delete Y;
    }
    else
      Q.push_back (Y);
  }

  while (Q.size () > 0)
  {
    list_of_minima.push_back (Q.back ());
    Q.pop_back ();
  }


  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();
  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}

