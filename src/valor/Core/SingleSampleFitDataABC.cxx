//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//________________________________________________________________________________________

#include <cassert>

#include "valor/Core/SingleSampleFitDataABC.h"

using namespace valor;

//________________________________________________________________________________________
SingleSampleFitDataABC::SingleSampleFitDataABC() :
FitDataABC()
{
  fDetector = conv::kdUndefined;
  fBeam     = conv::kbUndefined;
  fSample   = conv::ksUndefined;
  fKSp      = conv::kspUndefined;
}
//________________________________________________________________________________________
SingleSampleFitDataABC::~SingleSampleFitDataABC() 
{

}
//________________________________________________________________________________________
void SingleSampleFitDataABC::Configure(
  conv::Detector_t d, conv::BeamConf_t b, conv::Sample_t s, conv::KineSpace_t k)
{
  fDetector = d;
  fBeam     = b;
  fSample   = s;
  fKSp      = k;

  assert(fDetector != conv::kdUndefined );
  assert(fBeam     != conv::kbUndefined );
  assert(fSample   != conv::ksUndefined );
  assert(fKSp      != conv::kspUndefined);
}
//________________________________________________________________________________________

