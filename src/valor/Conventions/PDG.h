//____________________________________________________________________________
/*!

\class    valor::conv

\brief    PDG Codes

\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          STFC, Rutherford Appleton Laboratory
          
\created  Oct 20, 2010
*/
//____________________________________________________________________________

#ifndef pdg_h
#define pdg_h

#include <string>

#include "valor/Conventions/NuFlavour.h"

// Particle codes taken from
// http://pdg.lbl.gov/2017/reviews/rpp2016-rev-monte-carlo-numbering.pdf

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

static const int kPdgNuFlavs[kNNuFlavour] = {
  12,
  14,
  16,
 -12,
 -14,
 -16
};

  static const int kPdgProton  =  2212; 
  static const int kPdgNeutron =  2112;

  static const int kPdgPiplus  =  211;
  static const int kPdgPiminus = -211;
  static const int kPdgPizero  =  111;

} // conv

namespace util {

  bool IsNeutrino         (int pdg_code);
  bool IsAntiNeutrino     (int pdg_code);

  string Pdg2String       (int pdg_code);
  string Pdg2StringTex    (int pdg_code);

} // util

} // valor
  
#endif 

