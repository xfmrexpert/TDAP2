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

class ElementTransform {

public:
	ElementTransform(uint8_t nsd, uint8_t npd) : nsd(nsd), npd(npd) { };
	virtual ~ElementTransform() = default;

    Matrix<double> Jacobian(const point& pt, const MeshEntity& entity, const Matrix<double>& dGds) const {
        // dXds:
        // [dX_dxi, dX_deta]
        // [dY_dxi, dY_deta]

        // Initialize a 2x2 Jacobian matrix with zeros
        Matrix<double> dXds = Matrix<double>::Zero(nsd, npd);

        auto nodes = entity.getNodes(); // Assumes nodes are ordered
        size_t nen = nodes.size(); // Number of element nodes

        for (size_t i = 0; i < nen; i++) { // Loop over shape functions (one per node)
            for (int j = 0; j < nsd; j++) { // Loop over global dimension (x, y)
                for (int k = 0; k < npd; k++) { // Loop over local (shape) dimension (xi, eta)
                    dXds(j, k) += dGds(i, k) * nodes[i]->pt().X(j);
                }
            }
        }

        return dXds;

    };

    /// Transform shape-function derivatives from reference to physical coords.
    /// dPhiRef: (nDofs x refDim)
    /// invJ:    (refDim x refDim)
    /// returns: (nDofs x refDim)
    Matrix<double> transformReferenceToPhysical(const Matrix<double>& dN_ds, const Matrix<double>& invJ) const
    {
        size_t nDofs = dN_ds.rows();     // e.g. number of shape functions
        size_t refDim = dN_ds.cols();     // 2 in 2D, or 3 in 3D

        // We'll create an output matrix of the same shape
        Matrix<double> dN_dx = Matrix<double>::Zero(nDofs, refDim);

        // Multiply each row of dN_ds by invJ
        // (row-vector) x (refDim x refDim) => (row-vector)
        for (size_t i = 0; i < nDofs; i++)
        {
            for (size_t c = 0; c < refDim; c++)
            {
                // compute dN_dx(i, c) by dot product of dN_ds row i with invJ column c
                double sum = 0.0;
                for (size_t k = 0; k < refDim; k++)
                {
                    sum += dN_ds(i, k) * invJ(k, c);
                }
                dN_dx(i, c) = sum;
            }
        }

        return dN_dx;
    }

    point transformReferenceToPhysical(const point& ptRef, const MeshEntity& entity, const Vector<double>& N) const
    {
        point pt;
        auto nodes = entity.getNodes();
        for (size_t i = 0; i < nodes.size(); i++) {
            pt += N(i) * nodes[i]->pt();
        }
        return pt;
    }

protected:

	uint8_t nsd; //# of spatial dimensions
	uint8_t npd; //# of parametric dimensions
};

class ElementTransform2D : public ElementTransform {
public:
    ElementTransform2D() : ElementTransform(2, 2) {};

};