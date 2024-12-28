/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "quadtrisf.h"

Vector<double> QuadTriSF::N(const point& pt){
   Vector<double> N(6);
   double r = pt.x;
   double s = pt.y;
   double t = 1.-r-s;
   N[0]=r*(2.*r-1.);
   N[1]=s*(2.*s-1.);
   N[2]=t*(2.*t-1.); //=(1-r-s)*(2*(1-r-s)-1)=
   N[3]=4.*r*s;
   N[4]=4.*s*t; //=4s-4rs-4s^2
   N[5]=4.*r*t; //=4r-4rs-4r^2
   return N;
};

Matrix<double> QuadTriSF::dNds(const point& pt){
   Matrix<double> dN(6,2);
   double r = pt.x;
   double s = pt.y;
   dN(0, 0)=4.*r-1.;
   dN(0, 1)=0.;
   dN(1, 0)=0.;
   dN(1, 1)=4.*s-1.;
   dN(2, 0)=-3.+4.*s+4.*r;
   dN(2, 1)=-3.+4.*r+4.*s;
   dN(3, 0)=4.*s;
   dN(3, 1)=4.*r;
   dN(4, 0)=-4.*s;
   dN(4, 1)=4.-4.*r-8.*s;
   dN(5, 0)=4.-4.*s-8.*r;
   dN(5, 1)=-4.*r;
   
   return dN;
};

const std::vector<point>& QuadTriSF::IntPts() const {
    static const std::vector<point> int_pts = []() {
        std::vector<point> points(3);
        points[0] = point(2.0 / 3.0, 1.0 / 6.0);
        points[1] = point(1.0 / 6.0, 1.0 / 6.0);
        points[2] = point(1.0 / 6.0, 2.0 / 3.0);
           
        return points;
    }();
    return int_pts;
};

const Vector<double>& QuadTriSF::Weights() const {
    static const Vector<double> weights = []() {
        Vector<double> w(3);
        w[0] = 1.0 / 6.0;
        w[1] = 1.0 / 6.0;
        w[2] = 1.0 / 6.0;
        return w;
        }();
    return weights;
}

int QuadTriSF::numIntPts() const{
   return 3;
};
