/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#include "electrostaticanalysis.h"
#include "mapping2d.h"
#include "mapping1d2d.h"
#include "lintrisf.h"
#include "quadtrisf.h"
#include "linquadsf.h"
#include "quadquadsf.h"
#include "lin1dsf.h"
#include "quad1dsf.h"

//#include "precisiontimer.h"
#include <iostream>


#define PI 3.141592653589793238512808959406186204433

#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1 

using namespace std;

ElectroStaticAnalysis::ElectroStaticAnalysis()
{
	if(ndof!=0){
		//WARNING: Indicates previous calculation was not properly destroyed!
		ndof=0;
	}
}


ElectroStaticAnalysis::~ElectroStaticAnalysis()
{
}

StiffnessContributor* ElectroStaticAnalysis::makeStiffContrib(MeshFace* face){
	//Note: StiffnessContributor will own the ShapeFunction and Mapping objects created here
	int nen = face->getNodes().size();  //inefficient...
	//Get a ShapeFunction for the current MeshFace
	ShapeFunction* sf;
	if(face->numEdges()==3){  //triangle
		if(nen==3){ //1st order triangle
			sf = new LinTriSF(face);
		}else if(nen==6){ //2nd order triangle
			sf = new QuadTriSF(face);
		}else{
			cerr << "Unknown element type!" << endl;
		}
	}else if(face->numEdges()==4){ //quad
		if(nen==4){ //1st order quad
			sf = new LinQuadSF(face);
		}else if(nen==9){ //2nd order quad
			sf = new QuadQuadSF(face);
		}else{
			cerr << "Unknown element type!" << endl;
		}
	}else{ //dunno
		cerr << "Unknown element type!" << endl;
		exit(1);
	}
	//Get mapping
	Mapping* mapping = new Mapping2D(face, sf);
	
	return new ElectroStaticSC(face, mapping, sf);
};

ForceContributor* ElectroStaticAnalysis::makeForceContrib(MeshFace* face){
   if(face->getClassification()->getAttribute("Rho")!=NO_ATTRIB){
      int nen = face->getNodes().size();  //inefficient...
      ShapeFunction* sf;
      if(face->numEdges()==3){  //triangle
         if(nen==3){ //1st order triangle
            sf = new LinTriSF(face);
         }else if(nen==6){ //2nd order triangle
            sf = new QuadTriSF(face);
         }else{
            cerr << "Unknown element type!" << endl;
         }
      }else if(face->numEdges()==4){ //quad
         if(nen==4){ //1st order quad
            sf = new LinQuadSF(face);
         }else if(nen==9){ //2nd order quad
            sf = new QuadQuadSF(face);
         }else{
            cerr << "Unknown element type!" << endl;
         }
      }else{ //dunno
         cerr << "Unknown element type!" << endl;
         exit(1);
      }
	  Mapping* mapping = new Mapping2D(face, sf);

      return new ElectroStaticFC(face, mapping, sf);
   }
   return NULL;
};

ForceContributor* ElectroStaticAnalysis::makeForceContrib(MeshEdge* edge){
   return NULL;
};

Constraint* ElectroStaticAnalysis::makeConstraint(MeshEdge* edge){
   if(edge->getClassification()->getAttribute("x_constraint")!=NO_ATTRIB || edge->getClassification()->getAttribute("y_constraint")!=NO_ATTRIB){
      DisplacementConstraint* new_constraint = new DisplacementConstraint(edge);
      //cout << "Adding constraint for edge #" << edge->ID << endl;
      return new_constraint;
   }
   return NULL;
};

Constraint* ElectroStaticAnalysis::makeConstraint(MeshVertex* vertex){
   if(vertex->getClassification()->getAttribute("x_constraint")!=NO_ATTRIB || vertex->getClassification()->getAttribute("y_constraint")!=NO_ATTRIB){
      DisplacementConstraint* new_constraint = new DisplacementConstraint(vertex);
      //cout << "Adding constraint for edge #" << vertex->ID << endl;
      return new_constraint;
   }
   return NULL;
};

void ElectroStaticAnalysis::solve(){
   LinearSystemAssembler assembler;
   AlgebraicSystem AS(&DS, &assembler, &theMesh);
   AS.solve();

   //get vector of nodal displacements from AlgebraicSystem
   BigVector d = AS.get_d();
   //assign nodal displacements to DOF value
   vector<unique_ptr<Node>>::iterator node_iter;
   vector<DOF*> DOFs;
   vector<DOF*>::iterator DOF_iter;

   for(node_iter=theMesh.getFirstNode();node_iter!=theMesh.getLastNode();++node_iter){
      DOFs = (*node_iter)->getDOFs();
      for(DOF_iter=DOFs.begin(); DOF_iter!=DOFs.end(); ++DOF_iter){
         if((*DOF_iter)->get_status() == DOF_Free){
			(*DOF_iter)->set_value(d[(*DOF_iter)->get_eqnumber()]);
         }
      }
   }
};

void ElectroStaticAnalysis::recover(){
	MeshFace* face;
	vector<unique_ptr<MeshFace>>::iterator face_iter;
	vector<DOF*> DOFs;

	E = Matrix<double>(theMesh.numNodes(), 2);
	D = Matrix<double>(theMesh.numNodes(), 2);

	int nnd = 1;
	//calculate stress
	for (face_iter = theMesh.getFirstFace(); face_iter != theMesh.getLastFace(); face_iter++) {
		face = (*face_iter).get();
		vector<Node*> nodes = face->getNodes();
		int nen = nodes.size();

		ShapeFunction* sf;

		if (face->numEdges() == 3) { //triangle
			if (nen == 3) { //1st order triangle
				sf = new LinTriSF(face);
			}
			else if (nen == 6) { //2nd order triangle
				sf = new QuadTriSF(face);
			}
		}
		else if (face->numEdges() == 4) { //quad
			if (nen == 4) { //1st order quad
				sf = new LinQuadSF(face);
			}
			else if (nen == 9) { //2nd order quad
				sf = new QuadQuadSF(face);
			}
			else {
				cerr << "Unknown element type!" << endl;
			}
		}
		else { //dunno
			cerr << "Uknown element type!" << endl;
			exit(1);
		}

		Mapping* mapping = new Mapping2D(face, sf);

		Vector<double> E_n(2);
		Vector<double> V_n(nen);

		double eps_r = face->getClassification()->getAttribute("eps");

		for (int n = 0; n<nen; n++) {
			DOFs = nodes[n]->getDOFs();
			V_n[n] = DOFs[0]->get_value();
		}

		for (int i = 0; i<nen; i++) {
			point pt = nodes[i]->pt();
			//Unlike Na, dNads at x!=a is not 0! (ie. the derivate at a node is a function of the other nodal values).
			//Therefore, E at a node must be evaluated using all nodal values (and hence another loop)
			Matrix<double> dNds = sf->dNds(pt);
			Matrix<double> dsdx = mapping->jacobianInverse(pt);
			Matrix<double> dNdx = dNds * dsdx;

			E_n = V_n * dNdx;

			for (int j = 0; j<1; j++) {
				//E((nodes[i]->ID - 1), j) = E_n[j];
				//D((nodes[i]->ID - 1), j) = eps_r * E_n[j];
			}
		}
	}
};
   
void ElectroStaticAnalysis::saveOut(const char* filename){
   vector<unique_ptr<MeshFace>>::iterator face_iter;
   vector<DOF*> DOFs;

   //output to file for viewing in gmsh
   ofstream outFile;

   //output to file for viewing in gmsh
   char stressfile[256];
   strcpy(stressfile, filename);
   strcat(stressfile, "_E.pos");
   outFile.open(stressfile, ios::out);

   outFile << "View \"output\" {" << endl;
   //vector<MeshFace*>::iterator face_iter;
   for(face_iter=theMesh.getFirstFace();face_iter!=theMesh.getLastFace();++face_iter){
      vector<MeshVertex*>::iterator vertex_iter;
      vector<MeshVertex*> vertexes = (*face_iter)->getVertexes();
      if(vertexes.size()==3){
         outFile << "VT (";
      }else{
         outFile << "VQ (";
      }
      for(vertex_iter=vertexes.begin();vertex_iter!=vertexes.end();++vertex_iter){
         outFile << (*vertex_iter)->node->x() << "," << (*vertex_iter)->node->y() << "," << (*vertex_iter)->node->z();
         if((*vertex_iter)!=vertexes.back()){
            outFile << ",";
         }
      }
      outFile << "){";
      for(vertex_iter=vertexes.begin();vertex_iter!=vertexes.end();++vertex_iter){
         //outFile << E((*vertex_iter)->node->ID-1, 0) << "," << E((*vertex_iter)->node->ID - 1, 1) << "," << 0;
         if((*vertex_iter)!=vertexes.back()){
            outFile << ",";
         }
      }
      outFile << "};" << endl;
   }
   outFile << "};" << endl;
   outFile.close();  
   
   //output to file for viewing in gmsh
   char dispfile[256];
   strcpy(dispfile, filename);
   strcat(dispfile, "_V.pos");
   outFile.open(dispfile, ios::out);

   outFile << "View \"output\" {" << endl;
   //vector<Node*>::iterator node_iter;
   for(face_iter=theMesh.getFirstFace();face_iter!=theMesh.getLastFace();++face_iter){
      vector<MeshVertex*>::iterator vertex_iter;
      vector<MeshVertex*> vertexes = (*face_iter)->getVertexes();
      if(vertexes.size()==3){
         outFile << "ST (";
      }else{
         outFile << "SQ (";
      }
      for(vertex_iter=vertexes.begin();vertex_iter!=vertexes.end();++vertex_iter){
         outFile << (*vertex_iter)->node->x() << "," << (*vertex_iter)->node->y() << "," << (*vertex_iter)->node->z();
         if((*vertex_iter)!=vertexes.back()){
            outFile << ",";
         }
      }
      outFile << "){";
      for(vertex_iter=vertexes.begin();vertex_iter!=vertexes.end();++vertex_iter){
         DOFs = (*vertex_iter)->node->getDOFs();
         outFile << DOFs[0]->get_value();
		 //outFile << DOFs[0]->get_value();
         if((*vertex_iter)!=vertexes.back()){
            outFile << ",";
         }
      }
      outFile << "};" << endl;
   }
   outFile << "};" << endl;
   outFile.close();
};

