/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "quadquadsf.h"

 // Compute the shape function values at a given point.
Vector<double> QuadQuadSF::N(const point& pt) {
    Vector<double> mN(9);
    double xi = pt.x;
    double eta = pt.y;

    // Corner nodes
    mN[0] = 0.25 * xi * eta * (xi + 1.0) * (eta + 1.0);
    mN[1] = 0.25 * xi * eta * (xi - 1.0) * (eta + 1.0);
    mN[2] = 0.25 * xi * eta * (xi - 1.0) * (eta - 1.0);
    mN[3] = 0.25 * xi * eta * (xi + 1.0) * (eta - 1.0);

    // Edge nodes
    mN[4] = 0.5 * eta * (eta + 1.0) * (1.0 - xi * xi);
    mN[5] = 0.5 * xi * (xi - 1.0) * (1.0 - eta * eta);
    mN[6] = 0.5 * eta * (eta - 1.0) * (1.0 - xi * xi);
    mN[7] = 0.5 * xi * (xi + 1.0) * (1.0 - eta * eta);

    // Face node
    mN[8] = (1.0 - xi * xi) * (1.0 - eta * eta);

    return mN;
}

// Compute the shape function derivatives with respect to local coordinates.
Matrix<double> QuadQuadSF::dNds(const point& pt) {
    Matrix<double> dN(9, 2);
    double xi = pt.x;
    double eta = pt.y;

    dN(0, 0)=xi*eta*eta/2.+xi*eta/2.+eta*eta/4.+eta/4.;  //corners
    dN(0, 1)=xi*xi*eta/2.+xi*xi/4.+xi*eta/2.+xi/4.;
    dN(1, 0)=xi*eta*eta/2.+xi*eta/2.-eta*eta/4.-eta/4.;
    dN(1, 1)=xi*xi*eta/2.+xi*xi/4.-xi*eta/2.-xi/4.;
    dN(2, 0)=xi*eta*eta/2.-xi*eta/2.-eta*eta/4.+eta/4.;
    dN(2, 1)=xi*xi*eta/2.-xi*xi/4.-xi*eta/2.+xi/4.;
    dN(3, 0)=xi*eta*eta/2.-xi*eta/2.+eta*eta/4.-eta/4.;
    dN(3, 1)=xi*xi*eta/2.-xi*xi/4.+xi*eta/2.-xi/4.;
    dN(4, 0)=-eta*eta*xi-eta*xi; //edges
    dN(4, 1)=eta-eta*xi*xi+1./2.-xi*xi/2.;
    dN(5, 0)=xi-xi*eta*eta-1./2.+eta*eta/2.;
    dN(5, 1)=-xi*xi*eta+xi*eta;
    dN(6, 0)=-xi*eta*eta+xi*eta;
    dN(6, 1)=eta-xi*xi*eta-1./2.+xi*xi/2.;
    dN(7, 0)=xi-xi*eta*eta+1./2.-eta*eta/2.;
    dN(7, 1)=-xi*xi*eta-xi*eta;
    dN(8, 0)=-2.*xi+2.*xi*eta*eta;  //face
    dN(8, 1)=-2.*eta+2.*xi*xi*eta;
   
   return dN;
};

// Get the integration points (static local variable).
const std::vector<point>& QuadQuadSF::IntPts() const {
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
const Vector<double>& QuadQuadSF::Weights() const {
    static const Vector<double> weights = []() {
        Vector<double> w(4);
        for (int i = 0; i < 4; ++i) {
            w[i] = 1.0;
        }
        return w;
    }();
    return weights;
}

// Get the number of integration points.
int QuadQuadSF::numIntPts() const {
    return 4; // Fixed number of integration points for quadratic quadrilateral
}
