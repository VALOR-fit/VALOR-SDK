//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

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

//____________________________________________________________________________
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
      double sinsq2thmue = sinsq2thee * sinsq2thmumu / 4.;
      return sinsq2thmue * sinsqdm;
    }
  }
  LOG("Flavours not supported",pFATAL);
  util::Die(-1);
  return conv::kBadResult;
}
//____________________________________________________________________________
double valor::util::Calculate2FSterileOscMuE(int init_pdg, int final_pdg, double lOverE, double sinsq2thmue, double sinsq2thmumu, double dmsq)
{
  int ipdg = abs(init_pdg);
  int fpdg = abs(final_pdg);
  
  double sindm = std::sin(kOscDimensFactor * lOverE * dmsq);
  double sinsqdm = sindm * sindm;
  
  // Right now, we are NOT oscillating nuE-nuMu since we want to be able to go to sinsq2thmumu point where that is nan
  if (ipdg == kPdgNuE) {
    //~ double sinsq2thee = 4 * sinsq2thmue / sinsq2thmumu;
    //~ double prob = sinsq2thee * sinsqdm;
    if (fpdg == kPdgNuMu) {
      return 0.;
      //~ return prob;
    }
    else if (fpdg == kPdgNuE) {
      return 1.;
      //~ return 1-prob;
    }
  }
  
  if (ipdg == kPdgNuMu) {
    if (fpdg == kPdgNuMu) {
      return 1-sinsq2thmumu * sinsqdm;
    }
    else if (fpdg == kPdgNuE) {
      return sinsq2thmue * sinsqdm;
    }
  }
  LOG("Flavours not supported",pFATAL);
  util::Die(-1);
  return conv::kBadResult;
}

//____________________________________________________________________________
double valor::util::Calculate2FSterileOsc(int init_pdg, int final_pdg, double sinsq2thee, double sinsq2thmumu, double sinsqdm)
{
  int ipdg = abs(init_pdg);
  int fpdg = abs(final_pdg);
  
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
      double sinsq2thmue = sinsq2thee * sinsq2thmumu / 4.;
      return sinsq2thmue * sinsqdm;
    }
  }
  LOG("Flavours not supported",pFATAL);
  util::Die(-1);
  return conv::kBadResult;
}
//____________________________________________________________________________
double valor::util::Calculate2FSterileOscMuE(int init_pdg, int final_pdg, double sinsq2thmue, double sinsq2thmumu, double sinsqdm)
{ 
	int ipdg = abs(init_pdg);
  int fpdg = abs(final_pdg);
   
  // Right now, we are NOT oscillating nuE-nuMu since we want to be able to go to sinsq2thmumu point where that is nan
	if (ipdg == kPdgNuE) {
	//~ double sinsq2thee = 4 * sinsq2thmue / sinsq2thmumu;
	//~ double prob = sinsq2thee * sinsqdm;
		if (fpdg == kPdgNuMu) {
			return 0.;
			//~ return prob;
		}
		else if (fpdg == kPdgNuE) {
			return 1.;
			//~ return 1-prob;
		}
	}
	  
	if (ipdg == kPdgNuMu) { 
		if (fpdg == kPdgNuMu) { 
			return 1-sinsq2thmumu * sinsqdm;
		}
		else if (fpdg == kPdgNuE) {
			return sinsq2thmue * sinsqdm;
		}
	}
	LOG("Flavours not supported",pFATAL);
	util::Die(-1);
	return conv::kBadResult;
}

 //________________________________________________________________ ____________
double valor::util::Calculate2FSterileSinSqDM(const double& lOverE, const double& dmsq)
{
 
	double sindm = std::sin(kOscDimensFactor * lOverE * dmsq);
	double sinsqdm = sindm * sindm;
  
  return sinsqdm;
}

//____________________________________________________________________________
double valor::util::FlipOctantAroundMaxDisap(double sinsqth23, double sinsqth13)
{
  double costh13 = std::sqrt(1-sinsqth13);
  double centre = 1./(2*costh13);
  double diff = centre - sinsqth23;
  double result = sinsqth23 + 2*diff;
  return result;
}
//____________________________________________________________________________
