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

#include "DiscreteSystem.h"
#include "Assembler.h"
#include "typedefs.h"  //Matrix, SparseMatrix, Vector
#include "MeshDB/mesh.h"
#include <iostream>

template <typename T>
class AlgebraicSystem{
public:
    AlgebraicSystem(DiscreteSystem<T>* in_DS, Assembler<T>* in_A){
       DS = in_DS;
       A = in_A;
    };

    ~AlgebraicSystem() = default;
	
	void solve() {
		DS->initializeSystem();
		// After this and before formSystem, we need to calculate the dof numbering
		createGlobalSystem();
		// The following just passes in the empty stiffness matrix and force vector
		A->initialize(K.get(), f.get());
		DS->formSystem(*A);
		solveLinearSystem();
	};

	void createGlobalSystem() {
		// TODO: Not sure why this was here.  Maybe reordering should be here?
		// It might be needed to set the equation number
		//field->reorder2();
		K = std::make_unique<BigMatrix<T>>(ndof, ndof);
		K->setZero();
		d = std::make_unique<BigVector<T>>(ndof);
		d->setZero();
		f = std::make_unique<BigVector<T>>(ndof);
		f->setZero();
	};

	void solveLinearSystem() {
		//Solve Kd = f

		// Initialize the solver
		Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Upper> solver;

		// Compute the decomposition to solve Kd = f
		solver.compute(*K);
		if (solver.info() != Eigen::Success) {
			throw std::runtime_error("Decomposition failed: Unable to decompose the matrix K.");
		}

		// Solve the system
		d = std::make_unique<BigVector<T>>(solver.solve(*f));
		if (solver.info() != Eigen::Success) {
			throw std::runtime_error("Solve failed: Unable to solve the linear system.");
		}

		// Output the number of iterations and the estimated error
		std::cout << "# Iterations: " << solver.iterations() << std::endl;
		std::cout << "Estimated error: " << solver.error() << std::endl;

		// Verify the solution
		if (solver.error() > 1e-6) {
			throw std::runtime_error("Solution accuracy is not sufficient.");
		}
	};

	BigVector<T>* get_d() {
		return d.get();
	};

    
protected:
   DiscreteSystem<T>* DS;
   Assembler<T>* A;
   std::unique_ptr<BigMatrix<T>> K;
   std::unique_ptr<BigVector<T>> d;
   std::unique_ptr<BigVector<T>> f;
};
