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
#include "LinearFormIntegrator.h"

class MagLinearIntegrator : LinearFormIntegrator
{
public:
	using LinearFormIntegrator::LinearFormIntegrator;
    ~MagLinearIntegrator() = default;

    Vector<double> evaluatePt(point ptRef, const FiniteElement& fe, const MeshEntity& entity, const ElementQuadratureData& quadData) const override
    {
        int nDofs = fe.numLocalDOFs();
        Vector<double> f = Vector<double>(nDofs);

        double J = entity.getClassification()->getAttribute("J"); //Constant current density in element

        double measure = quadData.detJ;

        const auto& phi = fe.N(ptRef); // shape function values

        for (int i = 0; i < nDofs; i++)
        {
            f(i) += J * phi(i) * measure;
        }
        return f;
    };

private:

};
