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

#include <string>
#include <iostream>
#include <iomanip>
#include <queue>
#include <sstream>

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
		throw std::runtime_error("ERROR: MeshRegion index out of range");
	}
}

MeshFace& Mesh::getFace(size_t n) const {
	if (n < MeshFaces.size()) {
		return *MeshFaces[n];
	}
	else {
		throw std::runtime_error("ERROR: MeshFace index out of range");
	}
}

MeshEdge& Mesh::getEdge(size_t n) const {
	if (n < MeshEdges.size()) {
		return *MeshEdges[n];
	}
	else {
		throw std::runtime_error("ERROR: MeshEdge index out of range");
	}
}

MeshVertex& Mesh::getVertex(size_t n) const {
	if (n < MeshVertexes.size()) {
		return *MeshVertexes[n];
	}
	else {
		throw std::runtime_error("ERROR: MeshVertex index out of range");
	}
}

Node& Mesh::getNode(size_t n) const {
	if (n < Nodes.size()) {
		return *Nodes[n];
	}
	else {
		throw std::runtime_error("ERROR: Node index out of range");
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
		node->setID(-2); //-2 indicates not labeled and not in queue
	}
	for (const auto& face : MeshFaces) {
		face->setID(-2); //-2 indicates not labeled and not in queue
	}
	for (const auto& region : MeshRegions) {
		region->setID(-2); //-2 indicates not labeled and not in queue
	}

	//cout << "Get start...\n";

	MeshEntity* entity = getStart();
	//cout << "Put first entity into queue...\n";
	q.push(entity);
	entity->setID(-1); //0 indicates entity in queue
	//cout << "Begin looping through queue...\n";
	while (q.size() > 0) {
		if (labelnode < 0) {
			cout << "ERROR! labelnode<0!\n";
			return;
		}
		entity = q.front();
		q.pop();
		//if(entity->get_dimensions()==1){cout << "dequeueing edge\n";}
		auto node = entity->getNode();
		if (node->getID() < 0) { //node is unlabeled
			labelnode = labelnode - 1;
			node->setID(labelnode);
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
					if (face->getID() < 0) { //face is unlabeled
						labelface = labelface - 1;
						face->setID(labelface);
					}
					if (face->getNode() != nullptr && face->getNode()->getID() < -1) {
						q.push(face);
						face->getNode()->setID(-1);
					}
				}
				auto othervertex = edge->otherVertex(*vertex);
				if (edge->getNode() != nullptr) {  //if edge has a node
					//cout << "Edge has a node\n";
					if (othervertex->getNode()->getID() >= -1 && edge->getNode()->getID() < -1) {
						//cout << "Labeling node directly\n";
						labelnode = labelnode - 1;
						edge->getNode()->setID(labelnode);
						const auto& DOFs = edge->getNode()->getDOFs();
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
					else if (othervertex->getNode()->getID() < -1) {
						//cout << "Queueing edge\n";
						q.push(edge);
						list.push(othervertex);
						othervertex->getNode()->setID(-1);
					}
				}
				else {
					if (othervertex->getNode()->getID() < -1) {
						//cout << "Queue edge's other vertex\n";
						list.push(othervertex);
						othervertex->getNode()->setID(-1);
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
				new_node.setID(id);
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
						vertex->setNode(*findNodebyID(nodeID));
						vertex->setID(MeshVertexes.size());
					}
					vertex->setClassification(*getGeomEntity(physical));
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
						new_edge->setID(MeshEdges.size());
					}
					new_edge->setClassification(*getGeomEntity(physical));

					if (type == 8) {
						size_t node3;
						meshFile >> node3;
						new_edge->setNode(*findNodebyID(node3));
					}
				}
				else if (type == 2 || type == 3 || type == 9 || type == 10) { //2D elements
					handleFaceElement(meshFile, id, type, numNodes, numEdges, physical);
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
		newVertex.setNode(*findNodebyID(nodeID));
		if (!newVertex.getNode()) {
			throw std::runtime_error("Node " + std::to_string(nodeID) + " does not exist.");
		}
		newVertex.setID(MeshVertexes.size());
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

void Mesh::handleFaceElement(std::ifstream& meshFile, size_t id, int type, size_t numNodes, size_t numEdges, int physical) {
	size_t firstNode, node1, node2;

	// Create a new face
	auto& face = newFace();

	face.setID(id);

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
			edge->setID(MeshEdges.size());
		}

		// Add the edge to the face
		edge->addFace(face);
		face.addEdge(*edge);
	}

	face.setClassification(*getGeomEntity(physical));

	if (type == 9 || type == 10) { // Higher-order elements
		size_t edgeNodeCount = (type == 9) ? 3 : 4; // Triangle (3 edges) or Quad (4 edges)
		for (size_t n = 0; n < edgeNodeCount; ++n) {
			size_t edgeNodeID;
			meshFile >> edgeNodeID;
			auto edge = face.getEdge(n);
			if (edge) {
				edge->setNode(*findNodebyID(edgeNodeID));
			}
		}
		if (type == 10) { // For quads, add the face node
			size_t faceNodeID;
			meshFile >> faceNodeID;
			face.setNode(*findNodebyID(faceNodeID));
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

	region.setClassification(*getGeomEntity(physical));
}

MeshVertex* Mesh::findVertexbyNode(size_t n) const {
	//TODO: This loops through all nodes until it finds the matching nodes.  SLOW!
	if (MeshVertexes.size() > 1) {
		for (const auto& vertex : MeshVertexes) {
			if (vertex->getNode()->getID() == n) {
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
	new_edge->setID(MeshEdges.size());
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
		if (node->getID() == n) {
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
			entity->ID = idx;
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

GeomEntity* Mesh::getGeomEntity(int idx) const {
    auto it = GeomEntities.find(idx);
    if (it != GeomEntities.end()) {
        return it->second.get();
    }
    throw std::runtime_error("Error: GeomEntity with index " + std::to_string(idx) + " not found.");
}

void Mesh::writeMesh(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + filename);
	}

	// Write the mesh format section
	file << "$MeshFormat\n";
	file << "2.2 0 8\n"; // GMSH format version 2.2, ASCII, double precision
	file << "$EndMeshFormat\n";

	// Write the nodes section
	file << "$Nodes\n";
	std::vector<Node*> validNodes;

	// Collect valid nodes (associated with classified elements)
	for (const auto& nodePtr : Nodes) {
		Node& node = *nodePtr;
		bool isValid = false;

		// Check if the node belongs to any valid classified entity
		for (const auto& edgePtr : MeshEdges) {
			if (edgePtr.get() == node.getParent() && edgePtr->getClassificationID() > 0) {
				isValid = true;
				break;
			}
		}
		for (const auto& facePtr : MeshFaces) {
			if (facePtr.get() == node.getParent() && facePtr->getClassificationID() > 0) {
				isValid = true;
				break;
			}
		}
		isValid = true; //shunt the above logic to force all nodes to be written
		if (isValid) {
			validNodes.push_back(&node);
		}
	}

	file << validNodes.size() << "\n";
	for (const auto* node : validNodes) {
		file << node->getID() << " "
			<< std::fixed << std::setprecision(6)
			<< node->x() << " " << node->y() << " " << node->z() << "\n";
	}
	file << "$EndNodes\n";

	// Write the elements section
	file << "$Elements\n";
	std::vector<std::string> validElements;

	// Collect valid elements
	for (const auto& vertexPtr : MeshVertexes) {
		MeshVertex& vertex = *vertexPtr;
		if (vertex.getClassificationID() > 0) {
			std::ostringstream oss;
			oss << vertex.getID() << " 15 1 " << vertex.getClassificationID() << " " << vertex.getNode()->getID();
			validElements.push_back(oss.str());
		}
	}

	// Collect valid elements
	for (const auto& edgePtr : MeshEdges) {
		MeshEdge& edge = *edgePtr;
		if (edge.getClassificationID() > 0) {
			std::ostringstream oss;
			oss << edge.getID() << " 1 1 " << edge.getClassificationID() << " ";
			for (const auto* vertex : edge.getVertices()) {
				oss << vertex->getNode()->getID() << " ";
			}
			validElements.push_back(oss.str());
		}
	}

	for (const auto& facePtr : MeshFaces) {
		MeshFace& face = *facePtr;
		if (face.getClassificationID() > 0) {
			std::ostringstream oss;
			oss << face.getID() << " 2 1 " << face.getClassificationID() << " ";
			for (const auto* vertex : face.getVertices()) {
				oss << vertex->getNode()->getID() << " ";
			}
			validElements.push_back(oss.str());
		}
	}

	/*for (const auto& regionPtr : MeshRegions) {
		MeshRegion& region = *regionPtr;
		if (region.getClassification()->ID > 0) {
			std::ostringstream oss;
			oss << region.getID() << " 4 1 " << region.getClassificationID() << " ";
			for (const auto* vertex : region.getVertices()) {
				oss << vertex->getNodeID() << " ";
			}
			validElements.push_back(oss.str());
		}
	}*/

	file << validElements.size() << "\n";
	for (const auto& element : validElements) {
		file << element << "\n";
	}
	file << "$EndElements\n";

	file.close();
}


//void Mesh::writeMesh(const std::string& filename) const {
//	std::ofstream file(filename);
//	if (!file.is_open()) {
//		throw std::runtime_error("Failed to open file: " + filename);
//	}
//
//	// Write the mesh format section
//	file << "$MeshFormat\n";
//	file << "2.2 0 8\n"; // GMSH format version 2.2, ASCII, double precision
//	file << "$EndMeshFormat\n";
//
//	// Write the nodes section
//	file << "$Nodes\n";
//	file << Nodes.size() << "\n"; // Number of nodes
//	for (const auto& nodePtr : Nodes) {
//		const Node& node = *nodePtr;
//		file << node.ID << " "
//			<< std::fixed << std::setprecision(6)
//			<< node.x() << " "
//			<< node.y() << " "
//			<< node.z() << "\n";
//	}
//	file << "$EndNodes\n";
//
//	// Write the elements section
//	file << "$Elements\n";
//	size_t totalElements = MeshFaces.size() + MeshEdges.size() + MeshRegions.size();
//	file << totalElements << "\n";
//
//	// Write faces
//	for (const auto& facePtr : MeshFaces) {
//		const MeshFace& face = *facePtr;
//		file << face.ID << " "        // Element ID
//			<< 2 << " " // GMSH element type (e.g., triangle)
//			<< "0 ";                     // No physical or geometrical tags
//		for (const auto& vertex : face.getVertices()) {
//			file << vertex->ID << " ";
//		}
//		file << "\n";
//	}
//
//	// Write edges
//	for (const auto& edgePtr : MeshEdges) {
//		const MeshEdge& edge = *edgePtr;
//		file << edge.ID << " "       // Element ID
//			<< 1 << " " // GMSH element type (e.g., line)
//			<< "0 ";                     // No physical or geometrical tags
//		for (const auto& vertex : edge.getVertices()) {
//			file << vertex->ID << " ";
//		}
//		file << "\n";
//	}
//
//	// Write regions
//	//for (const auto& regionPtr : MeshRegions) {
//	//	const MeshRegion& region = *regionPtr;
//	//	file << region.ID << " "        // Element ID
//	//		<< region.getGMSHType() << " " // GMSH element type (e.g., tetrahedron)
//	//		<< "0 ";                       // No physical or geometrical tags
//	//	for (const auto& vertex : region.getVertices()) {
//	//		file << vertex->ID << " ";
//	//	}
//	//	file << "\n";
//	//}
//
//	file << "$EndElements\n";
//
//	file.close();
//}

//void Mesh::writeMesh(const std::string& filename) const {
//	std::ofstream meshFile(filename);
//	if (!meshFile.is_open()) {
//		std::cerr << "Unable to open file: " << filename << std::endl;
//		return;
//	}
//
//	// Write the mesh format
//	meshFile << "$MeshFormat\n";
//	meshFile << "2.2 0 8\n";
//	meshFile << "$EndMeshFormat\n";
//
//	// Write the nodes
//	meshFile << "$Nodes\n";
//	meshFile << Nodes.size() << "\n";
//	for (const auto& node : Nodes) {
//		meshFile << node->ID << " " << node->pt().x << " " << node->pt().y << " " << node->pt().z << "\n";
//	}
//	meshFile << "$EndNodes\n";
//
//	// Write the elements
//	meshFile << "$Elements\n";
//	size_t numElements = MeshVertexes.size() + MeshEdges.size() + MeshFaces.size() + MeshRegions.size();
//	meshFile << numElements << "\n";
//
//	// Write vertices
//	for (const auto& vertex : MeshVertexes) {
//		meshFile << vertex->ID << " 15 2 " << vertex->getClassificationID() << " " << vertex->ID << " " << vertex->node->ID << "\n";
//	}
//
//	// Write edges
//	for (const auto& edge : MeshEdges) {
//		meshFile << edge->ID << " 1 2 " << edge->getClassificationID() << " " << edge->ID << " " << edge->getVertex(0)->node->ID << " " << edge->getVertex(1)->node->ID << "\n";
//	}
//
//	// Write faces
//	for (const auto& face : MeshFaces) {
//		meshFile << face->ID << " 2 2 " << face->getClassificationID() << " " << face->ID;
//		for (const auto& edge : face->MeshEdges) {
//			meshFile << " " << edge->getVertex(0)->node->ID << " " << edge->getVertex(1)->ID;
//		}
//		meshFile << "\n";
//	}
//
//	// Write regions
//	/*for (const auto& region : MeshRegions) {
//		meshFile << region->ID << " 4 2 " << region->getClassification().lock()->AttribID << " " << region->ID;
//		for (const auto& face : region->Faces) {
//			if (auto locked_face = face.lock()) {
//				for (const auto& edge : locked_face->MeshEdges) {
//					if (auto locked_edge = edge.lock()) {
//						meshFile << " " << locked_edge->getVertex(0).lock()->node.lock()->ID << " " << locked_edge->getVertex(1).lock()->ID;
//					}
//				}
//			}
//		}
//		meshFile << "\n";
//	}*/
//
//	meshFile << "$EndElements\n";
//	meshFile.close();
//}


