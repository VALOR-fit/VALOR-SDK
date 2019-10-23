//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include "valor/Conventions/ConfLevel.h"

using namespace valor;
using namespace valor::conv;

using std::string;

//________________________________________________________________________________________
string util::AsString(ConfLevel_t cl)
{
  switch (cl) {
    case kCL68: return "68pct";
    case kCL90: return "90pct"; 
    case kCL95: return "95pct"; 
    case kCL99: return "99pct";
    case kCL1Sigma: return "1sigma";
    case kCL2Sigma: return "2sigma";
    case kCL3Sigma: return "3sigma";
    case kCL4Sigma: return "4sigma";
    case kCL5Sigma: return "5sigma";
    case kCLUndefined: return "undefinedcl";
  }
  return "shouldneverhappen";
}
//________________________________________________________________________________________
std::string util::ConfLevelAsString(ConfLevel_t cl) {return AsString(cl);};
//________________________________________________________________________________________
