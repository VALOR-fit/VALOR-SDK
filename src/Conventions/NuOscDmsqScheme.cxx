//________________________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include "TString.h"

#include "Conventions/NuOscDmsqScheme.h"
#include "Conventions/Opt.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(conv::NuOscDmsqScheme_t dmsqs)
{
  switch(dmsqs) {
  case(kNuDmsq12andAbs23) :
    return "dm^2 = Dm^2_{21}, Dm^2 = |Dm^2_{32}|";
    break;
  case(kNuDmsq12andAbs31) :
    return "dm^2 = Dm^2_{21}, Dm^2 = |Dm^2_{31}|";
    break;
  case(kNuDmsqFogliLisi ) :
    return "dm^2 = Dm^2_{21}, Dm^2 = |m^2_{3} - (m^2_{1} + m^2_{1})/2|";
    break;
  case(kNuDmsqAuto ) :
    if (Opt::Instance()->gMassHierarchyAssumed==kNmhInverted) {
      return Form("%s (auto)",util::AsString(kNuDmsq12andAbs31).c_str());
    }
    return Form("%s (auto)",util::AsString(kNuDmsq12andAbs23).c_str());
  default :
    return "Undefined";
    break;
  }
}
//________________________________________________________________________________________
string util::DmsqSchemeAsString(conv::NuOscDmsqScheme_t dmsqs) { return AsString(dmsqs); }
//________________________________________________________________________________________
NuOscDmsqScheme_t util::DmsqSchemeFromString(string h)
{
  if(h == "abs23"    ) return kNuDmsq12andAbs23;
  if(h == "abs31"    ) return kNuDmsq12andAbs31;
  if(h == "fogli"    ) return kNuDmsqFogliLisi;
  if(h == "auto"     ) return kNuDmsqAuto;

  return kNuDmsqUndefined;
}
//________________________________________________________________________________________
conv::NuOscDmsqScheme_t util::GetAutoDmsqSchemeFromHierarchy(conv::NuMassHierarchy_t hierarchy)
{
  if (hierarchy == kNmhUndefined) {hierarchy = Opt::Instance()->gMassHierarchyAssumed;}
  if (hierarchy == kNmhInverted ) {return kNuDmsq12andAbs31;}
  if (hierarchy == kNmhNormal   ) {return kNuDmsq12andAbs23;}
  return kNuDmsqUndefined;
}
//________________________________________________________________________________________
