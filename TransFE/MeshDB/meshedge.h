/************************************************************************
			meshedge.h - Copyright T. C. Raymond


**************************************************************************/

#ifndef MESHEDGE_H
#define MESHEDGE_H

#include "meshentity.h"
#include <vector>
#include <set>

using namespace std;

class MeshVertex;
class MeshFace;

/// This class represents a mesh edge.  A mesh edge is a 1D object bounded by two
/// vertices.  A mesh edge bounds mesh faces.  This class is derived from MeshEntity.
/// Note that adjacencies are stored using a one-level representation and that use
/// entities are included.

class MeshEdge : public MeshEntity {

public:

	/// Default constructor.  Currently empty.
	MeshEdge() {};

	~MeshEdge() {};

	/// Returns a pointer to the n-th vertex of this edge (either 0 or 1)
	shared_ptr<MeshVertex> getVertex(int n);

	/// Returns an iterator to the first face that this edge bounds
	vector<shared_ptr<MeshFace>>::iterator getFirstFace();

	/// Returns an iterator to the last face that this edge bounds
	vector<shared_ptr<MeshFace>>::iterator getLastFace();

	/// Returns a pointer to the n-th face that this edge bounds
	shared_ptr<MeshFace> getFace(int n);

	/// Adds a vertex to the adjacency information for this edge
	void addVertex(shared_ptr<MeshVertex> new_vertex, int n);

	/// Adds a face to the adjacency information for this edge (should be deleted)
	void addFace(shared_ptr<MeshFace> new_face);

	/// Get a list of all mesh faces that this edge bounds
	set<shared_ptr<MeshFace>> getFaces();

	/// Returns the other vertex for this edge
	shared_ptr<MeshVertex> otherVertex(shared_ptr<MeshVertex>);

	/// Returns whether or not this edge is connected to the given edge
	bool isConnected(shared_ptr<MeshEdge>);

	/// Returns whether or not the given vertex bounds this edge
	bool isConnected(shared_ptr<MeshVertex>);

	int get_dimensions() const {
		return 1;
	}

	/// Returns an ordered list of nodes for this edge
	/// Vertex nodes 1st, edge node 2nd
	virtual vector<shared_ptr<Node>> getNodes();

protected:

private:

	/// Indicates the "level" or dimensions of edges, may be used someday
	static const int dimensions = 1;

	/// A constant-sized array to hold pointers to each vertex of this edge
	shared_ptr<MeshVertex> MeshVertexes[2];

	/// An STL Vector for holding pointers to Face adjacenecies
	vector<shared_ptr<MeshFace>> MeshFaces;

};
#endif //MESHEDGE_H

