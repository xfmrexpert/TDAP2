/************************************************************************
			meshface.h - Copyright T. C. Raymond


**************************************************************************/

#ifndef MESHFACE_H
#define MESHFACE_H

#include "meshentity.h"
#include "meshvertex.h"
#include <vector>

using namespace std;

class MeshEdge;
class MeshRegion;

/// This class represents a mesh face.  A mesh face is a 2D object bounded by edges.
/// A mesh face bounds mesh regions. This class is derived from MeshEntity.
/// Note that adjacencies are stored using a one-level representation.

class MeshFace : public MeshEntity {

public:

	/// Default constructor.  Currently empty.
	MeshFace() {};

	~MeshFace() {};

	/// Returns an iterator for the first edge
	vector<shared_ptr<MeshEdge>>::iterator getFirstEdge();

	/// Returns an iterator for the last edge
	vector<shared_ptr<MeshEdge>>::iterator getLastEdge();

	/// Returns the n-th edge bounding this face (unordered)
	shared_ptr<MeshEdge> getEdge(int n);

	/// Returns the n-th region that this face bounds (unordered)
	shared_ptr<MeshRegion> getRegion(int n);

	/// Adds an edge to the list of adjacent edges
	void addEdge(shared_ptr<MeshEdge> new_edge);

	/// Adds a region to the list of adjacent regions
	void addRegion(shared_ptr<MeshRegion> new_region);

	/// Returns the number of edges bounding this face
	size_t numEdges();

	/// Returns the area of the face
	double area();

	int get_dimensions() const {
		return 2;
	}

	/// Get an ordered listing of the nodes
	/// vertex nodes 1st, edge nodes 2nd, face node last
	virtual vector<shared_ptr<Node>> getNodes();

	/// Get an ordered listing of the face vertexes (CCW)
	vector<shared_ptr<MeshVertex>> getVertexes();

	//Containers for adjacency information (via pointers)
	 /// List of edges attached to this face (currently unordered)
	vector<shared_ptr<MeshEdge>> MeshEdges;

protected:

private:

	/// indicates that faces are 2D objects
	static const int dimensions = 2;



	/// Always 2 regions per face, one on each side
	vector<shared_ptr<MeshRegion>> MeshRegions;

};
#endif //MESHFACE_H

