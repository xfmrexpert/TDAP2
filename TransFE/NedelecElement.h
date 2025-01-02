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
class NedelecElement : public FiniteElement
{
public:
    NedelecElement(int order, int dim)
        : m_order(order), m_dim(dim)
    {
    }

    virtual ~NedelecElement() = default;

    int referenceDimension() const override { return m_dim; }
    int spatialDimension() const override { return m_dim; }

    int numLocalDOFs() const override {
        // depends on dimension and polynomial order, etc.
        return 3;
    }

    // Here shapeValue is a vector in 2D or 3D
    Vector<double> N(const point& pt) const override
    {
        Vector<double> val = Vector<double>(m_dim);

        // A real implementation would define the vector shape function 
        // in the reference domain. This is just a placeholder.
        // For instance, in 2D, we might define an edge-based shape 
        // that has tangential continuity.
        if (i == 0) {
            // example: val = (some function, some function)
        }
        else if (i == 1) {
            // ...
        }
        else if (i == 2) {
            // ...
        }

        return val;
    }

    // shapeGrad: for a vector element, might be a 2D or 3D matrix.
    // But in H(curl), we often care more about shapeCurl.
    Matrix<double> grad_N(const point& pt) const override
    {
        // Possibly not used or used differently. 
        return Matrix<double>(m_dim, m_dim);
    }

    // shapeCurl: return the curl of the vector shape function in reference coords.
    Vector<double> curl_N(const point& pt) const override
    {
        // In 2D, curl is effectively scalar out of plane; 
        // in 3D, it's a 3-component vector. 
        // We'll just do a placeholder:
        return Vector<double>(m_dim);
    }

private:
    int m_order;
    int m_dim;
};