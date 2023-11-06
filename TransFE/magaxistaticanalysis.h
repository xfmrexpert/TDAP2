/***************************************************************************
 *   Copyright (C) 2005-2023 by T. C. Raymond                              *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef MAGAXISTATICANALYSIS_H
#define MAGAXISTATICANALYSIS_H

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

	virtual shared_ptr<StiffnessContributor> makeStiffContrib(shared_ptr<MeshFace> face); //face "element" stiffness
	virtual shared_ptr<ForceContributor> makeForceContrib(shared_ptr<MeshFace> face); //face body force
	virtual shared_ptr<ForceContributor> makeForceContrib(shared_ptr<MeshEdge> edge); //edge forces
	virtual shared_ptr<Constraint> makeConstraint(shared_ptr<MeshEdge> edge);
	virtual shared_ptr<Constraint> makeConstraint(shared_ptr<MeshVertex> vertex);

	/// Create the appropriate StiffnessContributor for the face "element"
	virtual shared_ptr<StiffnessContributor> makeStiffContrib(shared_ptr<MeshRegion> region) { return NULL; };

	virtual shared_ptr<ForceContributor> makeForceContrib(shared_ptr<MeshRegion> region) { return NULL; };

	/// Create the appropriate Constraint for a face
	virtual shared_ptr<Constraint> makeConstraint(shared_ptr<MeshFace> face) { return NULL; };

	virtual void saveOut(const char* filename);

	int formulation;

protected:
	Vector<double> B;

};

#endif
