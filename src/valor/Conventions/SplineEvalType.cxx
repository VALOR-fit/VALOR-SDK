//________________________________________________________________________________________
/*

Copyright (c) 2010-2019, The VALOR Fitting Group, All Rights Reserved.

ChangeLog:

*/
//________________________________________________________________________________________

//fmwk
#include "valor/Utilities/UtlStr.h"
#include "valor/Conventions/SplineEvalType.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(SplEvalType_t t, Option_t * opt)
{
  string name = "";

  switch(t) {
    case (kSplEvalCurrVal   ) : name = "curr_val";   break;
    case (kSplEvalCurrTweak ) : name = "curr_tweak"; break;
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
string util::SplEvalTypeAsString(SplEvalType_t t, Option_t * opt) {return util::AsString(t,opt);}
//________________________________________________________________________________________
SplEvalType_t util::SplEvalTypeFromString(string type)
{
  string lc_type = util::ToLowerCase(type);
  for (size_t i = 0 ; i <= conv::kNSplineEvalTypes ; i++ ) {
    SplEvalType_t t = (SplEvalType_t)i;
    string this_type = util::ToLowerCase(util::AsString(t));
    if(lc_type==this_type && this_type!="--") {
      return t;
    }
  }
    
  return kSplEvalUndefined;
}
//________________________________________________________________________________________
