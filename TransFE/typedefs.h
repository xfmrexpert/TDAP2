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

template <typename T>
using BigMatrix =  Eigen::SparseMatrix<T>;

template <typename T>
using BigVector = Eigen::VectorX<T>;
