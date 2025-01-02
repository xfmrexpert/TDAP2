/***************************************************************************
 *   Copyright (C) 2005-2025 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   Use of this source code is governed by an MIT-style                   *
 *   license that can be found in the LICENSE.txt file or at               *
 *   https://opensource.org/licenses/MIT.                                  *
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
	ID = -1;
}

GeomEntity* MeshEntity::getClassification() const {
	return myClassification;
}

void MeshEntity::setClassification(GeomEntity& new_class) {
	myClassification = &new_class;
}

int MeshEntity::getClassificationID() const {
	if (myClassification) {
		return myClassification->ID;
	}
	return -1;
}

