//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include "TString.h"

#include "valor/Conventions/NuOscDmsqScheme.h"
#include "valor/Conventions/Opt.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
std::string util::AsString(conv::NuOscDmsqScheme_t dmsqs)
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
      return std::string(TString::Format("%s (auto)",util::AsString(kNuDmsq12andAbs31).c_str()).Data());
    }
    return std::string(TString::Format("%s (auto)",util::AsString(kNuDmsq12andAbs23).c_str()).Data());
  default :
    return "Undefined";
    break;
  }
}
//________________________________________________________________________________________
std::string util::DmsqSchemeAsString(conv::NuOscDmsqScheme_t dmsqs) { return AsString(dmsqs); }
//________________________________________________________________________________________
std::string util::DmsqSchemeAsStringSimple(conv::NuOscDmsqScheme_t dmsqs)
{
  switch(dmsqs) {
    case(kNuDmsq12andAbs23) :
      return "abs23";
      break;
    case(kNuDmsq12andAbs31) :
      return "abs31";
      break;
    case(kNuDmsqFogliLisi ) :
      return "fogli";
      break;
    case(kNuDmsqAuto ) :
      return "auto";
    default :
      return "Undefined";
      break;
  }
}
//________________________________________________________________________________________
NuOscDmsqScheme_t util::DmsqSchemeFromString(std::string h)
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
