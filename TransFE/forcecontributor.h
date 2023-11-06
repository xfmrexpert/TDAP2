/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef FORCECONTRIBUTOR_H
#define FORCECONTRIBUTOR_H

#include "MeshDB/meshentity.h"
#include "mapping.h"
#include "shapefunction.h"
#include "assembler.h"

class ForceContributor {
public:
	ForceContributor(shared_ptr<MeshEntity> Element_in, shared_ptr<Mapping> Map_in, shared_ptr<ShapeFunction> SF_in) {
		Element = Element_in;
		Map = Map_in;
		SF = SF_in;

		nen = Element->getNodes().size();
	};

	virtual ~ForceContributor();

	void evaluate(Assembler*);

	virtual Vector<double> evaluatePt(point) = 0;

	vector<shared_ptr<DOF>> getDOFs();

protected:
	shared_ptr<MeshEntity> Element;
	shared_ptr<Mapping> Map;
	shared_ptr<ShapeFunction> SF;

	int nnd; // # of DOFs/node - must be set by derived class
	size_t nen; // # of nodes/element

};

#endif
