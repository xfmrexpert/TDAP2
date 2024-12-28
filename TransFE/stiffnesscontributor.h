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

#include <vector>
#include "MeshDB/meshentity.h"
#include "mapping.h"
#include "shapefunction.h"
#include "assembler.h"
#include "typedefs.h" //Matrix, Vector, etc.
#include "MeshDB/point.h"

class StiffnessContributor {
public:
	StiffnessContributor(MeshEntity* Element_in, std::shared_ptr<Mapping> Map_in, std::shared_ptr<ShapeFunction> SF_in) {
		Element = Element_in;
		Map = Map_in;
		SF = SF_in;
		nen = Element->getNodes().size();
	};

	virtual ~StiffnessContributor() = default;

	void evaluate(Assembler*);

	virtual Matrix<double> evaluatePt(point) = 0;

	std::vector<DOF*> getDOFs();

protected:
	MeshEntity* Element;
	std::shared_ptr<Mapping> Map;
	std::shared_ptr<ShapeFunction> SF;
	Matrix<double> dsdx; //Overwritten on each call to evaluate
	Matrix<double> dNdx; //Overwritten on each call to evaluate
	size_t nnd; // # of DOFs/node - must be set by derived class
	size_t nen; // # of nodes/element

};
