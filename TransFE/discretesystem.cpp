/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "discretesystem.h"
//#include "precisiontimer.h"

DiscreteSystem::~DiscreteSystem(){
	
};

void DiscreteSystem::add(shared_ptr<StiffnessContributor> sc){
   StiffnessContributors.push_back(sc);
};

void DiscreteSystem::add(shared_ptr<ForceContributor> fc){
   ForceContributors.push_back(fc);
};

void DiscreteSystem::add(shared_ptr<Constraint> c){
   Constraints.push_back(c);
};

void DiscreteSystem::initializeSystem(){
   for(auto constraint : Constraints){
      constraint->apply(); //each essential boundary condition will eliminate possible dof from the global system
                               //in the case of non-zero essential boundary conditions must also get the non-zero value
   }
}

void DiscreteSystem::formSystem(Assembler* assem){
   for(auto FC : ForceContributors){
      FC->evaluate(assem);
   }
      
   for(auto SC : StiffnessContributors){
      SC->evaluate(assem);
   }
}
