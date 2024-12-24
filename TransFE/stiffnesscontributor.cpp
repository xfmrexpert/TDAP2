/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#include "stiffnesscontributor.h"
 //#include "precisiontimer.h"
#include <iostream>

StiffnessContributor::~StiffnessContributor() {

}

void StiffnessContributor::evaluate(Assembler* assem) {
	const auto& DOFs = getDOFs();  //70us
	Matrix<double> k(DOFs.size(), DOFs.size()); //10-20us
	vector<point> IntPt = SF->IntPts();
	Vector<double> Weight = SF->Weights();
	int numIntPts = SF->numIntPts();

	//Perform integration over element
	//The majority of the time here is spent in evaluatePt
	for (int i = 0; i < numIntPts; i++) {
		point pt = IntPt[i];
		double weight = Weight[i];

		Matrix<double> dNds = SF->dNds(pt);
		Matrix<double> dXds = Map->dXds(pt);
		dsdx = dXds.CalculateInverse();
		dNdx = dNds * dsdx;

		//approx. 830us
	    //This is a matrix equation
		k = k + evaluatePt(pt) * dXds.CalculateDeterminant() * weight; //Map->detJacobian(pt) * weight;
	}

	//cout << "Adding stiffness contributor:\n";
	//cout << k;
	assem->accept(k, DOFs);
};

vector<DOF*> StiffnessContributor::getDOFs() {
	vector<DOF*> DOFs;

	for (const auto& node : Element->getNodes()) {
		const auto& node_DOFs = node->getDOFs();
		for (const auto& dof : node_DOFs) {
			DOFs.push_back(dof);
		}
	}

	return DOFs;
}


