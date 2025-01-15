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

 /// Nédélec finite element (vector, H(curl) conforming).
class NedelecElement : public FiniteElement<NedelecShapeFunction, ElementTransform>
{
public:
    NedelecElement(int order, int dim)
        : m_order(order), m_dim(dim), FiniteElement(dim, dim, order)
    {
    }

    virtual ~NedelecElement() = default;

    //int referenceDimension() const override { return m_dim; }
    //int spatialDimension() const override { return m_dim; }

    //int numLocalDOFs() const override {
    //    // depends on dimension and polynomial order, etc.
    //    return 3;
    //}

private:
    int m_order;
    int m_dim;
};