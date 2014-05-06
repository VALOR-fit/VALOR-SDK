//________________________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include "NDFit/NDSample.h"

using namespace t2kana;
using namespace t2kana::conv;

//________________________________________________________________________________________
string util::AsString(NDSample_t s)
{
  switch(s) {
    case (ksNDNuMuCC       ) : return "numuCCincl";   break; 
    case (ksNDNuMuCC1Trk   ) : return "numuCC1trk";   break; 
    case (ksNDNuMuCC2TrkQE ) : return "numuCC2trkQE"; break; 
    case (ksNDNuMuCC1PiChg ) : return "numuCC1piC";   break; 
    case (ksNDNuMuCC1Pi0   ) : return "numuCC1pi0";   break; 
    case (ksNDNC           ) : return "NC";           break; 
    case (ksNDNC1pi0       ) : return "NC1pi0";       break; 

    default: return "--"; 
  }
  return "";
}
//________________________________________________________________________________________

	
