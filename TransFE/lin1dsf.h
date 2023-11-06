/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef LIN1DSF_H
#define LIN1DSF_H

 // This class represents the shape functions for a first order (linear) triangle. 

#include "shapefunction.h"
#include "MeshDB/meshedge.h"
#include "typedefs.h"
#include "MeshDB/point.h"
#include "matrix.h"
#include "vector.h"
#include <vector>

class Lin1DSF : public ShapeFunction {

public:

	Lin1DSF(shared_ptr<MeshEdge> Element) : ShapeFunction(Element) {};

	virtual Vector<double> N(point);
	virtual Matrix<double> dNds(point);

	virtual vector<point> IntPts();
	virtual Vector<double> Weights();
	virtual int numIntPts();

protected:

private:

};

#endif
