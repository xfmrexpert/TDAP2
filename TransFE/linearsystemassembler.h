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

#include "assembler.h"
#include <vector>
#include <memory>
#include "dof.h"
#include "typedefs.h"
#include "matrix.h"
#include "vector.h"

template <typename T>
class DOF;

/// This class represents a linear system assembler. 
template <typename T>
class LinearSystemAssembler : public Assembler<T>{
public:
   virtual void accept(Matrix<T>, std::vector<DOF<T>*> dofs);   
   virtual void accept(Vector<T>, std::vector<DOF<T>*> dofs);
protected:
private:
};

