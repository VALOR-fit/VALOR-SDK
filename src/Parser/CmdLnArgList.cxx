//________________________________________________________________________________________
/*

 Copyright (c) 2010-2016, The VALOR group. All rights reserved.

*/
//________________________________________________________________________________________

#include <fstream>

#include "Parser/CmdLnArgList.h"
#include "Utilities/UtlStr.h"

using std::ifstream;
using namespace valor;

//________________________________________________________________________________________
CmdLnArgList::CmdLnArgList()
{

}
//________________________________________________________________________________________
CmdLnArgList::~CmdLnArgList()
{

}
//________________________________________________________________________________________
bool CmdLnArgList::Configure (string files)
{
  vector<string> filename_list = util::Tokens(files, ":");
  vector<string>::const_iterator it = filename_list.begin();
  for( ; it != filename_list.end(); ++it) {
     string filename = *it;
     ifstream file;
     file.open(filename.c_str());
     if (! file.is_open() ) {
        continue;
     }
     while (!file.eof()) {

     }
     file.close();
  }

  return true;
}
//________________________________________________________________________________________
bool CmdLnArgList::Parse(int argc, char ** argv)
{

  return true;
}
//________________________________________________________________________________________


