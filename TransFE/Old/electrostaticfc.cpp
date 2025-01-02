/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#include "electrostaticfc.h"

Vector<double> ElectroStaticFC::evaluatePt(point pt){
   Vector<double> currents(nen);
      
   //Matrix dXds = Map->dXds(pt);
   
   double J = Element->getClassification()->getAttribute("J"); //Constant current density in element

   Vector<double> N = SF->N(pt);
   double r = 0;
   vector<Node*> nodes = Element->getNodes();
   for(int i=0; i<nen; i++){
		r = r + nodes[i]->x() * N[i];
   }

   double s = r*r;
   
   //These are the z component source current densities in the region
   
   // N * J_s   
   //currents = N * r * r * (J/sqrt(r)) * Map->detJacobian(pt);
   
   double detJacobian = Map->dXds(pt).CalculateDeterminant();

	currents = N * r * J * detJacobian;
      
   return currents;
   
};



