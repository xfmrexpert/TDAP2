/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "forcecontributor.h"
#include <iostream>

template <typename T>
void ForceContributor<T>::evaluate(Assembler<T>* assem) {
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

template <typename T>
std::vector<DOF<T>*> ForceContributor<T>::getDOFs() {
	return field->getDOFsForEntity(*Element);
}
