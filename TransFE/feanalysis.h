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
	   fe_space_ptr->setupGlobalDofs();

	   for (size_t i = 0; i < mesh->numVertexes(); i++) { //process all mesh vertexes
		   auto& v = mesh->getVertex(i); //get vertex object
		   //cout << "Processing vertex #" << i << "(" << v->ID << ")" << endl;
		   auto c = makeConstraint(v); //create a constraint for the vertex
		   if (c != nullptr) { //check if the vertex is contrained
			   DS->add(std::move(c)); //if so, add the vertex to the list of constraints
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
   FESpace<T>* fe_space_ptr;
   std::unique_ptr<DiscreteSystem<T>> DS;
   std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

private:

};

