/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#ifndef MAPPING_H
#define MAPPING_H

#include "typedefs.h"
#include "MeshDB/point.h"
#include "MeshDB/meshentity.h"
#include "shapefunction.h"

class Mapping {
public:
	Mapping(MeshEntity* mElement, shared_ptr<ShapeFunction> mSF) {
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
	shared_ptr<ShapeFunction> SF;

	int nsd; //# of spatial dimensions
	int npd; //# of parametric dimensions
	size_t nen; //# of element nodes

};

#endif
