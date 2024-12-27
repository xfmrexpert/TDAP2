/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "displacementconstraint.h"
#include <iostream>

DisplacementConstraint::~DisplacementConstraint() {

};

void DisplacementConstraint::apply() {
	auto nodes = entity->getNodes();

	if (entity->getClassification()->getAttribute("x_constraint") != NO_ATTRIB) {  //x component is constrained
		double x_constraint = entity->getClassification()->getAttribute("x_constraint");

		for (const auto& node : nodes) {
			const auto& DOFs = node->getDOFs();
			if (DOFs.size() > 0) {
				//if(x_constraint == 0){  //zero essential BC
				//   DOFs[0]->set_status(DOF_Zero);
				//   DOFs[0]->set_value(0);
				//} else {  //non-zero essential BC
				DOFs[0]->set_status(DOF_Fixed);
				DOFs[0]->set_value(x_constraint);
				//}
			}
			else {
				std::cerr << "Tried to access unitialized DOFs!" << std::endl;
				exit(1);
			}
		}
	}
	if (entity->getClassification()->getAttribute("y_constraint") != NO_ATTRIB) { //y component is constrained
		double y_constraint = entity->getClassification()->getAttribute("y_constraint");
		for (const auto& node : nodes) {
			const auto& DOFs = node->getDOFs();
			if (y_constraint == 0) {  //zero essential BC
				DOFs[1]->set_status(DOF_Zero);
				DOFs[1]->set_value(0);
			}
			else {  //non-zero essential BC
				DOFs[1]->set_status(DOF_Fixed);
				DOFs[1]->set_value(y_constraint);
			}
		}
	}
	if (entity->getClassification()->getAttribute("z_constraint") != NO_ATTRIB) { //z component is constrained
		double y_constraint = entity->getClassification()->getAttribute("z_constraint");
		for (const auto& node : nodes) {
			const auto& DOFs = node->getDOFs();
			if (y_constraint == 0) {  //zero essential BC
				DOFs[2]->set_status(DOF_Zero);
				DOFs[2]->set_value(0);
			}
			else {  //non-zero essential BC
				DOFs[2]->set_status(DOF_Fixed);
				DOFs[2]->set_value(y_constraint);
			}
		}
	}
};
