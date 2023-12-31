/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef MAGAXISTATICSC_H
#define MAGAXISTATICSC_H

#include "stiffnesscontributor.h"
#include "MeshDB/meshface.h"
#include "mapping.h"
#include "shapefunction.h"
#include "MeshDB/point.h"
#include <vector>
#include "dof.h"
#include "typedefs.h"

class MagAxiStaticSC : public StiffnessContributor {
public:
	MagAxiStaticSC(shared_ptr<MeshFace> Element, shared_ptr<Mapping> Map, shared_ptr<ShapeFunction> SF, int formul) :StiffnessContributor(Element, Map, SF) {
		auto nodes = Element->getNodes();

		//check the region type of this face and set nnd (and dofs) accordingly
		nnd = 1; //Only have A component in z-direction

		formulation = formul;

		//create a new DOF for each node
		for (auto node_iter = nodes.begin(); node_iter != nodes.end(); ++node_iter) {

			auto DOFs = (*node_iter)->getDOFs();
			if (DOFs.size() < nnd) {
				for (size_t i = DOFs.size(); i < nnd; i++) {
					shared_ptr<DOF> newDOF = (*node_iter)->newDOF();
					//newDOF->set_eqnumber((*node_iter)->ID*2+i);
				}
			}
		}
	};

	//~MagAxiStaticSC(){};

	virtual Matrix<double> evaluatePt(point);

private:
	int formulation;
};

#endif
