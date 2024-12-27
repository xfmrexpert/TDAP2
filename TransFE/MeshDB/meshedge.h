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
    const MeshVertex* getVertex(int n) const;

    /// Returns a const reference to the vector of faces
    const std::vector<MeshFace*>& Faces() const;

    /// Returns a pointer to the n-th face that this edge bounds
    MeshFace* getFace(int n);

    /// Adds a vertex to the adjacency information for this edge
    void addVertex(MeshVertex& new_vertex, int n);

    /// Adds a face to the adjacency information for this edge
    void addFace(MeshFace& new_face);

    /// Returns the other vertex for this edge
    MeshVertex* otherVertex(const MeshVertex&) const;

    /// Returns whether or not this edge is connected to the given edge
    bool isConnected(const MeshEdge&) const;

    /// Returns whether or not the given vertex bounds this edge
    bool isConnected(const MeshVertex&) const;

    /// Returns an ordered list of nodes for this edge
    /// Vertex nodes first, edge node second
    std::vector<Node*> getNodes() const override;

    /// Returns an ordered list of vertices for this edge
    std::vector<const MeshVertex*> getVertices() const;

    int get_dimensions() const override {
        return 1;
    }

protected:

private:
    /// Indicates the "level" or dimensions of edges, may be used someday
    static const int dimensions = 1;

    /// A fixed-size array to hold pointers to each vertex of this edge
    std::array<MeshVertex*, 2> MeshVertices{ nullptr, nullptr };

    /// An STL vector for holding pointers to face adjacencies
    std::vector<MeshFace*> MeshFaces;
};


