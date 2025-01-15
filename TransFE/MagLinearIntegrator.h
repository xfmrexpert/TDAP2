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
#include "assembler.h"
#include "LinearFormIntegrator.h"

class MagLinearIntegrator : public LinearFormIntegrator
{
public:
	using LinearFormIntegrator::LinearFormIntegrator;
    ~MagLinearIntegrator() = default;

    Vector<double> evaluatePt(point ptRef, const FiniteElementBase& fe, const MeshEntity& entity, const ElementQuadratureData& quadData) const override
    {
        size_t nDofs = fe.numLocalDOFs();
        Vector<double> f = Vector<double>::Zero(nDofs);

        double J = entity.getClassification()->getAttribute("J"); //Constant current density in element

        double measure = quadData.detJ;

        // Attempt to cast to a scalar shape function 
        auto scalar_sf = dynamic_cast<const ScalarShapeFunction*>(fe.ShapeFunction());
        if (!scalar_sf) {
            throw std::runtime_error("MagLinearIntegrator: The provided FE is not scalar (H1).");
        }

        const auto& phi = scalar_sf->N(ptRef); // shape function values

        for (int i = 0; i < nDofs; i++)
        {
            f(i) += J * phi(i) * measure;
        }
        return f;
    };

private:

};
