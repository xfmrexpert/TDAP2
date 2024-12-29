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

#include "dof.h"
#include "MeshDB/node.h"
#include "MeshDB/mesh.h"
#include <vector>
#include <memory>

/// This class represents a Node.  A node is the geometric (or parametric?)
/// manifestation of a point on the mesh.  A node may also be termed a
/// "degree of freedom holder".  Each vertex must have one node.  In addition, edges
/// may also have a node for 2nd order elements.

template <typename T>
class Field {

public:

	Field() = default;
	~Field() = default;

	Field<T>(Mesh* mesh_in, int nnd_in) : mesh(mesh_in), nnd(nnd_in) {
		initializeField();
	}

	std::vector<DOF<T>*> getDOFsForEntity(const MeshEntity& entity);
	std::vector<DOF<T>*> getDOFsForNode(const Node& node);

	void initializeField();

	/// a Reverse Cuthill-McKee reordering algorithm
	//void reorder();

	//void reorder2();

private:
	Mesh* mesh;
	std::vector<std::vector<std::unique_ptr<DOF<T>>>> DOFs;
	int nnd;

};


