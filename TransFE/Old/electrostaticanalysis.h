/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef ELECTROSTATICANALYSIS_H
#define ELECTROSTATICANALYSIS_H

#include "feanalysis.h"
#include "electrostaticsc.h"
#include "electrostaticfc.h"
#include "displacementconstraint.h"
#include "linearsystemassembler.h"
#include "algebraicsystem.h"

class ElectroStaticAnalysis : public FEAnalysis{
public:
   ElectroStaticAnalysis();

   virtual ~ElectroStaticAnalysis();
   
   virtual void solve();
    
   virtual void recover(); //recover them all important secondary variables
   
   virtual StiffnessContributor* makeStiffContrib(MeshFace* face); //face "element" stiffness
   virtual ForceContributor* makeForceContrib(MeshFace* face); //face body force
   virtual ForceContributor* makeForceContrib(MeshEdge* edge); //edge forces
   virtual Constraint* makeConstraint(MeshEdge* edge);
   virtual Constraint* makeConstraint(MeshVertex* vertex);

   /// Create the appropriate StiffnessContributor for the face "element"
   virtual StiffnessContributor* makeStiffContrib(MeshRegion* region){return NULL;};

   virtual ForceContributor* makeForceContrib(MeshRegion* region){return NULL;};

   /// Create the appropriate Constraint for a face
   virtual Constraint* makeConstraint(MeshFace* face){return NULL;};

   virtual void saveOut(const char* filename);

protected:
	Matrix<double> E;
	Matrix<double> D;

};

#endif
