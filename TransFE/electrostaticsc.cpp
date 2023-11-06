/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
 
#include "electrostaticsc.h"

#define PI 3.141592653589793238512808959406186204433

//Constructor
ElectroStaticSC::ElectroStaticSC(MeshFace* Element, Mapping* Map, ShapeFunction* SF) : StiffnessContributor(Element, Map, SF) {
	vector<Node*> nodes = Element->getNodes();
	vector<Node*>::iterator node_iter;
	vector<DOF*> DOFs;

	//check the region type of this face and set nnd (and dofs) accordingly
	nnd = 1; //Only have V scalar component

			 //create a new DOF for each node
	for (node_iter = nodes.begin(); node_iter != nodes.end(); ++node_iter) {

		DOFs = (*node_iter)->getDOFs();
		if (DOFs.size()<nnd) {
			for (int i = DOFs.size(); i<nnd; i++) {
				DOF* newDOF = (*node_iter)->newDOF();
				//newDOF->set_eqnumber((*node_iter)->ID*2+i);
			}
		}
	}
};


Matrix<double> ElectroStaticSC::evaluatePt(point pt){
   //nnd = # DOF/node
   //nen = # nodes/element   
	
   Matrix<double> k(nnd*nen, nnd*nen);
   Vector<double> N = SF->N(pt);

   // dsdx:
   // [drdx, drdy]
   // [dsdx, dsdy]
   // dNds:
   // [dN1dr, dN1ds]
   // [dN2dr, dN2ds]
   // [dN3dr, dN3ds]
   // dNds * dsdx
   // [dN1dr*drdx+dN1ds*dsdx, dN1dr*drdy+dN1ds*dsdy]
   // [dN2dr*drdx+dN2ds*dsdx, dN2dr*drdy+dN2ds*dsdy]
   // [dN2dr*drdx+dN2ds*dsdx, dN2dr*drdy+dN2ds*dsdy]
   // -equals-
   // 1    [dN1dr*dyds-dN1ds*dydr, -dN1dr*dxds+dN1ds*dxdr]
   //--- * [dN2dr*dyds-dN1ds*dydr, -dN2dr*dxds+dN1ds*dxdr]
   // J    [dN3dr*dyds-dN1ds*dydr, -dN3dr*dxds+dN1ds*dxdr]
   // -equals-
   // [dN1dx, dN1dy]
   // [dN2dx, dN2dy]
   // [dN3dx, dN3dy]
   
   //The following call is a bit wasteful...
   //Matrix dsdx = Map->jacobianInverse(pt);
   
   double eps_r = Element->getClassification()->getAttribute("eps");
   double eps = eps_r * 8.854187818e-12;
    
   // The matrix dNdx contains the derivatives of the shape function
   // with respect to the global coordinates (x, y, z)
   // The first index of dNdx refers to the shape function number, in
   // this case the node number.  The second index refers to the global
   // coordinate number (ie. x=0, y=1, z=2)
      
	for(int i=0; i<nen; i++){
		for(int j=0; j<nen; j++){
			k(i, j) = (1 / eps)*((dNdx(i, 0)*dNdx(j, 0) + dNdx(i, 1)*dNdx(j, 1)));// +(N[i] * dNdx(j, 0) + N[j] * dNdx(i, 0)) + (N[i] * N[j]));
		}
	}
   
   return k;
   
}

