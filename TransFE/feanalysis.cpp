/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#include "feanalysis.h"
#include "discretesystem.h"
 //#include "precisiontimer.h"

FEAnalysis::FEAnalysis() {
	//theMesh = new Mesh();
};

FEAnalysis::~FEAnalysis() {

};

shared_ptr<Mesh> FEAnalysis::getMesh() {
	return theMesh;
};

void FEAnalysis::run() {
	setup(); //determine the contributors
	solve(); //process the contributors to determine the system
	//perform integrations over contributors, assemble and solve system
	recover(); //recover secondary variables
};

void FEAnalysis::setup() {

	for (size_t i = 0; i < theMesh->numRegions(); i++) {
		auto r = theMesh->getRegion(i);

		auto sc = makeStiffContrib(r);
		if (sc != nullptr) {
			DS.add(sc);
		}
		auto fc = makeForceContrib(r);
		if (fc != nullptr) {
			DS.add(fc);
		}
	}

	for (size_t i = 0; i < theMesh->numFaces(); i++) { //process all the mesh faces
		//cout << "Processing face #" << i << endl;
		auto f = theMesh->getFace(i); //get face object
		auto sc = makeStiffContrib(f); //set-up type of stiffness contributor for the face
		if (sc != nullptr) {
			DS.add(sc);
		}
		auto fc = makeForceContrib(f); //set-up force contributor for the face
		if (fc != nullptr) { //check if the face contributes to the force vector
			DS.add(fc); //if so, add the face to the list of force contributors
		}
		auto c = makeConstraint(f); //setup contraint for the edge
		if (c != nullptr) { //check if the edge is constrained
			DS.add(c); //if so, add the edge to the list of constraints
		}
	}

	for (size_t i = 0; i < theMesh->numEdges(); i++) { //process all mesh edges
		auto e = theMesh->getEdge(i); //get edge object
		//cout << "Processing edge #" << i << "(" << e->ID << ")" << endl;
		auto fc = makeForceContrib(e); //setup force contributor for the edge
		if (fc != nullptr) { //check if the edge contributes to the force vector
			DS.add(fc); //if so, add the edge to the list of force contributors
		}
		auto c = makeConstraint(e); //setup contraint for the edge
		if (c != nullptr) { //check if the edge is constrained
			DS.add(c); //if so, add the edge to the list of constraints
		}
	}

	for (size_t i = 0; i < theMesh->numVertexes(); i++) { //process all mesh vertexes
		auto v = theMesh->getVertex(i); //get vertex object
		//cout << "Processing vertex #" << i << "(" << v->ID << ")" << endl;
		auto c = makeConstraint(v); //create a constraint for the vertex
		if (c != nullptr) { //check if the vertex is contrained
			DS.add(c); //if so, add the vertex to the list of constraints
		}
	}
};
