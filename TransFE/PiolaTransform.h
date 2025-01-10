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
#include "typedefs.h"
#include "MeshDB/point.h"
#include "ElementTransform.h"

class PiolaTransformHCurl {
public:
    // Takes a reference to an ElementTransform (the geometry-based mapping)
    PiolaTransformHCurl(const ElementTransform& geomMapping)
        : geometryMapping(geomMapping)
    {
    }

    // Covariant Piola transform: v_phys = (J^-1)^T * v_ref
    Matrix<double> mapHcurlVectorToPhysical(
        const point& pt_ref,
        const MeshEntity& entity,
        const Matrix<double>& refVectors
    ) const
    {
        Matrix<double> J = geometryMapping.Jacobian(pt_ref, entity);
        Matrix<double> invJ = J.inverse();
        Matrix<double> invJ_T = invJ.transpose();

        // Suppose refVectors is [n x refDim], invJ_T is [spaceDim x refDim].
        // We want outVectors = refVectors * invJ_T^T if carefully matching dimensions. 
        // But typically it's: v_phys = (J^-T) * v_ref. 
        // So each row in refVectors is multiplied on the left by invJ_T^T
        // or on the right by invJ_T, depending on how you store shapes.

        // Let's assume a row-based approach: outVectors[i, :] = refVectors[i, :] * invJ_T.
        // So outVectors is [n x spaceDim].
        size_t n = refVectors.rows();
        size_t sd = geometryMapping.spatialDimensions(); // e.g. 2 or 3
        Matrix<double> outVectors(n, sd);

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < sd; j++) {
                double sum = 0.0;
                for (size_t k = 0; k < geometryMapping.referenceDimensions(); k++) {
                    sum += refVectors(i, k) * invJ_T(k, j);
                }
                outVectors(i, j) = sum;
            }
        }
        return outVectors;
    }

    // Curl transform in 2D: curl_phys = (1/detJ) * curl_ref
    // TODO: Expand to arbitrary dimensions
    Matrix<double> mapHcurlCurlToPhysical(
        const point& pt_ref,
        const MeshEntity& entity,
        const Matrix<double>& refCurls // e.g. [numBasis x 1] in 2D, [numBasis x 3] in 3D
    ) const
    {
        // 1) Compute J, detJ
        Matrix<double> J = geometryMapping.Jacobian(pt_ref, entity);
        double detJ = J.determinant();

        // If in 2D, the curl is scalar => multiply by 1/det(J).
        // If 3D, there's a different formula: (1/detJ)*J*(curl_ref).
        size_t n = refCurls.rows();
        size_t c = refCurls.cols();
        Matrix<double> outCurls(n, c);

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < c; j++) {
                outCurls(i, j) = refCurls(i, j) / detJ;
            }
        }
        return outCurls;
    }

private:
    const ElementTransform& geometryMapping;
};
