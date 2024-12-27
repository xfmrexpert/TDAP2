/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "mapping2daxi.h"

 //There has to be a better way to do this...
 //jacobianInverse calls dXds, detJacobian calls dXds and stiffnesscontributor::evaluate
 //calls detJacobian.  Therefore, dXds is called three times and detJacobian twice to
 //get the exact same answer.

Matrix<double> Mapping2DAxi::jacobianInverse(point pt) {
	Matrix<double> j_inv(npd, nsd);
	Matrix<double> mdXds = dXds(pt);
	double j = mdXds.CalculateDeterminant(); //detJacobian(pt);

	// j_inv = dSdX
	// [drdx, drdy]
	// [dsdx, dsdy]
	// -equals-
	// 1/j * [dyds, -dxds]
	//       [-dydr, dxdr]

	j_inv(0, 0) = mdXds(1, 1) / j;
	j_inv(0, 1) = -1 * mdXds(0, 1) / j;
	j_inv(1, 0) = -1 * mdXds(1, 0) / j;
	j_inv(1, 1) = mdXds(0, 0) / j;

	return j_inv;
};

Matrix<double> Mapping2DAxi::dXds(point pt) {
	// dXds:
	// [dXdr, dXds]
	// [dYdr, dYds]

	Matrix<double> dXds(nsd, npd);
	Matrix<double> dNds = SF->dNds(pt);
	auto nodes = Element->getNodes(); //assumes nodes are ordered

	for (size_t i = 0; i < nen; ++i) { //loop over shape functions (one per node)
		auto node = nodes[i];
		for (uint8_t j = 0; j < nsd; ++j) { //loop over global dimension (X)
			for (uint8_t k = 0; k < npd; ++k) { //loop over local (shape) dimension (R)
				if (j == 0) {
					//coordinate transform on r-dimension
					dXds(j, k) += dNds(i, k) * node->pt().X(j) * node->pt().X(j);
				}
				else {
					dXds(j, k) += dNds(i, k) * node->pt().X(j);
				}
			}
		}
	}

	return dXds;

};

//double Mapping2DAxi::detJacobian(point pt){
//   Matrix mdXds = dXds(pt);
//   //Determinant of 2x2 Jacobian matrix
//   double j = mdXds[0][0] * mdXds[1][1] - mdXds[0][1] * mdXds[1][0];
//   
//   return j;
//};


