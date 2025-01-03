/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include <map>
#include <string>

//Attributes are currently restricted to numbers of type double
//Often times, as in the case of contraints, a value of 0 is meaningful
//When we want to indicate that the attribute does not exist ("a null attribute")
//we need to return a number that we can check for the hopefully is not used in real cases
#define NO_ATTRIB -999

class GeomEntity{
public:
   ///Default constructor
    GeomEntity();

   ///Default destructor
    ~GeomEntity();
   
   ///Retrieve an attribute
    double getAttribute(std::string key);
    
   ///Add an attribute designated by a string "key"
    void addAttribute(std::string key, double value);
    
   ///Clear all attributes associated with the instance
    void clearAttributes();

    int ID = 0;

protected:
   ///This is a multimap to hold all of the attributes (key/value pairs)
   ///The multimap allows for quick (and brainless) retrieval of a value given
   ///a key string.  This could (and probably should) be a map.
   std::multimap<std::string, double> attributes;

};

