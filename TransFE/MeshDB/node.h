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

#include "../dof.h"
#include "point.h"
#include <vector>
#include <memory>

class MeshEntity;

/// This class represents a Node.  A node is the geometric (or parametric?)
/// manifestation of a point on the mesh.  A node may also be termed a
/// "degree of freedom holder".  Each vertex must have one node.  In addition, edges
/// may also have a node for 2nd order elements.

class Node {

public:
	/// Default constructor.
	Node() = default;

	~Node() = default;

	/// The x-coordinates
	double x() const {
		return m_pt.X(0);
	}

	/// The y-coordinate
	double y() const {
		return m_pt.X(1);
	}

	/// The z-coordinate
	double z() const {
		return m_pt.X(2);
	}

	/// Set the nodes point
	void pt(const point& in_pt) {
		m_pt = in_pt;
	}

	///Retrieve the nodes point
	const point& pt() const {
		return m_pt;
	}

	size_t getID() const {
		return ID;
	}

	void setID(size_t in_ID) {
		ID = in_ID;
	}

	/// The equals operator is overridden in attempt to provide a meaningful definition
	friend bool operator== (const Node&, const Node&);

	/// Returns ordering based upon the x-dimension
	friend bool operator< (const Node&, const Node&);

	/// Return a vector with pointers to the nodal DOFs
	//std::vector<DOF<T>*> getDOFs() const;

	/// Create a new DOF for this node
	//DOF<T>* newDOF();

	MeshEntity* getParent() const {
		return parent;
	}

	void setParent(MeshEntity* in_parent) {
		parent = in_parent;
	}

protected:

private:
	/// An optional integer ID
	size_t ID = -9999;

	/// A vector with pointers to all of the nodal DOFs
	/// This vector owns the DOFs for this node
	//std::vector<std::unique_ptr<DOF<T>>> DOFs;
	
	/// A point for the spatial coordinates of this node
	point m_pt;

	MeshEntity* parent = nullptr;

};

