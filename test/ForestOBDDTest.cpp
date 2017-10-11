//
// ForestOBDDTest.cpp -- implementation of the namespace "ForestOBDDTest".
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

#include "ForestOBDDTest.h"
#include "../src/ElementSet.h"
#include "../src/ForestOBDD.h"
#include "../src/ElementSubset.h"
#include "../src/Vertex.h"

namespace ForestOBDDTest
{

  bool it_should_add_a_node ()
  {
    bool answ;
    ElementSet set ("", 4, 10);
    ForestOBDD obdd (&set);
    ElementSubset subset ("", &set);
    subset.add_element (1);
    subset.add_element (2);
    PFSNode node;
    node.vertex = &subset;

    obdd.add_node (&node);
    answ = obdd.get_node (subset.print_subset ()) == &node;
    return answ;
  }

}