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

#include "mapping.h"
#include "MeshDB/point.h"
#include "MeshDB/meshedge.h"
#include "shapefunction.h"

class Mapping1D2D : public Mapping {
public:
	Mapping1D2D(MeshEdge* Element, std::shared_ptr<ShapeFunction> SF) : Mapping(Element, SF) {
		nsd = 2;
		npd = 1;
	};

	virtual ~Mapping1D2D() {};

	virtual Matrix<double> jacobianInverse(point pt);

	virtual Matrix<double> dXds(point pt);

	//virtual double detJacobian(point pt);

};

