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

#include "constraint.h"
#include "MeshDB/meshedge.h"
#include "mapping.h"
#include "shapefunction.h"


/// This class represents a displacement constraint. 

class DisplacementConstraint : public Constraint {

public:

	DisplacementConstraint(MeshEntity* Element_in) : Constraint(Element_in) {};

	~DisplacementConstraint();

	void apply();

protected:

private:

};


