/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "field.h"

template<typename T>
inline std::vector<DOF<T>*> Field<T>::getDOFsForEntity(const MeshEntity& entity) {
	std::vector<DOF<T>*> rtnDOFs;
	const auto& nodes = entity.getNodes();
	for (const auto& node : nodes) {
		rtnDOFs.push_back(rtnDOFs.end(), DOFs[node->getID()].begin(), DOFs[node->getID()].end());
	}
}

template<typename T>
inline std::vector<DOF<T>*> Field<T>::getDOFsForNode(const Node& node) {
	std::vector<DOF<T>*> rtnDOFs;
	rtnDOFs.push_back(rtnDOFs.end(), DOFs[node.getID()].begin(), DOFs[node.getID()].end());
}

template<typename T>
void Field<T>::initializeField() {
	const auto& nodes = mesh->getNodes();
	DOFs.resize(nodes.size() * nnd);
	for (size_t i = 0; i < nodes.size(); i++) {
		DOFs[i].resize(nnd);
		for (size_t j = 0; j < nnd; j++) {
			DOFs[i][j] = std::make_unique<DOF<T>>();
		}
	}
}