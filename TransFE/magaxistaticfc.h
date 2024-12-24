/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
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

using namespace std;

class MagAxiStaticFC : public ForceContributor {
public:

	MagAxiStaticFC(MeshFace* Element_in, shared_ptr<Mapping> Map_in, shared_ptr<ShapeFunction> SF_in, int form) : ForceContributor(Element_in, Map_in, SF_in) {
		nnd = 1;
		formulation = form;
	};

	MagAxiStaticFC(MeshEdge* Element_in, shared_ptr<Mapping> Map_in, shared_ptr<ShapeFunction> SF_in, int form) : ForceContributor(Element_in, Map_in, SF_in) {
		nnd = 1;
		formulation = form;
	};

	//virtual ~MagAxiStaticFC(){};

	virtual Vector<double> evaluatePt(point);

private:
	int formulation;

};

