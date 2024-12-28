/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "discretesystem.h"

void DiscreteSystem::add(std::unique_ptr<StiffnessContributor> sc) {
   StiffnessContributors.push_back(std::move(sc));
};

void DiscreteSystem::add(std::unique_ptr<ForceContributor> fc) {
   ForceContributors.push_back(std::move(fc));
};

void DiscreteSystem::add(std::unique_ptr<Constraint> c) {
   Constraints.push_back(std::move(c));
};

void DiscreteSystem::initializeSystem(){
   for(const auto& constraint : Constraints){
      constraint->apply(); //each essential boundary condition will eliminate possible dof from the global system
                               //in the case of non-zero essential boundary conditions must also get the non-zero value
   }
}

void DiscreteSystem::formSystem(Assembler* assem){
   for(const auto& FC : ForceContributors){
      FC->evaluate(assem);
   }
      
   for(const auto& SC : StiffnessContributors){
      SC->evaluate(assem);
   }
}
