//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  Define overloadings of the stream operator for
///  various containers.
///  (Should really be templated eventually)
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///   
///  \author Pat Scott  
///          (patscott@physics.mcgill.ca)
///  \date 2013 Jan, Dec
///
///  \author Ben Farmer  
///          (benjamin.farmer@monash.edu)
///  \date 2013 Jun
///
///  *********************************************

#include "gambit/Utils/stream_overloads.hpp"

namespace Gambit
{

  // Spacing utility for stream overloads below
  std::string spacing(int len, int maxlen)
  {
    int offset = 0;
    if (len < maxlen) {offset=maxlen-len;}
    return std::string(offset+5,' ');
  }   

  // Map: string-to-string
  std::ostream& operator<<(std::ostream& os, const std::map<std::string,std::string>& map) 
  {
    unsigned int maxlen = 0;
    std::map<std::string,std::string>::const_iterator it;

    for (it = map.begin(); it != map.end(); it++)
    {
      if ((*it).first.length() > maxlen) maxlen = (*it).first.length(); 
    }
    
    for (it = map.begin(); it != map.end(); it++)
    {
      if (it != map.begin()) {os << std::endl;}
      os << " " << (*it).first << spacing((*it).first.length(), maxlen) << "(" << (*it).second << ")";
    }

    return os;
  }


  // Map: string-to-int
  std::ostream& operator<<(std::ostream& os, const std::map<std::string,int>& map)
  {
    unsigned int maxlen = 0;
    std::map<std::string,int>::const_iterator it;

    for (it = map.begin(); it != map.end(); it++)
    {
      if ((*it).first.length() > maxlen) maxlen = (*it).first.length();
    }
    
    for (it = map.begin(); it != map.end(); it++)
    {
      if (it != map.begin()) {os << std::endl;}
      os << " " << (*it).first << spacing((*it).first.length(), maxlen) << "(" << (*it).second << ")";
    }

    return os;
  }

}
