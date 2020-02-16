//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <algorithm>

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
string util::AsStringRooTex(ConfLevel_t cl)
{
  switch (cl) {
    case kCL68: return "68%";
    case kCL90: return "90%";
    case kCL95: return "95%";
    case kCL99: return "99%";
    case kCL1Sigma: return "1#sigma";
    case kCL2Sigma: return "2#sigma";
    case kCL3Sigma: return "3#sigma";
    case kCL4Sigma: return "4#sigma";
    case kCL5Sigma: return "5#sigma";
    case kCLUndefined: return "undefinedcl";
  }
  return "shouldneverhappen";
}
//________________________________________________________________________________________
std::string util::ConfLevelAsString(ConfLevel_t cl) {return AsString(cl);};
//________________________________________________________________________________________
std::string util::ConfLevelAsStringRooTex(ConfLevel_t cl) {return AsStringRooTex(cl);};
//________________________________________________________________________________________
ConfLevel_t util::ConfLevelFromString(string confname) {
  std::transform(confname.begin(), confname.end(), confname.begin(), ::tolower);
  for (int i = 0 ; i < kNConfLevels ; i++ ) {
    ConfLevel_t cl = (ConfLevel_t)i;
    string confname2 = util::ConfLevelAsString(cl);
    std::transform(confname2.begin(), confname2.end(), confname2.begin(), ::tolower);
    if(confname==confname2 && confname2!="undefinedcl") {
      return cl;
    }
  }
  return kCLUndefined;
}
//________________________________________________________________________________________
