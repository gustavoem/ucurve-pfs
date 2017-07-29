// PosetForestSearchTest.h automatically generated by bin/add_new_algorithm.pl
// in Tue Jul 25 13:47:38 2017.

//
// PosetForestSearchTest.h -- definition of the namespace "PosetForestSearchTest".
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

#ifndef POSETFORESTSEARCHTEST_H_
#define POSETFORESTSEARCHTEST_H_

#include "../../src/algorithms/PosetForestSearch.h"
#include "../../src/functions/SubsetSum.h"
#include "../../src/functions/HammingDistance.h"

namespace PosetForestSearchTest
{

  bool it_should_store_all_the_visited_subsets ();

  bool it_should_give_the_number_of_the_visited_subsets ();

}

#endif /* POSETFORESTSEARCHTEST_H_ */
