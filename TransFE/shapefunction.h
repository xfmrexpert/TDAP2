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

 /// This class represents a generic shape function. 

#include "MeshDB/meshentity.h"
#include "typedefs.h"
#include "MeshDB/point.h"
#include "matrix.h"
#include "vector.h"
#include <vector>

class ShapeFunction {

public:

	explicit ShapeFunction(MeshEntity* mElement) : Element(mElement) {}

	virtual ~ShapeFunction() = default;

	virtual Vector<double> N(const point& pt) = 0;
	virtual Matrix<double> dNds(const point& pt) = 0;

	virtual const std::vector<point>& IntPts() const = 0;
	virtual const Vector<double>& Weights() const = 0;
	virtual int numIntPts() const = 0;

protected:
	/// Non-owning reference to the associated mesh entity
	MeshEntity* Element;

private:

};

