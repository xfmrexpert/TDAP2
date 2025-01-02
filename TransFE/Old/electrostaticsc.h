/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef ELECTROSTATICSC_H
#define ELECTROSTATICSC_H

#include "stiffnesscontributor.h"
#include "MeshDB/meshface.h"
#include "mapping.h"
#include "shapefunction.h"
#include "MeshDB/point.h"
#include <vector>
#include "dof.h"
#include "typedefs.h"

class ElectroStaticSC : public StiffnessContributor{
public:
	ElectroStaticSC(MeshFace* Element, Mapping* Map, ShapeFunction* SF);
    virtual Matrix<double> evaluatePt(point);
};

#endif
