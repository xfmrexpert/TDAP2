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

	DiscreteSystem(){};
	~DiscreteSystem();

   void add(shared_ptr<StiffnessContributor>);
   void add(shared_ptr<ForceContributor>);
   void add(shared_ptr<Constraint>);
   void initializeSystem();
   void formSystem(Assembler*);
  
protected:
  
private:

   list<shared_ptr<StiffnessContributor>> StiffnessContributors;
   list<shared_ptr<ForceContributor>> ForceContributors;
   list<shared_ptr<Constraint>> Constraints;

};

