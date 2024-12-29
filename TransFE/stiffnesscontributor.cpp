/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "stiffnesscontributor.h"
#include <iostream>

template <class T>
void StiffnessContributor<T>::evaluate(Assembler<T>* assem) {
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

	//cout << "Adding stiffness contributor:\n";
	//cout << k;
	assem->accept(k, DOFs);
};

template <class T>
std::vector<DOF<T>*> StiffnessContributor<T>::getDOFs() {
	return field->getDOFsForEntity(*Element);
}


