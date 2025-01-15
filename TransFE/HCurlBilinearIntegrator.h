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
#include "FESpace.h"
#include "ElementTransform.h"
#include "IntegrationRule.h"
#include "Assembler.h"
#include "BilinearFormIntegrator.h"

class HCurlBilinearIntegrator : public BilinearFormIntegrator<double>
{
public:
    using BilinearFormIntegrator<double>::BilinearFormIntegrator;

    Matrix<double> evaluatePt(point ptRef, const FiniteElementBase& fe, MeshEntity& entity, const ElementQuadratureData& quadData) const override
    {
        // 1) The # of local DOFs for a Nedelec element (vector field).
        size_t nDofs = fe.numLocalDOFs();

        // 2) Build an nDofs x nDofs local stiffness contribution.
        Matrix<double> Ke = Matrix<double>::Zero(nDofs, nDofs);

        // 3) Material property "mu" from the geometry classification
        double mu_r = entity.getClassification()->getAttribute("mu");
        double mu = 4.0 * PI * 1.0e-7 * mu_r; // typical formula

        // 4) Attempt to cast to a vector shape function 
        //    If this is not a vector shape function, throw or handle error.
        auto vectorSF = dynamic_cast<const VectorShapeFunction*>(fe.ShapeFunction());
        if (!vectorSF) {
            throw std::runtime_error("MagBilinearIntegrator: PDE expects an H(curl) vector element!");
        }

        // 5) Evaluate the *reference* curl of all shape functions at ptRef
        //    In 2D, curl_N(...) might return a [nDofs x 1] matrix (scalar out-of-plane).
        //    In 3D, it would be [nDofs x 3].
        Matrix<double> refCurls = vectorSF->curl_N(ptRef);

        // 6) The geometry transform data at this quadrature point is in quadData.
        //    Typically: quadData.detJ, quadData.invJ, etc. 
        //    For H(curl), the physical curl is often:
        //      curl_phys = (1 / det(J)) * refCurl  (in 2D)
        //    or in 3D: curl_phys = (1 / detJ) * J * (curl_ref).
        //    We'll do a minimal approach for 2D:

        // We might do it ourselves or call a Piola transform helper:
        double detJ = quadData.detJ;
        Matrix<double> physCurls = Matrix<double>::Zero(nDofs, 1);
        for (int i = 0; i < nDofs; i++) {
            // For 2D, refCurls(i,0) is the scalar curl in reference coords
            // => physical = (1/detJ) * refCurls(i,0)
            physCurls(i, 0) = (1.0 / detJ) * refCurls(i, 0);
        }

        // 7) Now compute the bilinear form:
        //    \int 1/mu * curl(N_i) dot curl(N_j) * dOmega
        //    => dot of 2D scalars is just multiplication
        //    => we scale by detJ for the actual integration measure
        for (int i = 0; i < nDofs; i++)
        {
            for (int j = 0; j < nDofs; j++)
            {
                double val = physCurls(i, 0) * physCurls(j, 0);
                Ke(i, j) += (1.0 / mu) * val * detJ;
            }
        }

        return Ke;
    }

private:

};