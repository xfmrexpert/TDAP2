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
#include "assembler.h"
//#include "field.h"

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

	virtual ~ForceContributor() = default;

	void evaluate(Assembler<T>* assem) {
		std::vector<DOF<T>*> DOFs = this->getDOFs();
		Vector<double> f(DOFs.size());  //This is a vector of DOFs for either the face or the edge
		std::vector<point> IntPt = SF->IntPts();
		Vector<double> Weight = SF->Weights();
		int numIntPts = SF->numIntPts();

		for (int i = 0; i < numIntPts; i++) {
			point pt = IntPt[i];
			double weight = Weight[i];
			f = f + evaluatePt(pt) * weight;
		}
		//cout << "Adding force contributor:\n";
		//cout << f;
		assem->accept(f, DOFs);
	};

	virtual Vector<double> evaluatePt(point) = 0;

	std::vector<DOF<T>*> getDOFs() {
		return field->getDOFsForEntity(*Element);
	}

protected:
	MeshEntity* Element;
	Field<T>* field;
	std::shared_ptr<Mapping> Map;
	std::shared_ptr<ShapeFunction> SF;

	int nnd; // # of DOFs/node - must be set by derived class
	size_t nen; // # of nodes/element

};
