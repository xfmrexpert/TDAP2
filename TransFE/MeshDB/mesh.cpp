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

Mesh::~Mesh() {}

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

MeshRegion& Mesh::getRegion(size_t n) {
	return *MeshRegions[n];
}

MeshFace& Mesh::getFace(size_t n) {
	return *MeshFaces[n];
}

MeshEdge& Mesh::getEdge(size_t n) {
	return *MeshEdges[n];
}

MeshVertex& Mesh::getVertex(size_t n) {
	return *MeshVertexes[n];
}

Node& Mesh::getNode(size_t n) {
	return *Nodes[n];
}

std::vector<std::unique_ptr<MeshRegion>>::iterator Mesh::getFirstRegion() {
	return MeshRegions.begin();
}

std::vector<std::unique_ptr<MeshRegion>>::iterator Mesh::getLastRegion() {
	return MeshRegions.end();
}

std::vector<std::unique_ptr<MeshFace>>::iterator Mesh::getFirstFace() {
	return MeshFaces.begin();
}

std::vector<std::unique_ptr<MeshFace>>::iterator Mesh::getLastFace() {
	return MeshFaces.end();
}

std::vector<std::unique_ptr<MeshEdge>>::iterator Mesh::getFirstEdge() {
	return MeshEdges.begin();
}

std::vector<std::unique_ptr<MeshEdge>>::iterator Mesh::getLastEdge() {
	return MeshEdges.end();
}

std::vector<std::unique_ptr<MeshVertex>>::iterator Mesh::getFirstVertex() {
	return MeshVertexes.begin();
}

std::vector<std::unique_ptr<MeshVertex>>::iterator Mesh::getLastVertex() {
	return MeshVertexes.end();
}

std::vector<std::unique_ptr<Node>>::iterator Mesh::getFirstNode() {
	return Nodes.begin();
}

std::vector<std::unique_ptr<Node>>::iterator Mesh::getLastNode() {
	return Nodes.end();
}

size_t Mesh::numRegions() {
	return MeshRegions.size();
}

size_t Mesh::numFaces() {
	return MeshFaces.size();
}

size_t Mesh::numEdges() {
	return MeshEdges.size();
}

size_t Mesh::numVertexes() {
	return MeshVertexes.size();
}

size_t Mesh::numNodes() {
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

int getNumNodes(int type)
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

int getNumEdges(int type)
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

void Mesh::readMesh(string filename)
{
	double version = 1.0;
	system("cd");
	string line;
	ifstream meshFile;
	meshFile.open(filename + ".msh");
	if (meshFile.is_open())
	{
		while (getline(meshFile, line))
		{
			if (line.empty() || line.front() != '$')
			{
				continue;
			}
			if (line.compare(1, 10, "MeshFormat") == 0) {
				int format, size;
				meshFile >> version >> format >> size;
				if (version < 2.0) {
					cerr << "Error: Wrong msh file version " << version << endl;
					exit(1);
				}
				if (format) {
					cerr << "Error: Unknown data format for mesh" << endl;
					exit(1);
				}
			}
			else if (line.compare(1, 5, "Nodes") == 0) {
				size_t num_nodes;
				meshFile >> num_nodes;
				for (size_t i = 0; i < num_nodes; i++) {
					size_t num;
					double x, y, z;
					meshFile >> num >> x >> y >> z;
					if (numNodes() == num) {
						cerr << "Error: node " << num << " already exists" << endl;
						exit(1);
					}
					else {
						Node& new_node = newNode(); //newNode() adds a new Node to the Nodes list
						point pt;
						pt.x = x;
						pt.y = y;
						pt.z = z;
						new_node.pt(pt);

						new_node.ID = num;
					}
				}
			}
			else if (line.compare(1, 8, "Elements") == 0) {
				//"Elements" in gmsh terms can be vertexes, edges, faces or regions
				//For 2D meshes, all faces are listed, as well as edges and vertexes that are
				//located on what gmsh calls "physical entities".
				size_t numElements;
				meshFile >> numElements;
				cout << "# of elements = " << numElements << endl;
				for (size_t i = 0; i < numElements; i++) {
					size_t num, numNodes, numTags;
					int type, physical, elementary, partition = 1;
					meshFile >> num >> type >> numTags;
					cout << "Reading element #" << num << ", type " << type << endl;
					elementary = physical = partition = 1;
					for (size_t j = 0; j < numTags; j++) {
						int tag;
						meshFile >> tag;
						if (j == 0) {
							physical = tag;
						}
						else if (j == 1) {
							elementary = tag;
						}
						else if (j == 2) {
							partition = tag;
						}
						// ignore any other tags for now
					}
					cout << "Physical tag = " << physical << endl;
					numNodes = getNumNodes(type);
					size_t numEdges = getNumEdges(type);
					if (!numNodes) {
						cerr << "Error: Unknown type " << type << " for element " << i << endl;
						exit(1);
					}
					if (type == 15) {
						//this is a point, read in node #
						size_t nodenum;
						meshFile >> nodenum;
						MeshVertex* vertex = findVertexbyNode(nodenum);
						if (!vertex) {
							vertex = &newVertex();
							vertex->node = findNodebyID(nodenum); //Nodes[nodenum-1];
							vertex->ID = MeshVertexes.size();
						}
						vertex->setClassification(*GeomEntities[physical]);
					}
					else if (type == 1 || type == 8) {
						//this is a line, read in node #
						size_t numNode1;
						size_t numNode2;
						meshFile >> numNode1 >> numNode2;
						MeshVertex* vertex1 = findVertexbyNode(numNode1);
						if (!vertex1) {
							vertex1 = &newVertex();
							vertex1->node = findNodebyID(numNode1); //Nodes[numNode1-1];
							vertex1->setClassification(*GeomEntities[0]);
							vertex1->ID = MeshVertexes.size();
						}
						MeshVertex* vertex2 = findVertexbyNode(numNode2);
						if (!vertex2) {
							vertex2 = &newVertex();
							vertex2->node = findNodebyID(numNode2); //Nodes[numNode2-1];
							vertex2->setClassification(*GeomEntities[0]);
							vertex2->ID = MeshVertexes.size();
						}
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
							size_t numNode3;
							meshFile >> numNode3;
							new_edge->node = findNodebyID(numNode3);
						}
					}
					else if (type == 2 || type == 3 || type == 9 || type == 10) { //2D
						//Assuming 2D for now
						size_t firstNode;
						size_t numNode1;
						size_t numNode2;
						MeshFace* e = &newFace();
						cout << "Reading Face#" << MeshFaces.size() << endl;
						cout << "# edges = " << numEdges << endl;
						meshFile >> firstNode;
						numNode2 = firstNode;
						for (size_t j = 0; j < numEdges; j++) {
							numNode1 = numNode2;
							if (j < (numEdges - 1)) {
								meshFile >> numNode2;
							}
							else {
								numNode2 = firstNode;
							}
							cout << "Setting up edge " << numNode1 << "-" << numNode2 << endl;
							MeshVertex* vertex1 = findVertexbyNode(numNode1);
							if (!vertex1) {
								vertex1 = &newVertex();
								vertex1->node = findNodebyID(numNode1); //Nodes[numNode1-1];
								vertex1->setClassification(*GeomEntities[0]);
								vertex1->ID = MeshVertexes.size();
							}
							MeshVertex* vertex2 = findVertexbyNode(numNode2);
							if (!vertex2) {
								vertex2 = &newVertex();
								vertex2->node = findNodebyID(numNode2); //Nodes[numNode2-1];
								vertex2->setClassification(*GeomEntities[0]);
								vertex2->ID = MeshVertexes.size();
							}
							MeshEdge* new_edge = findEdgebyVertexes(*vertex1, *vertex2);
							if (!new_edge) { //edge hasn't been added yet
								new_edge = &newEdge();
								new_edge->addVertex(*vertex1, 0);
								new_edge->addVertex(*vertex2, 1);
								new_edge->setClassification(*GeomEntities[0]);
								vertex1->addEdge(*new_edge);
								vertex2->addEdge(*new_edge);
								new_edge->ID = MeshEdges.size();
							}
							new_edge->addFace(*e);
							e->addEdge(*new_edge);
							e->ID = MeshFaces.size();
						}
						e->setClassification(*GeomEntities[physical]);
						if (type == 9) { //2nd order triangle
							//get edge nodes
							for (int n = 0; n < 3; n++) {
								int edge_node;
								meshFile >> edge_node;
								e->getEdge(n)->node = findNodebyID(edge_node);
							}
						}
						if (type == 10) { //2nd order quad
							//get edge nodes
							for (int n = 0; n < 4; n++) {
								int edge_node;
								meshFile >> edge_node;
								e->getEdge(n)->node = findNodebyID(edge_node);
							}
							//get face node
							int face_node;
							meshFile >> face_node;
							e->node = findNodebyID(face_node);
						}
					}
					else if (type == 4) { //tetrahedron
						int numNode1 = 0;
						int numNode2 = 0;
						int numNode3 = 0;
						int numNode4 = 0;
						meshFile >> numNode1 >> numNode3 >> numNode3 >> numNode4;
						MeshVertex* vertex1 = findVertexbyNode(numNode1);
						if (!vertex1) {
							vertex1 = &newVertex();
							vertex1->node = findNodebyID(numNode1); //Nodes[numNode1-1];
							vertex1->setClassification(*GeomEntities[0]);
							vertex1->ID = MeshVertexes.size();
						}
						MeshVertex* vertex2 = findVertexbyNode(numNode2);
						if (!vertex2) {
							vertex2 = &newVertex();
							vertex2->node = findNodebyID(numNode2); //Nodes[numNode2-1];
							vertex2->setClassification(*GeomEntities[0]);
							vertex2->ID = MeshVertexes.size();
						}
						MeshVertex* vertex3 = findVertexbyNode(numNode3);
						if (!vertex3) {
							vertex3 = &newVertex();
							vertex3->node = findNodebyID(numNode3); //Nodes[numNode2-1];
							vertex3->setClassification(*GeomEntities[0]);
							vertex3->ID = MeshVertexes.size();
						}
						MeshVertex* vertex4 = findVertexbyNode(numNode4);
						if (!vertex4) {
							vertex4 = &newVertex();
							vertex4->node = findNodebyID(numNode4); //Nodes[numNode2-1];
							vertex4->setClassification(*GeomEntities[0]);
							vertex4->ID = MeshVertexes.size();
						}
						MeshEdge* edge1 = findEdgebyVertexes(*vertex1, *vertex2);
						MeshEdge* edge2 = findEdgebyVertexes(*vertex2, *vertex3);
						MeshEdge* edge3 = findEdgebyVertexes(*vertex3, *vertex1);
						MeshEdge* edge4 = findEdgebyVertexes(*vertex1, *vertex2);
						MeshEdge* edge5 = findEdgebyVertexes(*vertex4, *vertex3);
						MeshEdge* edge6 = findEdgebyVertexes(*vertex2, *vertex4);

						//tets have 4 faces, we need to loop through each face and check to
						//see if the face has already been added
						//if not, add the face
						//first up, Face #1 - 1-2-3
						pair<MeshFace*, bool> face1 = findFacebyEdge(*edge1, *edge2, *edge3);
						pair<MeshFace*, bool> face2 = findFacebyEdge(*edge3, *edge5, *edge4);
						pair<MeshFace*, bool> face3 = findFacebyEdge(*edge6, *edge5, *edge2);
						pair<MeshFace*, bool> face4 = findFacebyEdge(*edge4, *edge6, *edge1);
						MeshRegion* region = &newRegion();
						region->addFace(face1.first);
						region->addFace(face2.first);
						region->addFace(face3.first);
						region->addFace(face4.first);
						face1.first->addRegion(*region);
						face2.first->addRegion(*region);
						face3.first->addRegion(*region);
						face4.first->addRegion(*region);
						region->setClassification(*GeomEntities[physical]);
						//one all 4 faces have been added, add the face to the region
					}
					else {
						cerr << "Unsupported element type!" << endl;
					}
				}
			}
		}
		meshFile.close();
	}
	else
	{
		cout << "Unable to open file" << endl;
		char error_str[256];
		strerror_s(error_str, _countof(error_str), errno);
		cerr << "Error: " << error_str;
	}
}

MeshVertex* Mesh::findVertexbyNode(size_t n) {
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

Node* Mesh::findNodebyID(size_t n) {
	for (const auto& node : Nodes) {
		if (node->ID == n) {
			return node.get();
		}
	}
	return nullptr;
}

void Mesh::readAttributes(const char* filename) {
	string key;
	double value;
	int idx;
	GeomEntity* entity{};
	char attribfile[256];
	strcpy_s(attribfile, _countof(attribfile), filename);
	strcat_s(attribfile, _countof(attribfile), ".att");

	ifstream fs(attribfile);

	while (!fs.eof() && !fs.fail()) {
		fs >> key >> value;

		if (key == "GeomEntity") {
			idx = (int)value;
			cout << "Adding GeomEntity at index " << idx << endl;
			GeomEntities[idx] = make_unique<GeomEntity>();
			entity = GeomEntities[idx].get();
		}
		else {
			cout << "Adding key " << key << " = " << value << endl;
			//Warning: if a GeomEntity key is not found (and therefore
			//a GeomEntity is not created) we will crash here on access!!!
			if (entity != nullptr)
			{
				entity->addAttribute(key, value);
			}
		}
	}
};

