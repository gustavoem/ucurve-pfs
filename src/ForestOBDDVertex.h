#ifndef FOREST_OBDD_VERTEX_H_
#define FOREST_OBDD_VERTEX_H_

#include "global.h"
#include "PFSNode.h"
#include "Vertex.h"
#include <list>

class ForestOBDDVertex : public Vertex
{

protected:

  // Stores a  PFS node
  //
  PFSNode * node;
 
public:

  // Default Constructor
  //
  ForestOBDDVertex (PFSNode *, unsigned int);

  // Default Destructor
  //
  virtual ~ForestOBDDVertex ();


  // Returns the node
  //
  PFSNode * get_node ();


  // Sets the node
  //
  void set_node (PFSNode *);

};

#endif /* FOREST_OBDD_VERTEX_H_ */  