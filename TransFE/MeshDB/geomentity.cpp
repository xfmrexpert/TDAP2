/***************************************************************************
 *   Copyright (C) 2005-2024 by T. C. Raymond                              *
 *   tcraymond@inductivereasoning.com                                      *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/

#include "geomentity.h"

GeomEntity::GeomEntity() : attributes()
{
   attributes.insert(std::pair<std::string, double>("crap", 0));
}


GeomEntity::~GeomEntity()
{
}

double GeomEntity::getAttribute(std::string key){
   if(!attributes.empty()){
      if(attributes.find(key)!=attributes.end()){
         return attributes.find(key)->second; //found the attribute in the list
      }else{
         return NO_ATTRIB;
      }
   }else{
      return NO_ATTRIB;
   }
}
    
void GeomEntity::addAttribute(std::string key, double value){
   attributes.insert(std::pair<std::string, double>(key, value));
}
    
void GeomEntity::clearAttributes(){
   attributes.clear();
}
