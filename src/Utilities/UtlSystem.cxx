//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>
#include <cstdlib>

#include "TRint.h"

#include "Utilities/UtlSystem.h"
#include "Conventions/Opt.h"

using namespace valor;

using std::cout;
using std::cerr;
using std::endl;

#define USE_TERMINAL_COLOR

//____________________________________________________________________________
void valor::util::Die(int status_code)
{
  if (Opt::Instance()->gDoNotDie) {
#ifdef USE_TERMINAL_COLOR
    cerr<<"\033[1;31m";
#endif
    cerr<<"util/Die() >> Would have died with code "<<status_code<<". "
        <<"Choosing life."<<endl;
#ifdef USE_TERMINAL_COLOR
    cerr<<"\033[0m";
#endif
    return;
  }
  exit(status_code);
}
//____________________________________________________________________________
void valor::util::ChangeInterpreterPrompt(std::string new_prompt)
{
    TRint* rint = dynamic_cast<TRint*>(gApplication);
    if (!rint) return;

    rint->SetPrompt(new_prompt.c_str());
}
//____________________________________________________________________________
