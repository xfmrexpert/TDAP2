/************************************************************************
			node.h - Copyright T. C. Raymond

**************************************************************************/

#ifndef NODE_H
#define NODE_H

#include "../dof.h"
#include "point.h"
#include <vector>
#include <memory>

using namespace std;

/// This class represents a Node.  A node is the geometric (or parametric?)
/// manifestation of a point on the mesh.  A node may also be termed a
/// "degree of freedom holder".  Each vertex must have one node.  In addition, edges
/// may also have a node for 2nd order elements.

class Node {

public:

	/// The x-coordinates
	double x() {
		return m_pt.X(0);
	}

	/// The y-coordinate
	double y() {
		return m_pt.X(1);
	}

	/// The z-coordinate
	double z() {
		return m_pt.X(2);
	}

	/// Set the nodes point
	void pt(point in_pt) {
		m_pt = in_pt;
	}

	///Retrieve the nodes point
	point pt() {
		return m_pt;
	}

	/// An optional integer ID
	long long ID = -9999;

	/// Default constructor.
	Node() { }

	~Node();

	/// The equals operator is overridden in attempt to provide a meaningful definition
	friend bool operator== (Node&, Node&);

	/// Returns ordering based upon the x-dimension
	friend bool operator< (Node&, Node&);

	/// Return a vector with pointers to the nodal DOFs
	const vector<shared_ptr<DOF>>& getDOFs();

	/// Create a new DOF for this node
	shared_ptr<DOF> newDOF();

protected:

private:
	/// A vector with pointers to all of the nodal DOFs
	vector<shared_ptr<DOF>> DOFs;
	
	/// A point for the spatial coordinates of this node
	point m_pt;

};
#endif //NODE_H

