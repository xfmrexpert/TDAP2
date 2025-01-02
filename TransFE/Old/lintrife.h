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

 /// This class represents the shape functions for a first order (linear) triangle. 

#include "finiteelement.h"
#include "typedefs.h"
#include "MeshDB/point.h"
#include "MeshDB/meshface.h"
#include "elementtransform2d.h"
#include <vector>

class LinTriFE : public FiniteElement {

public:

	explicit LinTriFE(MeshFace* entity, IntegrationRule* integration_rule) : FiniteElement(entity, 2, 1, nullptr, integration_rule) {
		//check the region type of this face and set nnd (and dofs) accordingly
		int nnd = 3; //Only have A component in z-direction
		transform = std::make_unique<ElementTransform2D>();
	}

	Vector<double> N(const point& pt) override;
	Matrix<double> dNds(const point& pt) override;

protected:

private:

};


