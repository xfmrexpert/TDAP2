/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#include "forcecontributor.h"
#include <iostream>

ForceContributor::~ForceContributor() {
	
};

void ForceContributor::evaluate(Assembler* assem) {
	vector<DOF*> DOFs = this->getDOFs();
	Vector<double> f(DOFs.size());  //This is a vector of DOFs for either the face or the edge
	vector<point> IntPt = SF->IntPts();
	Vector<double> Weight = SF->Weights();
	int numIntPts = SF->numIntPts();

	for (int i = 0; i < numIntPts; i++) {
		point pt = IntPt[i];
		double weight = Weight[i];
		f = f + evaluatePt(pt) * weight;
	}
	cout << "Adding force contributor:\n";
	cout << f;
	assem->accept(f, DOFs);
};

vector<DOF*> ForceContributor::getDOFs() {
	vector<DOF*> DOFs;

	for (const auto& node : Element->getNodes()) {
		const auto& node_DOFs = node->getDOFs();
		for (const auto& dof : node_DOFs) {
			DOFs.push_back(dof);
		}
	}

	return DOFs;
}
