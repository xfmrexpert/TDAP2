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
#include "MeshDB/meshentity.h"
#include "typedefs.h"
#include "GeometryLagrangeShapeFunction.h"

class ElementTransform {

public:
    ElementTransform(size_t nsd, size_t npd, int order = 1)
        : shape_function(std::make_unique<GeometryLagrangeShapeFunction>(nsd, order)), numSpatialDims(nsd), numParametricDims(npd)
    {
    };

	virtual ~ElementTransform() = default;

    size_t referenceDimensions() const {
        return numParametricDims;
    };

    size_t spatialDimensions() const {
        return numSpatialDims;
    };

    virtual Matrix<double> Jacobian(const point& pt_ref, const MeshEntity& entity) const {
        // dG_ds: Gradients of shape function used for geometry transformation
         // [dX_dxi, dX_deta]
         // [dY_dxi, dY_deta]
        Matrix<double> dG_ds = shape_function->grad_N(pt_ref);

        // Initialize a nsd x npd Jacobian matrix with zeros
        Matrix<double> dXds = Matrix<double>::Zero(numSpatialDims, numParametricDims);

        auto nodes = entity.getNodes(); // Assumes nodes are ordered
        size_t nen = nodes.size(); // Number of element nodes

        for (size_t i = 0; i < nen; i++) { // Loop over shape functions (one per node)
            for (int j = 0; j < numSpatialDims; j++) { // Loop over global dimension (x, y)
                for (int k = 0; k < numParametricDims; k++) { // Loop over local (shape) dimension (xi, eta)
                    dXds(j, k) += dG_ds(i, k) * nodes[i]->pt().X(j);
                }
            }
        }

        return dXds;

    };

    virtual point mapReferencePointToPhysical(const point& ptRef, const MeshEntity& entity) const
    {
        point pt;
        const auto& N_ref = shape_function->N(ptRef);
        auto nodes = entity.getNodes();
        for (size_t i = 0; i < nodes.size(); i++) {
            pt += N_ref(i) * nodes[i]->pt();
        }
        return pt;
    };

protected:
    std::unique_ptr<GeometryLagrangeShapeFunction> shape_function;
    size_t numSpatialDims;    // Number of spatial dimensions (e.g., 2 for 2D)
    size_t numParametricDims; // Number of parametric dimensions (e.g., 2 for quadrilateral)
};

