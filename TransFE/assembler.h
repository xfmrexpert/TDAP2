/***************************************************************************
 *   Copyright (C) 2005-2025 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   Use of this source code is governed by an MIT-style                   *
 *   license that can be found in the LICENSE.txt file or at               *
 *   https://opensource.org/licenses/MIT.                                  *
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
//#include "vector.h"
#include <vector>

/// This class represents a generic assembler. 

template<typename T>
class Assembler {

public:

   void initialize(BigMatrix<T>* K_in, BigVector<T>* f_in){
      K = K_in;
      f = f_in;
   };

   virtual void accept(Matrix<T> k, std::vector<DOF<T>*> dofs) = 0;   

   virtual void accept(Vector<T> k, std::vector<DOF<T>*> dofs) = 0;
  
protected:
  
   BigMatrix<T>* K;
   BigVector<T>* f;

private:

};

