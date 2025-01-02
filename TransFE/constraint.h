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
//#include "field.h"
#include "FESpace.h"

template <class T>
class Constraint {

public:

	Constraint(MeshEntity* Element_in, FESpace<T>* fe_space) {
		entity = Element_in;
		fe_space = fe_space;
	};

	virtual ~Constraint() = default;

	virtual void apply() = 0;

protected:

	MeshEntity* entity;
	FESpace<T>* fe_space;

private:

};
