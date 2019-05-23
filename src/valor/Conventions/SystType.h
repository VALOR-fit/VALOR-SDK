//____________________________________________________________________________
/*!

\class     valor::conv::SystType_t

\brief     Systematic type enumeration

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Francis Bench <f.bench \at liverpool.ac.uk>
           University of Liverpool
          
\created   June 27, 2018

\copyright (c) 2010-2018, The VALOR Fitting Group, All Rights Reserved

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
      kSystResponse,
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

