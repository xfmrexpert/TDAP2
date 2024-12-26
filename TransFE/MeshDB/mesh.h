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

#include "meshregion.h"
#include "meshface.h"
#include "meshedge.h"
#include "meshvertex.h"
#include "node.h"
#include <vector>
#include <queue>
#include <set>
#include <fstream>
#include <map>
#include <memory>
#include <optional>

/// This class acts as a container for all of the mesh data structures
/// and functions.  Lists of each mesh entity type (regions, faces, etc.)
/// are kept here.  New entities are first added here and then connected
/// by adding adjacency information to the approriate entities.

class Mesh {

public:

	/// Mesh class constructor.  Currently empty.
	Mesh() {
		//add blank GeomEntity for entities with no "physical" attributes
		//GeomEntities[0] = make_unique<GeomEntity>();
	}

	//Mesh(const Mesh&);

	~Mesh();

	/// Adds new MeshRegion, both allocating memory and adding it to the master list of 
	/// regions
	MeshRegion& newRegion();

	/// Adds new MeshFace, both allocating memory and adding it to the master list of faces
	MeshFace& newFace();

	/// Adds new MeshEdge, both allocating memory and adding it to the master list of edges
	MeshEdge& newEdge();

	/// Adds new MeshVertex, both allocating memory and adding it to the master list of 
	/// vertices
	MeshVertex& newVertex();

	/// Adds new Node, both allocating memory and adding it to the master list of nodes
	Node& newNode();

	/// Gets the MeshRegion corresponding to n in the master list of regions
	MeshRegion& getRegion(size_t n);

	/// Gets the n-th MeshFace in the master list of faces
	MeshFace& getFace(size_t n);

	/// Gets the n-th MeshEdge in the master list of regions
	MeshEdge& getEdge(size_t n);

	/// Gets the n-th MeshVertex in the master list of vertices
	MeshVertex& getVertex(size_t n);

	/// Gets the n-th Node in the master list of nodes
	Node& getNode(size_t n);

	std::vector<MeshFace*> getFaces() const;

	std::vector<Node*> getNodes() const;

	size_t numRegions();

	/// Returns the total number of faces in this mesh
	size_t numFaces();

	/// Returns the total number of edges in this mesh
	size_t numEdges();

	/// Returns the total number of vertices in this mesh
	size_t numVertexes();

	/// Returns the total number of nodes in this mesh
	size_t numNodes();

	/// a Reverse Cuthill-McKee reordering algorithm
	void reorder();

	void reorder2();

	/// Reads a GMSH file (version 2.0)
	void readMesh(const std::string& meshfile);

	/// Reads an attribute file (.att)
	void readAttributes(const std::string& attribfile);

	

protected:

private:
	///  Master list of pointers to all Nodes in this mesh
	std::vector<std::unique_ptr<Node>> Nodes;

	///  Master list of pointers to all MeshFaces in this mesh
	std::vector<std::unique_ptr<MeshFace>> MeshFaces;

	///  Master list of pointers to all MeshRegions in this mesh
	std::vector<std::unique_ptr<MeshRegion>> MeshRegions;

	///  Master list of pointers to all MeshEdges in this mesh
	std::vector<std::unique_ptr<MeshEdge>> MeshEdges;

	///  Master list of pointers to all MeshVertexes in this mesh
	std::vector<std::unique_ptr<MeshVertex>> MeshVertexes;

	/// Master list of GeomEntities in this mesh (don't remember why I used map here)
	/// This list owns the GeomEntities
	std::map<int, std::unique_ptr<GeomEntity>> GeomEntities;

	/// Retrieves the start entity for the reordering algorithm
	MeshEntity* getStart();

	/// Find a vertex by a Node ID
	MeshVertex* findVertexbyNode(size_t n);

	/// Find an edge by it's vertexes
	MeshEdge* findEdgebyVertexes(MeshVertex& vertex1, MeshVertex& vertex2);

	std::pair<MeshFace*, bool> findFacebyEdge(MeshEdge& edge1, MeshEdge& edge2, MeshEdge& edge3);

	/// Find a node by its ID
	Node* findNodebyID(size_t n);

};

