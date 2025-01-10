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

class AxiMagLinearIntegrator : public LinearFormIntegrator
{
public:
    using LinearFormIntegrator::LinearFormIntegrator;
    ~AxiMagLinearIntegrator() = default;

    Vector<double> evaluatePt(point ptRef, const FiniteElementBase& fe, const MeshEntity& entity, const ElementQuadratureData& quadData) const override
    {
        int nDofs = fe.numLocalDOFs();
        Vector<double> f = Vector<double>::Zero(nDofs);
        double J = entity.getClassification()->getAttribute("J"); //Constant current density in element
        if (J == NO_ATTRIB) return f;

        double s_phys = quadData.ptPhys.x;
        double detJ = quadData.detJ;

        // Attempt to cast to a scalar shape function 
        auto scalar_sf = dynamic_cast<const ScalarShapeFunction*>(fe.ShapeFunction());
        if (!scalar_sf) {
            throw std::runtime_error("AxiMagLinearIntegrator: The provided FE is not scalar (H1).");
        }

        const auto& N = scalar_sf->N(ptRef); // shape function values
        for (int i = 0; i < nDofs; i++)
        {
            f(i) += sqrt(s_phys) * J * N(i) * detJ;
        }
        return f;
    };

private:

};
