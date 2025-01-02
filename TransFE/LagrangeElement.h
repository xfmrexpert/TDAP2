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
class LagrangeElement : public FiniteElement
{
public:
    LagrangeElement()
    {
		m_order = 1; // default to linear
		m_dim = 2; // default to 2D
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

    Vector<double> N(const point& pt) const override
    {
        Vector<double> mN(3);
        double r = pt.x;
        double s = pt.y;
        double t = 1.0 - r - s;
        mN[0] = r;
        mN[1] = s;
        mN[2] = t;
        return mN;
    }

    Matrix<double> grad_N(const point& pt) const override
    {
        Matrix<double> dN(3, 2);
        double r = pt.x;
        double s = pt.y;
        dN(0, 0) = 1.;
        dN(0, 1) = 0.;
        dN(1, 0) = 0.;
        dN(1, 1) = 1.;
        dN(2, 0) = -1.;
        dN(2, 1) = -1.;
        return dN;
    }

    Vector<double> curl_N(const point& pt) const override
    {
        // For a scalar H^1 element in 2D, the "curl" is effectively out-of-plane (a scalar).
        // We can just return zero or some scalar if needed, or not implemented. 
        // We'll return a 1D vector with 0.0 as a placeholder.
        return Vector<double>(1);
    }

private:
    int m_order;
    int m_dim;
};
