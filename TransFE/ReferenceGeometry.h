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

#include "typedefs.h"
#include "MeshDB/point.h"
#include "MeshDB/meshentity.h"
#include "MeshDB/meshedge.h"

class ReferenceGeometry {
public:
	virtual ~ReferenceGeometry() {}

	// Returns the ordered set of node points
	virtual std::vector<point> getOrderedNodes() const = 0;

	// Returns the ordered set of edges
	virtual std::vector<std::pair<int, int>> getOrderedEdges() const = 0;

	// Validates and optionally corrects the orientation of the given entity
	virtual bool validateOrientation(const MeshEntity& entity) const = 0;

	// Factory method to create the appropriate subclass
    static std::unique_ptr<ReferenceGeometry> create(const MeshEntity& entity) {
        //switch (entity.getGeometryType()) {
        //case GeometryType::Point:
        //    return std::make_unique<PointReferenceGeometry>();
        //case GeometryType::Segment:
        //    return std::make_unique<SegmentReferenceGeometry>(entity.getOrder());
        //case GeometryType::Triangle:
        //    return std::make_unique<TriangleReferenceGeometry>(entity.getOrder());
        //case GeometryType::Quadrilateral:
        //    return std::make_unique<QuadrilateralReferenceGeometry>(entity.getOrder());
        //    // Add cases for higher dimensions (tetrahedron, hexahedron) as needed
        //default:
        //    throw std::invalid_argument("Unsupported geometry type");
        //}
    }

};
