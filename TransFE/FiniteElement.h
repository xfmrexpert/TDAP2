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

#include "MeshDB/point.h"
#include "typedefs.h"

class FiniteElement {

public:
	virtual ~FiniteElement() = default;

	virtual int referenceDimensions() const = 0;
	virtual int spatialDimensions() const = 0;

	virtual int numLocalDOFs() const = 0;

	virtual Vector<double> N(const point& pt) const = 0;
	virtual Matrix<double> grad_N(const point& pt) const = 0;
	virtual Vector<double> curl_N(const point& pt) const = 0;

protected:

};
