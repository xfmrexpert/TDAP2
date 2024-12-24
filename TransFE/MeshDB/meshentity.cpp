/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
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

GeomEntity* MeshEntity::getClassification() {
	return myClassification;
}

void MeshEntity::setClassification(GeomEntity& new_class) {
	myClassification = &new_class;
}

