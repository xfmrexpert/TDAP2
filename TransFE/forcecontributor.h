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

#include "MeshDB/meshentity.h"
#include "mapping.h"
#include "shapefunction.h"
#include "assembler.h"
#include "field.h"

template <class T>
class ForceContributor {
public:
	ForceContributor(MeshEntity* Element_in, Field<T>* Field_in, std::shared_ptr<Mapping> Map_in, std::shared_ptr<ShapeFunction> SF_in) {
		Element = Element_in;
		field = Field_in;
		Map = Map_in;
		SF = SF_in;

		nen = Element->getNodes().size();
	};

	virtual ~ForceContributor();

	void evaluate(Assembler<T>*);

	virtual Vector<double> evaluatePt(point) = 0;

	std::vector<DOF<T>*> getDOFs();

protected:
	MeshEntity* Element;
	Field<T>* field;
	std::shared_ptr<Mapping> Map;
	std::shared_ptr<ShapeFunction> SF;

	int nnd; // # of DOFs/node - must be set by derived class
	size_t nen; // # of nodes/element

};
