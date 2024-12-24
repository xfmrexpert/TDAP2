/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "meshface.h"
#include "meshregion.h"
#include "meshedge.h"
#include "meshvertex.h"
#include "math.h"
#include <iostream>

const std::vector<MeshEdge*>& MeshFace::Edges() const {
    return MeshEdges;
}

MeshEdge* MeshFace::getEdge(int n) {
     if (n < 0 || n >= MeshEdges.size()) {
         throw std::out_of_range("Invalid edge index");
     }
     return MeshEdges[n];
}

MeshRegion* MeshFace::getRegion(int n) {
     if (n < 0 || n >= MeshRegions.size()) {
         throw std::out_of_range("Invalid face index");
     }
     return MeshRegions[n];
}

void MeshFace::addEdge(MeshEdge& new_edge) {
     if (std::find(MeshEdges.begin(), MeshEdges.end(), &new_edge) == MeshEdges.end()) {
         MeshEdges.push_back(&new_edge);
     }
}

void MeshFace::addRegion(MeshRegion& new_region) {
     if (std::find(MeshRegions.begin(), MeshRegions.end(), &new_region) == MeshRegions.end()) {
         MeshRegions.push_back(&new_region);
     }
}

size_t MeshFace::numEdges() {
     return MeshEdges.size();
}

double MeshFace::area() {
     double Area = 0;
     
     auto edge_iter = MeshEdges.begin();
     auto firstEdge = *edge_iter;
     auto firstVertex = firstEdge->getVertex(0);
     
     for(edge_iter++;edge_iter!=MeshEdges.end();++edge_iter){
        if(!(*edge_iter)->isConnected(*firstVertex)){	   
	       double X1;
	       double X2;
	       double X3;
	       double Y1;
	       double Y2;
	       double Y3;
	     
	       X1 = firstVertex->node->x();
	       X2 = (*edge_iter)->getVertex(0)->node->x();
	       X3 = (*edge_iter)->getVertex(1)->node->x();
	       Y1 = firstVertex->node->y();
	       Y2 = (*edge_iter)->getVertex(0)->node->y();
	       Y3 = (*edge_iter)->getVertex(1)->node->y();
           
	       Area = Area + 0.5 * fabs(X1*(Y3-Y2)+X2*(Y1-Y3)+X3*(Y2-Y1));
	    }
     }
     
     return Area;
}

vector<Node*> MeshFace::getNodes() const{
   vector<Node*> nodes;
   //add nodes for each vertex of face first, in order
   //This assumes mesh data structure is already ordered!!!
   auto edge_iter=MeshEdges.begin(); //Get first edge;
   auto first_edge=(*edge_iter);
   ++edge_iter; //Get second edge;
   if((*edge_iter)->getVertex(0)==first_edge->getVertex(0) || (*edge_iter)->getVertex(1)==first_edge->getVertex(0)){
      nodes.push_back(first_edge->getVertex(1)->node);
      nodes.push_back(first_edge->getVertex(0)->node);
   }else{ 
      nodes.push_back(first_edge->getVertex(0)->node);
      nodes.push_back(first_edge->getVertex(1)->node);
   }

   for(/*edge_iter=second edge*/;(*edge_iter)!=MeshEdges.back();++edge_iter){
      if((*edge_iter)->getVertex(0)->node == nodes.back()){
         nodes.push_back((*edge_iter)->getVertex(1)->node);
      }else{
         nodes.push_back((*edge_iter)->getVertex(0)->node);
      }
   }

   //add nodes for all edges (if present) next
   for(const auto& edge : MeshEdges){
      if(edge->node != nullptr){
         nodes.push_back((*edge_iter)->node);
      }
   }

   //add node in face (if present) last
   if(this->node != nullptr){
      nodes.push_back(this->node);
   }

   return nodes;
}

vector<const MeshVertex*> MeshFace::getVertices() const{
    vector<const MeshVertex*> vertices;

    // Add nodes for each vertex of face first, in order
    auto edge_iter = MeshEdges.begin(); // Get first edge
    auto first_edge = (*edge_iter);
    edge_iter++; // Get second edge
    if ((*edge_iter)->getVertex(0) == first_edge->getVertex(0) ||
        (*edge_iter)->getVertex(1) == first_edge->getVertex(0)) {
        vertices.push_back(first_edge->getVertex(1));
        vertices.push_back(first_edge->getVertex(0));
    }
    else {
        vertices.push_back(first_edge->getVertex(0));
        vertices.push_back(first_edge->getVertex(1));
    }

    for (/* edge_iter = second edge */; (*edge_iter) != MeshEdges.back(); ++edge_iter) {
        if ((*edge_iter)->getVertex(0) == vertices.back()) {
            vertices.push_back((*edge_iter)->getVertex(1));
        }
        else {
            vertices.push_back((*edge_iter)->getVertex(0));
        }
    }

    return vertices; // Return by value
}

