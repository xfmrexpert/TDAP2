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
#include "meshvertex.h"
#include "meshedge.h"
#include "EdgeUse.h"
#include <vector>

class MeshEdge;
class MeshRegion;

/// This class represents a mesh face.  A mesh face is a 2D object bounded by edges.
/// A mesh face bounds mesh regions. This class is derived from MeshEntity.
/// Note that adjacencies are stored using a one-level representation.

class MeshFace : public MeshEntity {

public:

	/// Default constructor.  Currently empty.
	MeshFace() = default;

	~MeshFace() = default;

	/// Returns the n-th edge bounding this face (unordered)
    EdgeUse* getEdgeUse(size_t n) {
        if (n < 0 || n >= MeshEdgeUses.size()) {
            throw std::out_of_range("Invalid edge use index");
        }
        return &MeshEdgeUses[n];
    }

	/// Returns the n-th region that this face bounds (unordered)
    MeshRegion* getRegion(size_t n) {
        if (n < 0 || n >= MeshRegions.size()) {
            throw std::out_of_range("Invalid face index");
        }
        return MeshRegions[n];
    }

	/// Adds an edge to the list of adjacent edges
    void addEdge(MeshEdge& new_edge) {
        int orientation = 0;
        for (const auto& edge_use : MeshEdgeUses) {
            if (edge_use.edge == &new_edge) return;
            if (edge_use.edge->isConnected(new_edge)) {
                // We know the edge is connected, now determine which way
                if (edge_use.edge->isConnected(*new_edge.getVertex(0))) { 
                    orientation = 1; 
                    //MeshEdgeUses.emplace(,EdgeUse(&new_edge, orientation));
                } else if (edge_use.edge->isConnected(*new_edge.getVertex(1))) { orientation = -1; }
                else { throw std::logic_error("Edge was reported as connected but does not appear to share a vertex."); }
            }
        }
        
        MeshEdgeUses.emplace_back(EdgeUse(&new_edge, orientation));
    }

	/// Adds a region to the list of adjacent regions
    void addRegion(MeshRegion& new_region) {
        if (std::find(MeshRegions.begin(), MeshRegions.end(), &new_region) == MeshRegions.end()) {
            MeshRegions.push_back(&new_region);
        }
    }

	/// Returns the number of edges bounding this face
    size_t numEdges() {
        return MeshEdgeUses.size();
    }

	/// Returns the area of the face
    double area() {
        double Area = 0;

        auto edge_use_iter = MeshEdgeUses.begin();
        auto firstEdge = edge_use_iter->edge;
        auto firstVertex = firstEdge->getVertex(0);

        for (edge_use_iter++; edge_use_iter != MeshEdgeUses.end(); ++edge_use_iter) {
            if (!edge_use_iter->edge->isConnected(*firstVertex)) {
                double X1;
                double X2;
                double X3;
                double Y1;
                double Y2;
                double Y3;

                X1 = firstVertex->getNode()->x();
                X2 = edge_use_iter->edge->getVertex(0)->getNode()->x();
                X3 = edge_use_iter->edge->getVertex(1)->getNode()->x();
                Y1 = firstVertex->getNode()->y();
                Y2 = edge_use_iter->edge->getVertex(0)->getNode()->y();
                Y3 = edge_use_iter->edge->getVertex(1)->getNode()->y();

                Area = Area + 0.5 * fabs(X1 * (Y3 - Y2) + X2 * (Y1 - Y3) + X3 * (Y2 - Y1));
            }
        }

        return Area;
    }

	int get_dimensions() const override {
		return dimensions;
	}

	/// Get an ordered listing of the nodes
	/// vertex nodes 1st, edge nodes 2nd, face node last
    std::vector<Node*> getNodes() const override {
        std::vector<Node*> nodes;
        //add nodes for each vertex of face first, in order
        //This assumes mesh data structure is already ordered!!!
        auto edge_use_iter = MeshEdgeUses.begin(); //Get first edge;
        auto first_edge = edge_use_iter->edge;
        ++edge_use_iter; //Get second edge;
        if (edge_use_iter->edge->getVertex(0) == first_edge->getVertex(0) || edge_use_iter->edge->getVertex(1) == first_edge->getVertex(0)) {
            nodes.push_back(first_edge->getVertex(1)->getNode());
            nodes.push_back(first_edge->getVertex(0)->getNode());
        }
        else {
            nodes.push_back(first_edge->getVertex(0)->getNode());
            nodes.push_back(first_edge->getVertex(1)->getNode());
        }

        for (/*edge_iter=second edge*/; edge_use_iter != MeshEdgeUses.end(); ++edge_use_iter) {
            if (edge_use_iter->edge->getVertex(0)->getNode() == nodes.back()) {
                nodes.push_back(edge_use_iter->edge->getVertex(1)->getNode());
            }
            else {
                nodes.push_back(edge_use_iter->edge->getVertex(0)->getNode());
            }
        }

        //add nodes for all edges (if present) next
        for (const auto& edge_use : MeshEdgeUses) {
            if (edge_use.edge->getNode() != nullptr) {
                nodes.push_back(edge_use.edge->getNode());
            }
        }

        //add node in face (if present) last
        if (this->getNode() != nullptr) {
            nodes.push_back(this->getNode());
        }

        return nodes;
    }

	/// Get an ordered listing of the face vertices (CCW)
    std::vector<const MeshVertex*> getVertices() const {
        std::vector<const MeshVertex*> vertices;

        // Add nodes for each vertex of face first, in order
        auto edge_use_iter = MeshEdgeUses.begin(); // Get first edge
        auto first_edge = edge_use_iter->edge;
        edge_use_iter++; // Get second edge
        if (edge_use_iter->edge->getVertex(0) == first_edge->getVertex(0) ||
            edge_use_iter->edge->getVertex(1) == first_edge->getVertex(0)) {
            vertices.push_back(first_edge->getVertex(1));
            vertices.push_back(first_edge->getVertex(0));
        }
        else {
            vertices.push_back(first_edge->getVertex(0));
            vertices.push_back(first_edge->getVertex(1));
        }

        for (/* edge_iter = second edge */; edge_use_iter != MeshEdgeUses.end(); ++edge_use_iter) {
            if (edge_use_iter->edge->getVertex(0) == vertices.back()) {
                vertices.push_back(edge_use_iter->edge->getVertex(1));
            }
            else {
                vertices.push_back(edge_use_iter->edge->getVertex(0));
            }
        }

        return vertices; // Return by value
    }

    std::vector<MeshEdge*> getEdges() const override {
        std::vector<MeshEdge*> edges;
        for (const auto& edge_use : MeshEdgeUses)
        {
            edges.push_back(edge_use.edge);
        }
        return edges;
    }

protected:

private:
    //Containers for adjacency information (via pointers)
     /// List of edges attached to this face (currently unordered)
    std::vector<EdgeUse> MeshEdgeUses;

	/// indicates that faces are 2D objects
	static const int dimensions = 2;

	/// Always 2 regions per face, one on each side
	std::vector<MeshRegion*> MeshRegions;

};

