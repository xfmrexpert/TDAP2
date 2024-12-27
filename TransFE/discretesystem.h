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

class DiscreteSystem {

public:

	DiscreteSystem() = default;
	~DiscreteSystem() = default;

   void add(unique_ptr<StiffnessContributor>);
   void add(unique_ptr<ForceContributor>);
   void add(unique_ptr<Constraint>);
   void initializeSystem();
   void formSystem(Assembler*);
  
protected:
  
private:

   std::list<std::unique_ptr<StiffnessContributor>> StiffnessContributors;
   std::list<std::unique_ptr<ForceContributor>> ForceContributors;
   std::list<std::unique_ptr<Constraint>> Constraints;

};

