/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef ELASTICITYFC_H
#define ELASTICITYFC_H

#include "forcecontributor.h"
#include "MeshDB/meshface.h"
#include "MeshDB/meshedge.h"
#include "mapping.h"
#include "shapefunction.h"
#include "typedefs.h"
#include "dof.h"
#include <vector>

using namespace std;

class ElasticityFC : public ForceContributor{
public:
    ElasticityFC(MeshFace* Element_in, shared_ptr<Mapping> Map_in, shared_ptr<ShapeFunction> SF_in) : ForceContributor(Element_in, Map_in, SF_in) {
      nnd = 2;
    };
   
    ElasticityFC(MeshEdge* Element_in, shared_ptr<Mapping> Map_in, shared_ptr<ShapeFunction> SF_in) : ForceContributor(Element_in, Map_in, SF_in) {
      nnd = 2;
    };
    
    virtual ~ElasticityFC() = default;
    
    virtual Vector<double> evaluatePt(point);
    
private:

};

#endif
