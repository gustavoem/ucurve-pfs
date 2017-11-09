// PUCS.cpp automatically generated by bin/add_new_algorithm.pl
// in Thu Nov  9 10:12:16 2017.

//
// PUCS.cpp -- implementation of the class "PUCS".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Gustavo Estrela, Marcelo S. Reis
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

#include "PUCS.h"


PUCS::PUCS ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
  cand_part = NULL;
  partition = NULL;
  this->p = 0;  // set later
  this->l = 1;
}

PUCS::PUCS (float p, unsigned int l)
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
  cand_part = NULL;
  partition = NULL;
  this->p = p;
  this->l = l;
}


PUCS::~PUCS ()
{
  if (list_of_visited_subsets != NULL)
    delete list_of_visited_subsets;
  if (cand_part != NULL)
    delete cand_part;
  if (partition != NULL)
    delete partition;
}


void PUCS::set_partition_model ()
{
  unsigned int set_size = set->get_set_cardinality ();
  unsigned int fixed_set_size = set_size * p;
  if (fixed_set_size == 0) fixed_set_size++;
  bool * fixed = new bool[set_size];
  for (unsigned int i = 0; i < set_size; i++)
    fixed[i] = false;
  
  ElementSubset X ("", set);
  X.set_complete_subset ();
  for (unsigned int i = 0; i < fixed_set_size; i++)
  {
    unsigned int e = X.remove_random_element ();
    fixed[e] = true;
  }
  this->partition = new Partition (set, fixed);
  delete[] fixed;
}


void PUCS::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program, end_walk, end_wait, end_clean;
  // t.i.
  gettimeofday (& begin_program, NULL);

  if (p == 0) {
    p = 10.0 / set->get_set_cardinality ();
    if (set->get_set_cardinality () > 30)
      l = 1;
    else
      l = 2;
  }
  if (p > .5)
    p = .5;

  this->max_size_of_minima_list = max_size_of_minima_list;
  list<ElementSubset *> * min_list = &list_of_minima;
  #pragma omp parallel shared (min_list)
  #pragma omp single nowait
  {
    ElementSubset * p_subset;
    set_partition_model ();

    cand_part = new ROBDD (partition->get_fixed_elm_set ());
    p_subset = cand_part->get_random_zero_evaluated_element ();
    while (p_subset != NULL)
    {
      PartitionNode * P = new PartitionNode (partition, p_subset);
      random_walk (P, min_list);
      delete p_subset;
      p_subset = cand_part->get_random_zero_evaluated_element ();
    }
    
    // t.d.
    gettimeofday (& end_walk, NULL);
    //time_walking = diff_us (end_walk, begin_program);
    #pragma omp taskwait
     
    // t.f.
    gettimeofday (& end_wait, NULL);
    //time_waiting = diff_us (end_wait, end_walk);
    
    clean_list_of_minima (max_size_of_minima_list);
    
    gettimeofday (& end_clean, NULL);
    //time_cleaning = diff_us (end_clean, end_wait);
    //#pragma omp critical
    //cout << "[" << this << "] ti = " << time_walking << "; td = " <<
        //time_waiting << "; tf = " << time_cleaning << "; l = "<< l 
        //<< endl;

  }

  number_of_visited_subsets = 
    cost_function->get_number_of_calls_of_cost_function ();
  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);
}


void PUCS::random_walk (PartitionNode * P, list<ElementSubset *> *
  min_list)
{
  unsigned int i = 0;
  unsigned int n = P->get_number_of_fixed_elms ();
  PartitionNode * Q;
  while (i < n)
  {
    Q = adjacent_part (P, i++);
    if (is_restricted (Q))
    {   
      delete Q;
      continue;
    }
    PartitionNode * next;
    next = prune_and_walk (P, Q);
    if (next == P)
    {
      delete Q;
    }
    else if (next == Q)
    {
      i = 0;
      if (!is_restricted (P))
      {
        solve_part (new PartitionNode (P), min_list);
        restrict_part (P);
      }
      delete P;
      P = Q;
    }
    else
    {
      delete P;
      delete Q;
      return;
    }
  }
  solve_part (new PartitionNode (P), min_list);
  restrict_part (P);
  delete P;
}


void PUCS::solve_part (PartitionNode * P, list<ElementSubset *> *
  min_list)
{
  #pragma omp task
  {
    Collection * L;
    L = part_minimum (P, max_size_of_minima_list);
    #pragma omp taskwait
    // TODO: insert minimum elements directly to the minima list
    while (L->size () > 0) 
    {
      ElementSubset * X = L->remove_last_subset ();
      #pragma omp critical
      min_list->push_back (X);
    }
    delete L;
    delete P;
  }
}


void PUCS::part_minima_collection (Collection * L, PartitionNode * P,
  list<ElementSubset *> * l)
{
  while (l->size () > 0) 
  {
    ElementSubset * pX = l->back ();
    l->pop_back ();
    ElementSubset * X = P->get_original_subset (pX);
    X->cost = pX->cost;
    L->add_subset (X);
    delete X;
    delete pX;
  }
}


Collection * PUCS::part_minimum (PartitionNode * P, 
  unsigned int max_size_of_minima_list)
{
  Solver * sub_solver = NULL;
  Collection * L = new Collection ();
  list<ElementSubset *> p_min_lst;
  ElementSet * p_elm_set = partition->get_unfixed_elm_set ();
  if (p_elm_set->get_set_cardinality () == 0)
  {
    ElementSubset * minimal;
    minimal = new ElementSubset (P->get_least_subset ());
    minimal->cost = cost_function->cost (minimal);
    p_min_lst.push_back (minimal);
  }
  else
  {
    Collection * visited_subsets;
    if (p_elm_set->get_set_cardinality () <= ES_CUTOFF)
        sub_solver = new ExhaustiveSearch ();
    else if (l <= 1)
      sub_solver = new SFS ();
    else
      sub_solver = new PUCS (p, l - 1);
    PartCost * P_cost = new PartCost (cost_function, P);
    sub_solver->set_parameters (P_cost, p_elm_set, store_visited_subsets);
    
    #pragma omp task
    sub_solver->get_minima_list (max_size_of_minima_list);
    #pragma omp taskwait
    p_min_lst = sub_solver->get_list_of_minima ();
    visited_subsets = sub_solver->get_list_of_visited_subsets ();
    update_visited_subsets (visited_subsets, P);
    delete P_cost;
  }
  part_minima_collection (L, P, &p_min_lst);
  if (sub_solver != NULL)
    delete sub_solver;
  return L;
}


PartitionNode * PUCS::adjacent_part (PartitionNode * P, unsigned int i)
{
  ElementSubset * sel_elms = new ElementSubset (P->get_selected_elements ());
  if (sel_elms->has_element (i))
    sel_elms->remove_element (i);
  else
    sel_elms->add_element (i);
  Partition * partition = P->get_partition ();
  PartitionNode * Q = new PartitionNode (partition, sel_elms);
  delete sel_elms;
  return Q;
}


PartitionNode * PUCS::prune_and_walk (PartitionNode * P, PartitionNode * Q)
{
  PartitionNode * P1, * P2, * next;
  ElementSubset * e1, * e2, * p1_sub, * p2_sub;
  ElementSubset * p_sub = P->get_selected_elements ();
  ElementSubset * q_sub = Q->get_selected_elements ();
  if (P->is_upper_adjacent (Q))
  {
    P1 = P;
    P2 = Q;
  }
  else
  {
    P1 = Q;
    P2 = P;
  }
  p1_sub = P1->get_selected_elements ();
  p2_sub = P2->get_selected_elements ();
  e1 = P1->get_least_subset ();
  e2 = P2->get_least_subset ();
  #pragma omp critical
  {
    store_visited_subset (e1);
    store_visited_subset (e2);
  }
  if (cost_function->cost (e1) > cost_function->cost (e2))
    cand_part->add_interval (p1_sub, true);
  e1 = P1->get_greatest_subset ();
  e2 = P2->get_greatest_subset ();
  #pragma omp critical
  {
    store_visited_subset (e1);
    store_visited_subset (e2);
  }
  if (cost_function->cost (e1) < cost_function->cost (e2))
    cand_part->add_interval (p2_sub, false);
  next = Q;
  if (cand_part->contains (q_sub))
  {
    if (cand_part->contains (p_sub))
      next = NULL;
    else
      next = P;
  }
  return next;
}


bool PUCS::is_restricted (PartitionNode * P) 
{
  bool answ = false;
  ElementSubset * p_subset = P->get_selected_elements ();
  if (cand_part->contains (p_subset))
    answ = true;
  return answ;
}


void PUCS::restrict_part (PartitionNode * P)
{
  ElementSubset * p_subset = P->get_selected_elements ();
  cand_part->add_subset (p_subset);
}


void PUCS::update_visited_subsets (Collection * T, PartitionNode * P)
{
  while (T->size () > 0) 
  {
    ElementSubset * pX = T->remove_last_subset ();
    ElementSubset * X = P->get_original_subset (pX);
    #pragma omp critical
    store_visited_subset (X);
    delete pX;
    delete X;
  }
}
