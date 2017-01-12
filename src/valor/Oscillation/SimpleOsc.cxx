//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cassert>

#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/PDG.h"
#include "valor/Oscillation/SimpleOsc.h"

using namespace valor;
using namespace valor::conv;

double valor::util::Calculate2FSterileOsc(int init_pdg, int final_pdg, double lOverE, double sinsq2thee, double sinsq2thmumu, double dmsq)
{
  int ipdg = abs(init_pdg);
  int fpdg = abs(final_pdg);
  
  double sindm = std::sin(kOscDimensFactor * lOverE * dmsq);
  double sinsqdm = sindm * sindm;
      
  if (ipdg == kPdgNuE) {
    double prob = sinsq2thee * sinsqdm;
    if (fpdg == kPdgNuMu) {
      return prob;
    }
    else if (fpdg == kPdgNuE) {
      return 1-prob;
    }
  }
  
  if (ipdg == kPdgNuMu) {
    if (fpdg == kPdgNuMu) {
      return 1-sinsq2thmumu * sinsqdm;
    }
    else if (fpdg == kPdgNuE) {
      double sinsq2mue = sinsq2thee * sinsq2thmumu / 4.;
      return sinsq2mue * sinsqdm;
    }
  }
  LOG("Flavours not supported",pFATAL);
  util::Die(-1);
  return conv::kBadResult;
}
