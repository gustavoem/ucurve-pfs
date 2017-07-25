// PointTest.h automatically generated by bin/add_new_cost_function.pl
// in Tue Feb 14 11:15:23 2017.

//
// PointTest.h -- definition of the namespace "PointTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2015  Marcelo S. Reis
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

#ifndef POINTTEST_TEST_H_
#define POINTTEST_TEST_H_

#include "../../src/global.h"
#include "../../src/ElementSet.h"
#include "../../src/ElementSubset.h"

#include "../../src/functions/Point.h"

namespace PointTest
{
  bool an_empty_set_should_have_cost_zero ();

  bool it_should_add_and_retrieve_the_elapsed_time ();

  bool it_should_compute_the_cost_of_a_point ();
}


#endif /* POINTTEST_TEST_H_ */
