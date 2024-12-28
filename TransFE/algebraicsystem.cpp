/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "algebraicsystem.h"
#include <iostream>

void AlgebraicSystem::solve() {
	DS->initializeSystem();
	createGlobalSystem();
	A->initialize(K.get(), f.get());
	DS->formSystem(A);
	solveLinearSystem();
};

void AlgebraicSystem::createGlobalSystem() {
	// TODO: Not sure why this was here.  Maybe reordering should be here?
    // It might be needed to set the equation number
	mesh->reorder2();
	K = std::make_unique<BigMatrix>(ndof, ndof);
	K->setZero();
	d = std::make_unique<BigVector>(ndof);
	d->setZero();
	f = std::make_unique<BigVector>(ndof);
	f->setZero();

	/*cout << endl << "Initial Values:" << endl;
	cout << "K = :" << endl;
	for (int i = 0; i < ndof; i++) {
		for (int j = 0; j < ndof; j++) {
			cout << K.coeff(i, j) << " ";
		}
		cout << endl;
	}

	cout << "f = :" << endl;
	for (int i = 0; i < ndof; i++) {
		cout << f[i] << endl;
	}*/
};

void AlgebraicSystem::solveLinearSystem() {
	//Solve Kd = f
	
	// Initialize the solver
	Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Upper> solver;

	// Compute the decomposition to solve Kd = f
	solver.compute(*K);
	if (solver.info() != Eigen::Success) {
		throw std::runtime_error("Decomposition failed: Unable to decompose the matrix K.");
	}

	// Solve the system
	d = std::make_unique<BigVector>(solver.solve(*f));
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

BigVector* AlgebraicSystem::get_d() {
	return d.get();
};
