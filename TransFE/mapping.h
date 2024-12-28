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

#include "typedefs.h"
#include "MeshDB/point.h"
#include "MeshDB/meshentity.h"
#include "shapefunction.h"
#include <cstdint>

class Mapping {
public:
	Mapping(MeshEntity* mElement, std::shared_ptr<ShapeFunction> mSF) {
		Element = mElement;
		SF = mSF;
		nen = Element->getNodes().size();
	};

	virtual ~Mapping() = default;

	virtual Matrix<double> jacobianInverse(point) = 0;

	virtual Matrix<double> dXds(point) = 0;

	//virtual double detJacobian(point) = 0;

protected:
	MeshEntity* Element;
	std::shared_ptr<ShapeFunction> SF;

	uint8_t nsd; //# of spatial dimensions
	uint8_t npd; //# of parametric dimensions
	size_t nen; //# of element nodes

};
