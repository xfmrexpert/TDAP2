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

vector<shared_ptr<MeshEdge>>::iterator MeshFace::getFirstEdge() {
     return MeshEdges.begin();
}

vector<shared_ptr<MeshEdge>>::iterator MeshFace::getLastEdge() {
     return MeshEdges.end();
}

shared_ptr<MeshEdge> MeshFace::getEdge(int n) {
     return MeshEdges[n];
}

shared_ptr<MeshRegion> MeshFace::getRegion(int n) {
     return MeshRegions[n];
}

void MeshFace::addEdge(shared_ptr<MeshEdge> new_edge) {
     MeshEdges.push_back(new_edge);
}

void MeshFace::addRegion(shared_ptr<MeshRegion> new_region) {
     MeshRegions.push_back(new_region);
}

size_t MeshFace::numEdges() {
     return MeshEdges.size();
}

double MeshFace::area() {
     double Area = 0;
     
     auto edge_iter = getFirstEdge();
     auto firstEdge = *edge_iter;
     auto firstVertex = firstEdge->getVertex(0);
     
     for(edge_iter++;edge_iter!=getLastEdge();++edge_iter){
        if(!(*edge_iter)->isConnected(firstVertex)){	   
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

vector<shared_ptr<Node>> MeshFace::getNodes(){
   vector<shared_ptr<Node>> nodes;
   //add nodes for each vertex of face first, in order
   //This assumes mesh data structure is already ordered!!!
   auto edge_iter=getFirstEdge(); //Get first edge;
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
   for(edge_iter=getFirstEdge();edge_iter!=getLastEdge();++edge_iter){
      if((*edge_iter)->node != NULL){
         nodes.push_back((*edge_iter)->node);
      }
   }

   //add node in face (if present) last
   if(this->node != NULL){
      nodes.push_back(this->node);
   }

   return nodes;
}

vector<shared_ptr<MeshVertex>> MeshFace::getVertexes(){
   vector<shared_ptr<MeshVertex>> vertexes;

   //add nodes for each vertex of face first, in order
   //This assumes mesh data structure is already ordered!!!
   auto edge_iter=getFirstEdge(); //Get first edge;
   auto first_edge=(*edge_iter);
   edge_iter++; //Get second edge;
   if((*edge_iter)->getVertex(0)==first_edge->getVertex(0) || (*edge_iter)->getVertex(1)==first_edge->getVertex(0)){
      vertexes.push_back(first_edge->getVertex(1));
      vertexes.push_back(first_edge->getVertex(0));
   }else{ 
      vertexes.push_back(first_edge->getVertex(0));
      vertexes.push_back(first_edge->getVertex(1));
   }

   for(/*edge_iter=second edge*/;(*edge_iter)!=MeshEdges.back();++edge_iter){
      if((*edge_iter)->getVertex(0) == vertexes.back()){
         vertexes.push_back((*edge_iter)->getVertex(1));
      }else{
         vertexes.push_back((*edge_iter)->getVertex(0));
      }
   }

   return vertexes;
}
