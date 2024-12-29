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

 /// This class represents a generic constraint. 

#include "MeshDB/meshentity.h"
#include "field.h"

template <class T>
class Constraint {

public:

	Constraint(MeshEntity* Element_in, Field<T>* Field_in) {
		entity = Element_in;
		field = Field_in;
	};

	virtual ~Constraint() = default;

	virtual void apply() = 0;

protected:

	MeshEntity* entity;
	Field<T>* field;

private:

};
