//
// ExpandableOBDD.h -- definition of the namespace "ExpandableOBDD".
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

#ifndef EXPANDABLEOBDD_H_
#define EXPANDABLEOBDD_H_

#include "global.h"
#include "ElementSet.h"
#include "Vertex.h"
#include "ElementSubset.h"
#include "CostFunction.h"

class ExpandableOBDD : public ROBDD
{
  protected:

    // Stores the current node of the OBDD expansion
    //
    Vertex * current_node;

    // Stores the last returned subset on expansion
    //
    ElementSubset * current_subset;

  public:

    // Default constructor
    //
    ExpandableOBDD (ElementSet *);


    // Default destructor
    //
    virtual ~ExpandableOBDD ();


    // Expandes the OBDD and returns the next unrestricted
    // element subset
    //
    ElementSubset * next_subset ();

};

#endif /* EXPANDABLEOBDD_H_ */