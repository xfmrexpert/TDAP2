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
#include "field.h"
#include "mapping.h"
#include "shapefunction.h"
#include "assembler.h"
#include "typedefs.h" //Matrix, Vector, etc.
#include "MeshDB/point.h"
#include <iostream>

template <typename T>
class StiffnessContributor {
public:
	StiffnessContributor(MeshEntity* Element_in, Field<T>* Field_in, std::shared_ptr<Mapping> Map_in, std::shared_ptr<ShapeFunction> SF_in) {
		Element = Element_in;
		field = Field_in;	
		Map = Map_in;
		SF = SF_in;
		nen = Element->getNodes().size();
	};

	virtual ~StiffnessContributor() = default;

	void evaluate(Assembler<T>* assem) {
		const auto& DOFs = getDOFs();
		Matrix<double> k(nnd * nen, nnd * nen);
		std::vector<point> IntPt = SF->IntPts(); // Integration points in reference coordinates
		Vector<double> Weight = SF->Weights(); // Integration weights
		int numIntPts = SF->numIntPts(); // Number of integration points

		//Perform integration over element
		//The majority of the time here is spent in evaluatePt
		for (int i = 0; i < numIntPts; i++) {
			point pt = IntPt[i];
			double weight = Weight[i];

			Matrix<double> dNds = SF->dNds(pt); // Gradient of shape functions in reference coordinates
			Matrix<double> dXds = Map->dXds(pt); // Gradient of mapping
			dsdx = dXds.CalculateInverse();
			dNdx = dNds * dsdx; // This is used in evaluatePt called below.

			//This is a matrix equation
			k = k + evaluatePt(pt) * dXds.CalculateDeterminant() * weight; //Map->detJacobian(pt) * weight;
		}

		//std::cout << "Adding stiffness contributor:\n";
		//std::cout << k;
		assem->accept(k, DOFs);
	};

	virtual Matrix<double> evaluatePt(point) = 0;

protected:
	MeshEntity* Element;
	Field<T>* field;
	std::shared_ptr<Mapping> Map;
	std::shared_ptr<ShapeFunction> SF;
	Matrix<double> dsdx; //Overwritten on each call to evaluate
	Matrix<double> dNdx; //Overwritten on each call to evaluate
	size_t nnd; // # of DOFs/node - must be set by derived class
	size_t nen; // # of nodes/element

	std::vector<DOF<T>*> getDOFs() {
		return field->getDOFsForEntity(*Element);
	}

};
