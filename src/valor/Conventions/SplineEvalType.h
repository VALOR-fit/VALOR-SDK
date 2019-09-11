//____________________________________________________________________________
/*!

\class     valor::conv::SplEvalType_t

\brief     Spline evaluation type enumeration

\author    Francis Bench <f.bench \at liverpool.ac.uk>
           University of Liverpool
          
\created   July 17, 2019

\copyright (c) 2010-2019, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_spl_eval_type_h_
#define _valor_spl_eval_type_h_

#include <TROOT.h>

#include <string>

namespace valor {
  namespace conv {

    typedef enum ESplEvalType
    {
      kSplEvalUndefined = -1,
      kSplEvalCurrVal,            // Splines are evaluated at the parameter current value
      kSplEvalCurrTweak           // Splines are evaluated at the parameter current value - untuned value
    }
    SplEvalType_t;

    static const size_t kNSplineEvalTypes = kSplEvalCurrTweak+1;

  } // conv

  //
  // Associated utilities
  //
  namespace util {

    std::string      AsString                 (conv::SplEvalType_t t, Option_t * opt=0);
    // Alias for above
    std::string         SplEvalTypeAsString   (conv::SplEvalType_t t, Option_t * opt=0);
    conv::SplEvalType_t SplEvalTypeFromString (string t);

  } // util
} // valor
  
#endif 

