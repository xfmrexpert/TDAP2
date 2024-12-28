/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "meshregion.h"
#include <iostream>

void MeshRegion::addFace(MeshFace* new_face){
     MeshFaces.push_back(new_face);
}

std::vector<Node*> MeshRegion::getNodes() const{
   //Ugly kludge to get ordered list of nodes for linear tetrahedron
   //Any other element type will fail miserably
   //FIXME
   
    auto face_iter = this->MeshFaces.begin();
   
    auto nodes = (*face_iter)->getNodes();

   if((*face_iter) == 0){
      std::vector<Node*> tmp;
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
   
   auto nodes2 = (*face_iter)->getNodes();
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
   std::cerr << "Region contains two identical faces!" << std::endl;
   exit(1);
}


