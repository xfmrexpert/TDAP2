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

#include "stiffnesscontributor.h"
#include "MeshDB/meshface.h"
#include "mapping.h"
#include "shapefunction.h"
#include "MeshDB/point.h"
#include <vector>
#include "dof.h"
#include "typedefs.h"

class ElasticitySC : public StiffnessContributor<double> {
public:
	ElasticitySC(MeshFace* Element, Field<double>* field, std::shared_ptr<Mapping> Map, std::shared_ptr<ShapeFunction> SF) : StiffnessContributor<double>(Element, field, Map, SF) {
		nnd = 2;
	};

	~ElasticitySC() {};

	virtual Matrix<double> evaluatePt(point);

private:

};

