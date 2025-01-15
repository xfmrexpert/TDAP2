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
#include "FiniteElement.h"
#include "ElementTransform.h"
#include "AxiElementTransform.h"

 /// Lagrange finite element (scalar, H^1 conforming).
class LagrangeElement : public FiniteElement<LagrangeShapeFunction, ElementTransform>
{
public:
    LagrangeElement(size_t dim, int order = 1)
        : FiniteElement<LagrangeShapeFunction, ElementTransform>(dim, dim, order)
    {
    }

    virtual ~LagrangeElement() = default;

    size_t numLocalDOFs() const override {
        return this->shape_function.N(point()).size();
    }

private:
   
};

/// Axisymmetric Lagrange finite element (scalar, H^1 conforming).
class AxiLagrangeElement : public FiniteElement<LagrangeShapeFunction, AxiElementTransform>
{
public:
    AxiLagrangeElement(int order = 1)
        : FiniteElement<LagrangeShapeFunction, AxiElementTransform>(2, 2, order)
    {
    }

    virtual ~AxiLagrangeElement() = default;

    size_t numLocalDOFs() const override {
        return this->shape_function.N(point()).size();
    }

private:

};
