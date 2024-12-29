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

class MagAxiStaticAnalysis : public FEAnalysis<double> {
public:
	MagAxiStaticAnalysis(int form);

	~MagAxiStaticAnalysis() = default;

	virtual void solve() override;

	virtual void recover() override; //recover them all important secondary variables

	virtual std::unique_ptr<StiffnessContributor<double>> makeStiffContrib(MeshFace& face) override; //face "element" stiffness
	virtual std::unique_ptr<ForceContributor<double>> makeForceContrib(MeshFace& face) override; //face body force
	virtual std::unique_ptr<ForceContributor<double>> makeForceContrib(MeshEdge& edge) override; //edge forces
	virtual std::unique_ptr<Constraint<double>> makeConstraint(MeshEdge& edge) override;
	virtual std::unique_ptr<Constraint<double>> makeConstraint(MeshVertex& vertex) override;

	/// Create the appropriate StiffnessContributor for the face "element"
	virtual std::unique_ptr<StiffnessContributor<double>> makeStiffContrib(MeshRegion& region) override { return nullptr; };

	virtual std::unique_ptr<ForceContributor<double>> makeForceContrib(MeshRegion& region) override { return nullptr; };

	/// Create the appropriate Constraint for a face
	virtual std::unique_ptr<Constraint<double>> makeConstraint(MeshFace& face) override { return nullptr; };

	virtual void saveOut(const std::string& filename) override;

	int formulation;

protected:
	Vector<double> B;

};
