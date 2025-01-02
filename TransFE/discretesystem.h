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

#include "Constraint.h"
#include "Assembler.h"
#include "BilinearForm.h"
#include "LinearForm.h"
#include <list>

/// This class represents a generic discrete system. 
template <typename T>
class DiscreteSystem {

public:

    DiscreteSystem(std::unique_ptr<FESpace<T>> fe_space, std::unique_ptr<BilinearForm<T>> bilinear, std::unique_ptr<LinearForm<T>> linear) 
        : fe_space(std::move(fe_space)), bilinearForm(std::move(bilinear)), linearForm(std::move(linear)) { };
	~DiscreteSystem() = default;

   void add(std::unique_ptr<Constraint<T>> c) {
       constraints.push_back(std::move(c));
   };

   void initializeSystem() {
       for (const auto& constraint : constraints) {
           constraint->apply(); //each essential boundary condition will eliminate possible dof from the global system
           //in the case of non-zero essential boundary conditions must also get the non-zero value
       }
   }

   void formSystem(Assembler<T>& assem) {
       fe_space->numberDOFs();
       // DOFs need to be re-numbering before we hit here.  At present, DiscreteSystem doesn't have a pointer to the FESpace.
	   bilinearForm->Assemble(assem);
	   linearForm->Assemble(assem);
   }
  
protected:
  
private:
   std::unique_ptr<FESpace<T>> fe_space;
   std::unique_ptr<BilinearForm<T>> bilinearForm;
   std::unique_ptr<LinearForm<T>> linearForm;
   std::list<std::unique_ptr<Constraint<T>>> constraints;

};

