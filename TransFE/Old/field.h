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

	inline std::vector<DOF<T>*> getDOFsForEntity(const MeshEntity& entity) {
		std::vector<DOF<T>*> rtnDOFs;
		const auto& nodes = entity.getNodes();
		for (const auto& node : nodes) {
			for (const auto& dof : DOFs[node->getID()]) {
				rtnDOFs.push_back(dof.get());
			}
		}
		return rtnDOFs;
	}

	inline std::vector<DOF<T>*> getDOFsForNode(const Node& node) {
		std::vector<DOF<T>*> rtnDOFs;
		for (const auto& dof : DOFs[node.getID()]) {
			rtnDOFs.push_back(dof.get());
		}
		return rtnDOFs;
	}

	void initializeField() {
		ndof = 0;
		const auto& nodes = mesh->getNodes();
		DOFs.resize(nodes.size() * nnd);
		for (size_t i = 0; i < nodes.size(); i++) {
			DOFs[i].resize(nnd);
			nodes[i]->setID(i);
			for (size_t j = 0; j < nnd; j++) {
				DOFs[i][j] = std::make_unique<DOF<T>>();
				ndof++;
			}
		}
	}

	void reorder2() {
		size_t labeldof = 0;
		const auto& nodes = mesh->getNodes();
		for (const auto& node : nodes) {
			const auto& nodeDOFs = DOFs[node->getID()];
			for (const auto& dof : nodeDOFs) {
				if (dof->get_status() == DOFStatus::Free) {
					dof->set_eqnumber(labeldof);
					labeldof++;
				}
			}
		}
	};

	/// a Reverse Cuthill-McKee reordering algorithm
	//void reorder();

	//void reorder2();

private:
	Mesh* mesh;
	std::vector<std::vector<std::unique_ptr<DOF<T>>>> DOFs;
	int nnd;
	int ndof = 0;

};


