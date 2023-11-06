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
	vector<shared_ptr<DOF>> DOFs = this->getDOFs();
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

vector<shared_ptr<DOF>> ForceContributor::getDOFs() {
	auto nodes = Element->getNodes();

	vector<shared_ptr<DOF>> DOFs;

	for (auto node_iter = nodes.begin(); node_iter != nodes.end(); ++node_iter) {
		auto node_DOFs = (*node_iter)->getDOFs();
		for (auto DOF_iter = node_DOFs.begin(); DOF_iter != node_DOFs.end(); ++DOF_iter) {
			DOFs.push_back((*DOF_iter));
		}
	}

	return DOFs;
}
