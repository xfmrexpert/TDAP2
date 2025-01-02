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
#include "MeshDB/meshentity.h"
#include "typedefs.h"

class ElementTransform {

public:
	ElementTransform() {};
	virtual ~ElementTransform() = default;
	virtual Matrix<double> Jacobian(const point& pt, const MeshEntity& entity, const Matrix<double>& dGds) const = 0;

protected:

	uint8_t nsd; //# of spatial dimensions
	uint8_t npd; //# of parametric dimensions
};