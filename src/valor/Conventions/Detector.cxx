//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include <algorithm>
#include <iostream>

//fmwk
#include "valor/Utilities/UtlStr.h"
#include "valor/Conventions/Detector.h"
#include "valor/Messenger/Messenger.h"

using namespace valor;
using namespace valor::conv;

using std::string;

//________________________________________________________________________________________
string util::AsString(Detector_t d, Option_t * /*opt*/)
{
  string name = "";

  switch(d) {
    case (kdDUNEND   ) : name = "dune_nd";    break;
    case (kdDUNEFD1ph) : name = "dune_fd1ph"; break;
    case (kdDUNEFD2ph) : name = "dune_fd2ph"; break;
    case (kdSBND     ) : name = "sbn_nd";     break;
    case (kdSBNuBooNE) : name = "sbn_uboone"; break;
    case (kdSBNICARUS) : name = "sbn_icarus"; break;
    case (kdSuperK   ) : name = "superk";     break;
    case (kdHyperK   ) : name = "hyperk";     break; 
    case (kdND280    ) : name = "nd280";      break; 
    case (kdND280Up  ) : name = "nd280up";    break; 
    case (kdINGRID   ) : name = "ingrid";     break; 
    case (kdNuPRISM  ) : name = "nuprism";    break; 
    case (kdNuTIT    ) : name = "titus";      break;
    case (kdNOvAND   ) : name = "nova_nd";    break;
    case (kdNOvAFD   ) : name = "nova_fd";    break;
    default            : name = "--";  
  }
/*
  if(opt) {
    string name2  = name;
    string optstr = opt;
    if(optstr.find("lowercase") != string::npos) {
       name2 = util::ToLowerCase(name);
    }
    return name2;
  }
*/
  return name;
}
//________________________________________________________________________________________
string util::AsStringTeX(Detector_t d, bool root)
{
  string name = "";

  switch(d) {
    case (kdDUNEND   ) : name = "DUNE ND";              break;
    case (kdDUNEFD1ph) : name = "DUNE FD Single Phase"; break;
    case (kdDUNEFD2ph) : name = "DUNE FD Dual Phase";   break;
    case (kdSBND     ) : name = "SBN-ND";               break;
    case (kdSBNuBooNE) : name = "SBN-#muBooNE";         break;
    case (kdSBNICARUS) : name = "SBN-ICARUS";           break;
    case (kdSuperK   ) : name = "Super-K";              break;
    case (kdHyperK   ) : name = "Hyper-K";              break; 
    case (kdND280    ) : name = "ND280";                break; 
    case (kdND280Up  ) : name = "ND280 Upgrade";        break; 
    case (kdINGRID   ) : name = "INGRID";               break; 
    case (kdNuPRISM  ) : name = "#nu Prism";            break; 
    case (kdNuTIT    ) : name = "TITUS";                break;
    case (kdNOvAND   ) : name = "NO{#nu}A ND";          break;
    case (kdNOvAFD   ) : name = "NO{#nu}A FD";          break;
    default            : name = "--";  
  }
  if(root) return name;

  // normal TeX, not ROOT TeX - change '#' with '\'
  replace( name.begin(), name.end(), '#', '\\');

  return name;
}
//________________________________________________________________________________________
string util::DetectorAsString(Detector_t d, Option_t * opt) {return util::AsString(d,opt);}
//________________________________________________________________________________________
Detector_t util::DetectorFromInt(int d)
{
// Cast an integer to Detector_t, but with boundary check
//            [first]                 [last]
  if(d >= (int)kdDUNEND && d <= (int) kdNOvAFD) return (Detector_t)d;
  return kdUndefined;
}
//________________________________________________________________________________________
Detector_t util::DetectorFromString(string detector)
{
  string lc_detector = util::ToLowerCase(detector);
  for (int i = 0 ; i < kNDetectorsDefined ; i++ ) {
    Detector_t d = (Detector_t)i;
    string this_lc_detector = util::ToLowerCase(util::AsString(d));
    if(lc_detector == this_lc_detector && this_lc_detector!="--") {
      return d;
    }
  }
    
  return kdUndefined;
}
//________________________________________________________________________________________
bool util::IsLongBaseline(Detector_t d)
{
  switch(d) {
    case (kdDUNEND   ) : return false;
    case (kdDUNEFD1ph) : return true;
    case (kdDUNEFD2ph) : return true;
    case (kdSBND     ) : return false;
    case (kdSBNuBooNE) : return false;
    case (kdSBNICARUS) : return false;
    case (kdSuperK   ) : return true;
    case (kdHyperK   ) : return true;
    case (kdND280    ) : return false;
    case (kdND280Up  ) : return false;
    case (kdINGRID   ) : return false;
    case (kdNuPRISM  ) : return false;
    case (kdNuTIT    ) : return false;
    case (kdNOvAND   ) : return false;
    case (kdNOvAFD   ) : return true;
    default: break;
  }
  VLOG("FitInputs", pWARN) << "Unrecognised detector \'" << d << "\'"<<" - "<<util::AsString(d);
  return false;
}
//________________________________________________________________________________________
bool util::IsShortBaseline(Detector_t d)
{
  switch(d) {
    case (kdDUNEND   ) : return true;
    case (kdDUNEFD1ph) : return false;
    case (kdDUNEFD2ph) : return false;
    case (kdSBND     ) : return true;
    case (kdSBNuBooNE) : return true;
    case (kdSBNICARUS) : return true;
    case (kdSuperK   ) : return false;
    case (kdHyperK   ) : return false;
    case (kdND280    ) : return true;
    case (kdND280Up  ) : return true;
    case (kdINGRID   ) : return true;
    case (kdNuPRISM  ) : return true; // well, probably
    case (kdNuTIT    ) : return true;
    case (kdNOvAND   ) : return true;
    case (kdNOvAFD   ) : return false;
    default: break;
  }
  VLOG("FitInputs", pWARN) << "Unrecognised detector \'" << d << "\'"<<" - "<<util::AsString(d);
  return false;
}
//________________________________________________________________________________________
