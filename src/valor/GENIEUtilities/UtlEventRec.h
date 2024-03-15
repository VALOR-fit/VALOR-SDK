//____________________________________________________________________________
/*!

\class     valor::UtlEventRec

\brief     Tools for getting VALOR info out of GENIE event records.

\author    Steve Dennis <s.r.dennis \at University of Liverpool>
           University of Liverpool
           
           Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   April 22, 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef __valor_dune_genie_utlevrec__
#define __valor_dune_genie_utlevrec__

#include <iostream>
#include <vector>

#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/Build.h"
#include "valor/Conventions/KineSpace.h"

namespace genie {
  class EventRecord;
}

namespace valor {
namespace util {
  
  bool GetHadronicSystems(const genie::EventRecord * event, std::vector<int> & prim_had_syst, std::vector<int> & final_had_syst);
  conv::ReacMode_t DetermineReactionMode(const genie::EventRecord * event, bool use_final_state = false);
  
  // Does not respect bar/nonbar
  bool EventContainsQuarkFlavour(const genie::EventRecord * event, int quark_pdg);

  /**
    Extracts kinematic variables from a GENIE vertex.

    \param evrec The event record from which kinematic variables are to be
      extracted.
    \param kv The required kinematic variables.

    \return A vector comprising the kinematic variables in the order specifed by
      kv.
  */
  std::vector<double> GetTrueKinematics(const genie::EventRecord* evrec,
        conv::KineSpace_t kv);
}      // util namespace
}      // valor namespace

#endif 
