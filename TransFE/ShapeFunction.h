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

enum class FEFamily { H1, HCURL, HDIV, L2 };

class ShapeFunction {
public:
    virtual ~ShapeFunction() = default;
    virtual FEFamily family() const = 0;
    //virtual size_t numShapeFunctions() const = 0;
};

class ScalarShapeFunction : public ShapeFunction
{
public:
    ScalarShapeFunction(size_t dim) : numDimensions(dim) {};

    virtual Vector<double> N(const point& pt) const = 0;
    virtual Matrix<double> grad_N(const point& pt) const = 0;

    FEFamily family() const override { return FEFamily::H1; } // or L2

    size_t numDimensions;
};

class LagrangeShapeFunction : public ScalarShapeFunction
{
public:
    LagrangeShapeFunction(size_t ref_dim, int order = 1)
        : ScalarShapeFunction(ref_dim), polynomial_order(order)
    {
    };

    int getOrder() const { return polynomial_order; }

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

private:
    int polynomial_order; // 1 for linear, 2 for quadratic, etc.
};

class VectorShapeFunction : public ShapeFunction {
public:
    // Evaluate the vector shape functions -> size = [numShapeFunctions x dimension]
    virtual Matrix<double> N_vector(const point& pt) const = 0;
    // Evaluate the curl (2D or 3D) -> size = [numShapeFunctions x (1 or 3)]
    virtual Matrix<double> curl_N(const point& pt) const = 0;

    FEFamily family() const override { return FEFamily::HCURL; }
};

class NedelecShapeFunction : public VectorShapeFunction {
public:
    NedelecShapeFunction()
        : order(1) // We can extend later for higher order
    {
        
    }

    virtual ~NedelecShapeFunction() = default;

    // Returns 3 for first-order Nedelec on a triangle
    /*size_t numShapeFunctions() const override {
        return 3;
    }*/

    // N_vector: Evaluate the vector shape functions at (r, s).
    // Return a 3 x 2 matrix => each row i is [Nx_i, Ny_i] for shape function i
    Matrix<double> N_vector(const point& ptRef) const override {
        double r = ptRef.x;
        double s = ptRef.y;
        double t = 1.0 - r - s; // convenience

        Matrix<double> Nvals(3, 2);
        // Each row => Nx, Ny
        // The polynomials can vary by reference, but here's a possible set:

        // For demonstration, let's do something symbolic:
        //   N1(r,s) = [ (1-r-s), -r ] ... etc.
        // These typically come from standard references/papers.

        // 1) N0
        Nvals(0, 0) = (1.0 - r - s); // Nx
        Nvals(0, 1) = -r;           // Ny

        // 2) N1
        Nvals(1, 0) = s;
        Nvals(1, 1) = -(1.0 - r - s);

        // 3) N2
        Nvals(2, 0) = -s;
        Nvals(2, 1) = r;

        return Nvals;
    }

    // curl_N: Evaluate the curl of each vector basis function.
    // In 2D, the curl of a vector field (u, v) is a scalar (∂v/∂x - ∂u/∂y).
    // We'll return a 3 x 1 matrix => row i => scalar curl of function i
    Matrix<double> curl_N(const point& ptRef) const override {
        Matrix<double> curlVals(3, 1);

        // Let's say from the polynomials above:
        //   N0(r,s) = ( (1-r-s), -r )
        //   curl(N0) = ∂(-r)/∂r - ∂((1-r-s))/∂s

        // Example for N0:
        //   Nx(r,s) = (1-r-s)
        //   Ny(r,s) = -r
        //   ∂Ny/∂r = -1, ∂Nx/∂s = -1
        //   curl(N0) = (-1) - (-1) = 0
        // We'll do this systematically:

        // N0:
        // Nx_0 = 1 - r - s => partial wrt r => -1, partial wrt s => -1
        // Ny_0 = -r        => partial wrt r => -1, partial wrt s =>  0
        // curl(N0) = d(Ny_0)/dr - d(Nx_0)/ds = (-1) - (-1) = 0
        curlVals(0, 0) = 0.0;

        // N1:
        // Nx_1 = s         => partial wrt r => 0, partial wrt s => 1
        // Ny_1 = -(1-r-s)  => partial wrt r => 1, partial wrt s => -1
        // curl(N1) = d(Ny_1)/dr - d(Nx_1)/ds = (1) - (1) = 0
        curlVals(1, 0) = 0.0;

        // N2:
        // Nx_2 = -s        => partial wrt r => 0, partial wrt s => -1
        // Ny_2 = r         => partial wrt r => 1, partial wrt s => 0
        // curl(N2) = d(Ny_2)/dr - d(Nx_2)/ds = (1) - (-1) = 2
        curlVals(2, 0) = 2.0;

        return curlVals;
    }

private:
    int order;
    /*ElementType elementType;*/
};