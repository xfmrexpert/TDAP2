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

class MagBilinearIntegrator : public BilinearFormIntegrator<double>
{
public:
	using BilinearFormIntegrator<double>::BilinearFormIntegrator;

    Matrix<double> evaluatePt(point ptRef, const FiniteElementBase& fe, MeshEntity& entity, const ElementQuadratureData& quadData) const override
    {
        int nDofs = fe.numLocalDOFs();

        Matrix<double> Ke = Matrix<double>::Zero(nDofs, nDofs);

        const double mu_r = entity.getClassification()->getAttribute("mu");
        const double mu = 4.0 * PI * 1.0e-7 * mu_r;

        // Evaluate shape function derivatives in reference coords
        //    => returns an (nDofs x nRefDims) matrix
        // Attempt to cast to a scalar shape function 
        auto scalar_sf = dynamic_cast<const ScalarShapeFunction*>(fe.ShapeFunction());
        if (!scalar_sf) {
            throw std::runtime_error("MagBilinearIntegrator: The provided FE is not scalar (H1).");
        }

        Matrix<double> dPhiRef = scalar_sf->grad_N(ptRef);

        // At each quadrature point, we need dPhiPhys and detJ
        Matrix<double> dPhiPhys = quadData.dN_dx;
        double detJ = quadData.detJ;

        // for i,j
        for (int i = 0; i < nDofs; i++)
        {
            for (int j = 0; j < nDofs; j++)
            {
                double dotVal = dPhiPhys.row(i).dot(dPhiPhys.row(j));
                Ke(i, j) += (1 / mu) * dotVal * detJ;
            }
        }
        //std::cout << "Ke: " << std::endl;
        //std::cout << Ke << std::endl;
        return Ke;
    };

private:

};