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

#include "MeshDB/meshregion.h"
#include "MeshDB/meshface.h"
#include "MeshDB/meshedge.h"
#include "MeshDB/meshvertex.h"
#include "MeshDB/mesh.h"
#include "discretesystem.h"
#include "constraint.h"
#include "FESpace.h"
#include "BilinearForm.h"
#include "LinearForm.h"

#include <sstream>

/// This class represents a generic finite element analysis.
/// This is the base class for specific finite element analysis implementations
/// For an example implemeentation, refer to ElasticityAnalysis

template <typename T>
class FEAnalysis {

public:

   /// Default constructor
   FEAnalysis() = default;

   /// Virtual default destructor
   ~FEAnalysis() = default;  

   /// Runs the analysis steps (setup, solve, recover)
   void run() {
	   setup(); //determine the contributors
	   solve(); //process the contributors to determine the system
	   //perform integrations over contributors, assemble and solve system
	   recover(); //recover secondary variables
   };

   /// Determines which finite element entities are contributors and creates
   /// instances of the appropriate classes
   void setup() {
	   //fe_space = std::make_unique<FESpace<T>>(); //initialize the field object
	   fe_space->setupGlobalDofs();

	   /*for (size_t i = 0; i < theMesh->numRegions(); i++) {
		   auto& r = theMesh->getRegion(i);

		   auto sc = makeStiffContrib(r);
		   if (sc != nullptr) {
			   DS.add(std::move(sc));
		   }
		   auto fc = makeForceContrib(r);
		   if (fc != nullptr) {
			   DS.add(std::move(fc));
		   }
	   }*/

	   for (size_t i = 0; i < mesh->numFaces(); i++) { //process all the mesh faces
		   //cout << "Processing face #" << i << endl;
		   auto& f = mesh->getFace(i); //get face object
		   // NOTE: Constraints need to be applied before any assembly occurs
		   // At present, evaluating the bilinear and linear forms will result in assembly
		   bilinear_form->evaluate(f); //evaluate the bilinear form for the face
		   linear_form->evaluate(f); //evaluate the linear form for the face
		   //auto sc = makeStiffContrib(f); //set-up type of stiffness contributor for the face
		   //if (sc != nullptr) {
			  // DS.add(std::move(sc));
		   //}
		   //auto fc = makeForceContrib(f); //set-up force contributor for the face
		   //if (fc != nullptr) { //check if the face contributes to the force vector
			  // DS.add(std::move(fc)); //if so, add the face to the list of force contributors
		   //}
		   //auto c = makeConstraint(f); //setup contraint for the edge
		   //if (c != nullptr) { //check if the edge is constrained
			  // DS.add(std::move(c)); //if so, add the edge to the list of constraints
		   //}
	   }

	   for (size_t i = 0; i < mesh->numEdges(); i++) { //process all mesh edges
		   //auto& e = theMesh->getEdge(i); //get edge object
		   ////cout << "Processing edge #" << i << "(" << e->ID << ")" << endl;
		   //auto fc = makeForceContrib(e); //setup force contributor for the edge
		   //if (fc != nullptr) { //check if the edge contributes to the force vector
			  // DS.add(std::move(fc)); //if so, add the edge to the list of force contributors
		   //}
		   //auto c = makeConstraint(e); //setup contraint for the edge
		   //if (c != nullptr) { //check if the edge is constrained
			  // DS.add(std::move(c)); //if so, add the edge to the list of constraints
		   //}
	   }

	   for (size_t i = 0; i < mesh->numVertexes(); i++) { //process all mesh vertexes
		   auto& v = mesh->getVertex(i); //get vertex object
		   //cout << "Processing vertex #" << i << "(" << v->ID << ")" << endl;
		   auto c = makeConstraint(v); //create a constraint for the vertex
		   if (c != nullptr) { //check if the vertex is contrained
			   DS.add(std::move(c)); //if so, add the vertex to the list of constraints
		   }
	   }
   };
   
   /// Returns a pointer to the mesh
   std::shared_ptr<Mesh> getMesh() {
	   return mesh;
   };
   
   // The following virtual functions must be implemented in each derived analysis class

   /// Processes the contributors to evaluate, assemble and solve
   virtual void solve() = 0;
   
   /// Recover them all important secondary variables
   virtual void recover() = 0;
   
   /*/// Create the appropriate StiffnessContributor for the face "element"
   virtual std::unique_ptr<StiffnessContributor<T>> makeStiffContrib(MeshFace& face) = 0;

   /// Create the appropriate StiffnessContributor for the face "element"
   virtual std::unique_ptr<StiffnessContributor<T>> makeStiffContrib(MeshRegion& region) = 0;

   virtual std::unique_ptr<ForceContributor<T>> makeForceContrib(MeshRegion& region) = 0;

   /// Create the appropriate ForceContributor for a face body force
   virtual std::unique_ptr<ForceContributor<T>> makeForceContrib(MeshFace& face) = 0;

   /// Create the appropriate ForceContributor for an edge force
   virtual std::unique_ptr<ForceContributor<T>> makeForceContrib(MeshEdge& edge) = 0;*/

   /// Create the appropriate Constraint for a face
   virtual std::unique_ptr<Constraint<T>> makeConstraint(MeshFace& face) = 0;

   /// Create the appropriate Constraint for an edge
   virtual std::unique_ptr<Constraint<T>> makeConstraint(MeshEdge& edge) = 0;

   /// Create the appropriate Constraint for a vertex
   virtual std::unique_ptr<Constraint<T>> makeConstraint(MeshVertex& vertex) = 0;

   /// For the saving of the output
   virtual void saveOut(const std::string& filename) = 0;

   std::stringstream outStream;
   
  
protected:
  
   DiscreteSystem<T> DS;
   std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
   std::unique_ptr<FESpace<T>> fe_space;
   std::unique_ptr<BilinearForm> bilinear_form;
   std::unique_ptr<LinearForm> linear_form;

private:

};

