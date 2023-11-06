/************************************************************************
  			meshvertex.cpp - Copyright T. C .Raymond

**************************************************************************/

#include "meshvertex.h"
#include "meshedge.h"

void MeshVertex::addEdge(shared_ptr<MeshEdge> new_edge){
	MeshEdges.push_back(new_edge); //should be ordered CCW
}

vector<shared_ptr<MeshEdge>>::iterator MeshVertex::getFirstEdge() {
     return MeshEdges.begin();
}

vector<shared_ptr<MeshEdge>>::iterator MeshVertex::getLastEdge() {
     return MeshEdges.end();
}

shared_ptr<MeshEdge> MeshVertex::getEdge(int n){
     return MeshEdges[n];
}

size_t MeshVertex::numEdges(){
     return MeshEdges.size();
}

set<shared_ptr<MeshEdge>> MeshVertex::getEdges(){
     set<shared_ptr<MeshEdge>> return_set;
     for(auto use_iter=getFirstEdge();use_iter!=getLastEdge();++use_iter){
	     return_set.insert(*use_iter);
     }
     return return_set;
}

vector<shared_ptr<Node>> MeshVertex::getNodes(){
   vector<shared_ptr<Node>> nodes;
   nodes.push_back(this->node);
   return nodes;
}

