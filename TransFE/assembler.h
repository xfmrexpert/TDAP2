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

#include "dof.h"
#include "typedefs.h"
#include "matrix.h"
#include "vector.h"
#include <vector>

/// This class represents a generic assembler. 

class Assembler {

public:

   void initialize(BigMatrix* K_in, BigVector* f_in){
      K = K_in;
      f = f_in;
   };

   virtual void accept(Matrix<double> k, std::vector<DOF*> dofs) = 0;   

   virtual void accept(Vector<double> k, std::vector<DOF*> dofs) = 0;
  
protected:
  
   BigMatrix* K;
   BigVector* f;

private:

};

