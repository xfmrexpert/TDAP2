/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "meshentity.h"
#include <iostream>

MeshEntity::MeshEntity() {
	node = nullptr;
}

shared_ptr<GeomEntity> MeshEntity::getClassification() {
	return myClassification;
}

void MeshEntity::setClassification(shared_ptr<GeomEntity> new_class) {
	if (new_class == nullptr) {
		cout << "Setting null classification!!!" << endl;
	}
	myClassification = new_class;
}

