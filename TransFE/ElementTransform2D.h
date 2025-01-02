/***************************************************************************
 *   Copyright (C) 2005-2025 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   Use of this source code is governed by an MIT-style                   *
 *   license that can be found in the LICENSE.txt file or at               *
 *   https://opensource.org/licenses/MIT.                                  *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include "MeshDB/point.h"
#include "MeshDB/meshface.h"
#include "ElementTransform.h"

class ElementTransform2D : public ElementTransform {
public:
	ElementTransform2D() : ElementTransform() {
		nsd = 2;
		npd = 2;
	};

	~ElementTransform2D() {};

	Matrix<double> Jacobian(const point& pt, const MeshEntity& entity, const Matrix<double>& dGds) const override {
		// dXds:
		// [dXdr, dXds]
		// [dYdr, dYds]

		Matrix<double> dXds(nsd, npd);

		auto nodes = entity.getNodes(); //assumes nodes are ordered
		size_t nen = nodes.size();

		for (size_t i = 0; i < nen; i++) { //loop over shape functions (one per node)
			for (int j = 0; j < nsd; j++) { //loop over global dimension (X)
				for (int k = 0; k < npd; k++) { //loop over local (shape) dimension (R)
					dXds(j, k) += dGds(i, k) * nodes[i]->pt().X(j);
				}
			}
		}

		return dXds;

	};


};

