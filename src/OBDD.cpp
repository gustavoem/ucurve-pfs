
//
// OBDD.cpp -- implementation of the class "OBDD".
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


#include "OBDD.h"
#include <set>

OBDD::OBDD (ElementSet * set)
{
  elm_set = set;
  unsigned int n = elm_set->get_set_cardinality ();
  root = new Vertex (false, n + 1);
  cardinality = 1;
  set_default_elm_order ();
  set_rev_elm_order ();
}


OBDD::OBDD (ElementSet * set, Vertex * root, int card)
{
  this->elm_set = set;
  this->root = root;
  this->cardinality = card;
  set_default_elm_order ();
  set_rev_elm_order ();
}


OBDD::OBDD (ElementSet * set, ElementSubset * subset)
{
  unsigned int set_card = set->get_set_cardinality ();
  elm_set = set;
  set_default_elm_order ();
  set_rev_elm_order ();
  Vertex * zero = new Vertex (false, set_card + 1);
  Vertex * one = new Vertex (true, set_card + 1);
  Element * root_elm = elm_set->get_element (0);
  root = new Vertex (root_elm, 1);
  cardinality = 3;
  build (root, 1, set_card, subset, zero, one);
}


void OBDD::set_default_elm_order ()
{
  unsigned int set_card = elm_set->get_set_cardinality ();
  elm_order = new unsigned int[set_card + 1];
  for (unsigned int i = 0; i < set_card; i++)
    elm_order[i] = set_card - i - 1;
  elm_order[set_card] = set_card;
}


void OBDD::set_rev_elm_order ()
{
  unsigned int set_card = elm_set->get_set_cardinality ();
  rev_elm_order = new unsigned int[set_card + 1];
  for (unsigned int i = 0; i < set_card; i++)
    rev_elm_order[elm_order[i]] = i;
  rev_elm_order[set_card] = set_card;
}


void OBDD::build (Vertex * v, unsigned int ord_idx, 
  unsigned int set_card, ElementSubset * subset, Vertex * zero, 
  Vertex * one)
{
  bool zeroside;
  zeroside = !subset->has_element (elm_order[ord_idx - 1]);
  v->set_child (zero, zeroside);

  if (ord_idx == set_card) 
  {
    v->set_child (one, !zeroside);
    return;
  }

  unsigned int child_index = elm_order[ord_idx] + 1;
  Vertex * next_vertice = 
    new Vertex (elm_set->get_element (child_index - 1), 
      ++ord_idx);
  v->set_child (next_vertice, !zeroside);
  cardinality++;
  build (next_vertice, ord_idx, set_card, subset, zero, one);   
}


void OBDD::unmark_all_vertice ()
{
  unmark_all_vertice (root);
}


void OBDD::unmark_all_vertice (Vertex * v)
{
  if (v == NULL)
    return;
  v->mark = false;
  unmark_all_vertice (v->get_child(true));
  unmark_all_vertice (v->get_child(false));
}


OBDD::~OBDD ()
{
  delete[] elm_order;
  delete[] rev_elm_order;
  delete_subtree (&root, &cardinality);
}


void OBDD::delete_subtree (Vertex ** v, unsigned int * n)
{
  Vertex ** vertice = get_all_vertex (*v, n);
  for (unsigned int i = 0; i < *n; i++)
  {
    delete vertice[i];
    vertice [i] = NULL;
  }
  *n = 0;
  *v = NULL;
  free (vertice);
}


Vertex ** OBDD::get_all_vertex (Vertex * root, unsigned int * n)
{
  Vertex ** v = (Vertex **) malloc (sizeof (Vertex *) * (*n) + 1);
  int * last_index = (int *) malloc (sizeof (int));
  *last_index = 0;
  unmark_all_vertice (root);
  fill_vertice (v, last_index, root);
  *n = *last_index;
  free (last_index);
  return v;
}


void OBDD::fill_vertice (Vertex ** vertice, int * last_index, Vertex * v)
{
  if (v == NULL || v->mark)
    return;

  vertice[*last_index] = v;
  v->mark = true;
  (*last_index)++;

  fill_vertice (vertice, last_index, v->get_child (true));
  fill_vertice (vertice, last_index, v->get_child (false));
}


Vertex * OBDD::get_root ()
{
  return root;
}


void OBDD::stack_root (Vertex * new_root)
{
  new_root->set_child (root, false);
  new_root->set_child (root, true);
  root = new_root;
  cardinality++;
}

void OBDD::print ()
{   
  cout << "  ";
  print (root);
}


void OBDD::print (Vertex * v) 
{
  if (v == NULL)
    return;

  Element * var = v->get_var ();
  if (var != NULL)
  {
    cout << var->get_element_name () << " & id: " << v->get_id () <<  " addres: " << v << " index: " << v->get_index () << endl;
    cout << "L ";
    print (v->get_child (false));
    cout << "R ";
    print (v->get_child (true));
    return;
  }
  cout << v->get_value () << "      & id: " << v->get_id () <<  " addres: " << v << " index: " << v->get_index () << " nof parents " << v->get_parents ().size () << endl;
  return;
}


bool OBDD::VerticeEntry::operator < (const VerticeEntry& y)
{  
  if (lo_id < y.lo_id)
    return true;
  else if (lo_id == y.lo_id && hi_id <= y.hi_id)
    return true;
  else
    return false;
}


void OBDD::union_to (Vertex * root2)
{
  unsigned int set_card = elm_set->get_set_cardinality ();
  map<pair<Vertex *, Vertex*>, Vertex *> pairs;
  unsigned int new_cardinality = 0;
  Vertex * one = new Vertex (true, set_card + 1);
  one->mark = false;
  Vertex * zero = new Vertex (false, set_card + 1);
  zero->mark = false;

  Vertex * new_root = union_step (root, root2, &pairs, &new_cardinality, \
    one, zero);

  if (one->mark)
    new_cardinality++;
  else
    delete one;
  if (zero->mark)
    new_cardinality++;
  else
    delete zero;

  delete_subtree (&root, &cardinality);
  cardinality = new_cardinality;
  root = new_root;
}


Vertex * OBDD::union_step (Vertex * v1, Vertex * v2, map<pair<Vertex *, Vertex*>,\
  Vertex *> * pairs, unsigned int * new_cardinality, \
  Vertex * one, Vertex * zero)
{
  pair<Vertex *, Vertex *> key (v1, v2);
  map<pair<Vertex *, Vertex *>, Vertex *>::iterator it = pairs->find (key);
  Vertex * u;
  if (it != pairs->end ())
  {
    u = it->second;
    return u;
  }

  // We want to map 0 and -1 to 0, and 1 to 1 => f(x) = (x * x + x)/2
  int value1 = (v1 != NULL) && (v1->get_value () * v1->get_value () + v1->get_value ());
  int value2 = (v2 != NULL) && (v2->get_value () * v2->get_value () + v2->get_value ());
  if ((value1) || (value2) || \
     ((value1 + value2 == 0) && (v1 != NULL && v1->is_terminal ()) \
     && (v2 != NULL && v2->is_terminal ())))
  {
    if (value1 + value2 == 0)
    {
      u = zero;
      u->mark = true;
    }
    else
    {
      u = one;
      u->mark = true;
    }
    pairs->insert(make_pair (key, u));
  }
  else
  {
    u = new Vertex ();
    (*new_cardinality)++;
    u->mark = false;
    pairs->insert (make_pair (key, u));

    Vertex * vlow1 = NULL;
    Vertex * vhigh1 = NULL;
    Vertex * vlow2 = NULL;
    Vertex * vhigh2 = NULL;
    int v1_ord_index = rev_elm_order[v1->get_index () - 1];
    int v2_ord_index = rev_elm_order[v2->get_index () - 1];
    int index = min (v1_ord_index, v2_ord_index) + 1;
    u->set_id (elm_order[index - 1] + 1);
    u->set_index (elm_order[index - 1] + 1);
    u->set_var (elm_set->get_element (elm_order[index - 1]));
    if (u->get_index () == v1->get_index ())
    {
      vlow1 = v1->get_child (false);
      vhigh1 = v1->get_child (true);
    }
    else
    {
      vlow1 = v1;
      vhigh1 = v1;
    }
    if (u->get_index () == v2->get_index ())
    {
      vlow2 = v2->get_child (false);
      vhigh2 = v2->get_child (true);
    }   
    else
    {
      vlow2 = v2;
      vhigh2 = v2;
    }
    Vertex * lo_chi = union_step (vlow1, vlow2, pairs, new_cardinality, one, zero);
    Vertex * hi_chi = union_step (vhigh1, vhigh2, pairs, new_cardinality, one, zero);
    u->set_child (lo_chi, false);
    u->set_child (hi_chi, true);
  }
  return u;
}

// orientation = true for upper
void OBDD::add_interval (ElementSubset * subset, bool orientation)
{
  int set_card = elm_set->get_set_cardinality ();
  Vertex * zero = new Vertex (false, set_card + 1);
  zero->mark = false;
  Vertex * one = new Vertex (true, set_card + 1);
  one->mark = false;
  unsigned int card2 = 0;
  Vertex * root2 = build_interval (0, &card2, subset, zero, one, orientation);
  union_to (root2);

  if (!one->mark)
    delete one;
  if (!zero->mark)
    delete zero;
  delete_subtree (&root2, &card2);
}


void OBDD::add_subset (ElementSubset * subset) 
{
  change_subset_value (subset, true);
}


void OBDD::remove_subset (ElementSubset * subset) 
{
  change_subset_value (subset, false);
}


Vertex * OBDD::build_interval (unsigned int index, unsigned int * card, 
  ElementSubset * subset, Vertex * zero, Vertex * one, bool orientation)
{
  if (index == elm_set->get_set_cardinality ())
  {
    (*card)++;
    one->mark = true;
    return one;
  }

  if ((orientation == false && subset->has_element (elm_order[index])) 
    || (orientation == true && !subset->has_element (elm_order[index])))
    return build_interval (index + 1, card, subset, zero, one, orientation);

  Vertex * v = new Vertex (elm_set->get_element (elm_order[index]), elm_order[index] + 1);
  (*card)++;
  v->set_child (zero, !orientation);
  if (!zero->mark)
  {
    zero->mark = true;
    (*card)++;
  }
  v->set_child (build_interval (index + 1, card, subset, zero, one, orientation), orientation);
  return v;
}


Vertex * OBDD::copy_subtree (Vertex * r) 
{
  Vertex * root_cpy = new Vertex (*r);
  cardinality += 1;
  if (root_cpy->get_child (false) != NULL) 
  {
    Vertex * lo_cpy = copy_subtree (r->get_child (false));
    root_cpy->set_child (lo_cpy, false);
  }
  if (root_cpy->get_child (true) != NULL) 
  {
    Vertex * hi_cpy = copy_subtree (r->get_child (true));
    root_cpy->set_child (hi_cpy, true);
  }
  return root_cpy;
}


Vertex * OBDD::get_leaf (bool value) 
{
  Vertex * v = root;
  while (!v->is_terminal ())
  {
    Vertex * hi_child = v->get_child (true);
    Vertex * lo_child = v->get_child (false);
    if (hi_child->get_value () == value) 
      v = hi_child;
    else if (lo_child->get_value () == value) 
      v = lo_child;
    else if (hi_child->get_value () == !value)
      v = lo_child;
    else if (lo_child->get_value () == !value)
      v = hi_child;
    else
      v = v->get_child ((int) rand() % 2);
  }
  if (v->get_value () == value)
    return v;
  else
    return NULL;
}


void OBDD::change_subset_value (ElementSubset * subset, bool new_value) 
{
  bool current_edge, last_edge;
  unsigned int set_card = elm_set->get_set_cardinality ();
  unsigned int idx = 0;
  Vertex * current = root;
  Vertex * last = NULL;
  Vertex * new_value_leaf, * old_value_leaf;

  if ((contains (subset) && new_value == true) ||
      (!contains (subset) && new_value == false))
    return;

  new_value_leaf = get_leaf (new_value);
  if (new_value_leaf == NULL) 
  {
    new_value_leaf = new Vertex (new_value, set_card + 1);
    cardinality++;
  }

  // cout << "Changing value of subset " << subset->print_subset () << endl;
  // print ();

  while (idx < set_card)
  {
    current_edge = subset->has_element (elm_order[idx]);
    // cout << "Iteration -\nCurrent vertex = " << current << endl;

    if ((current->get_index () - 1) != elm_order[idx])
    {
      if (last == NULL) 
      {
        Vertex * new_root;
        new_root = new Vertex (elm_set->get_element (elm_order[idx]), 
          elm_order[idx] + 1);
        cardinality++;
        new_root->set_child (root, current_edge);
        new_root->set_child (copy_subtree (root), !current_edge);
        current = root = new_root;
      }
      else
      {
        last_edge = subset->has_element (elm_order[idx - 1]);
        current = new Vertex (elm_set->get_element (elm_order[idx]), 
          elm_order[idx] + 1);
        insert_vertex (last, current, last_edge);
      }
    }

    // Actual change of values
    if (idx == set_card - 1) 
    {
      old_value_leaf = current->get_child (current_edge);
      current->set_child (new_value_leaf, current_edge);
      if (old_value_leaf->get_parents ().size () < 1) 
      {
        cardinality--;
        delete old_value_leaf;
      }
    }

    // cout << "Resulted in: " << endl;
    // print ();
    // cout << "\n";
    last = current;
    current = current->get_child (current_edge);
    idx++;
  }
}


bool OBDD::contains (ElementSubset * subset)
{
  Vertex * v = root;
  while (!v->is_terminal ()) 
  {
    if (subset->has_element (v->get_index () - 1))
      v = v->get_child (true);
    else
      v = v->get_child (false);
  }
  return v->get_value ();
}


ElementSubset * OBDD::get_random_zero_evaluated_element ()
{
  // cout << "Getting a zero-element from obdd: " << endl;
  // print ();
  Vertex * v = root;
  if (v->is_terminal () && v->get_value ())
    return NULL;
  ElementSubset * subset = new ElementSubset ("", elm_set);


  while (!v->is_terminal ())
  {
    Vertex * next_v;
    if (v->get_child (true)->is_terminal () && v->get_child (true)->get_value ())
      next_v = v->get_child (false);
    else if (v->get_child (false)->is_terminal () && v->get_child (false)->get_value ())
      next_v = v->get_child (true);
    else
      next_v = v->get_child ((int) rand() % 2);

    if (next_v == v->get_child (true))
      subset->add_element (elm_order[v->get_index () - 1]);
    v = next_v;
  }
  return subset;
}


ElementSet * OBDD::get_element_set ()
{
  return elm_set;
}


unsigned int OBDD::get_cardinality ()
{
  return cardinality;
}


bool OBDD::is_full ()
{
  return root->is_terminal () && root->get_value () == 1;
}


void OBDD::insert_vertex (Vertex * u, Vertex * v, bool side)
{
  Vertex * w;
  cardinality++;
  w = u->get_child (side);
  u->set_child (v, side);
  v->set_child (w, false);
  v->set_child (copy_subtree (w), true);
}


void OBDD::replace_subtree (Vertex * u, Vertex * v)
{
  unsigned int n1, n2;
  Vertex * parent = u->get_parents ().front ();
  Vertex * lo = u->get_child (false);
  Vertex * hi = u->get_child (true);
  u->set_child (NULL, false);
  u->set_child (NULL, true);
  n1 = cardinality;
  n2 = cardinality;
  
  if (parent != NULL)
    parent->set_child (v, parent->get_child (true) == u);
  else
    root = v;

  if (lo == hi)
  {
    if (lo && !lo->has_parent () && lo != root)
    {
      delete_subtree (&lo, &n1);
    }
  }
  else
  {
    if (lo && !lo->has_parent () && lo != root)
    {
      delete_subtree (&lo, &n1);
    }
    if (hi && !hi->has_parent () && hi != root)
    {
      delete_subtree (&hi, &n2);
    }
  }

  if (u != root)
  {
    cardinality--;
    delete u;
  }
}


void OBDD::simplify (Vertex * v)
{
  Vertex * lo = v->get_child (false);
  Vertex * hi = v->get_child (true);
  if (lo->get_value () != hi->get_value () || !lo->is_terminal () ||
   !hi->is_terminal ())
    return;

  replace_subtree (v, v->get_child (true));
}


void OBDD::restrict_subtree (Vertex * v)
{
  unsigned int set_card = elm_set->get_set_cardinality ();
  Vertex * one = get_leaf (true);
  
  if (one == NULL) {
    one = new Vertex (true, set_card + 1);
    cardinality++;
  }

  replace_subtree (v, one);
}