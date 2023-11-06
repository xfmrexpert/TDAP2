/************************************************************************
  			meshregion.cpp - Copyright T. C. Raymond

**************************************************************************/

#include "meshregion.h"
#include <iostream>

vector<pair<shared_ptr<MeshFace>, bool>>::iterator MeshRegion::getFirstFace() {
     return MeshFaces.begin();
}

vector<pair<shared_ptr<MeshFace>, bool>>::iterator MeshRegion::getLastFace() {
     return MeshFaces.end();
}

void MeshRegion::addFace(pair<shared_ptr<MeshFace>, bool> new_face){
     MeshFaces.push_back(new_face);
}

vector<shared_ptr<Node>> MeshRegion::getNodes(){
   //Ugly kludge to get ordered list of nodes for linear tetrahedron
   //Any other element type will fail miserably
   //FIXME
   
    auto face_iter = this->getFirstFace();
   
   auto nodes = (*face_iter).first->getNodes();

   if((*face_iter).second == 0){
      vector<shared_ptr<Node>> tmp;
      for(auto node_riter=nodes.rbegin();node_riter!=nodes.rend();++node_riter){
         tmp.push_back((*node_riter));
      }
      nodes = tmp;
   }

   //cout << "nodes.size()=" << nodes.size() << endl;

   //cout << "Nodes in first face" << endl;
   //int i = 0;
   //for(node_iter=nodes.begin();node_iter!=nodes.end();node_iter++){
   //   cout << "Node #" << i << ": x=" << (*node_iter)->pt().X(0) << " y=" << (*node_iter)->pt().X(1) << " z=" << (*node_iter)->pt().X(2) << endl;
   //   i++;
   //}
   
   face_iter++;
   
   auto nodes2 = (*face_iter).first->getNodes();
   //cout << "nodes2.size()=" << nodes2.size() << endl;

   //cout << "Nodes in second face" << endl;
   //for(int i=0;i<3;i++){
   //   cout << "Node #" << i << ": x=" << nodes2[i]->pt().X(0) << " y=" << nodes2[i]->pt().X(1) << " z=" << nodes2[i]->pt().X(2) << endl;
   //}

   bool node_found = false;

   for(auto node_iter2=nodes2.begin();node_iter2!=nodes2.end();++node_iter2){ //loop through nodes in second face
      node_found = false;
      for(auto node_iter=nodes.begin();node_iter!=nodes.end();++node_iter){
         if((*node_iter)==(*node_iter2)){ //check to see if the node in the 2nd face is in the 1st face
            node_found = true;
         }
      }
      if(node_found==false){
         nodes.push_back((*node_iter2));
         //cout << "Nodes in region" << endl;
         //for(int i=0;i<4;i++){
         //   cout << "Node #" << i << ": x=" << nodes[i]->pt().X(0) << " y=" << nodes[i]->pt().X(1) << " z=" << nodes[i]->pt().X(2) << endl;
         //}
         return nodes;
      }
   }
   cerr << "Region contains two identical faces!" << endl;
   exit(1);
}


