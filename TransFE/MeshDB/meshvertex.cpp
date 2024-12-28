/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "meshvertex.h"
#include "meshedge.h"

void MeshVertex::addEdge(MeshEdge& edge){
	MeshEdges.push_back(&edge); //should be ordered CCW
}

const std::vector<MeshEdge*>& MeshVertex::Edges() const {
    return MeshEdges;
}

MeshEdge* MeshVertex::getEdge(int n) const {
     return MeshEdges[n];
}

size_t MeshVertex::numEdges() const {
     return MeshEdges.size();
}

std::vector<Node*> MeshVertex::getNodes() const {
   std::vector<Node*> nodes;
   nodes.push_back(node);
   return nodes;
}

