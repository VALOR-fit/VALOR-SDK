//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//________________________________________________________________________________________

#include "valor/Conventions/Baseline.h"
#include "valor/Messenger/Messenger.h"

using namespace valor;

//________________________________________________________________________________________
double util::Baseline(conv::BeamConf_t b, conv::Detector_t d)
{
  if(b == conv::kbLBNF_FHC || b == conv::kbLBNF_RHC) {
      if (d == conv::kdDUNEND   ) return conv::kLBNFTgt2DUNEND;
      if (d == conv::kdDUNEFD1ph) return conv::kLBNFTgt2DUNEFD;
      if (d == conv::kdDUNEFD2ph) return conv::kLBNFTgt2DUNEFD;
  }

if(b == conv::kbNuMI_FHC || b == conv::kbNuMI_RHC) {
	if (d == conv::kdNOvAND)    return conv::kNuMITgt2NOvAND;
    if (d == conv::kdNOvAFD)    return conv::kNuMITgt2NOvAFD;
}

  if(b == conv::kbBNB_FHC || b == conv::kbBNB_RHC) {
      if (d == conv::kdSBND     ) return conv::kBNBTgt2SBND;
      if (d == conv::kdSBNuBooNE) return conv::kBNBTgt2uBooNE;
      if (d == conv::kdSBNICARUS) return conv::kBNBTgt2ICARUS;
  }

  if(b == conv::kbJPARC_FHC || b == conv::kbJPARC_RHC) {
      if (d == conv::kdSuperK ) return conv::kJPARCTgt2SuperK;
      if (d == conv::kdHyperK ) return conv::kJPARCTgt2SuperK;
      if (d == conv::kdND280  ) return conv::kJPARCTgt2ND280;
      if (d == conv::kdND280Up) return conv::kJPARCTgt2ND280;
      if (d == conv::kdINGRID ) return conv::kJPARCTgt2ND280;
  }

  if(b == conv::kbAtmo_JUNONuMu || b == conv::kbAtmo_JUNONuE || b == conv::kbAtmo_JUNONuMuBar || b == conv::kbAtmo_JUNONuEBar) {
    if (d == conv::kdAtmoJUNO) return conv::kAtmoJUNO;
  }
  
  LOG("Baseline", pERROR)
     << "Undefined baseline for detector: " << util::AsString(d)
     << " in beam: " << util::AsString(b);

  return -1;
}
//________________________________________________________________________________________
