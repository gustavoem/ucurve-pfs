// PosetForestSearchTest.cpp automatically generated by bin/add_new_algorithm.pl
// in Tue Jul 25 13:47:38 2017.

//
// PosetForestSearchTest.cpp -- implementation of the namespace "PosetForestSearchTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Author
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

#include "PosetForestSearchTest.h"

namespace PosetForestSearchTest
{

  bool it_should_store_all_the_visited_subsets ()
  {
    ElementSet set ("S1", 3, 1);    // |S1| = 3
    PosetForestSearch t;
    string list;
    SubsetSum c (&set);
    t.set_parameters (&c, &set, true);
    t.get_minima_list (1);
    list = t.print_list_of_visited_subsets ();
    //
    // For 2^3 it should have at least 7 elements (# visited nodes by SFS)!
    //
    if ((list.find ("<000>") != string::npos) &&
        (list.find ("<001>") != string::npos) &&
        (list.find ("<011>") != string::npos) &&
        (list.find ("<111>") != string::npos))
      return true;
    else
      return false;
  }


  bool it_should_give_the_number_of_the_visited_subsets ()
  {
    ElementSet set ("S1", 3, 1);    // |S1| = 3
    PosetForestSearch t;
    SubsetSum c (&set);
    t.set_parameters (&c, &set, true);
    t.get_minima_list (1);
    if ((t.print_list_of_visited_subsets ().size () /
        (set.get_set_cardinality() + 4)) >= 7)
      return true;
    else
      return false;
  }


} // end of namespace
