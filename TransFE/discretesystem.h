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

#include "stiffnesscontributor.h"
#include "forcecontributor.h"
#include "constraint.h"
#include "assembler.h"
#include <list>

/// This class represents a generic discrete system. 
template <typename T>
class DiscreteSystem {

public:

	DiscreteSystem() = default;
	~DiscreteSystem() = default;

   void add(std::unique_ptr<StiffnessContributor<T>>);
   void add(std::unique_ptr<ForceContributor<T>>);
   void add(std::unique_ptr<Constraint<T>>);
   void initializeSystem();
   void formSystem(Assembler<T>*);
  
protected:
  
private:

   std::list<std::unique_ptr<StiffnessContributor<T>>> StiffnessContributors;
   std::list<std::unique_ptr<ForceContributor<T>>> ForceContributors;
   std::list<std::unique_ptr<Constraint<T>>> Constraints;

};

