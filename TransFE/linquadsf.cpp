/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "linquadsf.h"

 // Compute the shape function values at a given point.
Vector<double> LinQuadSF::N(const point& pt) {
    Vector<double> mN(4);
    double xi = pt.x;
    double eta = pt.y;
    mN[0] = (1.0 - xi) * (1.0 - eta) / 4.0;
    mN[1] = (1.0 + xi) * (1.0 - eta) / 4.0;
    mN[2] = (1.0 + xi) * (1.0 + eta) / 4.0;
    mN[3] = (1.0 - xi) * (1.0 + eta) / 4.0;
    return mN;
}

// Compute the shape function derivatives with respect to local coordinates.
Matrix<double> LinQuadSF::dNds(const point& pt) {
    Matrix<double> dN(4, 2);
    double xi = pt.x;
    double eta = pt.y;

    // Partial derivatives with respect to xi and eta
    dN(0, 0) = (-1.0 + eta) / 4.0; // dN1/dxi
    dN(0, 1) = (-1.0 + xi) / 4.0; // dN1/deta
    dN(1, 0) = (1.0 - eta) / 4.0;  // dN2/dxi
    dN(1, 1) = (-1.0 - xi) / 4.0;  // dN2/deta
    dN(2, 0) = (1.0 + eta) / 4.0;  // dN3/dxi
    dN(2, 1) = (1.0 + xi) / 4.0;   // dN3/deta
    dN(3, 0) = (-1.0 - eta) / 4.0; // dN4/dxi
    dN(3, 1) = (1.0 - xi) / 4.0;   // dN4/deta

    return dN;
}

// Get the integration points (static local variable).
const std::vector<point>& LinQuadSF::IntPts() const {
    static const std::vector<point> int_pts = []() {
        const double n = 1.0 / std::sqrt(3.0);
        return std::vector<point>{
            point(-n, -n),
            point(n, -n),
            point(n, n),
            point(-n, n)
        };
    }();
    return int_pts;
}

// Get the integration weights (static local variable).
const Vector<double>& LinQuadSF::Weights() const {
    static const Vector<double> weights = []() {
        Vector<double> w(4); // Create a Vector of size 4
        w[0] = 1.0;
        w[1] = 1.0;
        w[2] = 1.0;
        w[3] = 1.0;
        return w;
    }();
    return weights;
}

// Get the number of integration points.
int LinQuadSF::numIntPts() const {
    return 4; // Fixed for this element type
}