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

#include <vector>
#include "./Eigen/Dense"
#include "./Eigen/Sparse"
#include "./Eigen/IterativeLinearSolvers"

typedef Eigen::SparseMatrix<double> BigMatrix;
typedef Eigen::VectorXd BigVector;
