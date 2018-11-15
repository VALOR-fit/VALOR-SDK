//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

ChangeLog:

*/
//________________________________________________________________________________________

//fmwk
#include "valor/Utilities/UtlStr.h"
//dune
#include "valor/Conventions/Detector.h"

using namespace valor;
using namespace valor::conv;

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
