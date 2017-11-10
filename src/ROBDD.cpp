
//
// ROBDD.cpp -- implementation of the class "ROBDD".
//
//    This file is part of the featsel program
//    Copyright (C) 2015  Marcelo S. Reis, Gustavo Estrela
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


#include "ROBDD.h"
#include <set>

ROBDD::ROBDD (ElementSet * set) : OBDD (set)
{
  return;
}


ROBDD::ROBDD (ElementSet * set, Vertex * root, int card) 
  : OBDD (set, root, card)
{
  return;
}


ROBDD::ROBDD (ElementSet * set, ElementSubset * subset) 
  : OBDD (set, subset)
{
  return;
}

ROBDD::~ROBDD ()
{
  return;
}


void ROBDD::fill_vlist (Vertex * v, list<Vertex *> ** vlists)
{   
  if (v == NULL || v->mark)
    return;
  // unsigned int i = elm_order[v->get_index () - 1] + 1;
  unsigned int i = v->get_index ();
  vlists[i]->push_back (v);
  v->mark = true;

  fill_vlist (v->get_child (false), vlists);
  fill_vlist (v->get_child (true), vlists);
  return;
}


void ROBDD::reduce ()
{
  unsigned int set_card = elm_set->get_set_cardinality ();
  Vertex ** subgraph = (Vertex **) calloc (cardinality + 1, 
      sizeof (Vertex *));
  list<Vertex *> ** vlists = (list<Vertex *> **) calloc (set_card + 2, 
      sizeof (list<Vertex *> *));
  for (unsigned int i = 1; i <= set_card + 1; i++) 
    vlists[i] = new list<Vertex *> ();
  set<Vertex *> trash_can;
  set<Vertex *>::iterator trash_it = trash_can.begin ();
  unmark_all_vertice ();
  fill_vlist (root, vlists);

  // cout << "\nReducing tree: " << endl;
  // print ();

  int next_id = 0;
  for (int i = set_card + 1; i > 0; i--)
  {
    list<MyVerticeEntry> Q;
    list<Vertex *> * l = vlists[elm_order[i - 1] + 1];
    for (list<Vertex*>::iterator it = l->begin (); it != l->end (); 
        it++)
    {
      Vertex * u = *it;
      Vertex * u_lo = u->get_child (false);
      Vertex * u_hi = u->get_child (true);
      VerticeEntry ve;
      // cout << "1) Queue vertex candidate: " << u << endl;
      // cout << "\tu_lo = " << u_lo;
      // cout << " | u_hi = " << u_hi;
      // cout << " | next_id = " << next_id << endl;
      if (u->get_index () == set_card + 1) 
      {
        // Terminal vertex
        ve.lo_id = -1;
        ve.hi_id = u->get_value ();
        ve.v = u;
        Q.push_back (ve);
      }
      else if (u_hi->get_id () == u_lo->get_id ()) 
      {
        // Redundant vertex
        u->set_id (u_lo->get_id ());
        if (subgraph[u_hi->get_id ()] != u_hi) 
          trash_can.insert (trash_it, u_hi);
        if (subgraph[u_hi->get_id ()] != u_lo)
          trash_can.insert (trash_it, u_lo);
        trash_can.insert (trash_it, u);
      }
      else
      {
        // Regular vertex
        ve.lo_id = u_lo->get_id () - 1;
        ve.hi_id = u_hi->get_id () - 1;
        ve.v = u;
        Q.push_back (ve);
      }
    }
    pair<int, int> oldkey (-1, -1);
    // sort Q by id
    Q.sort ();
    for (list<VerticeEntry>::iterator it = Q.begin(); it != Q.end(); 
        it++)
    {
      VerticeEntry ve = *it;
      Vertex * u = ve.v;
      if (ve.lo_id == oldkey.first && ve.hi_id == oldkey.second)
      {
        u->set_id (next_id);
        trash_can.insert (trash_it, u);
      }
      else
      {
        Vertex * u_lo = u->get_child (false);
        Vertex * u_hi = u->get_child (true);
        next_id++;
        // cout << "2) From queue vertex: " << u << endl;
        // cout << "\tu_lo = " << u_lo;
        // cout << " | u_hi = " << u_hi;
        // cout << " | next_id = " << next_id << endl;
        u->set_child (NULL, false);
        u->set_child (NULL, true);
        u->set_id (next_id);
        subgraph[next_id] = u;
        u->clean_parents ();
        if (u_lo != NULL)
        {
          Vertex * actual_lo_child = subgraph[u_lo->get_id ()];
          if (actual_lo_child != u_lo) 
            trash_can.insert (trash_it, u_lo);
          u->set_child (actual_lo_child, false);
        }
        if (u_hi != NULL)
        {
          Vertex * actual_hi_child = subgraph[u_hi->get_id ()];
          if (actual_hi_child != u_hi)
            trash_can.insert (trash_it, u_hi);
          u->set_child (actual_hi_child, true);
        }
        oldkey = make_pair (ve.lo_id, ve.hi_id);
      }
    }
  }

  Vertex * new_root = subgraph[root->get_id ()];
  for (trash_it = trash_can.begin (); trash_it != trash_can.end (); )
  {
    Vertex * x = *trash_it;
    trash_it++;
    delete x;
  }
  root = new_root;
  cardinality = root->get_id ();
  for (unsigned int i = 1; i <= set_card + 1; i++)
    delete vlists[i];
  free (vlists);
  free (subgraph);    
  // cout << "After reduction: " << endl;
  // print ();
}


void ROBDD::union_to (Vertex * root2)
{
  OBDD::union_to (root2);
  reduce ();
}


void ROBDD::change_subset_value (ElementSubset * subset, 
  bool new_value) 
{
  // cout << "\n\n\nBefore adding subset " << subset->print_subset () << endl;
  // print ();
  OBDD::change_subset_value (subset, new_value);
  // cout << "After adding subset " << endl;
  // print ();
  reduce ();
  
}


void ROBDD::add_subset (ElementSubset * subset) 
{
  ROBDD * subset_robdd = new ROBDD (elm_set, subset);
  Vertex * root2 = subset_robdd->get_root ();
  union_to (root2);
  delete subset_robdd;
}


void ROBDD::remove_subset (ElementSubset * subset) 
{
  change_subset_value (subset, false);
}


void ROBDD::insert_vertex (Vertex * u, Vertex * v, bool side)
{
  OBDD::insert_vertex (u, v, side);
  reduce ();
}


void ROBDD::add_interval (ElementSubset * subset, bool orientation)
{
  // cout << "\n\n------------------------------------- \nBefore adding interval " << subset->print_subset () << endl;
  // print ();
  OBDD::add_interval (subset, orientation);
  // cout << "After adding interval " << endl;
  // print ();
  reduce ();
}