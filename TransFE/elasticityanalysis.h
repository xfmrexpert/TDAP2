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
#include "elasticitysc.h"
#include "elasticityfc.h"
#include "displacementconstraint.h"
#include "linearsystemassembler.h"
#include "algebraicsystem.h"

class ElasticityAnalysis : public FEAnalysis<double>{
public:
   ElasticityAnalysis();

   virtual ~ElasticityAnalysis();
   
   virtual void solve();
    
   virtual void recover(); //recover them all important secondary variables
   
   virtual StiffnessContributor<double>* makeStiffContrib(MeshFace* face); //face "element" stiffness
   virtual StiffnessContributor<double>* makeStiffContrib(MeshRegion* region){return nullptr;};
   virtual ForceContributor<double>* makeForceContrib(MeshRegion* region){return nullptr;};
   virtual ForceContributor<double>* makeForceContrib(MeshFace* face); //face body force
   virtual ForceContributor<double>* makeForceContrib(MeshEdge* edge); //edge forces
   virtual Constraint<double>* makeConstraint(MeshFace* face){return nullptr;};
   virtual Constraint<double>* makeConstraint(MeshEdge* edge);
   virtual Constraint<double>* makeConstraint(MeshVertex* vertex);

   virtual void saveOut(const char* filename);

protected:
   Vector<double> stress;

};
