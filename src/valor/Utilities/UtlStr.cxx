//____________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//____________________________________________________________________________

#include "valor/Utilities/UtlStr.h"

#include <algorithm>

//___________________________________________________________________________
string valor::util::TrimSpaces(string input)
{
  if( input.find_first_not_of(" \n") != 0)
    input.erase( 0,  input.find_first_not_of(" \n")  );

  if( input.find_last_not_of(" \n") != input.length() )
    input.erase( input.find_last_not_of(" \n")+1, input.length() );

  return input;
}
//____________________________________________________________________________
vector<string> valor::util::Tokens (string input, string delimiter)
{
// split a string of 'delimiter' separated values and return each string
// part as a vector<string> element.

  vector<string> tokens;

  while(input.find_first_of(delimiter) < input.length()) {

    tokens.push_back( input.substr(0, input.find_first_of(delimiter)) );
    input = input.substr(input.find_first_of(delimiter)+1, input.length());
  }
  tokens.push_back(input);
  return tokens;
}
//___________________________________________________________________________
string valor::util::ToLowerCase(string data)
{
  std::transform(data.begin(), data.end(), data.begin(), ::tolower);
  return data;
}
//___________________________________________________________________________
string valor::util::ReplaceAll(string data,string from, string to)
{
  size_t pos = 0;
  while ((pos = data.find(from, pos)) != std::string::npos) {
    data.replace(pos, from.length(), to);
    pos += to.length();
  }
  return data;
}
//___________________________________________________________________________
string valor::util::PadToLengthFromLeft(string data, unsigned int length, char pad_with)
{
  if(length > data.size()) {
    data.insert(0, length - data.size(), pad_with);
  }
  return data;
}
//___________________________________________________________________________
string valor::util::PadToLength(string data, unsigned int length, char pad_with)
{
  data.append(length-data.length(),pad_with);
  return data;
}
//___________________________________________________________________________
