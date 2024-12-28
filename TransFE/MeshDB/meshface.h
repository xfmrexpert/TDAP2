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

#include "meshentity.h"
#include "meshvertex.h"
#include <vector>

class MeshEdge;
class MeshRegion;

/// This class represents a mesh face.  A mesh face is a 2D object bounded by edges.
/// A mesh face bounds mesh regions. This class is derived from MeshEntity.
/// Note that adjacencies are stored using a one-level representation.

class MeshFace : public MeshEntity {

public:

	/// Default constructor.  Currently empty.
	MeshFace() : MeshEntity() {};

	~MeshFace() {};

	/// Returns an const reference to the vector of edges
	const std::vector<MeshEdge*>& Edges() const;

	/// Returns the n-th edge bounding this face (unordered)
	MeshEdge* getEdge(size_t n);

	/// Returns the n-th region that this face bounds (unordered)
	MeshRegion* getRegion(size_t n);

	/// Adds an edge to the list of adjacent edges
	void addEdge(MeshEdge& new_edge);

	/// Adds a region to the list of adjacent regions
	void addRegion(MeshRegion& new_region);

	/// Returns the number of edges bounding this face
	size_t numEdges();

	/// Returns the area of the face
	double area();

	int get_dimensions() const override {
		return 2;
	}

	/// Get an ordered listing of the nodes
	/// vertex nodes 1st, edge nodes 2nd, face node last
	std::vector<Node*> getNodes() const override;

	/// Get an ordered listing of the face vertices (CCW)
	std::vector<const MeshVertex*> getVertices() const;

	//Containers for adjacency information (via pointers)
	 /// List of edges attached to this face (currently unordered)
	std::vector<MeshEdge*> MeshEdges;

protected:

private:

	/// indicates that faces are 2D objects
	static const int dimensions = 2;



	/// Always 2 regions per face, one on each side
	std::vector<MeshRegion*> MeshRegions;

};

