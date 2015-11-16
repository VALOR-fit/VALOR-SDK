//________________________________________________________________________________________
/*

Copyright (c) 2010-2015, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>

#include "Utilities/UtlSystem.h"
#include "Conventions/Opt.h"

using namespace valor;

using std::cout;
using std::cerr;
using std::endl;

//____________________________________________________________________________
void valor::util::Die(int status_code)
{
  if (Opt::Instance()->gDoNotDie) {
    cerr<<"Would have died with code "<<status_code<<"."<<endl;
    cerr<<"Choosing life."<<endl;
    return;
  }
  exit(status_code);
}
//____________________________________________________________________________
