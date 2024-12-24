/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "node.h"

bool operator== (Node& lhs, Node& rhs) {
	if (lhs.x() == rhs.x()) {
		if (lhs.y() == rhs.y()) {
			if (lhs.z() == rhs.z()) {
				return true;
			}
		}
	}
	return false;
}

bool operator< (Node& lhs, Node& rhs) {
	if (lhs.x() < rhs.x()) {
		return true;
	}
	else {
		return false;
	}
}

// Function to get non-owning pointers to DOFs
std::vector<DOF*> Node::getDOFs() const {
	std::vector<DOF*> result;
	result.reserve(DOFs.size()); // Reserve space for efficiency
	for (const auto& dof : DOFs) {
		result.push_back(dof.get()); // Extract raw pointer
	}
	return result; // Return non-owning pointers
}

DOF* Node::newDOF() {
	DOFs.push_back(std::make_unique<DOF>());
	return DOFs.back().get();;
}

