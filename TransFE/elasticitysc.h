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

class ElasticitySC : public StiffnessContributor {
public:
	ElasticitySC(MeshFace* Element, std::shared_ptr<Mapping> Map, std::shared_ptr<ShapeFunction> SF) : StiffnessContributor(Element, Map, SF) {
		auto nodes = Element->getNodes();
		
		nnd = 2;

		for (const auto& node : Element->getNodes()) {

			const auto& DOFs = node->getDOFs();
			if (DOFs.empty()) {
				for (int i = 0; i < nnd; i++) {
					DOF* newDOF = node->newDOF();
					//newDOF->set_eqnumber((*node_iter)->ID*2+i);
				}
			}
		}
	};

	~ElasticitySC() {};

	virtual Matrix<double> evaluatePt(point);

private:

};

