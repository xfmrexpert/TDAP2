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

class LinearFormIntegrator
{
public:
	LinearFormIntegrator(FESpace<double>* feSpace) : feSpace(feSpace) { };
	~LinearFormIntegrator() = default;
	
    void evaluate(const MeshEntity& entity, Assembler<double>& assem) const
    {
        const auto& fe = feSpace->getFiniteElement();
        int nDofs = fe->numLocalDOFs();

        Vector<double> f = Vector<double>(nDofs);

		const auto& integration = feSpace->getIntegrationRule();

        const auto& quadPointsRef = integration->IntPts();
        const auto& quadWeights = integration->Weights();

        // For each quadrature point in the reference domain
        for (size_t q = 0; q < integration->numIntPts(); q++)
        {
            point ptRef = quadPointsRef[q];
            double wq = quadWeights(q);

            const auto& element_data = feSpace->computeElementData(entity);
            
            double detJ = element_data.quadData[q].detJ;

            double J = entity.getClassification()->getAttribute("J"); //Constant current density in element

            const auto& phi = fe->N(ptRef); // shape function values
            for (int i = 0; i < nDofs; i++)
            {
                f(i) += J * phi(i) * (wq * detJ);
            }
        }

        const auto& DOFs = feSpace->getDOFsForEntity(entity);
        assem.accept(f, DOFs);
    }

private:
    FESpace<double>* feSpace;

};