//____________________________________________________________________________
/*!

\class    valor::conv

\brief    PDG Codes

\author   Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
          STFC, Rutherford Appleton Laboratory
          
\created  Oct 20, 2010
*/
//____________________________________________________________________________

#ifndef pdg_h
#define pdg_h

#include <string>

#include "valor/Conventions/NuFlavour.h"

using std::string;

namespace valor {
namespace conv   {

static const int kPdgNuE      =   12;
static const int kPdgNuMu     =   14;
static const int kPdgNuTau    =   16;
static const int kPdgNuEBar   =  -12;
static const int kPdgNuMuBar  =  -14;
static const int kPdgNuTauBar =  -16;

//sterile neutrinos (PDG codes are made up)
static const int kPdgNuS4     =   80;
static const int kPdgNuS5     =  100;
static const int kPdgNuS4Bar  =  -80;
static const int kPdgNuS5Bar  = -100;

// External BGs, also made up and obviously unphysical
static const int kPdgCosmic   =  -99;

static const int kPdgNuFlavs[kNNuFlavour] = {
  12,
  14,
  16,
 -12,
 -14,
 -16
};

} // conv

namespace util {

  bool IsNeutrino         (int pdg_code);
  bool IsAntiNeutrino     (int pdg_code);

  string Pdg2String       (int pdg_code);
  string Pdg2StringTex    (int pdg_code);

} // util

} // valor
  
#endif 

