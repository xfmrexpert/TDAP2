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
#include "MeshDB/meshentity.h"

/// This class represents a displacement constraint. 
template <typename T>
class DisplacementConstraint : public Constraint<T> {

public:

	DisplacementConstraint(MeshEntity* Element_in, Field<T>* Field_in) : Constraint<T>(Element_in, Field_in) {};

	~DisplacementConstraint() = default;

	void apply();

protected:

private:

};


