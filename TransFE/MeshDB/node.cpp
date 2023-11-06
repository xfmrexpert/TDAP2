/************************************************************************
			node.cpp - Copyright T. C. Raymond

**************************************************************************/

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

const vector<shared_ptr<DOF>>& Node::getDOFs() {
	return DOFs;
}

shared_ptr<DOF> Node::newDOF() {
	shared_ptr<DOF> new_DOF = make_shared<DOF>();
	DOFs.push_back(new_DOF);
	return new_DOF;
}

Node::~Node() {

}
