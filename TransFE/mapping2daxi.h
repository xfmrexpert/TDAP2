/***************************************************************************
 *   Copyright (C) 2005-2023 by T. C. Raymond                              *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#ifndef MAPPING2DAXI_H
#define MAPPING2DAXI_H

#include "mapping.h"
#include "MeshDB/point.h"
#include "MeshDB/meshface.h"
#include "shapefunction.h"

class Mapping2DAxi : public Mapping {
public:
	Mapping2DAxi(shared_ptr<MeshFace> Element, shared_ptr<ShapeFunction> SF) : Mapping(Element, SF) {
		nsd = 2;
		npd = 2;
	};

	~Mapping2DAxi() {};

	virtual Matrix<double> jacobianInverse(point pt);

	virtual Matrix<double> dXds(point pt);

	//virtual double detJacobian(point pt);

};

#endif
