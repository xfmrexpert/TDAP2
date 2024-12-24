/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "meshedge.h"
#include "meshvertex.h"
#include "meshface.h"
#include <stdexcept>
#include <algorithm>

const MeshVertex* MeshEdge::getVertex(int n) const {
    if (n < 0 || n >= MeshVertices.size()) {
        throw std::out_of_range("Invalid vertex index");
    }
    return MeshVertices[n];
}

/// Returns an const reference to the vector of facess
const vector<MeshFace*>& MeshEdge::Faces() const {
    return MeshFaces;
}

MeshFace* MeshEdge::getFace(int n) {
    if (n < 0 || n >= MeshFaces.size()) {
        throw std::out_of_range("Invalid face index");
    }
    return MeshFaces[n];
}

void MeshEdge::addVertex(MeshVertex& new_vertex, int n){
    if (n < 0 || n >= MeshVertices.size()) {
        throw std::out_of_range("Invalid vertex index");
    }
    MeshVertices[n] = &new_vertex;
}

void MeshEdge::addFace(MeshFace& new_face){
    if (std::find(MeshFaces.begin(), MeshFaces.end(), &new_face) == MeshFaces.end()) {
        MeshFaces.push_back(&new_face);
    }
}

MeshVertex* MeshEdge::otherVertex(const MeshVertex& vertex) const{
    if (MeshVertices[0] == &vertex) {
        return MeshVertices[1];
    }
    else if (MeshVertices[1] == &vertex) {
        return MeshVertices[0];
    }
    else {
        throw std::invalid_argument("Vertex does not belong to this edge");
    }
}

bool MeshEdge::isConnected(const MeshVertex& vertex) const {
    return std::any_of(MeshVertices.begin(), MeshVertices.end(),
        [&vertex](const MeshVertex* v) { return v == &vertex; });
}

bool MeshEdge::isConnected(const MeshEdge& edge) const {
    for (const auto* v : edge.MeshVertices) {
        if (isConnected(*v)) {
            return true;
        }
    }
    return false;
}

vector<Node*> MeshEdge::getNodes() const{
   vector<Node*> nodes;
   
   for(int i=0;i<2;i++){
      nodes.push_back(this->getVertex(i)->node);
   }
   if(this->node != nullptr){
      nodes.push_back(this->node);
   }
   return nodes;
}
