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
	LinearFormIntegrator(FESpaceBase<double>* feSpace) : feSpace(feSpace) { };
	~LinearFormIntegrator() = default;

    virtual Vector<double> evaluatePt(point ptRef, const FiniteElementBase& fe, const MeshEntity& entity, const ElementQuadratureData& quadData) const = 0;
	
    void evaluate(MeshEntity& entity, Assembler<double>& assem) const
    {
        const auto& fe = feSpace->getFiniteElement();
        int nDofs = fe->numLocalDOFs();

        Vector<double> f = Vector<double>::Zero(nDofs);

        const auto& integration = feSpace->getIntegrationRule();

        const auto& quadPointsRef = integration->IntPts();
        const auto& quadWeights = integration->Weights();

        const auto& element_data = feSpace->computeElementData(entity);

        // For each quadrature point in the reference domain
        for (size_t q = 0; q < integration->numIntPts(); q++)
        {
            point ptRef = quadPointsRef[q];
            double wq = quadWeights(q);

            f = f + evaluatePt(ptRef, *fe, entity, element_data.quadData[q]) * wq;

        }
        //std::cout << "Adding force contributor:\n";
        //std::cout << f;
        const auto& DOFs = feSpace->getDOFsForEntity(entity);
        assem.accept(f, DOFs);
    };

protected:
    FESpaceBase<double>* feSpace;

};