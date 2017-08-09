//
// VertexTest.cpp -- implementation of the namespace "VertexTest".
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

#include "VertexTest.h"

namespace VertexTest {

  bool a_new_vertex_should_have_no_child ()
  {
    Element * elm = new Element (1, "elm_name");
    Vertex * new_vertex = new Vertex (elm, 1);
    bool answ = (new_vertex->get_child (false) == NULL) & (new_vertex->get_child (true) == NULL);
    delete elm;
    delete new_vertex;
    return answ;
  }

  bool a_terminal_vertex_should_have_no_var () 
  {
    Vertex * new_vertex = new Vertex (true, 1);
    bool answ = new_vertex->get_var () == NULL;
    delete new_vertex;
    return answ;
  }

  bool a_nonterminal_vertex_should_have_no_value ()
  {
    Element * elm = new Element (1, "elm_name");
    Vertex * new_vertex = new Vertex (elm, 1);
    bool answ = new_vertex->get_value () == -1;
    delete new_vertex;
    delete  elm;
    return answ;
  }


  bool it_should_keep_parents ()
  {
    Vertex a ((Element *) NULL, 1);
    list<Vertex *> v_list;
    list<Vertex *> parents;
    unsigned int n = 10;
    
    for (unsigned int i = 0; i < n; i++)
    {
      Vertex * v = new Vertex ((Element *) NULL, 0);
      v_list.push_back (v);
      a.add_parent (v);
    }
    while (!v_list.empty ())
    {
      Vertex * v = v_list.back ();
      v_list.pop_back ();
      parents.remove (v);
      delete v;
    }

    return parents.empty ();
  }

} // end of namespace

