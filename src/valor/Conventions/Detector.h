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

  // TODO - This enum contains each phase of each detector as a separate entity.
typedef enum EDetectorPhase
{
   kdphUndefined = -1, 
   kdphDUNEND,          // Generic DUNE near detector
   kdphDUNEFD1ph,       // Single-phase LArTPC DUNE far detector
   kdphDUNEFD2ph,       // Double-phase LArTPC DUNE far detector
   kdphSBND,            // SBN near detector 
   kdphSBNuBooNE,       // MicroBooNE detector in SBN
   kdphSBNICARUS,       // Refurshished ICARUS detector in SBN
   kdphSuperK,          // Super-Kamiokande
   kdphHyperK,          // Hyper-Kamiokande
   kdphND280,           // ND280
   kdphND280Up,         // Upgraded ND280
   kdphINGRID,          // INGRID
   kdphNuPRISM,         // NuPRISM
   kdphNuTIT,           // appropriate shorter name for TITUS
   kdphNOvAND,          // NOvA near detector
   kdphNOvAFD,          // NOvA far detector
   kdphJUNO,            // JUNO
} DetectorPhase_t;

  // TODO - It is probably best to define unique labels for a detector that may have multiple
  // phases of life. So this enum will not be sequential, per se.
typedef enum EDetector
{
  kdUndefined = -1,
  kdDUNEND    = static_cast<int>( kdphDUNEND ),     //  0
  kdDUNEFD    = static_cast<int>( kdphDUNEFD2ph ),  //  1
  kdSBND      = static_cast<int>( kdphSBND ),       //  2
  kdSBNuBooNE = static_cast<int>( kdphSBNuBooNE ),  //  3
  kdSBNICARUS = static_cast<int>( kdphSBNICARUS ),  //  4
  kdSuperK    = static_cast<int>( kdphSuperK ),     //  5
  kdHyperK    = static_cast<int>( kdphHyperK ),     //  6
  kdND280     = static_cast<int>( kdphND280Up ),    //  7
  kdINGRID    = static_cast<int>( kdphINGRID ),     //  8
  kdNuPRISM   = static_cast<int>( kdphNuPRISM ),    //  9
  kdNuTIT     = static_cast<int>( kdphNuTIT ),      // 10
  kdNOvAND    = static_cast<int>( kdphNOvAND ),     // 11
  kdNOvAFD    = static_cast<int>( kdphNOvAFD ),     // 12
  kdJUNO      = static_cast<int>( kdphJUNO ),        // 13
  kdOnePastEnd = static_cast<int>( kdJUNO ) + 1
} Detector_t; 

// I really don't have an elegant way to count the above...
  static const int kNDetectorsDefined = 14;

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
  bool             IsAtmospheric      (conv::Detector_t d);
  
} // util

} // valor
  
#endif 

