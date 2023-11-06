/************************************************************************
			meshvertex.h - Copyright T. C. Raymond

**************************************************************************/

#ifndef MESHVERTEX_H
#define MESHVERTEX_H

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

	/// Returns an iterator to the first edge
	vector<shared_ptr<MeshEdge>>::iterator getFirstEdge();

	/// Returns an iterator to the last edge
	vector<shared_ptr<MeshEdge>>::iterator getLastEdge();

	/// Returns the n-th edge
	shared_ptr<MeshEdge> getEdge(int n);

	/// Adds an edge to the list of adjacent edges
	void addEdge(shared_ptr<MeshEdge>);

	/// Returns the number of adjacent edges
	size_t numEdges();

	/// Returns a set of the edges that this vertex bounds
	set<shared_ptr<MeshEdge>> getEdges();

	int get_dimensions() const {
		return 0;
	}

	/// Returns an ordered list of nodes for this vertex
	/// Not much to do here since there is only one node per vertex
	virtual vector<shared_ptr<Node>> getNodes();

protected:

private:
	/// Indicates that a vertex is 0D
	static const int dimensions = 0;

	/// An STL Vector holding pointers to edges using this vertex (currently unordered)
	vector<shared_ptr<MeshEdge>> MeshEdges;

};
#endif //MESHVERTEX_H

