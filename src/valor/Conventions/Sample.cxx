//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//________________________________________________________________________________________

#include <algorithm>

// fmwk
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
// dune
#include "valor/Conventions/Sample.h"

using namespace valor;
using namespace valor::conv;

using std::string;

//________________________________________________________________________________________
string util::AsString(Sample_t s)
{
  switch(s) {
  case ksNearNuMuCC    : return "ND,NuMuCC,inclusive"; break;
  case ksNearNuMuCCOAA1: return "ND,NuMuCC,inclusive,OAA1"; break;
  case ksNearNuMuCCOAA2: return "ND,NuMuCC,inclusive,OAA2"; break;
  case ksNearNuMuCCOAA3: return "ND,NuMuCC,inclusive,OAA3"; break;
  case ksNearNuMuCCOAA4: return "ND,NuMuCC,inclusive,OAA4"; break;
  case ksNearNuMuCCOAA5: return "ND,NuMuCC,inclusive,OAA5"; break;
  case ksNearNuMuCCOAA6: return "ND,NuMuCC,inclusive,OAA6"; break;
  case ksNearNuMuCCOAA7: return "ND,NuMuCC,inclusive,OAA7"; break;
  case ksNearNuMuCCOAA8: return "ND,NuMuCC,inclusive,OAA8"; break;
  case ksNearNuMuCCOAA9: return "ND,NuMuCC,inclusive,OAA9"; break;
  case ksNearNuMuCC_0p0pi    : return "ND,NuMuCC,0p0pi"; break;
  case ksNearNuMuCC_0p0piOAA1: return "ND,NuMuCC,0p0pi,OAA1"; break;
  case ksNearNuMuCC_0p0piOAA2: return "ND,NuMuCC,0p0pi,OAA2"; break;
  case ksNearNuMuCC_0p0piOAA3: return "ND,NuMuCC,0p0pi,OAA3"; break;
  case ksNearNuMuCC_0p0piOAA4: return "ND,NuMuCC,0p0pi,OAA4"; break;
  case ksNearNuMuCC_0p0piOAA5: return "ND,NuMuCC,0p0pi,OAA5"; break;
  case ksNearNuMuCC_0p0piOAA6: return "ND,NuMuCC,0p0pi,OAA6"; break;
  case ksNearNuMuCC_0p0piOAA7: return "ND,NuMuCC,0p0pi,OAA7"; break;
  case ksNearNuMuCC_0p0piOAA8: return "ND,NuMuCC,0p0pi,OAA8"; break;
  case ksNearNuMuCC_0p0piOAA9: return "ND,NuMuCC,0p0pi,OAA9"; break;
  case ksNearNuMuCC_1p0pi    : return "ND,NuMuCC,1p0pi"; break;
  case ksNearNuMuCC_1p0piOAA1: return "ND,NuMuCC,1p0pi,OAA1"; break;
  case ksNearNuMuCC_1p0piOAA2: return "ND,NuMuCC,1p0pi,OAA2"; break;
  case ksNearNuMuCC_1p0piOAA3: return "ND,NuMuCC,1p0pi,OAA3"; break;
  case ksNearNuMuCC_1p0piOAA4: return "ND,NuMuCC,1p0pi,OAA4"; break;
  case ksNearNuMuCC_1p0piOAA5: return "ND,NuMuCC,1p0pi,OAA5"; break;
  case ksNearNuMuCC_1p0piOAA6: return "ND,NuMuCC,1p0pi,OAA6"; break;
  case ksNearNuMuCC_1p0piOAA7: return "ND,NuMuCC,1p0pi,OAA7"; break;
  case ksNearNuMuCC_1p0piOAA8: return "ND,NuMuCC,1p0pi,OAA8"; break;
  case ksNearNuMuCC_1p0piOAA9: return "ND,NuMuCC,1p0pi,OAA9"; break;
  case ksNearNuMuCC_Np0pi    : return "ND,NuMuCC,Np0pi"; break;
  case ksNearNuMuCC_Np0piOAA1: return "ND,NuMuCC,Np0pi,OAA1"; break;
  case ksNearNuMuCC_Np0piOAA2: return "ND,NuMuCC,Np0pi,OAA2"; break;
  case ksNearNuMuCC_Np0piOAA3: return "ND,NuMuCC,Np0pi,OAA3"; break;
  case ksNearNuMuCC_Np0piOAA4: return "ND,NuMuCC,Np0pi,OAA4"; break;
  case ksNearNuMuCC_Np0piOAA5: return "ND,NuMuCC,Np0pi,OAA5"; break;
  case ksNearNuMuCC_Np0piOAA6: return "ND,NuMuCC,Np0pi,OAA6"; break;
  case ksNearNuMuCC_Np0piOAA7: return "ND,NuMuCC,Np0pi,OAA7"; break;
  case ksNearNuMuCC_Np0piOAA8: return "ND,NuMuCC,Np0pi,OAA8"; break;
  case ksNearNuMuCC_Np0piOAA9: return "ND,NuMuCC,Np0pi,OAA9"; break;
  case ksNearNuMuCC_1pi    : return "ND,NuMuCC,1pi"; break;
  case ksNearNuMuCC_1piOAA1: return "ND,NuMuCC,1pi,OAA1"; break;
  case ksNearNuMuCC_1piOAA2: return "ND,NuMuCC,1pi,OAA2"; break;
  case ksNearNuMuCC_1piOAA3: return "ND,NuMuCC,1pi,OAA3"; break;
  case ksNearNuMuCC_1piOAA4: return "ND,NuMuCC,1pi,OAA4"; break;
  case ksNearNuMuCC_1piOAA5: return "ND,NuMuCC,1pi,OAA5"; break;
  case ksNearNuMuCC_1piOAA6: return "ND,NuMuCC,1pi,OAA6"; break;
  case ksNearNuMuCC_1piOAA7: return "ND,NuMuCC,1pi,OAA7"; break;
  case ksNearNuMuCC_1piOAA8: return "ND,NuMuCC,1pi,OAA8"; break;
  case ksNearNuMuCC_1piOAA9: return "ND,NuMuCC,1pi,OAA9"; break;
  case ksNearNuMuCC_Other    : return "ND,NuMuCC,Other"; break;
  case ksNearNuMuCC_OtherOAA1: return "ND,NuMuCC,Other,OAA1"; break;
  case ksNearNuMuCC_OtherOAA2: return "ND,NuMuCC,Other,OAA2"; break;
  case ksNearNuMuCC_OtherOAA3: return "ND,NuMuCC,Other,OAA3"; break;
  case ksNearNuMuCC_OtherOAA4: return "ND,NuMuCC,Other,OAA4"; break;
  case ksNearNuMuCC_OtherOAA5: return "ND,NuMuCC,Other,OAA5"; break;
  case ksNearNuMuCC_OtherOAA6: return "ND,NuMuCC,Other,OAA6"; break;
  case ksNearNuMuCC_OtherOAA7: return "ND,NuMuCC,Other,OAA7"; break;
  case ksNearNuMuCC_OtherOAA8: return "ND,NuMuCC,Other,OAA8"; break;
  case ksNearNuMuCC_OtherOAA9: return "ND,NuMuCC,Other,OAA9"; break;
  case ksFarNuMuCC    : return "FD,NuMuCC,Inclusive"; break;
  case ksFarNuMuCC_0pi: return "FD,NuMuCC,0pi"; break;
  case ksFarNuMuCC_1pi: return "FD,NuMuCC,1pi"; break;
  case ksFarNuMuCC_Npi: return "FD,NuMuCC,Npi"; break;
  default                  : return "--";
  }
  return "";
}
//________________________________________________________________________________________
string util::AsStringTeX(Sample_t s, bool root)
{
  string name = "";
  
  switch(s) {
  case ksNearNuMuCC    : name = "#nu_{#mu} CC, inclusive, SBND"; break;
  case ksNearNuMuCCOAA1: name = "#nu_{#mu} CC, inclusive, 0^{th} OAA bin, SBND"; break;
  case ksNearNuMuCCOAA2: name = "#nu_{#mu} CC, inclusive, 1^{st} OAA bin, SBND"; break;
  case ksNearNuMuCCOAA3: name = "#nu_{#mu} CC, inclusive, 2^{nd} OAA bin, SBND"; break;
  case ksNearNuMuCCOAA4: name = "#nu_{#mu} CC, inclusive, 3^{rd} OAA bin, SBND"; break;
  case ksNearNuMuCCOAA5: name = "#nu_{#mu} CC, inclusive, 4^{th} OAA bin, SBND"; break;
  case ksNearNuMuCCOAA6: name = "#nu_{#mu} CC, inclusive, 5^{th} OAA bin, SBND"; break;
  case ksNearNuMuCCOAA7: name = "#nu_{#mu} CC, inclusive, 6^{th} OAA bin, SBND"; break;
  case ksNearNuMuCCOAA8: name = "#nu_{#mu} CC, inclusive, 7^{th} OAA bin, SBND"; break;
  case ksNearNuMuCCOAA9: name = "#nu_{#mu} CC, inclusive, 8^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_0p0pi    : name = "#nu_{#mu} CC, 0p 0#pi, SBND"; break;
  case ksNearNuMuCC_0p0piOAA1: name = "#nu_{#mu} CC, 0p 0#pi, 0^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_0p0piOAA2: name = "#nu_{#mu} CC, 0p 0#pi, 1^{st} OAA bin, SBND"; break;
  case ksNearNuMuCC_0p0piOAA3: name = "#nu_{#mu} CC, 0p 0#pi, 2^{nd} OAA bin, SBND"; break;
  case ksNearNuMuCC_0p0piOAA4: name = "#nu_{#mu} CC, 0p 0#pi, 3^{rd} OAA bin, SBND"; break;
  case ksNearNuMuCC_0p0piOAA5: name = "#nu_{#mu} CC, 0p 0#pi, 4^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_0p0piOAA6: name = "#nu_{#mu} CC, 0p 0#pi, 5^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_0p0piOAA7: name = "#nu_{#mu} CC, 0p 0#pi, 6^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_0p0piOAA8: name = "#nu_{#mu} CC, 0p 0#pi, 7^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_0p0piOAA9: name = "#nu_{#mu} CC, 0p 0#pi, 8^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1p0pi    : name = "#nu_{#mu} CC, 1p 0#pi, SBND"; break;
  case ksNearNuMuCC_1p0piOAA1: name = "#nu_{#mu} CC, 1p 0#pi, 0^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1p0piOAA2: name = "#nu_{#mu} CC, 1p 0#pi, 1^{st} OAA bin, SBND"; break;
  case ksNearNuMuCC_1p0piOAA3: name = "#nu_{#mu} CC, 1p 0#pi, 2^{nd} OAA bin, SBND"; break;
  case ksNearNuMuCC_1p0piOAA4: name = "#nu_{#mu} CC, 1p 0#pi, 3^{rd} OAA bin, SBND"; break;
  case ksNearNuMuCC_1p0piOAA5: name = "#nu_{#mu} CC, 1p 0#pi, 4^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1p0piOAA6: name = "#nu_{#mu} CC, 1p 0#pi, 5^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1p0piOAA7: name = "#nu_{#mu} CC, 1p 0#pi, 6^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1p0piOAA8: name = "#nu_{#mu} CC, 1p 0#pi, 7^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1p0piOAA9: name = "#nu_{#mu} CC, 1p 0#pi, 8^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_Np0pi    : name = "#nu_{#mu} CC, Np 0#pi, SBND"; break;
  case ksNearNuMuCC_Np0piOAA1: name = "#nu_{#mu} CC, Np 0#pi, 0^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_Np0piOAA2: name = "#nu_{#mu} CC, Np 0#pi, 1^{st} OAA bin, SBND"; break;
  case ksNearNuMuCC_Np0piOAA3: name = "#nu_{#mu} CC, Np 0#pi, 2^{nd} OAA bin, SBND"; break;
  case ksNearNuMuCC_Np0piOAA4: name = "#nu_{#mu} CC, Np 0#pi, 3^{rd} OAA bin, SBND"; break;
  case ksNearNuMuCC_Np0piOAA5: name = "#nu_{#mu} CC, Np 0#pi, 4^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_Np0piOAA6: name = "#nu_{#mu} CC, Np 0#pi, 5^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_Np0piOAA7: name = "#nu_{#mu} CC, Np 0#pi, 6^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_Np0piOAA8: name = "#nu_{#mu} CC, Np 0#pi, 7^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_Np0piOAA9: name = "#nu_{#mu} CC, Np 0#pi, 8^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1pi    : name = "#nu_{#mu} CC, 1#pi, SBND"; break;
  case ksNearNuMuCC_1piOAA1: name = "#nu_{#mu} CC, 1#pi, 0^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1piOAA2: name = "#nu_{#mu} CC, 1#pi, 1^{st} OAA bin, SBND"; break;
  case ksNearNuMuCC_1piOAA3: name = "#nu_{#mu} CC, 1#pi, 2^{nd} OAA bin, SBND"; break;
  case ksNearNuMuCC_1piOAA4: name = "#nu_{#mu} CC, 1#pi, 3^{rd} OAA bin, SBND"; break;
  case ksNearNuMuCC_1piOAA5: name = "#nu_{#mu} CC, 1#pi, 4^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1piOAA6: name = "#nu_{#mu} CC, 1#pi, 5^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1piOAA7: name = "#nu_{#mu} CC, 1#pi, 6^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1piOAA8: name = "#nu_{#mu} CC, 1#pi, 7^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_1piOAA9: name = "#nu_{#mu} CC, 1#pi, 8^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_Other    : name = "#nu_{#mu} CC, Other, SBND"; break;
  case ksNearNuMuCC_OtherOAA1: name = "#nu_{#mu} CC, Other, 0^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_OtherOAA2: name = "#nu_{#mu} CC, Other, 1^{st} OAA bin, SBND"; break;
  case ksNearNuMuCC_OtherOAA3: name = "#nu_{#mu} CC, Other, 2^{nd} OAA bin, SBND"; break;
  case ksNearNuMuCC_OtherOAA4: name = "#nu_{#mu} CC, Other, 3^{rd} OAA bin, SBND"; break;
  case ksNearNuMuCC_OtherOAA5: name = "#nu_{#mu} CC, Other, 4^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_OtherOAA6: name = "#nu_{#mu} CC, Other, 5^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_OtherOAA7: name = "#nu_{#mu} CC, Other, 6^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_OtherOAA8: name = "#nu_{#mu} CC, Other, 7^{th} OAA bin, SBND"; break;
  case ksNearNuMuCC_OtherOAA9: name = "#nu_{#mu} CC, Other, 8^{th} OAA bin, SBND"; break;
  case ksFarNuMuCC: name = "#nu_{#mu} CC, inclusive, ICARUS"; break;
  case ksFarNuMuCC_0pi: name = "#nu_{#mu} CC, 0#pi, ICARUS"; break;
  case ksFarNuMuCC_1pi: name = "#nu_{#mu} CC, 1#pi, ICARUS"; break;
  case ksFarNuMuCC_Npi: name = "#nu_{#mu} CC, N#pi, ICARUS"; break;
  default                  : name = "--";
  }

  if(root) return name;

  // normal TeX, not ROOT TeX - change '#' with '\'
  std::replace( name.begin(), name.end(), '#', '\\');

  return name;
}
//________________________________________________________________________________________
bool util::IsFar(Sample_t s)
{
  switch(s) {
  case (ksFarNuMuCC    ) :
  case (ksFarNuMuCC_0pi) :
  case (ksFarNuMuCC_1pi) :
  case (ksFarNuMuCC_Npi) :  return true; break;
  default: return false;
  }
  return false;
}

//________________________________________________________________________________________
bool util::IsCC(Sample_t s)
{
  switch(s) {
  case (ksUndefined      ) : return false;
  default : return true;
  }
  return true;
}
//______________________________________________________________________________________________
// IS CCQE Like, simply to be used to define binning, at the moment different for CC0pi1trk, etc
// elastic scattering and IDM samples included here at the moment
bool util::IsCCQELike(Sample_t s)
{
  switch(s) {
    return false; break;
    default: return false;
  }
  return false;
}
//________________________________________________________________________________________
bool util::IsNC(Sample_t s)
{
  switch(s) {
  case (ksUndefined      ) : return false;
  default: return false;
  }
  return false;
}
//________________________________________________________________________________________
Sample_t util::SampleFromInt(int s)
{
// Cast an integer to Sample_t, but with boundary check
//             [first]                  [last]
  if((s >= (int)ksNearNuMuCC && s <= (int) ksFarNuMuCC_Npi))
    return (Sample_t)s;
  return ksUndefined;
}
//________________________________________________________________________________________
Sample_t util::SampleFromString(string sample)
{
  string lc_sample = util::ToLowerCase(sample);
  for (int i = 0 ; i < kMaxNSamples ; i++ ) {
    Sample_t s = (Sample_t)i;
    string this_samp = util::ToLowerCase(util::AsString(s));
    if(lc_sample==this_samp && this_samp!="--") {
      return s;
    }
  }

  return ksUndefined;
}
//________________________________________________________________________________________
// aliases
string valor::util::SampleAsString   (conv::Sample_t s)            { return util::AsString(s);}
string valor::util::SampleAsStringTeX(conv::Sample_t s, bool root) { return util::AsStringTeX(s,root);}
bool   valor::util::SampleIsCC       (conv::Sample_t s)            { return util::IsCC      (s);};
bool   valor::util::SampleIsCCQELike (conv::Sample_t s)            { return util::IsCCQELike(s);};
bool   valor::util::SampleIsNC       (conv::Sample_t s)            { return util::IsNC      (s);};
bool   valor::util::SampleIsFar      (conv::Sample_t s)            { return util::IsFar     (s);};
bool   valor::util::SampleIsMuLike   (conv::Sample_t s)            { return util::IsMuLike  (s);};
bool   valor::util::SampleIsELike    (conv::Sample_t s)            { return util::IsELike   (s);};
//________________________________________________________________________________________
bool util::IsMuLike(Sample_t s)
{
  switch (s) {
  case (ksUndefined      ) : return false ; break;
  default: return true; break;
  }
  VLOG("Sample",pFATAL) << "Unknown sample: "<<util::AsString(s)<<" ("<<s<<")";
  util::Die(-1) ; return false;
}
//________________________________________________________________________________________
bool util::IsELike(Sample_t s)
{
  switch (s) {
  default: return false; break;
  }
  VLOG("Sample",pFATAL) << "Unknown sample: "<<util::AsString(s)<<" ("<<s<<")";
  util::Die(-1) ; return false;
}
//________________________________________________________________________________________
