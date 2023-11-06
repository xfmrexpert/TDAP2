/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#include "magaxistaticfc.h"

Vector<double> MagAxiStaticFC::evaluatePt(point pt){
   Vector<double> currents(nen);
      
   //Matrix dXds = Map->dXds(pt);
   
   double J = Element->getClassification()->getAttribute("J"); //Constant current density in element

   Vector<double> N = SF->N(pt);
   double r = 0;
   auto nodes = Element->getNodes();
   for(int i=0; i<nen; i++){
		r = r + nodes[i]->x() * N[i];
   }

   double s = r*r;
   
   //These are the z component source current densities in the region
   
   // N * J_s   
   //currents = N * r * r * (J/sqrt(r)) * Map->detJacobian(pt);
   
   double detJacobian = Map->dXds(pt).CalculateDeterminant();

   if (formulation == -1)
   {
	   currents = N * J * detJacobian;
   }
   else if(formulation==0){
		currents = N * r * J * detJacobian;
   }else if(formulation==1){
		currents = N * r * sqrt(r) * J * detJacobian;
   }else{
	    currents = N * (1/r) * J * detJacobian;
   }
      
   return currents;
   
};



