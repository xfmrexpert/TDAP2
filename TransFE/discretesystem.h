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

//#include "stiffnesscontributor.h"
//#include "forcecontributor.h"
#include "constraint.h"
#include "assembler.h"
#include <list>

/// This class represents a generic discrete system. 
template <typename T>
class DiscreteSystem {

public:

	DiscreteSystem() = default;
	~DiscreteSystem() = default;

   /*void add(std::unique_ptr<StiffnessContributor<T>> sc) {
       StiffnessContributors.push_back(std::move(sc));
   };

   void add(std::unique_ptr<ForceContributor<T>> fc) {
       ForceContributors.push_back(std::move(fc));
   };*/

   void add(std::unique_ptr<Constraint<T>> c) {
       Constraints.push_back(std::move(c));
   };

   void initializeSystem() {
       for (const auto& constraint : Constraints) {
           constraint->apply(); //each essential boundary condition will eliminate possible dof from the global system
           //in the case of non-zero essential boundary conditions must also get the non-zero value
       }
   }

   void formSystem(Assembler<T>* assem) {
       /*for (const auto& FC : ForceContributors) {
           FC->evaluate(assem);
       }

       for (const auto& SC : StiffnessContributors) {
           SC->evaluate(assem);
       }*/
   }
  
protected:
  
private:

   /*std::list<std::unique_ptr<StiffnessContributor<T>>> StiffnessContributors;
   std::list<std::unique_ptr<ForceContributor<T>>> ForceContributors;*/
   std::list<std::unique_ptr<Constraint<T>>> Constraints;

};

