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
    ForestOBDD fobdd (&set);
    ElementSubset subset ("", &set);
    subset.add_element (1);
    subset.add_element (2);
    PFSNode node;
    node.vertex = &subset;

    fobdd.add_node (&node);
    answ = fobdd.get_node (subset.print_subset ()) == &node;
    return answ;
  }


  bool it_should_reduce_the_tree ()
  {
    bool answ;
    ElementSet set ("", 4, 10);
    ForestOBDD fobdd (&set);
    unsigned int size_before, size_after;
    
    ElementSubset subset1 ("", &set);
    subset1.add_element (1);
    subset1.add_element (3);
    PFSNode node1;
    node1.vertex = &subset1;
    fobdd.add_node (&node1);

    ElementSubset subset2 ("", &set);
    subset2.add_element (1);
    PFSNode node2;
    node2.vertex = &subset2;
    fobdd.add_node (&node2);

    ElementSubset subset3 ("", &set);
    PFSNode node3;
    node3.vertex = &subset3;
    fobdd.add_node (&node3);

    size_before = fobdd.get_cardinality ();
    fobdd.remove_node (&node3);
    size_after = fobdd.get_cardinality ();
    answ = size_after < size_before;

    size_before = fobdd.get_cardinality ();
    fobdd.remove_node (&node1);
    size_after = fobdd.get_cardinality ();
    answ = answ && size_after == size_before;
    return answ;
  }


  bool it_should_store_the_number_of_nodes ()
  {
    bool answ;
    ElementSet set ("", 4, 10);
    ForestOBDD fobdd (&set);
    
    ElementSubset subset1 ("", &set);
    subset1.add_element (0);
    PFSNode node1;
    node1.vertex = &subset1;
    ElementSubset subset2 ("", &set);
    subset2.add_element (1);
    PFSNode node2;
    node2.vertex = &subset2;
    ElementSubset subset3 ("", &set);
    PFSNode node3;
    node3.vertex = &subset3;

    fobdd.add_node (&node1);
    fobdd.add_node (&node2);
    fobdd.add_node (&node3);
    answ = (fobdd.size () == 3); 

    fobdd.remove_node (&node3);
    answ = answ && (fobdd.size () == 2);
    return answ;
  }
}