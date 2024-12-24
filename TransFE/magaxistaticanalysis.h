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

#include "feanalysis.h"
#include "magaxistaticsc.h"
#include "magaxistaticfc.h"
#include "displacementconstraint.h"
#include "linearsystemassembler.h"
#include "algebraicsystem.h"

class MagAxiStaticAnalysis : public FEAnalysis {
public:
	MagAxiStaticAnalysis(int form);

	virtual ~MagAxiStaticAnalysis();

	virtual void solve();

	virtual void recover(); //recover them all important secondary variables

	virtual shared_ptr<StiffnessContributor> makeStiffContrib(MeshFace& face); //face "element" stiffness
	virtual shared_ptr<ForceContributor> makeForceContrib(MeshFace& face); //face body force
	virtual shared_ptr<ForceContributor> makeForceContrib(MeshEdge& edge); //edge forces
	virtual shared_ptr<Constraint> makeConstraint(MeshEdge& edge);
	virtual shared_ptr<Constraint> makeConstraint(MeshVertex& vertex);

	/// Create the appropriate StiffnessContributor for the face "element"
	virtual shared_ptr<StiffnessContributor> makeStiffContrib(MeshRegion& region) { return nullptr; };

	virtual shared_ptr<ForceContributor> makeForceContrib(MeshRegion& region) { return nullptr; };

	/// Create the appropriate Constraint for a face
	virtual shared_ptr<Constraint> makeConstraint(MeshFace& face) { return nullptr; };

	virtual void saveOut(const char* filename);

	int formulation;

protected:
	Vector<double> B;

};
