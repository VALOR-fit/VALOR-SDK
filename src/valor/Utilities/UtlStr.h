//____________________________________________________________________________
/*!

\class     valor::utils

\brief     Utilities for string manipulation

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_str_utils_h_
#define _valor_str_utils_h_

#include <ostream>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;

namespace valor {
namespace util {

  string         TrimSpaces                (string input);
  string         RemoveAllWhitespace       (string input);
  vector<string> Tokens                    (string input, string delimiter);
  string         ToLowerCase               (string data);
  string         ReplaceAll                (string data,string from, string to);
  string         PadToLength               (string data, unsigned int length, char pad_with = ' ');
  string         PadToLengthFromLeft       (string data, unsigned int length, char pad_with = ' ');
  string         GetStringBetweenDelimiters(const string &s, const string &start_delim, const string &stop_delim);
  
} // util
} // valor

// Operator to stream a std::vector [like,this]
// Maybe we should do make sure this never clashes with another template to do the same thing?
template<class T> std::ostream& operator<<(std::ostream& os, const std::vector<T> & v)
{
  for (unsigned int i = 0; i < v.size() ; i++) {
    os<<(i?",":"[")<<v[i];
  }
  return os<<"]";
}
// Similar for map
template<typename Key,typename Val> std::ostream& operator<<(std::ostream& os, const std::map<Key,Val> & m)
{
  unsigned int i = 0;
  for (typename std::map<Key,Val>::const_iterator it = m.begin() ; it != m.end() ; ++it) {
    os<<(i?",":"{")<<"\n  ("<<it->first<<": "<<it->second<<")";
    i++;
  }
  return os<<"\n}";
}

#endif


