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
#include "IntegrationRule.h"

class LinTriIntegrationRule : public IntegrationRule {
public:
	const std::vector<point>& IntPts() const override {
	static const std::vector<point> int_pts = []() {
		std::vector<point> points(3);
		points[0] = point(2.0 / 3.0, 1.0 / 6.0);
		points[1] = point(1.0 / 6.0, 1.0 / 6.0);
		points[2] = point(1.0 / 6.0, 2.0 / 3.0);
		return points;
		}();
	return int_pts;
}

	const Vector<double>& Weights() const override {
		static const Vector<double> weights = []() {
			Vector<double> w(3);
			w[0] = 1.0 / 6.0;
			w[1] = 1.0 / 6.0;
			w[2] = 1.0 / 6.0;
			return w;
			}();
		return weights;
	}

	int numIntPts() const override {
		return 3;
	};


};
