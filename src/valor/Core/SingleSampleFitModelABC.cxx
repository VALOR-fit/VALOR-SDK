//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//________________________________________________________________________________________

#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Core/SingleSampleFitModelABC.h"

using namespace valor;

//________________________________________________________________________________________
SingleSampleFitModelABC::SingleSampleFitModelABC() :
FitModelABC(),
fSystematics(NULL),
fOwnerOfSystematics(false)
{
  fDetector = conv::kdUndefined;
  fBeam     = conv::kbUndefined;
  fSample   = conv::ksUndefined;
  fKSp      = conv::kspUndefined;
  fName     = "";
}
//________________________________________________________________________________________
SingleSampleFitModelABC::~SingleSampleFitModelABC() 
{

}
//________________________________________________________________________________________
void SingleSampleFitModelABC::Configure(
  conv::Detector_t d, conv::BeamConf_t b, conv::Sample_t s, conv::KineSpace_t k)
{
  fDetector = d;
  fBeam     = b;
  fSample   = s;
  fKSp      = k;

  if (fDetector == conv::kdUndefined ) { VLOG("SingleSampleFitModelABC",pFATAL) << "Bad Detector"  << fDetector ; util::Die(-1); }
  if (fBeam     == conv::kbUndefined ) { VLOG("SingleSampleFitModelABC",pFATAL) << "Bad Beam"      << fBeam     ; util::Die(-1); }
  if (fSample   == conv::ksUndefined ) { VLOG("SingleSampleFitModelABC",pFATAL) << "Bad Sample"    << fSample   ; util::Die(-1); }
  if (fKSp      == conv::kspUndefined) { VLOG("SingleSampleFitModelABC",pFATAL) << "Bad KineSpace" << fKSp      ; util::Die(-1); }

}
//________________________________________________________________________________________
void SingleSampleFitModelABC::SetSystConf(CorrelatedSystematics * syst, Option_t * opt)
{
  TString ts(opt);
  if (ts.Contains("copy") ) {
    fSystematics = dynamic_cast<CorrelatedSystematics *>(syst->Clone());
    fOwnerOfSystematics = true;
  }
  else {
    fSystematics = syst;
    fOwnerOfSystematics = ts.Contains("adopt");
  }

  return;
}
//________________________________________________________________________________________
