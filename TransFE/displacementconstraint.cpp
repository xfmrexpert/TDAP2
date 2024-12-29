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

template <typename T>
void DisplacementConstraint<T>::apply() {
	auto nodes = this->entity->getNodes();

	if (this->entity->getClassification()->getAttribute("x_constraint") != NO_ATTRIB) {  //x component is constrained
		T x_constraint = this->entity->getClassification()->getAttribute("x_constraint");

		for (const auto& node : nodes) {
			const auto& DOFs = this->field->getDOFsForNode(node);
			if (DOFs.size() > 0) {
				//if(x_constraint == 0){  //zero essential BC
				//   DOFs[0]->set_status(DOF_Zero);
				//   DOFs[0]->set_value(0);
				//} else {  //non-zero essential BC
				DOFs[0]->set_status(DOFStatus::Fixed);
				DOFs[0]->set_value(x_constraint);
				//}
			}
			else {
				std::cerr << "Tried to access unitialized DOFs!" << std::endl;
				exit(1);
			}
		}
	}
	if (this->entity->getClassification()->getAttribute("y_constraint") != NO_ATTRIB) { //y component is constrained
		T y_constraint = this->entity->getClassification()->getAttribute("y_constraint");
		for (const auto& node : nodes) {
			const auto& DOFs = this->field->getDOFsForNode(node);
			if (y_constraint == 0) {  //zero essential BC
				DOFs[1]->set_status(DOFStatus::Zero);
				DOFs[1]->set_value(0);
			}
			else {  //non-zero essential BC
				DOFs[1]->set_status(DOFStatus::Fixed);
				DOFs[1]->set_value(y_constraint);
			}
		}
	}
	if (this->entity->getClassification()->getAttribute("z_constraint") != NO_ATTRIB) { //z component is constrained
		T y_constraint = this->entity->getClassification()->getAttribute("z_constraint");
		for (const auto& node : nodes) {
			const auto& DOFs = this->field->getDOFsForNode(node);
			if (y_constraint == 0) {  //zero essential BC
				DOFs[2]->set_status(DOFStatus::Zero);
				DOFs[2]->set_value(0);
			}
			else {  //non-zero essential BC
				DOFs[2]->set_status(DOFStatus::Fixed);
				DOFs[2]->set_value(y_constraint);
			}
		}
	}
};
