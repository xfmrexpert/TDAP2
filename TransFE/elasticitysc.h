/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef ELASTICITYSC_H
#define ELASTICITYSC_H

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
	ElasticitySC(shared_ptr<MeshFace> Element, shared_ptr<Mapping> Map, shared_ptr<ShapeFunction> SF) : StiffnessContributor(Element, Map, SF) {
		auto nodes = Element->getNodes();
		
		nnd = 2;

		for (auto node_iter = nodes.begin(); node_iter != nodes.end(); node_iter++) {

			auto DOFs = (*node_iter)->getDOFs();
			if (DOFs.empty()) {
				for (int i = 0; i < nnd; i++) {
					shared_ptr<DOF> newDOF = (*node_iter)->newDOF();
					//newDOF->set_eqnumber((*node_iter)->ID*2+i);
				}
			}
		}
	};

	~ElasticitySC() {};

	virtual Matrix<double> evaluatePt(point);

private:

};

#endif
