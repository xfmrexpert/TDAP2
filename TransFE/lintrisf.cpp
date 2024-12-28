/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "lintrisf.h"

Vector<double> LinTriSF::N(const point& pt) {
	Vector<double> mN(3);
	double r = pt.x;
	double s = pt.y;
	double t = 1.0 - r - s;
	mN[0] = r;
	mN[1] = s;
	mN[2] = t;
	return mN;
};

Matrix<double> LinTriSF::dNds(const point& pt) {
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

const std::vector<point>& LinTriSF::IntPts() const {
	static const std::vector<point> int_pts = []() {
		std::vector<point> points(3);
		points[0] = point(2.0 / 3.0, 1.0 / 6.0);
		points[1] = point(1.0 / 6.0, 1.0 / 6.0);
		points[2] = point(1.0 / 6.0, 2.0 / 3.0);
		return points;
		}();
	return int_pts;
}

const Vector<double>& LinTriSF::Weights() const {
	static const Vector<double> weights = []() {
		Vector<double> w(3);
		w[0] = 1.0 / 6.0;
		w[1] = 1.0 / 6.0;
		w[2] = 1.0 / 6.0;
		return w;
		}();
	return weights;
}

int LinTriSF::numIntPts() const {
	return 3;
};
