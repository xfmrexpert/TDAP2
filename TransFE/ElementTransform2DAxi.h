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

#include "ElementTransform.h"
#include "MeshDB/point.h"
#include "MeshDB/meshface.h"

class ElementTransform2DAxi : public ElementTransform {
public:
	ElementTransform2DAxi() : ElementTransform(2, 2) { };

	Matrix<double> Jacobian(const point& pt, const MeshEntity& entity, const Matrix<double>& dGds) const override {
		// dXds:
		// [dXdr, dXds]
		// [dYdr, dYds]

		// Initialize a 2x2 Jacobian matrix with zeros
		Matrix<double> dXds(nsd, npd);

		auto nodes = entity.getNodes(); // Assumes nodes are ordered
		size_t nen = nodes.size(); // Number of element nodes

		for (size_t i = 0; i < nen; ++i) { // Loop over shape functions (one per node)
			auto node = nodes[i];
			double r_i = node->pt().X(0); // X(0) = r-coordinate
			double z_i = node->pt().X(1); // X(1) = z-coordinate

			for (uint8_t k = 0; k < npd; ++k) { // Loop over local (xi, eta) dimensions
				dXds(0, k) += dGds(i, k) * r_i; // dr/dxi, dr/deta
				dXds(1, k) += dGds(i, k) * z_i; // dz/dxi, dz/deta
			}
		}

		return dXds;

	};

};
