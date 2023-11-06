/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "meshedge.h"
#include "meshvertex.h"
#include "meshface.h"

shared_ptr<MeshVertex> MeshEdge::getVertex(int n) {
     return MeshVertexes[n];
}

vector<shared_ptr<MeshFace>>::iterator MeshEdge::getFirstFace() {
     return MeshFaces.begin();
}

vector<shared_ptr<MeshFace>>::iterator MeshEdge::getLastFace() {
     return MeshFaces.end();
}

shared_ptr<MeshFace> MeshEdge::getFace(int n) {
     return MeshFaces[n];
}

void MeshEdge::addVertex(shared_ptr<MeshVertex> new_vertex, int n){
     MeshVertexes[n] = new_vertex;
}

void MeshEdge::addFace(shared_ptr<MeshFace> new_face){
     MeshFaces.push_back(new_face);
}

set<shared_ptr<MeshFace>> MeshEdge::getFaces(){
    set<shared_ptr<MeshFace>> return_set;
    for(auto use_iter=getFirstFace();use_iter!=getLastFace();++use_iter){
	    return_set.insert(*use_iter);
    }
    return return_set;
}

shared_ptr<MeshVertex> MeshEdge::otherVertex(shared_ptr<MeshVertex> vertex){
   if(vertex==MeshVertexes[0]){
      return MeshVertexes[1];
   }else{
      return MeshVertexes[0];
   }
}

bool MeshEdge::isConnected(shared_ptr<MeshEdge> edge){
   if(edge->getVertex(0)==MeshVertexes[0] || edge->getVertex(0)==MeshVertexes[1]
   || edge->getVertex(1)==MeshVertexes[0] || edge->getVertex(1)==MeshVertexes[1]){
      return true;
   }else{
      return false;
   }
}

bool MeshEdge::isConnected(shared_ptr<MeshVertex> vertex){
   if(MeshVertexes[0]==vertex || MeshVertexes[1]==vertex){
      return true;
   }else{
      return false;
   }
}

vector<shared_ptr<Node>> MeshEdge::getNodes(){
   vector<shared_ptr<Node>> nodes;
   
   for(int i=0;i<2;i++){
      nodes.push_back(this->getVertex(i)->node);
   }
   if(this->node != NULL){
      nodes.push_back(this->node);
   }
   return nodes;
}
