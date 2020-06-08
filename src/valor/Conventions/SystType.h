//____________________________________________________________________________
/*!

\class     valor::conv::SystType_t

\brief     Systematic type enumeration

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Francis Bench <f.bench \at liverpool.ac.uk>
           University of Liverpool
          
\created   June 27, 2018

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_syst_type_h_
#define _valor_syst_type_h_

#include <TROOT.h>

#include <string>

namespace valor {
  namespace conv {

    typedef enum ESystType
    {
      kSystUndefined = -1,
      kSystNorm,
      kSystLateral,
      kSystLateralPostOsc,
      kSystResponse,
      kSystSpecial,
      kSystDisabled
    }
    SystType_t;
    
    static const size_t kNSystTypes = kSystDisabled+1;

  } // conv

  //
  // Associated utilities
  //
  namespace util {

    std::string      AsString           (conv::SystType_t t, Option_t * opt=0);
    // Alias for above
    std::string      SystTypeAsString   (conv::SystType_t t, Option_t * opt=0);
    conv::SystType_t SystTypeFromInt    (int t);
    conv::SystType_t SystTypeFromString (string t);

  } // util
} // valor
  
#endif 

