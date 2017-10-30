// PPFS.cpp automatically generated by bin/add_new_algorithm.pl
// in Tue Oct 24 19:26:08 2017.

//
// PPFS.cpp -- implementation of the class "PPFS".
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

#include "PPFS.h"


PPFS::PPFS ()
{
  list_of_visited_subsets = new Collection ();
  cost_function = NULL;
  bound = FLT_MAX;
  min_list_ptr = &list_of_minima;
}


PPFS::~PPFS ()
{
  if (list_of_visited_subsets != NULL)
  delete list_of_visited_subsets;
}


void PPFS::get_minima_list (unsigned int max_size_of_minima_list)
{
  timeval begin_program, end_program;
  gettimeofday (& begin_program, NULL);

  //
  unsigned int direction;
  ForestMap Forest_A, Forest_B;
  PFSNode * N;

  // the spanning tree T
  N = new PFSNode;
  N->vertex = new ElementSubset ("", set);
  store_visited_subset (N->vertex);
  N->adjacent = new ElementSubset ("", set);
  N->adjacent->set_complete_subset ();
  N->leftmost = 0;  // the first index is zero
  N->cost = FLT_MAX;
  N->vertex->cost = FLT_MAX;
  Forest_A.insert (ForestEntry (N->vertex->print_subset (), N));

  // the spanning tree T'
  N = new PFSNode;
  N->vertex = new ElementSubset ("", set);
  N->vertex->set_complete_subset ();
  store_visited_subset (N->vertex);
  N->adjacent = new ElementSubset ("", set);
  N->adjacent->set_complete_subset ();
  N->leftmost = 0;  // the first index is zero
  N->cost = FLT_MAX;
  N->vertex->cost = FLT_MAX;
  Forest_B.insert (ForestEntry (N->vertex->print_subset (), N));

  while (Forest_A.size () > 0 && Forest_B.size () > 0 && 
    !cost_function->has_reached_threshold ())
  {
    cout << "batch started" << endl;
    direction = rand () % 2;
    number_of_iterations++;

    if (direction == 0)
      lower_forest_iteration (&Forest_A, &Forest_B);
    else
      upper_forest_iteration (&Forest_A, &Forest_B);
    cout << "batch finished!" << endl << endl;
  }
  //

  number_of_visited_subsets =
  cost_function->get_number_of_calls_of_cost_function ();
  clean_list_of_minima (max_size_of_minima_list);
  gettimeofday (& end_program, NULL);
  elapsed_time_of_the_algorithm = diff_us (end_program, begin_program);
}


void PPFS::lower_forest_iteration (ForestMap * Forest_A, 
  ForestMap * Forest_B)
{
  #pragma omp parallel
  {
    PFSNode * N;
    N = lower_forest_branch (Forest_A, Forest_B);

    #pragma omp barrier

    #pragma omp critical
    cout << "[" << omp_get_thread_num () << "]" <<
      "started pruning" << endl;

    if (N != NULL)
      if (!cost_function->has_reached_threshold ())
        upper_forest_pruning (Forest_B, N);

    delete_node (N);
    #pragma omp critical
    cout << "thread " << omp_get_thread_num () << " finished its work" << endl;
  }
}


void PPFS::upper_forest_iteration (ForestMap * Forest_A, 
  ForestMap * Forest_B)
{
  #pragma omp parallel
  {
    PFSNode * N;
    N = upper_forest_branch (Forest_A, Forest_B);

    #pragma omp barrier

    #pragma omp critical
    cout << "[" << omp_get_thread_num () << "]" <<
      "started pruning" << endl;

    if (N != NULL)
      if (!cost_function->has_reached_threshold ())
        lower_forest_pruning (Forest_A, N);

    delete_node (N);
    #pragma omp critical
    cout << "thread " << omp_get_thread_num () << " finished its work" << endl;
  }
}


PFSNode * PPFS::lower_forest_branch (ForestMap * Forest_A, 
  ForestMap * Forest_B)
{
  ForestMap::iterator it;
  PFSNode * R, * M, * N;
  unsigned int i, m;

  #pragma omp critical
  R = pop_node (Forest_A);  
  #pragma omp barrier

  if (R == NULL)
    return NULL;

  #pragma omp critical
  cout << "[" << omp_get_thread_num () << "]" <<
    "selected " << R->vertex->print_subset () << endl;

  #pragma omp critical
  calculate_node_cost (R, Forest_B);
  M = R;
  N = R;

  // Walk on the tree whose root is R[vertex]
  while (!N->adjacent->is_empty () && N->cost <= M->cost)
  {
    #pragma omp critical
    {
      set_update_root (&N, Forest_A);
      M = N;
      m = M->adjacent->remove_random_element ();
      N = new PFSNode;
      N->vertex = new ElementSubset (M->vertex);
    }
    N->vertex->add_element (m);
    N->leftmost = m + 1;  // the index starts with zero
    N->adjacent = new ElementSubset ("", set);
    for (i = N->leftmost; i < set->get_set_cardinality (); i++)
    {
      N->vertex->add_element (i);
      #pragma omp critical
      {
        if (Forest_A->find (N->vertex->print_subset ()) != 
          Forest_A->end ())
          N->adjacent->add_element (i);
      }
      N->vertex->remove_element (i);
    }
    N->cost = FLT_MAX;

    #pragma omp critical
    calculate_node_cost (N, Forest_B);

    #pragma omp critical
    store_visited_subset (N->vertex);
  
    if (N->cost <= M->cost)
    {
      N->vertex->cost = N->cost;
      #pragma omp critical
      parallel_store_minimum_subset (N->vertex);
    }

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    if (cost_function->has_reached_threshold ())
      return N;
  }

  #pragma omp critical
  cout << "[" << omp_get_thread_num () << "]" <<
    "prunes on " << N->vertex->print_subset () << endl;

  return N;
}


PFSNode * PPFS::upper_forest_branch (ForestMap * Forest_A, 
  ForestMap * Forest_B)
{
  ForestMap::iterator it;
  PFSNode * R, * M, * N;
  unsigned int i, m;

  #pragma omp critical
  R = pop_node (Forest_B);
  #pragma omp barrier

  if (R == NULL)
    return NULL;

  #pragma omp critical
  cout << "[" << omp_get_thread_num () << "]" <<
    "selected " << R->vertex->print_subset () << endl;

  #pragma omp critical
  calculate_node_cost (R, Forest_A);
  M = R;
  N = R;

  // Walk on the tree whose root is R[vertex]
  while (!N->adjacent->is_empty () && N->cost <= M->cost)
  {
    #pragma omp critical
    {
      cout << "[" << omp_get_thread_num () << "]" <<
        "upper branch iteration on N = " << N->vertex->print_subset () << endl;

      set_update_root (&N, Forest_B);
      M = N;
      m = M->adjacent->remove_random_element ();
      N = new PFSNode;
      N->vertex = new ElementSubset (M->vertex);
    }
    N->vertex->remove_element (m);
    N->leftmost = m + 1;  // the index starts with zero
    N->adjacent = new ElementSubset ("", set);
    N->cost = FLT_MAX;
    for (i = N->leftmost; i < set->get_set_cardinality (); i++)
    {
      N->vertex->remove_element (i);
      #pragma omp critical
      {
        if (Forest_B->find (N->vertex->print_subset ()) != 
          Forest_B->end ())
          N->adjacent->add_element (i);
      }
      N->vertex->add_element (i);
    }

    #pragma omp critical
    calculate_node_cost (N, Forest_A);

    #pragma omp critical
    store_visited_subset (N->vertex);

    if (N->cost <= M->cost)
    {
      N->vertex->cost = N->cost;
      #pragma omp critical
      parallel_store_minimum_subset (N->vertex);
    }

    // if the algorithm is working under heuristic mode 1 or 2
    // and has reached threshold, then the search is stopped.
    if (cost_function->has_reached_threshold ())
      return N;
  }

  #pragma omp critical
  cout << "[" << omp_get_thread_num () << "]" <<
    "prunes on " << N->vertex->print_subset () << endl;

  return N;
}


void PPFS::search_upper_root (ForestMap * Forest_B, ElementSubset * M,
  SubsetSet * deleted_subsets)
{
  ForestMap::iterator it;
  PFSNode * N, * _M;
  string N_str;
  unsigned int i;
  int m, k = set->get_set_cardinality () - 1;

  while (k >= 0 && M->has_element ((unsigned int) k))
    k--;

  while (k >= 0)
  {
    M->add_element (k);
    #pragma omp critical
    it = Forest_B->find (M->print_subset ());
    if (it != Forest_B->end ())
    {
      #pragma omp critical
      {
        _M = get_node (Forest_B, M->print_subset ());
        if (_M != NULL)
          _M->adjacent->remove_element (k);
      }
      k = -1;
    }
    else
    {
      N = new PFSNode;
      N->vertex = new ElementSubset (M);
      m = k;
      while (k >= 0 && M->has_element ((unsigned int) k))
        k--;
      N->leftmost = k + 1; 
      N->adjacent = new ElementSubset ("", set);
      for (i = N->leftmost; i < set->get_set_cardinality (); i++)
      {
        N->vertex->remove_element (i);
        N_str = N->vertex->print_subset ();
        #pragma omp critical
        {
          if (Forest_B->find (N_str) != Forest_B->end () && 
              deleted_subsets->count (N_str) == 0)
            N->adjacent->add_element (i);
        }
        N->vertex->add_element (i);
      }
      N->adjacent->remove_element ((unsigned int) m);
      N->cost = FLT_MAX; // infinity
      
      #pragma omp critical
      store_visited_subset (N->vertex);

      N_str = N->vertex->print_subset ();
      #pragma omp critical
      {
        if (!deleted_subsets->count (N_str))
          set_update_root (&N, Forest_B);
      }
    } 
  }
}


void PPFS::search_lower_root (ForestMap * Forest_A, ElementSubset * M, 
  SubsetSet * deleted_subsets)
{
  ForestMap::iterator it;
  PFSNode * N, * _M;
  string N_str;
  unsigned int i;
  int m, k = set->get_set_cardinality () - 1;

  while ((k >= 0) && (! M->has_element ((unsigned int) k)) )
    k--;

  while (k >= 0)
  {
    M->remove_element (k);
    #pragma omp critical
    it = Forest_A->find (M->print_subset ());
    if (it != Forest_A->end ())
    {
      #pragma omp critical
      {
        _M = get_node (Forest_A, M->print_subset ());
        if (_M != NULL)
          _M->adjacent->remove_element (k);
      }
      k = -1;
    }
    else
    {
      N = new PFSNode;
      N->vertex = new ElementSubset (M);
      m = k;
      while (k >= 0 && !M->has_element ((unsigned int) k))
        k--;
      N->leftmost = k + 1;
      N->adjacent = new ElementSubset ("", set);
      for (i = N->leftmost; i < set->get_set_cardinality (); i++)
      {
        N->vertex->add_element (i);
        N_str = N->vertex->print_subset ();
        #pragma omp critical
        {
          if (Forest_A->find (N_str) != Forest_A->end () && 
              deleted_subsets->count (N_str) == 0)
            N->adjacent->add_element (i);
        }
        N->vertex->remove_element (i);
      }
      N->adjacent->remove_element ((unsigned int) m);
      N->cost = FLT_MAX;

      #pragma omp critical
      store_visited_subset (N->vertex);

      N_str = N->vertex->print_subset ();
      #pragma omp critical
      {
        if (!deleted_subsets->count (N_str))
          set_update_root (&N, Forest_A);
      }
    }
  }
}


void PPFS::search_lower_children (ForestMap * Forest_B, PFSNode * N, 
  ElementSubset * M, ElementSubset * Y, SubsetSet * deleted_subsets)
{
  ForestMap::iterator it;
  string M_str;
  PFSNode * B;
  int i;
  unsigned int j; 

  i = set->get_set_cardinality () - 1;  // i = n
  while (i >= 0 && M->has_element (i))
  {
    M->remove_element (i); // A = M - {s_i}
    string M_str = M->print_subset ();
    if (M->contains (Y)) // if B contains Y
    {
      #pragma omp critical
      {
        it = Forest_B->find (M_str);
        if (it != Forest_B->end ())
        {
          delete_node (it->second);
          Forest_B->erase (it);
        }
        deleted_subsets->insert (M_str);
      }
    }
    else
    {
      if (N == NULL || N->adjacent->has_element (i))
      {
        B = new PFSNode;
        B->vertex = new ElementSubset (M);
        B->leftmost = i + 1;
        B->adjacent = new ElementSubset ("", set);
        for (j = B->leftmost; j < set->get_set_cardinality (); j++)
        {
          B->vertex->remove_element (j);
          string B_str = B->vertex->print_subset ();
          #pragma omp critical
          {
            if (Forest_B->find (B_str) != Forest_B->end () && 
                deleted_subsets->count (B_str) == 0)
              B->adjacent->add_element (j);
          }
          B->vertex->add_element (j);
        }
        B->cost = FLT_MAX; // infinity

        #pragma omp critical
        store_visited_subset (B->vertex);
      
        #pragma omp critical
        {
          if (!deleted_subsets->count (M_str))
            set_update_root (&B, Forest_B);
        }
      }
    }
    M->add_element (i);
    i--;
  }
}


void PPFS::search_upper_children (ForestMap * Forest_A, PFSNode * N, 
  ElementSubset * M, ElementSubset * Y, SubsetSet * deleted_subsets)
{
  ForestMap::iterator it;
  string M_str;
  PFSNode * A;
  int i;
  unsigned int j; 

  i = set->get_set_cardinality () - 1;
  while (i >= 0 && !M->has_element (i))
  {
    M->add_element (i);
    M_str = M->print_subset ();
    if (M->is_contained_by (Y))
    {
      #pragma omp critical
      {
        it = Forest_A->find (M_str);
        if (it != Forest_A->end ())
        {
          delete_node (it->second);
          Forest_A->erase (it);
        }
        deleted_subsets->insert (M_str);
      }
    }
    else
    {
      if (N == NULL || N->adjacent->has_element (i))
      {
        A = new PFSNode;
        A->vertex = new ElementSubset (M);
        A->leftmost = i + 1;
        A->adjacent = new ElementSubset ("", set);
        for (j = A->leftmost; j < set->get_set_cardinality (); j++)
        {
          A->vertex->add_element (j);
          string A_str = A->vertex->print_subset ();
          #pragma omp critical
          {
            if (Forest_A->find (A_str) != Forest_A->end () && 
                deleted_subsets->count (A_str) == 0)
              A->adjacent->add_element (j);
          }
          A->vertex->remove_element (j);
        }
        A->cost = FLT_MAX;

        #pragma omp critical
        store_visited_subset (A->vertex);

        #pragma omp critical
        {
          if (!deleted_subsets->count (M_str))
            set_update_root (&A, Forest_A);
        }
      }
    }
    M->remove_element (i);
    i--;
  }
}


void PPFS::upper_forest_pruning (ForestMap * Forest_B, PFSNode * N)
{
  SubsetSet deleted_subsets;
  ForestMap::iterator it;
  PFSNode * _M, * _M_copy;
  ElementSubset M ("", set);
  string M_str;
  int k;

  M.copy (N->vertex);
  k = set->get_set_cardinality () - 1;  // k = n - 1

  while (!N->adjacent->is_empty () && k >= (int) N->leftmost)
  {
    M_str = M.print_subset ();
    #pragma omp critical
    {
      it = Forest_B->find (M_str);
      if (it != Forest_B->end ())
      {
        _M = it->second;
        Forest_B->erase (it);
      }
      else
        _M = NULL;
      deleted_subsets.insert (M_str);
      _M_copy = copy_node (_M);
    }

    search_lower_children (Forest_B, _M_copy, &M, N->vertex, 
      &deleted_subsets);
    delete_node (_M_copy);
    
    #pragma omp critical
    {
      it = Forest_B->find (M_str);
      if (it != Forest_B->end ())
      {
        delete_node (it->second);
        Forest_B->erase (it);
      }
    }

    M.add_element (k);
    k--;
  }

  M_str = M.print_subset ();
  #pragma omp critical
  {
    deleted_subsets.insert (M_str);
    it = Forest_B->find (M.print_subset ());
    if (it != Forest_B->end ())
      _M = it->second;
    else
      _M = NULL;
    _M_copy = copy_node (_M);
  }
  
  search_lower_children (Forest_B, _M_copy, &M, N->vertex, 
    &deleted_subsets);
  delete_node (_M_copy);
  
  if (_M == NULL)
    search_upper_root (Forest_B, &M, &deleted_subsets);
  
  #pragma omp critical
  {
    cout << "[" << omp_get_thread_num () << "]" << 
      "Deleted children on upper forest pruning" << endl;
    SubsetSet::iterator sub_it;
    for (sub_it = deleted_subsets.begin (); sub_it != deleted_subsets.end (); sub_it++)
    {
      cout << *sub_it << endl;
    }
    cout << "List end" << endl;
  }

  _M = NULL;
  #pragma omp critical
  {
    it  = Forest_B->find (M_str);
    if (it != Forest_B->end ())
    {
      _M = it->second;
      delete_node (it->second);
      Forest_B->erase (it);
    }
  }

}


void PPFS::lower_forest_pruning (ForestMap * Forest_A, PFSNode * N)
{
  SubsetSet deleted_subsets;
  ForestMap::iterator it;
  PFSNode * _M, * _M_copy;
  ElementSubset M ("", set);
  string M_str;
  int k;

  M.copy (N->vertex);
  k = set->get_set_cardinality () - 1;  // k = n - 1

  while (!N->adjacent->is_empty () && k >= (int) N->leftmost) 
  {
    cout << "[" << omp_get_thread_num () << "]" <<
      "    pruning iteration on M = " << M.print_subset () << endl;
    M_str = M.print_subset ();
    #pragma omp critical
    {
      it  = Forest_A->find (M_str);
      if (it != Forest_A->end ())
      {
        _M = it->second;
        Forest_A->erase (it);
      }
      else
        _M = NULL;
      deleted_subsets.insert (M_str);
      _M_copy = copy_node (_M);
    }

    search_upper_children (Forest_A, _M_copy, &M, N->vertex,
      &deleted_subsets);
    delete_node (_M_copy);

    #pragma omp critical
    {
      it = Forest_A->find (M_str);
      if (it != Forest_A->end ())
      {
        delete_node (it->second);
        Forest_A->erase (it);
      }
    }

    M.remove_element (k);
    k--;
  }

  M_str = M.print_subset ();
  #pragma omp critical
  {
    deleted_subsets.insert (M_str);
    it  = Forest_A->find (M.print_subset ());
    if (it != Forest_A->end ())
      _M = it->second;
    else
      _M = NULL;
    _M_copy = copy_node (_M);
  }

  search_upper_children (Forest_A, _M_copy, & M, N->vertex, 
    &deleted_subsets);
  delete_node (_M_copy);
  
  if (_M == NULL)
    search_lower_root (Forest_A, &M, &deleted_subsets);
  
  #pragma omp critical
  {
    cout << "[" << omp_get_thread_num () << "]" << 
      "Deleted children on upper forest pruning" << endl;
    SubsetSet::iterator sub_it;
    for (sub_it = deleted_subsets.begin (); sub_it != deleted_subsets.end (); sub_it++)
    {
      cout << *sub_it << endl;
    }
    cout << "List end" << endl;
  }

  _M = NULL;
  #pragma omp critical
  {
    it = Forest_A->find (M_str);
    if (it != Forest_A->end ())
    {
      _M = it->second;
      delete_node (it->second);
      Forest_A->erase (it);
    }
  }

}

void PPFS::calculate_node_cost (PFSNode * R, ForestMap * Forest_Dual)
{
  if (R->cost != FLT_MAX)
    return;
  
  // copy elementsubset here?
  PFSNode * RB_cpy = NULL;
  ForestMap::iterator it; 
    

  it = Forest_Dual->find (R->vertex->print_subset ());
  if (it != Forest_Dual->end ())
    RB_cpy = copy_node (it->second);

  if (RB_cpy == NULL)
  {
    R->cost = cost_function->cost (R->vertex);
    R->vertex->cost = R->cost;
    parallel_store_minimum_subset (R->vertex);
  }
  else if (RB_cpy->cost == FLT_MAX)
  {
    R->cost = cost_function->cost (R->vertex);
    R->vertex->cost = R->cost;
    RB_cpy->cost = R->cost;
    RB_cpy->vertex->cost = RB_cpy->cost;
    parallel_store_minimum_subset (R->vertex);
  }
  else
  {
    R->cost = RB_cpy->cost;
    R->vertex->cost = R->cost;
  } 
  delete_node (RB_cpy);
}


void PPFS::parallel_store_minimum_subset (ElementSubset * X)
{
  ElementSubset * Y;
  Y = new ElementSubset ("", set);
  Y->copy (X);
  min_list_ptr->push_back (Y);
}


PFSNode * PPFS::get_node (ForestMap * F, string subset)
{
  ForestMap::iterator it;
  PFSNode * N = NULL;
  it = F->find (subset);
  if (it != F->end ())
    N = it->second;
  return N;
}


PFSNode * PPFS::copy_node (PFSNode * N)
{
  if (N == NULL)
    return NULL;

  PFSNode * M = new PFSNode;
  M->vertex = new ElementSubset (N->vertex);
  M->adjacent = new ElementSubset (N->adjacent);
  M->leftmost = N->leftmost;
  M->cost = N->cost;
  return M;
}


void PPFS::delete_node (PFSNode * N)
{
  if (N == NULL)
    return;
  
  delete N->vertex;
  delete N->adjacent;
  delete N;
}


PFSNode * PPFS::pop_node (ForestMap * F)
{
  PFSNode * R;
  ForestMap::iterator it;
  if (F->size () == 0)
      R = NULL;
  else
  {
    if (rand () % 2)
      it = F->begin ();
    else
    {
      it = F->end ();
      it--;
    }
    R = it->second;
    F->erase (it);
  }
  return R;
}


void PPFS::set_update_root (PFSNode ** N_ptr, ForestMap * F)
{
  ForestMap::iterator it;
  string subset_str = (*N_ptr)->vertex->print_subset ();
  it = F->find (subset_str);
  if (it == F->end ()) 
  {
    cout << "    [" << omp_get_thread_num () << "] adds root " << subset_str << endl;
    F->insert (ForestEntry (subset_str, *N_ptr));
  }
  else
  {
    it->second->adjacent->subset_intersection ((*N_ptr)->adjacent);
    // todo: we should see which one has a calculated cost
    it->second->cost = (*N_ptr)->cost;
    delete_node (*N_ptr);
    *N_ptr = it->second;
  }
  return;
}