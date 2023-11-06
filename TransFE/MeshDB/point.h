/***************************************************************************
 *   Copyright (C) 2005 by T. C. Raymond                                   *
 *   tc.raymond@ieee.org                                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
 
#ifndef POINT_H
#define POINT_H

///The point class is a container for 3D (and 2D) spatial coordinates.

class point{
public:

   ///Default contructor
   point(){
      //initialize to an oddball number in hopes of catching unitialized uses
      x=-999;
      y=-999;
      z=-999;
   };
   
   ///A contructor for 2D points, passing in x & y coordinates
   point(double in_x, double in_y){
      x=in_x;
      y=in_y;
      z=0;
   };
   
   ///A contructor for 3D points, passing in x, y & z coordinates
   point(double in_x, double in_y, double in_z){
      x=in_x;
      y=in_y;
      z=in_z;
   };

   ///Default destructor
   ~point(){};
   
   ///The x coordinate
   double x;
   ///The y coordinate
   double y;
   ///The z coordinate
   double z;
   
   ///Returns the i-th coordinate.  This is useful in loops.
   double X(int n){
      if(n==0){
         return x;
      } else if(n==1){
         return y;
      } else if(n==2){
         return z;
      } else {
         return -1;
      }
   };

};

#endif
