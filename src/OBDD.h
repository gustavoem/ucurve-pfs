//
// OBDD.h -- definition of the namespace "OBDD".
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

#ifndef OBDD_H_
#define OBDD_H_

#include "global.h"
#include "ElementSet.h"
#include "Vertex.h"
#include "ElementSubset.h"
#include "CostFunction.h"

class OBDD
{

protected:

  // The set of elements this OBDD will represent
  //
  ElementSet * elm_set;
  // OBDD cardinality
  //
  unsigned int cardinality;

  // The root of the OBDD
  //
  Vertex * root;  

  // Prints the sub-tree that has the parameter Vertex as root
  //
  void print (Vertex *);

  // Auxiliar function to OBDD (ElementSet *, ElementSubset*)
  //
  void build (Vertex *, unsigned int, unsigned int, ElementSubset *, 
      Vertex *, Vertex *);

  // Unmarks all the vertices from the obdd
  //
  void unmark_all_vertice ();

  // Unmarks all the vertices from the subtree with root Vertex *
  //
  void unmark_all_vertice (Vertex *);

  // Deletes the subtree with root Vertex *
  //
  void delete_subtree (Vertex **, unsigned int *);

  // Returns a list with all vertex of the OBDD with root Vertex *
  //
  Vertex ** get_all_vertex (Vertex *, unsigned int *);

  // Auxiliar function to get_all_vertex that traverse the obdd assgning
  // pointers of the vertex to the vector vertice
  //
  void fill_vertice (Vertex **, int *, Vertex *);

  // Auxiliar function to add_lower_interval (). This method builds a 
  // tree of a OBDD that has ones for subsets covered by the subset 
  // passed by argument.
  //
  Vertex * build_interval (unsigned int, unsigned int *, 
      ElementSubset *, Vertex *, Vertex *, bool);

  // Given a vertex, returns a copy of the subtree with that vertex as 
  // root
  //
  Vertex * copy_subtree (Vertex *);

  // Given a value, returns the leaf of the OBDD with this value. If
  // there is no node such node (empty or full OBDD), then returns NULL
  //
  Vertex * get_leaf (bool);

  // Updates a subset value
  //
  void change_subset_value (ElementSubset *, bool);

  // Replaces a subtree of the OBDD with root U by a subtree with 
  // root V.
  //
  void replace_subtree (Vertex *, Vertex *);

  // Defines the structure used to save vertice on the reducing procedure
  //
  typedef struct MyVerticeEntry
  {
    int lo_id;
    int hi_id;
    Vertex * v;
    bool operator < (const MyVerticeEntry& y);
  } VerticeEntry;


public:

  // Builds a OBDD representing the function "0"
  //
  OBDD (ElementSet *);

  // Creates a OBDD represented by the tree with root root.
  //
  OBDD (ElementSet * set, Vertex * root, int card);

  // Builds a OBDD representing the function "0" using the ordering in 
  // unsigned int *
  //
  OBDD (ElementSet *, unsigned int *);

  // Builds an OBDD where the only path valued "1", is the path representing
  // the subset
  //
  OBDD (ElementSet *, ElementSubset *);

  // Default destructor.
  //
  virtual ~OBDD ();  

  // Returns the root vertex
  //
  Vertex * get_root ();

  // Changes OBDD root stacking a new root on top of the old root
  //
  void stack_root (Vertex *);

  // Prints the entire OBDD
  //
  void print ();

  // Makes the union of this OBDD and the one passed by argument
  //
  void union_to (Vertex *);

  // Makes the union of the subtrees passed by argument
  //
  Vertex * union_step (Vertex * v1, Vertex * v2, 
      map<pair<Vertex *, Vertex *>, Vertex *> *, unsigned int *, Vertex *, Vertex *);

  // Adds to the OBDD all the subsets covered (if bool false) or that covers (if bool
  // true) by ElementSubset *
  //
  void add_interval (ElementSubset *, bool);

  // Adds to the OBDD a subset. Does nothing if subset already in the
  // OBDD
  //
  void add_subset (ElementSubset *);

  // Removes a subset from the OBDD. Does nothing if subset not in the
  // OBDD.
  //
  void remove_subset (ElementSubset *);

  // Returns true if the ElementSubset * path of the obdds leads to a 1
  //
  bool contains (ElementSubset *);

  // Returns a random subset that is evaluated by zero.
  //
  ElementSubset * get_random_zero_evaluated_element ();

  // Returns the ElementSet used to build the OBDD
  //
  ElementSet * get_element_set ();

  // Returns the number of vertice of the OBDD
  //
  unsigned int get_cardinality ();

  // Returns true if OBDD is full
  //
  bool is_full ();

  // Inserts a vertex V as a child of a OBDD vertex U. This method will
  // replace the current child W of U by V. V will have as child W and a 
  // copy of the subtree started in W. This method may make the OBDD 
  // not reduced
  //
  void insert_vertex (Vertex *, Vertex *, bool);

  // Simplifies a node. This method receives a node with both children 
  // having value 1 and updates itself to be the value one.
  //
  void simplify (Vertex *);


  // Replaces a node by a terminal value of 1
  //
  void restrict_subtree (Vertex *);
  
};

#endif /* OBDD_H_ */
