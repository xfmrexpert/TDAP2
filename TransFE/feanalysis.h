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
#include "stiffnesscontributor.h"
#include "forcecontributor.h"
#include "constraint.h"

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
   void run();

   /// Determines which finite element entities are contributors and creates
   /// instances of the appropriate classes
   void setup();
   
   /// Returns a pointer to the mesh
   std::shared_ptr<Mesh> getMesh();
   
   // The following virtual functions must be implemented in each derived analysis class

   /// Processes the contributors to evaluate, assemble and solve
   virtual void solve() = 0;
   
   /// Recover them all important secondary variables
   virtual void recover() = 0;
   
   /// Create the appropriate StiffnessContributor for the face "element"
   virtual std::unique_ptr<StiffnessContributor<T>> makeStiffContrib(MeshFace& face) = 0;

   /// Create the appropriate StiffnessContributor for the face "element"
   virtual std::unique_ptr<StiffnessContributor<T>> makeStiffContrib(MeshRegion& region) = 0;

   virtual std::unique_ptr<ForceContributor<T>> makeForceContrib(MeshRegion& region) = 0;

   /// Create the appropriate ForceContributor for a face body force
   virtual std::unique_ptr<ForceContributor<T>> makeForceContrib(MeshFace& face) = 0;

   /// Create the appropriate ForceContributor for an edge force
   virtual std::unique_ptr<ForceContributor<T>> makeForceContrib(MeshEdge& edge) = 0;

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
   std::shared_ptr<Mesh> theMesh = std::make_shared<Mesh>();
   std::unique_ptr<Field<T>> field = std::make_unique<Field<T>>();

private:

};

