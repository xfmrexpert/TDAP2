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

#include "MeshDB/point.h"
#include "typedefs.h"
#include <type_traits>
#include <concepts>

class ShapeFunction
{
public:
    ShapeFunction(uint8_t dim) : m_dim(dim) {};

    virtual Vector<double> N(const point& pt) const = 0;
    virtual Matrix<double> grad_N(const point& pt) const = 0;
    virtual Vector<double> curl_N(const point& pt) const = 0;

    uint8_t m_dim;
};

class FiniteElementBase {
public:
    virtual ~FiniteElementBase() = default;

    // Pure virtual functions to be implemented by derived classes
    virtual int referenceDimensions() const = 0;
    virtual int spatialDimensions() const = 0;
    virtual int numLocalDOFs() const = 0;

    // Access to shape functions via base class pointers
    virtual const ShapeFunction* Geom() const = 0;
    virtual const ShapeFunction* Sol() const = 0;
};


template <typename G, typename F>
requires std::derived_from<G, ShapeFunction>&& std::derived_from<F, ShapeFunction>
class FiniteElement : public FiniteElementBase {

public:
    FiniteElement(uint8_t dim) : geomSF(dim), solSF(dim) { };
    virtual ~FiniteElement() = default;

    // Implement virtual functions from FiniteElementBase
    int referenceDimensions() const override {
        // Implementation specific to FiniteElement<G, F>
        // For example:
        return geomSF.m_dim; // Assuming m_dim is accessible
    }

    int spatialDimensions() const override {
        // Implementation specific to FiniteElement<G, F>
        return solSF.m_dim; // Assuming m_dim is accessible
    }

    int numLocalDOFs() const override {
        // Implementation specific to FiniteElement<G, F>
        // Example:
        return geomSF.N(point()).rows(); // Placeholder
    }

    const ShapeFunction* Geom() const override {
        return &geomSF;
    }

    const ShapeFunction* Sol() const override {
        return &solSF;
    }

protected:
	G geomSF;
	F solSF;

};

class LagrangeShapeFunction : public ShapeFunction
{
public:
    LagrangeShapeFunction(uint8_t dim) : ShapeFunction(dim) {};
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
    };
};

// Example with both geometric and solution shape functions being Lagrange
class LagrangeFiniteElement : public FiniteElement<LagrangeShapeFunction, LagrangeShapeFunction> {
public:
    LagrangeFiniteElement() = default;
    ~LagrangeFiniteElement() override = default;

    // Override or extend functionality if necessary
};

