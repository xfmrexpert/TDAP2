/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#pragma once

 /// This class represents the shape functions for a second order (linear) triangle. 

#include "shapefunction.h"
#include "MeshDB/meshface.h"
#include "typedefs.h"
#include "MeshDB/point.h"
#include <vector>

class QuadTriSF : public ShapeFunction {

public:

	/// Constructor taking a non-owning pointer to the MeshFace.
	explicit QuadTriSF() : ShapeFunction() {}

    /// Compute the shape function values at a given point.
    Vector<double> N(const point& pt) override;

    /// Compute the shape function derivatives with respect to local coordinates.
    Matrix<double> dNds(const point& pt) override;

    /// Get the integration points.
    const std::vector<point>& IntPts() const override;

    /// Get the weights associated with the integration points.
    const Vector<double>& Weights() const override;

    /// Get the number of integration points.
    int numIntPts() const override;

protected:

private:

};

