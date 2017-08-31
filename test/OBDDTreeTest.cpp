//
// OBDDTreeTest.cpp -- implementation of the namespace "OBDDTreeTest".
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


#include "OBDDTreeTest.h"

namespace OBDDTreeTest 
{

  bool it_should_return_empty_subset_first ()
  {
    bool answ;
    ElementSet elm_set ("", 3, 100);
    OBDD R (&elm_set);
    OBDDTree T (&elm_set, &R);
    ElementSubset X ("", &elm_set);
    ElementSubset * Y = T.next_subset ();
    answ = X.is_equal (Y);
    delete Y;
    return answ;
  }


  bool it_should_return_subsets_in_a_particular_order ()
  {
    bool answ;
    ElementSet elm_set ("", 3, 100);
    OBDD R (&elm_set);
    OBDDTree T (&elm_set, &R);
    ElementSubset X ("", &elm_set);

    ElementSubset * Y = T.next_subset ();
    answ = X.is_equal (Y);
    delete Y;

    X.add_element (2);
    Y = T.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.remove_element (2);

    X.add_element (1);
    Y = T.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.add_element (2);
    Y = T.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.remove_element (1);
    X.remove_element (2);

    X.add_element (0);
    Y = T.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.add_element (2);
    Y = T.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.remove_element (2);
    X.add_element (1);
    Y = T.next_subset ();
    answ = answ && X.is_equal (Y);
    delete Y;
    X.add_element (2);
    Y = T.next_subset ();
    // cout << "Y = " << Y->print_subset () << endl;
    answ = answ && X.is_equal (Y);
    delete Y;
    
    answ = answ && T.next_subset () == NULL;

    return answ;
  }


  bool it_should_restrict_a_branch ()
  {
    bool answ = true;
    ElementSet elm_set ("", 3, 100);
    OBDD R (&elm_set);
    OBDDTree T (&elm_set, &R);
    
    ElementSubset * X;

    // 000
    X = T.next_subset ();
    delete X;

    // 001
    X = T.next_subset ();
    delete X;

    // 010
    X = T.next_subset ();
    T.restrict_branch ();
    delete X;

    // 011 got restricted, next should be 100
    ElementSubset Y ("", &elm_set);
    Y.add_element (0);

    // 100
    X = T.next_subset ();
    answ = Y.is_equal (X);
    delete X;

    ElementSet elm_set2 ("", 4, 100);
    OBDD R2 (&elm_set2);
    OBDDTree T2 (&elm_set2, &R2);

    // Fast forward to 0100
    for (int i = 0; i < 4; i++)
    {
      X = T2.next_subset ();
      delete X;
    }
    X = T2.next_subset ();

    // Restricting 0100 should also
    // restrict 0101, 0110 and 0111
    // next subset should be 1000
    ElementSubset E1 (X);
    E1.add_element (3);
    ElementSubset E2 (X);
    E2.add_element (2);
    ElementSubset E3 (&E1);
    E3.add_element (2);
    T2.restrict_branch ();
    answ = answ && R2.contains (&E1);
    answ = answ && R2.contains (&E2);
    answ = answ && R2.contains (&E3);
    delete X;
    return answ;
  }

} // end of namespace

