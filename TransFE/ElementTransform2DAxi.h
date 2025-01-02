/***************************************************************************
 *   Copyright (C) 2005-2023 by T. C. Raymond                              *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include "elementtransform.h"
#include "MeshDB/point.h"
#include "MeshDB/meshface.h"

class ElementTransform2DAxi : public ElementTransform {
public:
	ElementTransform2DAxi() : ElementTransform() {
		nsd = 2;
		npd = 2;
	};

	~ElementTransform2DAxi() {};

	Matrix<double> Jacobian(const point& pt, const MeshEntity& entity, const Matrix<double>& dGds) {
		// dXds:
		// [dXdr, dXds]
		// [dYdr, dYds]

		Matrix<double> dXds(nsd, npd);

		auto nodes = entity.getNodes(); //assumes nodes are ordered
		size_t nen = nodes.size();

		for (size_t i = 0; i < nen; ++i) { //loop over shape functions (one per node)
			auto node = nodes[i];
			for (uint8_t j = 0; j < nsd; ++j) { //loop over global dimension (X)
				for (uint8_t k = 0; k < npd; ++k) { //loop over local (shape) dimension (R)
					if (j == 0) {
						//coordinate transform on r-dimension
						dXds(j, k) += dGds(i, k) * node->pt().X(j) * node->pt().X(j);
					}
					else {
						dXds(j, k) += dGds(i, k) * node->pt().X(j);
					}
				}
			}
		}

		return dXds;

	};

};
