//____________________________________________________________________________
/*!

\class     valor::utils

\brief     Utilities for string manipulation

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright (c) 2010-2016, The VALOR group.

*/
//____________________________________________________________________________

#ifndef _valor_str_utils_h_
#define _valor_str_utils_h_

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace valor {
namespace util {

  string         TrimSpaces         (string input);
  vector<string> Tokens             (string input, string delimiter);
  string         ToLowerCase        (string data);
  string         ReplaceAll         (string data,string from, string to);
  string         PadToLength        (string data, unsigned int length, char pad_with = ' ');
  string         PadToLengthFromLeft(string data, unsigned int length, char pad_with = ' ');
  
} // util
} // valor
  
#endif


