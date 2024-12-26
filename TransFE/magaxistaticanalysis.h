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

	virtual void solve() override;

	virtual void recover() override; //recover them all important secondary variables

	virtual shared_ptr<StiffnessContributor> makeStiffContrib(MeshFace& face) override; //face "element" stiffness
	virtual shared_ptr<ForceContributor> makeForceContrib(MeshFace& face) override; //face body force
	virtual shared_ptr<ForceContributor> makeForceContrib(MeshEdge& edge) override; //edge forces
	virtual shared_ptr<Constraint> makeConstraint(MeshEdge& edge) override;
	virtual shared_ptr<Constraint> makeConstraint(MeshVertex& vertex) override;

	/// Create the appropriate StiffnessContributor for the face "element"
	virtual shared_ptr<StiffnessContributor> makeStiffContrib(MeshRegion& region) override { return nullptr; };

	virtual shared_ptr<ForceContributor> makeForceContrib(MeshRegion& region) override { return nullptr; };

	/// Create the appropriate Constraint for a face
	virtual shared_ptr<Constraint> makeConstraint(MeshFace& face) override { return nullptr; };

	virtual void saveOut(const std::string& filename) override;

	int formulation;

protected:
	Vector<double> B;

};
