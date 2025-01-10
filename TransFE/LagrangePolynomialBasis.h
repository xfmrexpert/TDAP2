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

inline double factorial(int n) {
    double result = 1.0;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

class LagrangePolynomialBasis {
public:
    LagrangePolynomialBasis(int dimension, int order)
        : dim(dimension), polyOrder(order)
    {
        if (dim != 2) {
            throw std::runtime_error("LagrangePolynomialBasis: only 2D triangle supported in this example.");
        }
        if (polyOrder < 1) {
            throw std::runtime_error("Polynomial order must be >= 1.");
        }

        // Precompute factorials if you want efficiency (optional)
        factCache.resize(polyOrder + 1);
        for (int i = 0; i <= polyOrder; i++) {
            factCache[i] = factorial(i);
        }

        // Build the node layout for a triangular Lagrange basis of order=p
        // i + j <= p, i,j >= 0
        // The total number of basis functions is (p+1)(p+2)/2
        // We'll store (i, j) for each basis function index.
        buildNodeLayout();
    }

    // Evaluate the shape function i at the reference point
    double evaluate(int iBF, const point& ptRef) const {
        // 1) Identify (i, j) from our nodeData
        int i = nodeData[iBF].i;
        int j = nodeData[iBF].j;
        int k = polyOrder - i - j;

        double r = ptRef.x;
        double s = ptRef.y;
        double sum = r + s;
        // If r<0, s<0, or r+s>1, the basis is typically zero or negative in extrapolation,
        // but we won't clamp it here.

        // 2) The multinomial coefficient C = p! / ( i! j! k! )
        double C = factCache[polyOrder] / (factCache[i] * factCache[j] * factCache[k]);

        // 3) Evaluate r^i * s^j * (1-r-s)^k
        double val = std::pow(r, i) * std::pow(s, j) * std::pow(1.0 - r - s, k);

        return C * val;
    }

    // Evaluate gradient, etc.
    Vector<double> evaluateGradient(int iBF, const point& ptRef) const {
        int i = nodeData[iBF].i;
        int j = nodeData[iBF].j;
        int k = polyOrder - i - j;

        double r = ptRef.x;
        double s = ptRef.y;

        // factor = C * r^i s^j (1-r-s)^k
        // We'll do partial wrt r, partial wrt s by product rule:
        double C = factCache[polyOrder] / (factCache[i] * factCache[j] * factCache[k]);

        double val_rs = std::pow(r, i) * std::pow(s, j) * std::pow(1.0 - r - s, k);

        // Partial wrt r:
        // derivative of r^i  => i * r^(i-1)
        // derivative of (1-r-s)^k => -k * (1-r-s)^(k-1)
        double dphi_dr = 0.0;
        if (i > 0) {
            // i * r^(i-1) * s^j * (1-r-s)^k
            double part = i * std::pow(r, i - 1) * std::pow(s, j) * std::pow(1.0 - r - s, k);
            dphi_dr += part;
        }
        if (k > 0) {
            // derivative wrt r of (1-r-s)^k => -k (1-r-s)^(k-1)
            double part = -k * std::pow(r, i) * std::pow(s, j) * std::pow(1.0 - r - s, k - 1);
            dphi_dr += part;
        }
        dphi_dr *= C;

        // Partial wrt s:
        // derivative of s^j => j * s^(j-1)
        // derivative of (1-r-s)^k => -k * (1-r-s)^(k-1)
        double dphi_ds = 0.0;
        if (j > 0) {
            double part = j * std::pow(r, i) * std::pow(s, j - 1) * std::pow(1.0 - r - s, k);
            dphi_ds += part;
        }
        if (k > 0) {
            double part = -k * std::pow(r, i) * std::pow(s, j) * std::pow(1.0 - r - s, k - 1);
            dphi_ds += part;
        }
        dphi_ds *= C;

        Vector<double> vec = Vector<double>::Zero(2);
        vec << dphi_dr, dphi_ds;
        return vec;
    }

    // Possibly return # of basis functions, etc.
    int numBasisFunctions() const {
        return (polyOrder + 1) * (polyOrder + 2) / 2;
    }

private:
    int dim;
    int polyOrder;

    // factorial cache for efficiency
    std::vector<double> factCache;

    struct NodeIJ {
        int i;
        int j;
    };
    std::vector<NodeIJ> nodeData;

    /// Build node layout for a triangle of order p:
    /// i, j >= 0, i+j <= p
    void buildNodeLayout() {
        for (int i = 0; i <= polyOrder; i++) {
            for (int j = 0; j <= polyOrder - i; j++) {
                NodeIJ n;
                n.i = i;
                n.j = j;
                nodeData.push_back(n);
            }
        }
    }
};