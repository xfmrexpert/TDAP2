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

#include "discretesystem.h"
#include "assembler.h"
#include "typedefs.h"  //Matrix, SparseMatrix, Vector
#include "MeshDB/mesh.h"

class AlgebraicSystem{
public:
    AlgebraicSystem(DiscreteSystem* in_DS, Assembler* in_A, Mesh* in_mesh){
       DS = in_DS;
       A = in_A;
       mesh = in_mesh;
    };

    ~AlgebraicSystem() = default;
    
    void solve();
    
    void createGlobalSystem();
    
    void solveLinearSystem();
    
    BigVector* get_d();
    
protected:
   DiscreteSystem* DS;
   Assembler* A;
   std::unique_ptr<BigMatrix> K;
   std::unique_ptr<BigVector> d;  //Save myself some grief and use MTL vectors for now
   std::unique_ptr<BigVector> f;
   Mesh* mesh;

};
