/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#ifndef SHAPEFUNCTION_H
#define SHAPEFUNCTION_H

 /// This class represents a generic shape function. 

#include "MeshDB/meshentity.h"
#include "typedefs.h"
#include "MeshDB/point.h"
#include "matrix.h"
#include "vector.h"
#include <vector>

class ShapeFunction {

public:

	ShapeFunction(shared_ptr<MeshEntity> mElement) {
		Element = mElement;
	};

	virtual ~ShapeFunction() = default; //Need to loop through mIntPt and trash the points!!!

	virtual Vector<double> N(point) = 0;
	virtual Matrix<double> dNds(point) = 0;

	virtual vector<point> IntPts() = 0;
	virtual Vector<double> Weights() = 0;
	virtual int numIntPts() = 0;

protected:

private:

	shared_ptr<MeshEntity> Element;

};
#endif //SHAPEFUNCTION_H

