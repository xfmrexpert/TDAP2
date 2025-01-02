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
#include "field.h"
#include "MeshDB/point.h"
#include <vector>
#include "dof.h"
#include "typedefs.h"

class MagAxiStaticSC : public StiffnessContributor<double> {
public:
	MagAxiStaticSC(FiniteElement* Element, Field<double>* field, int formul) : StiffnessContributor(Element, field) {
		//check the region type of this face and set nnd (and dofs) accordingly
		nnd = 1; //Only have A component in z-direction

		formulation = formul;
	};

	//~MagAxiStaticSC(){};

	virtual Matrix<double> evaluatePt(point);

private:
	int formulation;
};
