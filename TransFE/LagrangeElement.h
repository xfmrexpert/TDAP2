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

/// Lagrange finite element (scalar, H^1 conforming).
class LagrangeElement : public FiniteElement<LagrangeShapeFunction, LagrangeShapeFunction>
{
public:
    LagrangeElement(uint8_t dim) : FiniteElement<LagrangeShapeFunction, LagrangeShapeFunction>(dim)
    {
		m_order = 1; // default to linear
		m_dim = dim; // default to 2D
    }

    virtual ~LagrangeElement() = default;

    int referenceDimensions() const override { return m_dim; }
    int spatialDimensions() const override { return m_dim; }

    // For a simple polynomial triangle or quadrilateral, 
    // the number of local DOFs depends on polynomial order. 
    int numLocalDOFs() const override {
        // example: for linear (order=1) in 2D, we might have 3 DOFs (triangle).
        // This is a toy example. Real logic is more elaborate.
        return /* some formula depending on m_order, m_dim */ 3;
    }

private:
    int m_order;
    int m_dim;
};
