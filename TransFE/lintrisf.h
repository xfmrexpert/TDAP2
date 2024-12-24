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

 /// This class represents the shape functions for a first order (linear) triangle. 

#include "shapefunction.h"
#include "MeshDB/meshface.h"
#include "typedefs.h"
#include "MeshDB/point.h"
#include "matrix.h"
#include <vector>

class LinTriSF : public ShapeFunction {

public:

	explicit LinTriSF(MeshFace* element) : ShapeFunction(element) {}

	Vector<double> N(const point& pt) override;
	Matrix<double> dNds(const point& pt) override;

	const std::vector<point>& IntPts() const override;
	const Vector<double>& Weights() const override;
	int numIntPts() const override;

protected:

private:

};


