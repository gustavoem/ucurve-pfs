//
// VertexTest.h -- definition of the namespace "VertexTest".
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

#ifndef VERTEXTEST_H_
#define VERTEXTEST_H_

#include "../src/Vertex.h"

namespace VertexTest {

  bool a_new_vertex_should_have_no_child ();

  bool a_terminal_vertex_should_have_no_var ();

  bool a_nonterminal_vertex_should_have_no_value ();

  bool it_should_keep_parents ();

  bool an_added_child_should_have_parents_updated ();

  bool a_removed_child_should_have_parents_updated ();
}

#endif /* VERTEXTEST_H_ */
