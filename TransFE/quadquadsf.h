/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#pragma once

/// This class represents the shape functions for a second order (quadratic) quadrilateral. 

#include "shapefunction.h"
#include "MeshDB/meshface.h"
#include "typedefs.h"
#include "MeshDB/point.h"
#include <vector>

/// Quadratic quadrilateral shape function for 2D quadrilateral elements.
class QuadQuadSF : public ShapeFunction {
public:
    /// Constructor taking a non-owning pointer to the MeshFace.
    explicit QuadQuadSF(MeshFace* element) : ShapeFunction(element) {}

    /// Compute the shape function values at a given point.
    Vector<double> N(const point& pt) override;

    /// Compute the shape function derivatives with respect to local coordinates.
    Matrix<double> dNds(const point& pt) override;

    /// Get the integration points (shared across all instances).
    const std::vector<point>& IntPts() const override;

    /// Get the weights associated with the integration points (shared across all instances).
    const Vector<double>& Weights() const override;

    /// Get the number of integration points.
    int numIntPts() const override;
};

