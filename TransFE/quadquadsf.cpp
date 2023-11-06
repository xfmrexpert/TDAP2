/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "quadquadsf.h"

Vector<double> QuadQuadSF::N(point pt){
   Vector<double> mN(9);
   double xi = pt.x;
   double eta = pt.y;
   mN[0] = 1./4.*xi*eta*(xi+1.)*(eta+1.);  //corners
   mN[1] = 1./4.*xi*eta*(xi-1.)*(eta+1.);
   mN[2] = 1./4.*xi*eta*(xi-1.)*(eta-1.);
   mN[3] = 1./4.*xi*eta*(xi+1.)*(eta-1.);
   mN[4] = 1./2.*eta*(eta+1.)*(1.-xi*xi);  //edges
   mN[5] = 1./2.*xi*(xi-1.)*(1.-eta*eta);
   mN[6] = 1./2.*eta*(eta-1.)*(1.-xi*xi);
   mN[7] = 1./2.*xi*(xi+1.)*(1.-eta*eta);
   mN[8] = (1.-xi*xi)*(1.-eta*eta);  //face

   return mN;
};

Matrix<double> QuadQuadSF::dNds(point pt){
   Matrix<double> dN(9,2);
   double xi = pt.x;
   double eta = pt.y;
   dN(0, 0)=xi*eta*eta/2.+xi*eta/2.+eta*eta/4.+eta/4.;  //corners
   dN(0, 1)=xi*xi*eta/2.+xi*xi/4.+xi*eta/2.+xi/4.;
   dN(1, 0)=xi*eta*eta/2.+xi*eta/2.-eta*eta/4.-eta/4.;
   dN(1, 1)=xi*xi*eta/2.+xi*xi/4.-xi*eta/2.-xi/4.;
   dN(2, 0)=xi*eta*eta/2.-xi*eta/2.-eta*eta/4.+eta/4.;
   dN(2, 1)=xi*xi*eta/2.-xi*xi/4.-xi*eta/2.+xi/4.;
   dN(3, 0)=xi*eta*eta/2.-xi*eta/2.+eta*eta/4.-eta/4.;
   dN(3, 1)=xi*xi*eta/2.-xi*xi/4.+xi*eta/2.-xi/4.;
   dN(4, 0)=-eta*eta*xi-eta*xi; //edges
   dN(4, 1)=eta-eta*xi*xi+1./2.-xi*xi/2.;
   dN(5, 0)=xi-xi*eta*eta-1./2.+eta*eta/2.;
   dN(5, 1)=-xi*xi*eta+xi*eta;
   dN(6, 0)=-xi*eta*eta+xi*eta;
   dN(6, 1)=eta-xi*xi*eta-1./2.+xi*xi/2.;
   dN(7, 0)=xi-xi*eta*eta+1./2.-eta*eta/2.;
   dN(7, 1)=-xi*xi*eta-xi*eta;
   dN(8, 0)=-2.*xi+2.*xi*eta*eta;  //face
   dN(8, 1)=-2.*eta+2.*xi*xi*eta;
   
   return dN;
};

vector<point> QuadQuadSF::IntPts(){
   //Should use explicit numbers to avoid extra floating point ops
   //for now leave as is so things are readable
   vector<point> int_pts(numIntPts());
   double n = 1/sqrt(3.);
   int_pts[0].x = -n;
   int_pts[0].y = -n;
   int_pts[1].x = n;
   int_pts[1].y = -n;
   int_pts[2].x = n;
   int_pts[2].y = n;
   int_pts[3].x = -n;
   int_pts[3].y = n;
   
   return int_pts;
};

Vector<double> QuadQuadSF::Weights(){
   Vector<double> weights(numIntPts());
   weights[0] = 1.;
   weights[1] = 1.;
   weights[2] = 1.;
   weights[3] = 1.;
   
   return weights;
};

int QuadQuadSF::numIntPts(){
   return 4;
};
