//
// ExpandableOBDDTest.cpp -- implementation of the namespace "ExpandableOBDDTest".
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


#include "ExpandableOBDDTest.h"

namespace ExpandableOBDDTest 
{

  bool it_should_return_empty_subset_first ()
  {
    bool answ;
    ElementSet elm_set ("", 3, 100);
    ROBDD R (&elm_set);
    ExpandableOBDD eobdd (&elm_set, &R);
    ElementSubset X ("", &elm_set);
    ElementSubset * Y = eobdd.next_subset ();
    answ = X.is_equal (Y);
    delete Y;
    return answ;
  }


  bool it_should_return_subsets_in_a_particular_order ()
  {
    bool answ;
    ElementSet elm_set ("", 3, 100);
    ROBDD R (&elm_set);
    ExpandableOBDD eobdd (&elm_set, &R);
    ElementSubset X ("", &elm_set);

    ElementSubset * Y = eobdd.next_subset ();
    answ = X.is_equal (Y);
    delete Y;

    X.add_element (2);
    Y = eobdd.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.remove_element (2);

    X.add_element (1);
    Y = eobdd.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.add_element (2);
    Y = eobdd.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.remove_element (1);
    X.remove_element (2);

    X.add_element (0);
    Y = eobdd.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.add_element (2);
    Y = eobdd.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.remove_element (2);
    X.add_element (1);
    Y = eobdd.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.add_element (2);
    Y = eobdd.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    
    return answ;
  }

} // end of namespace

