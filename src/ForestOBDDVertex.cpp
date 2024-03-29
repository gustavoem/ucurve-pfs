//
// ForestOBDDVertex.cpp -- implementation of the class "ForestOBDDVertex".
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

#include "ForestOBDDVertex.h"

ForestOBDDVertex::ForestOBDDVertex (PFSNode * node, unsigned int id) 
  : Vertex ()
{
  this->node = node;
  this->id = id;
  this->index = id;
}


ForestOBDDVertex::~ForestOBDDVertex () 
{
  return;
}


PFSNode * ForestOBDDVertex::get_node ()
{
  return node;
}


void ForestOBDDVertex::set_node (PFSNode * node)
{
  this->node = node;
}