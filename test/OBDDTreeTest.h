//
// OBDDTreeTest.h -- definition of the namespace "OBDDTreeTest".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Marcelo S. Reis, Gustavo Estrela
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

#ifndef EXPANDABLEOBDDTEST_H_
#define EXPANDABLEOBDDTEST_H_

#include "../src/OBDDTree.h"
#include "../src/global.h"

namespace OBDDTreeTest
{

  bool it_should_return_empty_subset_first ();

  bool it_should_return_subsets_in_a_particular_order ();
  
  bool it_should_restrict_a_branch ();
}

#endif /* EXPANDABLEOBDDTEST */
