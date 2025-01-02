/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "lintrife.h"

Vector<double> LinTriFE::N(const point& pt) {
	Vector<double> mN(3);
	double r = pt.x;
	double s = pt.y;
	double t = 1.0 - r - s;
	mN[0] = r;
	mN[1] = s;
	mN[2] = t;
	return mN;
};

Matrix<double> LinTriFE::dNds(const point& pt) {
	Matrix<double> dN(3, 2);
	double r = pt.x;
	double s = pt.y;
	dN(0, 0) = 1.;
	dN(0, 1) = 0.;
	dN(1, 0) = 0.;
	dN(1, 1) = 1.;
	dN(2, 0) = -1.;
	dN(2, 1) = -1.;
	return dN;
};

