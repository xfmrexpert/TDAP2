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
#include <vector>
#include <set>
#include <memory>

using namespace std;

class MeshEdge;

/// This class represents a mesh vertex. A vertex is the lowest entity in the hierarchy.
/// Each edge is bounded by two vertices.  A vertex does not contain geometric data directly.
/// This information is contained by a node.

class MeshVertex : public MeshEntity {

public:

	/// Default constructor.  Currently empty.
	MeshVertex() {};

	~MeshVertex() {};

	/// Returns an const reference to the vector of edges
	const vector<MeshEdge*>& Edges() const;

	/// Returns the n-th edge
	MeshEdge* getEdge(int n);

	/// Adds an edge to the list of adjacent edges
	void addEdge(MeshEdge&);

	/// Returns the number of adjacent edges
	size_t numEdges();

	int get_dimensions() const override {
		return 0;
	}

	/// Returns an ordered list of nodes for this vertex
	/// Not much to do here since there is only one node per vertex
	vector<Node*> getNodes() const override;

protected:

private:
	/// Indicates that a vertex is 0D
	static const int dimensions = 0;

	/// An STL Vector holding pointers to edges using this vertex (currently unordered)
	vector<MeshEdge*> MeshEdges;

};


