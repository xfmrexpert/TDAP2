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

template<typename T>
class MagBilinearIntegrator : public BilinearFormIntegrator<double>
{
public:
	using BilinearFormIntegrator<double>::BilinearFormIntegrator;

    Matrix<double> evaluatePt(point ptRef, const FiniteElement& fe, const MeshEntity& entity, const ElementQuadratureData& quadData) const override
    {
        int nDofs = fe.numLocalDOFs();

        Matrix<double> Ke = Matrix<double>(nDofs, nDofs);

        const double mu_r = entity.getClassification()->getAttribute("mu");
        const double mu = 4.0 * PI * 1.0e-7 * mu_r;

        // 1. Evaluate shape function derivatives in reference coords
            //    => returns an (nDofs x nRefDims) matrix
        Matrix<double> dPhiRef = fe.grad_N(ptRef);

        // At each quadrature point, we need dPhiPhys and detJ
        Matrix<double> dPhiPhys = quadData.dPhiPhys;
        double detJ = quadData.detJ;

        // for i,j
        for (int i = 0; i < nDofs; i++)
        {
            for (int j = 0; j < nDofs; j++)
            {
                double dotVal = dPhiPhys.GetRow(i).dot(dPhiPhys.GetRow(j));
                Ke(i, j) += (1 / mu) * dotVal * detJ;
            }
        }
        return Ke;
    };

private:

};