/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "mesh.h"

#include <stdio.h>
#include <string.h>
#include <iostream>

 /* Mesh::Mesh(const Mesh& old_mesh){
	MeshRegions.reserve(old_mesh.MeshRegions.size()); // preallocate the space we need so push_back doesn't have to

	for (const auto& e : old_mesh.MeshRegions)
	{
	   MeshRegions.push_back(e->clone());
	}
	 MeshRegions.assign(old_mesh.MeshRegions.begin(), old_mesh.MeshRegions.end());
	 MeshFaces.assign(old_mesh.MeshFaces.begin(), old_mesh.MeshFaces.end());
	 MeshEdges.assign(old_mesh.MeshEdges.begin(), old_mesh.MeshEdges.end());
	 MeshVertexes.assign(old_mesh.MeshVertexes.begin(), old_mesh.MeshVertexes.end());
	 Nodes.assign(old_mesh.Nodes.begin(), old_mesh.Nodes.end());
	 //MeshRegions = old_mesh.MeshRegions;
	 //MeshFaces = old_mesh.MeshFaces;
	 //MeshEdges = old_mesh.MeshEdges;
	 //MeshVertexes = old_mesh.MeshVertexes;
	 //Nodes = old_mesh.Nodes;
	 GeomEntities = old_mesh.GeomEntities;
 } */

MeshRegion& Mesh::newRegion() {
	MeshRegions.push_back(std::make_unique<MeshRegion>());
	return *MeshRegions.back();
}

MeshFace& Mesh::newFace() {
	MeshFaces.push_back(std::make_unique<MeshFace>());
	return *MeshFaces.back();
}

MeshEdge& Mesh::newEdge() {
	MeshEdges.push_back(std::make_unique<MeshEdge>());
	return *MeshEdges.back();
}

MeshVertex& Mesh::newVertex() {
	MeshVertexes.push_back(std::make_unique<MeshVertex>());
	return *MeshVertexes.back();
}

Node& Mesh::newNode() {
	Nodes.push_back(std::make_unique<Node>());
	return *Nodes.back();
}

MeshRegion& Mesh::getRegion(size_t n) const {
	if (n < MeshRegions.size()) {
		return *MeshRegions[n];
	}
	else {
		std::runtime_error("ERROR: MeshRegion index out of range");
	}
}

MeshFace& Mesh::getFace(size_t n) const {
	if (n < MeshFaces.size()) {
		return *MeshFaces[n];
	}
	else {
		std::runtime_error("ERROR: MeshFace index out of range");
	}
}

MeshEdge& Mesh::getEdge(size_t n) const {
	if (n < MeshEdges.size()) {
		return *MeshEdges[n];
	}
	else {
		std::runtime_error("ERROR: MeshEdge index out of range");
	}
}

MeshVertex& Mesh::getVertex(size_t n) const {
	if (n < MeshVertexes.size()) {
		return *MeshVertexes[n];
	}
	else {
		std::runtime_error("ERROR: MeshVertex index out of range");
	}
}

Node& Mesh::getNode(size_t n) const {
	if (n < Nodes.size()) {
		return *Nodes[n];
	}
	else {
		std::runtime_error("ERROR: Node index out of range");
	}
}

std::vector<MeshFace*> Mesh::getFaces() const {
	std::vector<MeshFace*> faces;
	for (const auto& face : MeshFaces) {
		faces.push_back(face.get());
	}
	return faces;
}

std::vector<Node*> Mesh::getNodes() const {
	std::vector<Node*> nodes;
	for (const auto& node : Nodes) {
		nodes.push_back(node.get());
	}
	return nodes;
}

size_t Mesh::numRegions() const {
	return MeshRegions.size();
}

size_t Mesh::numFaces() const {
	return MeshFaces.size();
}

size_t Mesh::numEdges() const {
	return MeshEdges.size();
}

size_t Mesh::numVertexes() const {
	return MeshVertexes.size();
}

size_t Mesh::numNodes() const {
	return Nodes.size();
}

void Mesh::reorder2() {
	size_t labeldof = 0;
	for (const auto& node : Nodes) {
		const auto& DOFs = node->getDOFs();
		for (const auto& dof : DOFs) {
			if (dof->get_status() == DOF_Free) {
				dof->set_eqnumber(labeldof);
				labeldof++;
			}
		}
	}
};

void Mesh::reorder() {
	size_t labeldof = ndof;
	size_t labelnode = Nodes.size();
	size_t labelface = MeshFaces.size();
	size_t labelregion = MeshRegions.size();

	std::queue<MeshEntity*> q;
	std::queue<MeshEntity*> list;

	//cout << "Beginning mesh reordering...\n";

	for (const auto& node : Nodes) {
		node->ID = -2; //-2 indicates not labeled and not in queue
	}
	for (const auto& face : MeshFaces) {
		face->ID = -2; //-2 indicates not labeled and not in queue
	}
	for (const auto& region : MeshRegions) {
		region->ID = -2; //-2 indicates not labeled and not in queue
	}

	//cout << "Get start...\n";

	MeshEntity* entity = getStart();
	//cout << "Put first entity into queue...\n";
	q.push(entity);
	entity->ID = -1; //0 indicates entity in queue
	//cout << "Begin looping through queue...\n";
	while (q.size() > 0) {
		if (labelnode < 0) {
			cout << "ERROR! labelnode<0!\n";
			return;
		}
		entity = q.front();
		q.pop();
		//if(entity->get_dimensions()==1){cout << "dequeueing edge\n";}
		auto node = entity->node;
		if (node->ID < 0) { //node is unlabeled
			labelnode = labelnode - 1;
			node->ID = labelnode;
			const auto& DOFs = node->getDOFs();
			for (const auto& dof : DOFs) {
				if (dof->get_status() == DOF_Free) {
					labeldof = labeldof - 1;
					if (labeldof < 0) {
						cout << "ERROR! labeldof<0!\n";
						return;
					}
					dof->set_eqnumber(labeldof);
				}
			}
		}

		if (entity->get_dimensions() == 0) { //entity is a vertex
			//cout << "Loop through edges of vertex\n";
			MeshVertex* vertex = dynamic_cast<MeshVertex*>(entity);
			for (const auto& edge : vertex->Edges()) {
				//cout << "Loop through faces of edge #" << edge->ID << "\n";
				for (const auto& face : edge->Faces()) {
					if (face->ID < 0) { //face is unlabeled
						labelface = labelface - 1;
						face->ID = labelface;
					}
					if (face->node != nullptr && face->node->ID < -1) {
						q.push(face);
						face->node->ID = -1;
					}
				}
				auto othervertex = edge->otherVertex(*vertex);
				if (edge->node != nullptr) {  //if edge has a node
					//cout << "Edge has a node\n";
					if (othervertex->node->ID >= -1 && edge->node->ID < -1) {
						//cout << "Labeling node directly\n";
						labelnode = labelnode - 1;
						edge->node->ID = labelnode;
						const auto& DOFs = edge->node->getDOFs();
						for (const auto& dof : DOFs) {
							if (dof->get_status() == DOF_Free) {
								labeldof = labeldof - 1;
								if (labeldof < 0) {
									cout << "ERROR! labeldof<0!\n";
									return;
								}
								dof->set_eqnumber(labeldof);
							}
						}
					}
					else if (othervertex->node->ID < -1) {
						//cout << "Queueing edge\n";
						q.push(edge);
						list.push(othervertex);
						othervertex->node->ID = -1;
					}
				}
				else {
					if (othervertex->node->ID < -1) {
						//cout << "Queue edge's other vertex\n";
						list.push(othervertex);
						othervertex->node->ID = -1;
					}
				}
			}
		}
		while (list.size() > 0) {
			//cout << "Queue list\n";
			q.push(list.front());
			list.pop();
		}
		//cout << "Get next entity...\n";
	}
	cout << labelnode;
}

MeshEntity* Mesh::getStart() {
	if (MeshVertexes.empty())
	{
		return nullptr;
	}
	else
	{
		MeshVertex& start_vertex = *MeshVertexes[0];
		return &start_vertex;
	}
}

static int getNumNodes(int type)
{
	switch (type) {
	case 15: return 1;              // point
	case 1: return 2;              // line 1
	case 8: return 3;          // line 2
	case 2: return 3;              // triangle 1
	case 9: return 6;          // triangle 2
	case 3: return 4;              // quadrangle 1
	case 10: return 9;      // quadrangle 2
	case 4: return 4;              // tetrahedron 1
	case 11: return 4 + 6;          // tetrahedron 2
	case 5: return 8;              // hexahedron 1
	case 12: return 8 + 12 + 6 + 1; // hexahedron 2 
	case 6: return 6;              // prism 1
	case 13: return 6 + 9 + 3;      // prism 2
	case 7: return 5;              // pyramid 1
	case 14: return 5 + 8 + 1;      // pyramid 2
	default: return 0;
	}
}

static int getNumEdges(int type)
{
	switch (type) {
	case 15: return 0;              // point
	case 1: return 1;              // line 1
	case 8: return 1;          // line 2
	case 2: return 3;              // triangle 1
	case 9: return 3;          // triangle 2
	case 3: return 4;              // quadrangle 1
	case 10: return 4;      // quadrangle 2
	case 4: return 4;              // tetrahedron 1
	case 11: return 4;          // tetrahedron 2
	case 5: return 8;              // hexahedron 1
	case 12: return 8; // hexahedron 2 
	case 6: return 6;              // prism 1
	case 13: return 6;      // prism 2
	case 7: return 5;              // pyramid 1
	case 14: return 5;      // pyramid 2
	default: return 0;
	}
}

void Mesh::readMesh(const std::string& filename)
{
	double version = 1.0;
	std::ifstream meshFile(filename);
	if (!meshFile.is_open()) {
		std::cerr << "Unable to open file: " << filename << std::endl;
		return;
	}

	std::string line;
	while (std::getline(meshFile, line)) {
		if (line.empty() || line.front() != '$')
		{
			continue;
		}

		if (line.compare(1, 10, "MeshFormat") == 0) {
			int format, size;
			meshFile >> version >> format >> size;
			if (version < 2.0) {
				throw std::runtime_error("Error: Unsupported msh file version " + std::to_string(version));
			}
			if (format) {
				throw std::runtime_error("Error: Unsupported mesh data format.");
			}
		}
		else if (line.compare(1, 5, "Nodes") == 0) {
			size_t num_nodes;
			meshFile >> num_nodes;

			for (size_t i = 0; i < num_nodes; ++i) {
				size_t id;
				double x, y, z;
				meshFile >> id >> x >> y >> z;
				if (findNodebyID(id)) {
					throw std::runtime_error("Error: Duplicate node ID " + std::to_string(id));
				}

				Node& new_node = newNode(); //newNode() adds a new Node to the Nodes list
				new_node.pt({ x, y, z });
				new_node.ID = id;
			}
		}
		else if (line.compare(1, 8, "Elements") == 0) {
			//"Elements" in gmsh terms can be vertexes, edges, faces or regions
			//For 2D meshes, all faces are listed, as well as edges and vertexes that are
			//located on what gmsh calls "physical entities".
			size_t numElements;
			meshFile >> numElements;
			
			for (size_t i = 0; i < numElements; ++i) {
				size_t id, numTags;
				int type;
				meshFile >> id >> type >> numTags;
				
				// Read element tags
				int physical = 1, elementary = 1, partition = 1;
				for (size_t j = 0; j < numTags; j++) {
					int tag;
					meshFile >> tag;
					if (j == 0) physical = tag;
					else if (j == 1) elementary = tag;
					else if (j == 2) partition = tag;
					// ignore any other tags for now
				}
				
				size_t numNodes = getNumNodes(type);
				size_t numEdges = getNumEdges(type);

				// Handle different element types
				if (type == 15) { // Point element
					size_t nodeID;
					meshFile >> nodeID;

					MeshVertex* vertex = findVertexbyNode(nodeID);
					if (!vertex) {
						vertex = &newVertex();
						vertex->node = findNodebyID(nodeID);
						vertex->ID = MeshVertexes.size();
					}
					vertex->setClassification(*GeomEntities[physical]);
				}
				else if (type == 1 || type == 8) { // Line element
					size_t node1, node2;
					meshFile >> node1 >> node2;

					MeshVertex* vertex1 = getOrCreateVertex(node1);
					MeshVertex* vertex2 = getOrCreateVertex(node2);

					MeshEdge* new_edge = findEdgebyVertexes(*vertex1, *vertex2);
					if (!new_edge) { //edge hasn't been added yet
						new_edge = &newEdge();
						new_edge->addVertex(*vertex1, 0);
						new_edge->addVertex(*vertex2, 1);
						vertex1->addEdge(*new_edge);
						vertex2->addEdge(*new_edge);
						new_edge->ID = MeshEdges.size();
					}
					new_edge->setClassification(*GeomEntities[physical]);

					if (type == 8) {
						size_t node3;
						meshFile >> node3;
						new_edge->node = findNodebyID(node3);
					}
				}
				else if (type == 2 || type == 3 || type == 9 || type == 10) { //2D elements
					handleFaceElement(meshFile, type, numNodes, numEdges, physical);
				}
				else if (type == 4) { //tetrahedron
					handleTetrahedronElement(meshFile, physical);
				}
				else {
					throw std::runtime_error("Error: Unsupported element type " + std::to_string(type));
				}
			}
		}
	}
	meshFile.close();
	
}

MeshVertex* Mesh::getOrCreateVertex(size_t nodeID) {
	// Attempt to find the vertex by its associated node
	auto vertex = findVertexbyNode(nodeID);
	if (!vertex) {
		// Create a new vertex if it doesn't exist
		auto& newVertex = this->newVertex();
		newVertex.node = findNodebyID(nodeID);
		if (!newVertex.node) {
			throw std::runtime_error("Node " + std::to_string(nodeID) + " does not exist.");
		}
		newVertex.ID = MeshVertexes.size();
		vertex = &newVertex;
	}
	return vertex;
}

MeshEdge* Mesh::getOrCreateEdge(MeshVertex* v1, MeshVertex* v2) {
	// Check if the edge already exists by searching for it
	for (const auto& edge : MeshEdges) {
		auto vertex1 = edge->getVertex(0);
		auto vertex2 = edge->getVertex(1);
		if ((vertex1 == v1 && vertex2 == v2) ||
			(vertex1 == v2 && vertex2 == v1)) {
			return edge.get();
		}
	}

	// If not found, create a new edge
	auto& newEdge = this->newEdge();
	
	newEdge.addVertex(*v1, 0);
	newEdge.addVertex(*v2, 1);
	v1->addEdge(newEdge);
	v2->addEdge(newEdge);

	return &newEdge;
}

MeshFace* Mesh::getOrCreateFace(MeshVertex* v1, MeshVertex* v2, MeshVertex* v3) {
	// Check if the face already exists by searching for it using its vertices
	for (const auto& face : MeshFaces) {
		auto vertices = face->getVertices(); // Assuming getVertices returns ordered vertices
		if (vertices.size() == 3 &&
			vertices[0] == v1 &&
			vertices[1] == v2 &&
			vertices[2] == v3) {
			return face.get();
		}
	}

	// If not found, create a new face
	auto& newFace = this->newFace();
	
	// Connect edges to the face
	auto edge1 = getOrCreateEdge(v1, v2);
	auto edge2 = getOrCreateEdge(v2, v3);
	auto edge3 = getOrCreateEdge(v3, v1);

	newFace.addEdge(*edge1);
	newFace.addEdge(*edge2);
	newFace.addEdge(*edge3);

	return &newFace;
}

void Mesh::handleFaceElement(std::ifstream& meshFile, int type, size_t numNodes, size_t numEdges, int physical) {
	size_t firstNode, node1, node2;

	// Create a new face
	auto& face = newFace();

	meshFile >> firstNode;
	node2 = firstNode;

	for (size_t j = 0; j < numEdges; ++j) {
		node1 = node2;
		if (j < (numEdges - 1)) {
			meshFile >> node2;
		}
		else {
			node2 = firstNode; // Close the loop for the last edge
		}

		// Get or create the vertices
		auto vertex1 = getOrCreateVertex(node1);
		auto vertex2 = getOrCreateVertex(node2);

		// Find or create the edge
		auto edge = findEdgebyVertexes(*vertex1, *vertex2);
		if (!edge) {
			edge = &newEdge();
			edge->addVertex(*vertex1, 0);
			edge->addVertex(*vertex2, 1);
			vertex1->addEdge(*edge);
			vertex2->addEdge(*edge);
			edge->ID = MeshEdges.size();
		}

		// Add the edge to the face
		edge->addFace(face);
		face.addEdge(*edge);
	}

	face.setClassification(*GeomEntities[physical]);

	if (type == 9 || type == 10) { // Higher-order elements
		size_t edgeNodeCount = (type == 9) ? 3 : 4; // Triangle (3 edges) or Quad (4 edges)
		for (size_t n = 0; n < edgeNodeCount; ++n) {
			size_t edgeNodeID;
			meshFile >> edgeNodeID;
			auto edge = face.getEdge(n);
			if (edge) {
				edge->node = findNodebyID(edgeNodeID);
			}
		}
		if (type == 10) { // For quads, add the face node
			size_t faceNodeID;
			meshFile >> faceNodeID;
			face.node = findNodebyID(faceNodeID);
		}
	}
}

void Mesh::handleTetrahedronElement(std::ifstream& meshFile, int physical) {
	size_t nodeIDs[4];
	for (int i = 0; i < 4; ++i) {
		meshFile >> nodeIDs[i];
	}

	// Get or create vertices for the tetrahedron
	MeshVertex* vertices[4];
	for (int i = 0; i < 4; ++i) {
		vertices[i] = getOrCreateVertex(nodeIDs[i]);
	}

	// Create or find the faces
	auto face1 = getOrCreateFace(vertices[0], vertices[1], vertices[2]);
	auto face2 = getOrCreateFace(vertices[1], vertices[2], vertices[3]);
	auto face3 = getOrCreateFace(vertices[2], vertices[3], vertices[0]);
	auto face4 = getOrCreateFace(vertices[3], vertices[0], vertices[1]);

	// Create a new region
	auto& region = newRegion();
	
	region.addFace(face1);
	region.addFace(face2);
	region.addFace(face3);
	region.addFace(face4);

	face1->addRegion(region);
	face2->addRegion(region);
	face3->addRegion(region);
	face4->addRegion(region);

	region.setClassification(*GeomEntities[physical]);
}

MeshVertex* Mesh::findVertexbyNode(size_t n) const {
	//TODO: This loops through all nodes until it finds the matching nodes.  SLOW!
	if (MeshVertexes.size() > 1) {
		for (const auto& vertex : MeshVertexes) {
			if (vertex->node->ID == n) {
				return vertex.get();
			}
		}
	}

	return nullptr;
};

MeshEdge* Mesh::findEdgebyVertexes(MeshVertex& vertex1, MeshVertex& vertex2) {
	for (const auto& edge : vertex1.Edges()) {
		if (edge->getVertex(0) == &vertex2 || edge->getVertex(1) == &vertex2) {
			return edge;
		}
	}
	//edge hasn't been added yet
	MeshEdge* new_edge = &newEdge();
	new_edge->addVertex(vertex1, 0);
	new_edge->addVertex(vertex2, 1);
	new_edge->setClassification(*GeomEntities[0]);
	vertex1.addEdge(*new_edge);
	vertex2.addEdge(*new_edge);
	new_edge->ID = MeshEdges.size();
	return new_edge;
};

std::pair<MeshFace*, bool> Mesh::findFacebyEdge(MeshEdge& edge1, MeshEdge& edge2, MeshEdge& edge3) {
	//FIXME this only works for triangles!
	for (const auto& face : edge1.Faces()) {
		if (face->getEdge(1) == &edge2 || face->getEdge(2) == &edge2) {
			std::pair<MeshFace*, bool> f(face, true);  //face already added, so this region uses face in opposite direction
			return f;
		}
	}
	MeshFace& face1 = newFace();
	face1.addEdge(edge1);
	face1.addEdge(edge2);
	face1.addEdge(edge3);
	edge1.addFace(face1);
	edge2.addFace(face1);
	edge3.addFace(face1);
	face1.setClassification(*GeomEntities[0]);
	std::pair<MeshFace*, bool> f(&face1, false);
	return f;
};

Node* Mesh::findNodebyID(size_t n) const {
	for (const auto& node : Nodes) {
		if (node->ID == n) {
			return node.get();
		}
	}
	return nullptr;
}

void Mesh::readAttributes(const std::string& attribfile) {
	string key;
	double value;
	int idx;
	GeomEntity* entity{};

	ifstream fs(attribfile);

	if (fs.fail()) {
		throw std::runtime_error("Unable to open file: " + attribfile);
	}

	while (!fs.eof() && !fs.fail()) {
		fs >> key >> value;

		if (key == "GeomEntity") {
			idx = (int)value;
			//cout << "Adding GeomEntity at index " << idx << endl;
			GeomEntities[idx] = make_unique<GeomEntity>();
			entity = GeomEntities[idx].get();
		}
		else {
			//cout << "Adding key " << key << " = " << value << endl;
			//Warning: if a GeomEntity key is not found (and therefore
			//a GeomEntity is not created) we will crash here on access!!!
			if (entity != nullptr)
			{
				entity->addAttribute(key, value);
			}
		}
	}
};

