//________________________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include "NDFit/NDSampleBinning.h"

using namespace t2kana;
using namespace t2kana::conv;

//________________________________________________________________________________________
int util::NumErecoBins(NDSample_t sample, int zone)
{
    case (ksNDNuMuCC       ) : return "numuCCincl";   break;
    case (ksNDNuMuCC1Trk   ) : return "numuCC1trk";   break;

}
//________________________________________________________________________________________
double * util::ErecoBinEdges(NDSample_t sample, int zone)
{
  return 0;
}
//________________________________________________________________________________________
int util:: NumYrecoBins(NDSample_t sample, int zone)
{
  return 0;
}
//________________________________________________________________________________________
double * util::YrecoBinEdges(NDSample_t sample, int zone)
{
  return 0;
}
//________________________________________________________________________________________

	
