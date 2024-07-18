//____________________________________________________________________________
/*!

\class     valor::conv::Detector_t

\brief     Detector enumeration

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   April 1, 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_dune_detector_h_
#define _valor_dune_detector_h_

#include <TROOT.h>

#include <string>

using std::string;

namespace valor {
namespace conv {

static const int kNDetectorsDefined = 18;

typedef enum EDetector
{
   kdUndefined = -1,
   kdDUNEND,          // Generic DUNE near detector
   kdDUNEFD1ph,       // Single-phase LArTPC DUNE far detector
   kdDUNEFD2ph,       // Double-phase LArTPC DUNE far detector
   kdSBND,            // SBN near detector
   kdSBNuBooNE,       // MicroBooNE detector in SBN
   kdSBNICARUS,       // Refurshished ICARUS detector in SBN
   kdSuperK,          // Super-Kamiokande
   kdHyperK,          // Hyper-Kamiokande
   kdND280,           // ND280
   kdND280Up,         // Upgraded ND280
   kdINGRID,          // INGRID
   kdNuPRISM,         // NuPRISM
   kdNuTIT,           // appropriate shorter name for TITUS
   kdNOvAND,          // NOvA near detector
   kdNOvAFD,          // NOvA far detector
   kdNuStormND,       // NuStorm near detector
   kdNuStormFD,       // NuStorm far detector
   kdJUNO         // JUNO Atmo
}
Detector_t;

} // conv


//
// Associated utilities
//
namespace util {

  string      AsString           (conv::Detector_t d, Option_t * opt=0);
  string      AsStringTeX        (conv::Detector_t d, bool root = false);
  string      DetectorAsString   (conv::Detector_t d, Option_t * opt=0);
  conv::Detector_t DetectorFromInt    (int b);
  conv::Detector_t DetectorFromString (string b);
  bool             IsLongBaseline     (conv::Detector_t d);
  bool             IsShortBaseline    (conv::Detector_t d);

} // util

} // valor

#endif
