#ifndef VERTEX_H_
#define VERTEX_H_

#include "global.h"
#include "Element.h"
#include "PFSNode.h"
#include <list>

class Vertex 
{

protected:
  
  Vertex * lo;
  Vertex * hi;
  Element * var;
  bool value;
  unsigned int id;      // just an integer for identification
  unsigned int index;   // the index of *var at the ordering plus one
  PFSNode * node;       // stores a PFS node. Only valid for leafs
  unsigned int weight;  // used to store the number of subsets "below"
                        // this vertex
  list<Vertex *> parents;
  
public:

  bool mark;

  // Default Vertex constructor
  //
  Vertex ();

  // Constructor for nonterminal vertices
  //
  Vertex (Element *, unsigned int);

  // Copy Constructor
  //
  Vertex (const Vertex&);

  // Constructor for terminal vertices
  //
  Vertex (bool, unsigned int);

  // Default destructor.
  //
  virtual ~Vertex ();

  // Returns the element of a vertex
  //
  Element * get_var ();

  // Sets the element of a vertex
  //
  void set_var (Element *);

  // Returns the child. True for high (right) and False for low (left)
  //
  Vertex * get_child (bool);

  // Sets the child. True for high (right) and False for low (left)
  //
  void set_child (Vertex *, bool);

  // Returns the list of parents of this vertex
  //
  list<Vertex *> get_parents ();

  // Adds a Vertex to the list of Vertice
  //
  void add_parent (Vertex *);

  // Removes a vertex parent 
  //
  void remove_parent (Vertex *);

  // Returns the value of a vertex. If vertex is not terminal, returns
  // -1
  //
  int get_value (); 

  // Sets the value of a vertex
  //
  void set_value (int);
  
  // Returns the id of a vertex
  //
  unsigned int get_id ();

  // Sets the id of a vertex
  //
  void set_id (unsigned int);

  // Returns the index of a vertex
  //
  unsigned int get_index ();

  // Sets the index of a vertex
  //
  void set_index (unsigned int);  

  // Says if the vertex is terminal
  //
  bool is_terminal ();

  // Clears the list of parents
  //
  void clean_parents ();

  // Returns true if vertex has parent
  //
  bool has_parent ();

  // Returns the node associated to this vertex
  //
  PFSNode * get_node ();

  // Sets the node of this vertex
  //
  void set_node (PFSNode *);

  // Returns the weight associated to this vertex
  //
  unsigned int get_weight ();

  // Sets the weight of this vertex
  //
  void set_weight (unsigned int);
};

#endif /* VERTEX_H_ */