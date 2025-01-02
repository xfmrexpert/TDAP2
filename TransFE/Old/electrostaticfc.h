/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef ELECTROSTATICFC_H
#define ELECTROSTATICFC_H

#include "forcecontributor.h"
#include "MeshDB/meshface.h"
#include "MeshDB/meshedge.h"
#include "mapping.h"
#include "shapefunction.h"
#include "typedefs.h"
#include "dof.h"
#include <vector>

using namespace std;

class ElectroStaticFC : public ForceContributor{
public:

    ElectroStaticFC(MeshFace* Element_in, Mapping* Map_in, ShapeFunction* SF_in) : ForceContributor(Element_in, Map_in, SF_in) {
      nnd = 1;
    };
   
    ElectroStaticFC(MeshEdge* Element_in, Mapping* Map_in, ShapeFunction* SF_in) : ForceContributor(Element_in, Map_in, SF_in) {
      nnd = 1;
    };
    
    virtual Vector<double> evaluatePt(point);

};

#endif
