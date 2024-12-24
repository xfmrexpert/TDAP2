/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
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

class DOF;

/// This class represents a linear system assembler. 

class LinearSystemAssembler : public Assembler{

public:

   virtual void accept(Matrix<double>, std::vector<DOF*> dofs);   

   virtual void accept(Vector<double>, std::vector<DOF*> dofs);
  
protected:
  
private:

};

