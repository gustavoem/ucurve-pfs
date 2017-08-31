#include "Vertex.h"


Vertex::Vertex ()
{
  var = NULL;
  lo = NULL;
  hi = NULL;
}


Vertex::Vertex (const Vertex& v)
{
  var = v.var;
  lo = v.lo;
  hi = v.hi;
  value = v.value;
  index = v.index;
  id = v.id;
}

Vertex::Vertex (Element * elm, unsigned int index) 
{
  var = elm;
  lo = NULL;
  hi = NULL;
  this->index = index;
  this->id = index;
}


Vertex::Vertex (bool value, unsigned int index)
{
  var = NULL;
  lo = NULL;
  hi = NULL;
  this->value = value;
  this->index = index;
  this->id = index;
}


Vertex::~Vertex () 
{
  this->hi = NULL;
  this->lo = NULL;
  this->var = NULL;
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