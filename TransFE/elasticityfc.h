/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include "forcecontributor.h"
#include "MeshDB/meshface.h"
#include "MeshDB/meshedge.h"
#include "mapping.h"
#include "shapefunction.h"
#include "typedefs.h"
#include "dof.h"
#include <vector>

class ElasticityFC : public ForceContributor<double>{
public:
    ElasticityFC(MeshFace* Element_in, Field<double>* Field_in, std::shared_ptr<Mapping> Map_in, std::shared_ptr<ShapeFunction> SF_in) : ForceContributor<double>(Element_in, Field_in, Map_in, SF_in) {
      nnd = 2;
    };
   
    ElasticityFC(MeshEdge* Element_in, Field<double>* Field_in, std::shared_ptr<Mapping> Map_in, std::shared_ptr<ShapeFunction> SF_in) : ForceContributor<double>(Element_in, Field_in, Map_in, SF_in) {
      nnd = 2;
    };
    
    virtual ~ElasticityFC() = default;
    
    virtual Vector<double> evaluatePt(point);
    
private:

};
