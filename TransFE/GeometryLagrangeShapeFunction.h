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
#include "MeshDB/point.h"
#include "LagrangePolynomialBasis.h"

class GeometryLagrangeShapeFunction {
public:
    GeometryLagrangeShapeFunction(size_t dimension, size_t order)
        : dim(dimension), basis(dimension, order)
    {
    }

    // Evaluate scalar shape functions (like N(...) returning a vector of length = # basis funcs)
    Vector<double> N(const point& ptRef) const {
        /*Vector<double> vals(basis.numBasisFunctions());
        for (int i = 0; i < basis.numBasisFunctions(); i++) {
            vals[i] = basis.evaluate(i, ptRef);
        }
        return vals;*/

        Vector<double> mN(3);
        double r = ptRef.x;
        double s = ptRef.y;
        double t = 1.0 - r - s;
        mN[0] = r;
        mN[1] = s;
        mN[2] = t;
        return mN;
    }

    // Evaluate gradient, etc.
    Matrix<double> grad_N(const point& ptRef) const { 
        /*Matrix<double> vals(basis.numBasisFunctions(), dim);
        for (int i = 0; i < basis.numBasisFunctions(); i++) {
            vals.row(i) = basis.evaluateGradient(i, ptRef);
        }
        return vals;*/

        Matrix<double> dN(3, 2);
        double r = ptRef.x;
        double s = ptRef.y;
        dN(0, 0) = 1.;
        dN(0, 1) = 0.;
        dN(1, 0) = 0.;
        dN(1, 1) = 1.;
        dN(2, 0) = -1.;
        dN(2, 1) = -1.;
        return dN;
    }

private:
    size_t dim;
    LagrangePolynomialBasis basis; // reuses the same polynomial computations
};
