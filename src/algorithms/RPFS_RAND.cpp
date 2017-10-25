// RPFS_RAND.cpp automatically generated by bin/add_new_algorithm.pl
// in Wed Oct 25 09:02:44 2017.

//
// RPFS_RAND.cpp -- implementation of the class "RPFS_RAND".
//
//    This file is part of the featsel program
//    Copyright (C) 2016  Marcelo S. Reis
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

#include "RPFS_RAND.h"


RPFS_RAND::RPFS_RAND ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
}


RPFS_RAND::~RPFS_RAND ()
{
  if (list_of_visited_subsets != NULL)
  delete list_of_visited_subsets;
}


void RPFS_RAND::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  //
  unsigned int direction;
  ForestOBDD * Forest_A, * Forest_B;
  PFSNode * N;
  Forest_A = new ForestOBDD (set);
  Forest_B = new ForestOBDD (set);

  // the spanning tree T
  N = new PFSNode;
  N->vertex = new ElementSubset ("", set);
  store_visited_subset (N->vertex);
  N->adjacent = new ElementSubset ("", set);
  N->adjacent->set_complete_subset ();
  N->leftmost = 0;  // the first index is zero
  N->cost = FLT_MAX;
  Forest_A->add_node (N);

  // the spanning tree T'
  N = new PFSNode;
  N->vertex = new ElementSubset ("", set);
  N->vertex->set_complete_subset ();
  store_visited_subset (N->vertex);
  N->adjacent = new ElementSubset ("", set);
  N->adjacent->set_complete_subset ();
  N->leftmost = 0;  // the first index is zero
  N->cost = FLT_MAX;
  // Forest_B.insert (pair<string, PFSNode *> (N->vertex->print_subset (), N));
  Forest_B->add_node (N);

  srand ((unsigned) time (NULL));
  while (Forest_A->size () > 0 && Forest_B->size () > 0 && 
         !cost_function->has_reached_threshold ())
  {
    direction = rand () % 2;

    number_of_iterations++;
    if (direction == 0)
    {
      N = lower_forest_branch (Forest_A, Forest_B);
      if (! cost_function->has_reached_threshold ())
        upper_forest_pruning (Forest_B, N);
    }
    else
    {
      N = upper_forest_branch (Forest_A, Forest_B);
      if (! cost_function->has_reached_threshold ())
        lower_forest_pruning (Forest_A, N);
    }

    delete N->vertex;
    delete N->adjacent;
    delete N;
  }
  delete Forest_A;
  delete Forest_B;
  //

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();
  clean_list_of_minima (max_size_of_minima_list);

  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);

}


PFSNode * RPFS_RAND::lower_forest_branch (ForestOBDD * Forest_A, 
  ForestOBDD * Forest_B)
{
  PFSNode * R, * M, * N;
  unsigned int i, m;

  R = Forest_A->get_best_pruning_potential_node1 ();
  Forest_A->remove_node (R);
  calculate_node_cost (R, Forest_B);
  M = R;
  N = R;

  // Walk on the tree whose root is R[vertex]
  while ((! N->adjacent->is_empty () ) && (N->cost <= M->cost) )
  {
    Forest_A->add_node (N);
    M = N;
    m = M->adjacent->remove_random_element ();
    N = new PFSNode;
    N->vertex = new ElementSubset ("", set);
    N->vertex->copy (M->vertex);
    N->vertex->add_element (m);
    store_visited_subset (N->vertex);

    N->leftmost = m + 1;  // the index starts with zero
    N->adjacent = new ElementSubset ("", set);
    N->cost = FLT_MAX;
    for (i = N->leftmost; i < set->get_set_cardinality (); i++)
      N->adjacent->add_element (i);
    calculate_node_cost (N, Forest_B);
  
    if (N->cost <= M->cost)
    {
      N->vertex->cost = N->cost;
      store_minimum_subset (N->vertex);
    }

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    if (cost_function->has_reached_threshold ())
      return N;
  }

  return N;
}


PFSNode * RPFS_RAND::upper_forest_branch (ForestOBDD * Forest_A, 
  ForestOBDD * Forest_B)
{
  PFSNode * R, * M, * N;
  unsigned int i, m;

  R = Forest_B->get_best_pruning_potential_node1 ();
  Forest_B->remove_node (R);
  calculate_node_cost (R, Forest_A);
  M = R;
  N = R;

  // Walk on the tree whose root is R[vertex]
  while ((!N->adjacent->is_empty ()) && (N->cost <= M->cost))
  {
    Forest_B->add_node (N);
    M = N;
    m = M->adjacent->remove_random_element ();
    N = new PFSNode;
    N->vertex = new ElementSubset ("", set);
    N->vertex->copy (M->vertex);
    N->vertex->remove_element (m);

    store_visited_subset (N->vertex);

    N->leftmost = m + 1;  // the index starts with zero
    N->adjacent = new ElementSubset ("", set);
    N->cost = FLT_MAX;
    for (i = N->leftmost; i < set->get_set_cardinality (); i++)
      N->adjacent->add_element (i);
    calculate_node_cost (N, Forest_A);

    if (N->cost <= M->cost)
    {
      N->vertex->cost = N->cost;
      store_minimum_subset (N->vertex);
    }

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    if (cost_function->has_reached_threshold ())
      return N;
  }

  return N;
}


void RPFS_RAND::search_upper_root (ForestOBDD * Forest_B, 
  ElementSubset * M)
{
  PFSNode * N;
  unsigned int i;
  int m, k = set->get_set_cardinality () - 1;

  while ((k >= 0) && (M->has_element ((unsigned int) k)) )
    k--;

  while (k >= 0)
  {
    M->add_element (k);
    if (Forest_B->contains (M))
    {
      PFSNode * M_node = Forest_B->get_node (M->print_subset ());
      M_node->adjacent->remove_element (k);
      k = -1;
    }
    else
    {
      N = new PFSNode;
      N->vertex = new ElementSubset ("", set);
      N->vertex->copy (M);
      store_visited_subset (N->vertex);

      m = k;
      while ((k >= 0) && (M->has_element ((unsigned int) k)) )
        k--;
      N->leftmost = k + 1;  // the index starts with zero
      N->adjacent = new ElementSubset ("", set);
      for (i = N->leftmost; i < set->get_set_cardinality (); i++)
        N->adjacent->add_element (i);
      N->adjacent->remove_element ((unsigned int) m);
      N->cost = FLT_MAX; // infinity

      Forest_B->add_node (N);

      // if the algorithm is working under heuristic mode 1 or 2
      // and has reached threshold, then the search is stopped.
      if (cost_function->has_reached_threshold ())
        return;
    }
  }
}


void RPFS_RAND::search_lower_root (ForestOBDD * Forest_A, 
  ElementSubset * M)
{
  PFSNode * N;
  unsigned int i;
  int m, k = set->get_set_cardinality () - 1;

  while ((k >= 0) && (!M->has_element ((unsigned int) k)))
    k--;

  while (k >= 0)
  {
    M->remove_element (k);
    if (Forest_A->contains (M))
    {
      PFSNode * M_node = Forest_A->get_node (M->print_subset ());
      M_node->adjacent->remove_element (k);
      k = -1;
    }
    else
    {
      N = new PFSNode;
      N->vertex = new ElementSubset ("", set);
      N->vertex->copy (M);
      store_visited_subset (N->vertex);

      m = k;
      while ((k >= 0) && (! M->has_element ((unsigned int) k)) )
        k--;
      N->leftmost = k + 1;  // the index starts with zero
      N->adjacent = new ElementSubset ("", set);
      for (i = N->leftmost; i < set->get_set_cardinality (); i++)
        N->adjacent->add_element (i);
      N->adjacent->remove_element ((unsigned int) m);
      N->cost = FLT_MAX; // infinity

      Forest_A->add_node (N);

      // if the algorithm is working under heuristic mode 1 or 2
      // and has reached threshold, then the search is stopped.
      if (cost_function->has_reached_threshold ())
        return;
    }
  } 
}


void RPFS_RAND::search_lower_children (ForestOBDD * Forest_B, 
  PFSNode * N, ElementSubset * M, ElementSubset * Y)
{
  int i;
  PFSNode * B;
  unsigned int j;

  i = set->get_set_cardinality () - 1;
  while ((i >= 0) && (M->has_element (i)))
  {
    M->remove_element (i);
    if (M->contains (Y)) // if B contains Y
    {
      B = Forest_B->get_node (M->print_subset ());
      if (B != NULL)
      {
        Forest_B->remove_node (B);
        delete B->vertex;
        delete B->adjacent;
        delete B;
      }
    }
    else
    {
      if (((N == NULL) || (N->adjacent->has_element (i))))
      {
        B = new PFSNode;
        B->vertex = new ElementSubset ("", set);
        B->vertex->copy (M);
        store_visited_subset (B->vertex);

        B->leftmost = i + 1;  // the index starts with zero
        B->adjacent = new ElementSubset ("", set);
        for (j = B->leftmost; j < set->get_set_cardinality (); j++)
          B->adjacent->add_element (j);
        B->cost = FLT_MAX; // infinity

        Forest_B->add_node (B);

        // if the algorithm is working under heuristic mode 1 or 2
        // and has reached threshold, then the search is stopped.
        if (cost_function->has_reached_threshold ())
          return;

      }
    }

    M->add_element (i);
    i--;
  }
}


void RPFS_RAND::search_upper_children (ForestOBDD * Forest_A, 
  PFSNode * N, ElementSubset * M, ElementSubset * Y)
{
  int i;
  PFSNode * A;
  unsigned int j;

  i = set->get_set_cardinality () - 1;
  while ((i >= 0) && (! M->has_element (i)) )
  {
    M->add_element (i);
    if (M->is_contained_by (Y)) // if A is contained in Y
    {
      A = Forest_A->get_node (M->print_subset ());
      if (A != NULL)
      {
        Forest_A->remove_node (A);
        delete A->vertex;
        delete A->adjacent;
        delete A;
      }
    }
    else
    {
      if (((N == NULL) || (N->adjacent->has_element (i))))
      {
        A = new PFSNode;
        A->vertex = new ElementSubset ("", set);
        A->vertex->copy (M);
        store_visited_subset (A->vertex);

        A->leftmost = i + 1;  // the index starts with zero
        A->adjacent = new ElementSubset ("", set);
        for (j = A->leftmost; j < set->get_set_cardinality (); j++)
          A->adjacent->add_element (j);
        A->cost = FLT_MAX; // infinity

        Forest_A->add_node (A);

        // if the algorithm is working under heuristic mode 1 or 2
        // and has reached threshold, then the search is stopped.
        if (cost_function->has_reached_threshold ())
          return;
      } 
    }
    M->remove_element (i);
    i--;
  }
}


void RPFS_RAND::upper_forest_pruning (ForestOBDD * Forest_B, 
  PFSNode * N)
{
  int k;
  PFSNode * _M;
  ElementSubset M ("", set);
  M.copy (N->vertex);
  k = set->get_set_cardinality () - 1;  // k = n - 1

  while (!N->adjacent->is_empty () && k >= (int) N->leftmost)
  {
    _M = Forest_B->get_node (M.print_subset ());
    if (_M != NULL)
      Forest_B->remove_node (_M);

    search_lower_children (Forest_B, _M, &M, N->vertex);

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    if (cost_function->has_reached_threshold ())
      return;

    if (_M != NULL)
    {
      delete _M->vertex;
      delete _M->adjacent;
      delete _M;
    }

    M.add_element (k);
    k--;
  }

  _M  = Forest_B->get_node (M.print_subset ());
  if (_M != NULL)
  {
    search_lower_children (Forest_B, _M, &M, N->vertex);

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    if (cost_function->has_reached_threshold ())
      return;

    Forest_B->remove_node (_M);
    delete _M->vertex;
    delete _M->adjacent;
    delete _M;
  }
  else
  {
    search_lower_children (Forest_B, NULL, &M, N->vertex);

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    if (cost_function->has_reached_threshold ())
      return;

    search_upper_root (Forest_B, & M); // warning: this function modifies "M" !!!

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    if (cost_function->has_reached_threshold ())
      return;
  }
}


void RPFS_RAND::lower_forest_pruning (ForestOBDD * Forest_A, 
  PFSNode * N)
{
  PFSNode * _M;
  ElementSubset M ("", set);
  int k;

  M.copy (N->vertex);
  k = set->get_set_cardinality () - 1;  // k = n - 1

  while (!N->adjacent->is_empty () && k >= (int) N->leftmost)
  {
    _M = Forest_A->get_node (M.print_subset ());
    if (_M != NULL)
      Forest_A->remove_node (_M);

    search_upper_children (Forest_A, _M, &M, N->vertex);

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    if (cost_function->has_reached_threshold ())
      return;

    if (_M != NULL)
    {
      delete _M->vertex;
      delete _M->adjacent;
      delete _M;
    }

    M.remove_element (k);
    k--;
  }

  _M = Forest_A->get_node (M.print_subset ());
  if (_M != NULL)
  {
    search_upper_children (Forest_A, _M, &M, N->vertex);

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    //
    if (cost_function->has_reached_threshold ())
      return;

    Forest_A->remove_node (_M);
    delete _M->vertex;
    delete _M->adjacent;
    delete _M;
  }
  else
  {
    search_upper_children (Forest_A, NULL, &M, N->vertex);

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    if (cost_function->has_reached_threshold ())
      return;

    search_lower_root (Forest_A, &M); // warning: this function modifies "M" !!!

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    if (cost_function->has_reached_threshold ())
      return;
  }
}


void RPFS_RAND::calculate_node_cost (PFSNode * R, ForestOBDD * Forest_Dual)
{
  if (R->cost == FLT_MAX)
  {
    PFSNode * RB = Forest_Dual->get_node (R->vertex->print_subset ());
    if (RB == NULL)
    {
      R->cost = cost_function->cost (R->vertex);
      R->vertex->cost = R->cost;
      store_minimum_subset (R->vertex);
    }
    else if (RB->cost == FLT_MAX)
    {
      R->cost = cost_function->cost (R->vertex);
      R->vertex->cost = R->cost;
      store_minimum_subset (R->vertex);
      RB->cost = R->cost;
      RB->vertex->cost = RB->cost;
    }
    else
    {
      R->cost = RB->cost;
      R->vertex->cost = R->cost;
    }
  }
}