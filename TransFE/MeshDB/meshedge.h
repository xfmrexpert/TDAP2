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

#include "meshentity.h"
#include <vector>
#include <array>
#include <algorithm>

class MeshVertex;
class MeshFace;

/// This class represents a mesh edge. A mesh edge is a 1D object bounded by two
/// vertices. A mesh edge bounds mesh faces. This class is derived from MeshEntity.
/// Note that adjacencies are stored using a one-level representation and that use
/// entities are included.

class MeshEdge : public MeshEntity {

public:
    /// Default constructor. Currently empty.
    MeshEdge() = default;

    ~MeshEdge() = default;

    /// Returns a pointer to the n-th vertex of this edge (either 0 or 1)
    const MeshVertex* getVertex(int n) const {
        if (n < 0 || n >= MeshVertices.size() || MeshVertices[n] == nullptr) {
            throw std::out_of_range("Invalid or null vertex index");
        }
        return MeshVertices[n];
    }

    /// Returns a const reference to the vector of faces
    const std::vector<MeshFace*>& Faces() const {
        return MeshFaces;
    }

    /// Returns a pointer to the n-th face that this edge bounds
    MeshFace* getFace(int n) {
        if (n < 0 || n >= MeshFaces.size()) {
            throw std::out_of_range("Invalid face index");
        }
        return MeshFaces[n];
    }

    /// Adds a vertex to the adjacency information for this edge
    void addVertex(MeshVertex& new_vertex, int n) {
        if (n < 0 || n >= MeshVertices.size()) {
            throw std::out_of_range("Invalid vertex index");
        }
        MeshVertices.at(n) = &new_vertex; // Use std::array::at for range checking
    }

    /// Adds a face to the adjacency information for this edge
    void addFace(MeshFace& new_face) {
        if (std::find(MeshFaces.begin(), MeshFaces.end(), &new_face) == MeshFaces.end()) {
            MeshFaces.push_back(&new_face);
        }
    }

    /// Returns the other vertex for this edge
    MeshVertex* otherVertex(const MeshVertex& vertex) const {
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

    /// Returns whether or not the given vertex bounds this edge
    bool isConnected(const MeshVertex& vertex) const {
        return std::any_of(MeshVertices.begin(), MeshVertices.end(),
            [&vertex](const MeshVertex* v) { return v == &vertex; });
    }

    /// Returns whether or not this edge is connected to the given edge
    bool isConnected(const MeshEdge& edge) const {
        for (const auto* v : edge.MeshVertices) {
            if (v && isConnected(*v)) { // Check for non-null and connectivity
                return true;
            }
        }
        return false;
    }

    /// Returns an ordered list of nodes for this edge
    /// Vertex nodes first, edge node second
    std::vector<Node*> getNodes() const override {
        std::vector<Node*> nodes;
        for (const auto* vertex : MeshVertices) {
            if (vertex && vertex->getNode()) {
                nodes.push_back(vertex->getNode());
            }
        }
        if (this->getNode() != nullptr) {
            nodes.push_back(this->getNode());
        }
        return nodes;
    }

    std::vector<MeshEdge*> getEdges() override {
        std::vector<MeshEdge*> edges;
        edges.push_back(this);
        return edges;
    }

    /// Returns an ordered list of vertices for this edge
    std::vector<const MeshVertex*> getVertices() const {
        std::vector<const MeshVertex*> vertices;
        for (const auto* vertex : MeshVertices) {
            if (vertex) {
                vertices.push_back(vertex);
            }
        }
        return vertices;
    }

    int get_dimensions() const override {
        return dimensions;
    }

    size_t getID() const {
        return ID;
    }

    void setID(size_t in_ID) {
        ID = in_ID;
    }

protected:

private:
    /// An optional integer ID
    size_t ID = -9999;

    /// Indicates the "level" or dimensions of edges, may be used someday
    static const int dimensions = 1;

    /// A fixed-size array to hold pointers to each vertex of this edge
    std::array<MeshVertex*, 2> MeshVertices{ nullptr, nullptr };

    /// An STL vector for holding pointers to face adjacencies
    std::vector<MeshFace*> MeshFaces;
};


