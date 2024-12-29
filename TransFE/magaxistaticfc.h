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

class MagAxiStaticFC : public ForceContributor<double> {
public:

	MagAxiStaticFC(MeshFace* Element_in, Field<double>* Field_in, std::shared_ptr<Mapping> Map_in, std::shared_ptr<ShapeFunction> SF_in, int form) : ForceContributor<double>(Element_in, Field_in, Map_in, SF_in) {
		nnd = 1;
		formulation = form;
	};

	MagAxiStaticFC(MeshEdge* Element_in, Field<double>* Field_in, std::shared_ptr<Mapping> Map_in, std::shared_ptr<ShapeFunction> SF_in, int form) : ForceContributor<double>(Element_in, Field_in, Map_in, SF_in) {
		nnd = 1;
		formulation = form;
	};

	virtual Vector<double> evaluatePt(point);

private:
	int formulation;

};

