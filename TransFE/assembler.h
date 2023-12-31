/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

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

   virtual void accept(Matrix<double> k, std::vector<std::shared_ptr<DOF>> dofs) = 0;   

   virtual void accept(Vector<double> k, std::vector<std::shared_ptr<DOF>> dofs) = 0;
  
protected:
  
   BigMatrix* K;
   BigVector* f;

private:

};
#endif //ASSEMBLER_H

