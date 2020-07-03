//________________________________________________________________________________________
/*

Copyright (c) 2010-2018, The VALOR Fitting Group, All Rights Reserved.

ChangeLog:

*/
//________________________________________________________________________________________

//fmwk
#include "valor/Utilities/UtlStr.h"
#include "valor/Conventions/SystType.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(SystType_t t, Option_t * opt)
{
  string name = "";

  switch(t) {
    case (kSystNorm    )        : name = "normalisation";     break;
    case (kSystLateral )        : name = "lateral";           break;
    case (kSystLateralPostOsc ) : name = "lateral_post_osc";  break;
    case (kSystLateralEBPostOsc ) : name = "lateral_eb_post_osc";  break;
    case (kSystResponse)        : name = "response";          break;
    case (kSystSpecial)         : name = "special";           break;
    case (kSystDisabled)        : name = "disabled";          break;
    default : name = "--";
  }

  if(opt) {
    string name2  = name;
    string optstr = opt;
    if(optstr.find("lowercase") != string::npos) {
       name2 = util::ToLowerCase(name);
    }
    return name2;
  }

  return name;
}
//________________________________________________________________________________________
string util::SystTypeAsString(SystType_t t, Option_t * opt) {return util::AsString(t,opt);}
//________________________________________________________________________________________
SystType_t util::SystTypeFromInt(int t)
{
// Cast an integer to SystType_t, but with boundary check
//              [first]                   [last]
  if(t >= 0 && t <= (int)conv::kNSystTypes) return (SystType_t)t;
  return kSystUndefined;
}
//________________________________________________________________________________________
SystType_t util::SystTypeFromString(string type)
{
  string lc_type = util::ToLowerCase(type);
  for (size_t i = 0 ; i <= conv::kNSystTypes ; i++ ) {
    SystType_t t = (SystType_t)i;
    string this_type = util::ToLowerCase(util::AsString(t));
    if(lc_type==this_type && this_type!="--") {
      return t;
    }
  }
    
  return kSystUndefined;
}
//________________________________________________________________________________________
