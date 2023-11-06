/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#ifndef MESH_H
#define MESH_H
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

using namespace std;

/// This class acts as a container for all of the mesh data structures
/// and functions.  Lists of each mesh entity type (regions, faces, etc.)
/// are kept here.  New entities are first added here and then connected
/// by adding adjacency information to the approriate entities.

class Mesh {

public:

	/// Mesh class constructor.  Currently empty.
	Mesh() {
		//add blank GeomEntity for entities with no "physical" attributes
		GeomEntities[0] = make_shared<GeomEntity>();
	}

	//Mesh(const Mesh&);

	~Mesh();

	/// Adds new MeshRegion, both allocating memory and adding it to the master list of 
	/// regions
	shared_ptr<MeshRegion> newRegion();

	/// Adds new MeshFace, both allocating memory and adding it to the master list of faces
	shared_ptr<MeshFace> newFace();

	/// Adds new MeshEdge, both allocating memory and adding it to the master list of edges
	shared_ptr<MeshEdge> newEdge();

	/// Adds new MeshVertex, both allocating memory and adding it to the master list of 
	/// vertices
	shared_ptr<MeshVertex> newVertex();

	/// Adds new Node, both allocating memory and adding it to the master list of nodes
	shared_ptr<Node> newNode();

	/// Gets the MeshRegion corresponding to n in the master list of regions
	shared_ptr<MeshRegion> getRegion(size_t n);

	/// Gets the n-th MeshFace in the master list of faces
	shared_ptr<MeshFace> getFace(size_t n);

	/// Gets the n-th MeshEdge in the master list of regions
	shared_ptr<MeshEdge> getEdge(size_t n);

	/// Gets the n-th MeshVertex in the master list of vertices
	shared_ptr<MeshVertex> getVertex(size_t n);

	/// Gets the n-th Node in the master list of nodes
	shared_ptr<Node> getNode(size_t n);

	/// Returns an iterator corresponding to the first MeshRegion in the master list of 
	/// regions
	vector<shared_ptr<MeshRegion>>::iterator getFirstRegion();

	vector<shared_ptr<MeshRegion>>::iterator getLastRegion();

	/// Returns an iterator corresponding to the first MeshFace in the master list of faces
	vector<shared_ptr<MeshFace>>::iterator getFirstFace();

	/// Returns an iterator corresponding to the last MeshFace in the master list of faces 
	/// (may be after last face)
	vector<shared_ptr<MeshFace>>::iterator getLastFace();

	/// Returns an iterator corresponding to the first MeshEdge in the master list of edges
	vector<shared_ptr<MeshEdge>>::iterator getFirstEdge();

	vector<shared_ptr<MeshEdge>>::iterator getLastEdge();

	/// Returns an iterator corresponding to the first MeshVertex in the master list of 
	/// vertices
	vector<shared_ptr<MeshVertex>>::iterator getFirstVertex();

	/// Returns an iterator corresponding to the last MeshRegion in the master list of 
	/// vertices
	vector<shared_ptr<MeshVertex>>::iterator getLastVertex();

	/// Returns an iterator corresponding to the first Node in the master list of nodes
	vector<shared_ptr<Node>>::iterator getFirstNode();

	/// Returns an iterator corresponding to the last Node in the master list of nodes  
	vector<shared_ptr<Node>>::iterator getLastNode();

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
	void readMesh(string meshfile);

	/// Reads an attribute file (.att)
	void readAttributes(const char* attribfile);

	///  Master list of pointers to all Nodes in this mesh
	vector<shared_ptr<Node>> Nodes;

	///  Master list of pointers to all MeshFaces in this mesh
	vector<shared_ptr<MeshFace>> MeshFaces;

protected:

private:
	///  Master list of pointers to all MeshRegions in this mesh
	vector<shared_ptr<MeshRegion>> MeshRegions;

	///  Master list of pointers to all MeshEdges in this mesh
	vector<shared_ptr<MeshEdge>> MeshEdges;

	///  Master list of pointers to all MeshVertexes in this mesh
	vector<shared_ptr<MeshVertex>> MeshVertexes;

	/// Master list of GeomEntities in this mesh (don't remember why I used map here)
	map<int, shared_ptr<GeomEntity>> GeomEntities;

	/// Retrieves the start entity for the reordering algorithm
	shared_ptr<MeshEntity> getStart();

	/// Find a vertex by a Node ID
	shared_ptr<MeshVertex> findVertexbyNode(size_t n);

	/// Find an edge by it's vertexes
	shared_ptr<MeshEdge> findEdgebyVertexes(shared_ptr<MeshVertex> vertex1, shared_ptr<MeshVertex> vertex2);

	pair<shared_ptr<MeshFace>, bool> findFacebyEdge(shared_ptr<MeshEdge> edge1, shared_ptr<MeshEdge> edge2, shared_ptr<MeshEdge> edge3);

	/// Find a node by its ID
	shared_ptr<Node> findNodebyID(size_t n);

};
#endif //MESH_H

