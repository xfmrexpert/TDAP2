/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#include "algebraicsystem.h"
#include <iostream>

 //#include "itl/interface/mtl.h"
 //#include "itl/preconditioner/ssor.h"
 //#include "itl/krylov/qmr.h"
 //#include "itl/krylov/bicgstab.h"
 //
 //using namespace itl;

AlgebraicSystem::~AlgebraicSystem() {

	//	delete d;
	//	delete f;
}

void AlgebraicSystem::solve() {
	DS->initializeSystem();
	createGlobalSystem();
	A->initialize(&K, &f);
	DS->formSystem(A);
	solveLinearSystem();
};

void AlgebraicSystem::createGlobalSystem() {
	// TODO: Not sure why this was here.  Maybe reordering should be here?
    // It might be needed to set the equation number
	mesh->reorder2();
	K = BigMatrix(ndof, ndof);
	d = BigVector(ndof);
	d.setZero();
	f = BigVector(ndof);
	f.setZero();

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
	int max_iter = 100000;

	/*cout << "K = :" << endl;
	for (int i = 0; i < ndof; i++) {
		for (int j = 0; j < ndof; j++) {
			cout << K.coeff(i,j) << " ";
		}
		cout << endl;
	}

	cout << "f = :" << endl;
	for(int i=0;i<ndof; i++){
	 cout << f[i] << endl;
	}*/


	// //SSOR preconditioner
	//SSOR<SparseMatrix> precond(K);
	////identity_preconditioner precond;
	////iteration
	//noisy_iteration<double> iter(f, max_iter, 1e-8);
	////qmr algorithm
	//qmr(K, d, f, precond.left(), precond.right(), iter);
	////end

  //#else
  //  
  //  //SSOR preconditioner
  //  itl::SSOR<SparseMatrix> precond(K);
  //  //iteration
  //  itl::noisy_iteration<double> iter(f, max_iter, 1e-6);
  //  //bicgstab algorithm
  //  itl::bicgstab(K, d, f, precond(), iter);
  //  //end
  //#endif
	//Eigen::BiCGSTAB<Eigen::SparseMatrix<double> > solver;
	//solver.compute(K);
	//  d = solver.solve(f);
	   //std::cout << "#iterations:     " << solver.iterations() << std::endl;
	//  std::cout << "estimated error: " << solver.error() << std::endl;
	//   // update b, and solve again
	//d = solver.solve(f);

	//  Eigen::SparseLU<Eigen::SparseMatrix<double, Eigen::ColMajor>, Eigen::COLAMDOrdering<int> >   solver;
	//  // fill A and b;
	   //// Compute the ordering permutation vector from the structural pattern of A
	//  K.makeCompressed();
	   //solver.analyzePattern(K);
	//  // Compute the numerical factorization 
	   //solver.factorize(K);
	//  //Use the factors to solve the linear system 
	   //  d = solver.solve(f);
	  //d = K.triangularView<Eigen::Upper>().solve(f);

	  Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Upper> solver;
	  d = solver.compute(K).solve(f);

	 /*cout << "d = :" << endl;
	 for(int i=0;i<ndof; i++){
	  cout << d[i] << endl;
	 }*/
};

BigVector AlgebraicSystem::get_d() {
	return d;
};
