//
// Vertex.cpp -- implementation of the class "Vertex".
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


#include "Vertex.h"

Vertex::Vertex ()
{
  var = NULL;
  lo = NULL;
  hi = NULL;
  node = NULL;
  weight = 0;
}


Vertex::Vertex (const Vertex& v)
{
  var = v.var;
  lo = v.lo;
  hi = v.hi;
  value = v.value;
  index = v.index;
  id = v.id;
  node = NULL;
  weight = v.weight;
}

Vertex::Vertex (Element * elm, unsigned int index) 
{
  var = elm;
  lo = NULL;
  hi = NULL;
  this->index = index;
  this->id = index;
  node = NULL;
  weight = 0;
}


Vertex::Vertex (bool value, unsigned int index)
{
  var = NULL;
  lo = NULL;
  hi = NULL;
  this->value = value;
  this->index = index;
  this->id = index;
  node = NULL;
  weight = 0;
}


Vertex::~Vertex () 
{
  this->hi = NULL;
  this->lo = NULL;
  this->var = NULL;
  node = NULL;
}

Element * Vertex::get_var ()
{
  return var;
}


void Vertex::set_var (Element * var)
{
  this->var = var;
}


Vertex * Vertex::get_child (bool side)
{
  if (side == true)
    return hi;
  return lo;
}


void Vertex::set_child (Vertex * vertex, bool side)
{
  Vertex * old_child;
  if (side == true)
  {
    old_child = hi;
    hi = vertex;
  }
  else 
  {
    old_child = lo;
    lo = vertex;
  }

  if (vertex != NULL)
    vertex->add_parent (this);

  if (old_child != NULL)
    old_child->remove_parent (this);
}


list<Vertex *> Vertex::get_parents () 
{
  return parents;
}


void Vertex::add_parent (Vertex * v)
{
  parents.push_back (v);
}


void Vertex::remove_parent (Vertex * v)
{
  list<Vertex *>::iterator it;
  for (it = parents.begin (); it != parents.end (); )
  {
    if (*it == v)
    {
      parents.erase (it);
      it = parents.end ();
    }
    else 
      ++it;
  }
}

int Vertex::get_value ()
{
  if (var != NULL)
    return -1;
  return value;
}


void Vertex::set_value (int value)
{
  this->value = value;
}


unsigned int Vertex::get_id ()
{
  return id;
}


void Vertex::set_id (unsigned int id)
{
  this->id = id;
}


unsigned int Vertex::get_index ()
{
  return index;
}


void Vertex::set_index (unsigned int index)
{
  this->index = index;
}


bool Vertex::is_terminal ()
{
  return var == NULL;
}


void Vertex::clean_parents () 
{
  parents.clear ();
}


bool Vertex::has_parent ()
{
  return parents.size () > 0;
}


PFSNode * Vertex::get_node ()
{
  return node;
}


void Vertex::set_node (PFSNode * node)
{
  this->node = node;
}


unsigned int Vertex::get_weight ()
{
  return this->weight;
}


void Vertex::set_weight (unsigned int w)
{
  this->weight = w;
}