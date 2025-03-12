//____________________________________________________________________________
/*!

\class     valor::conv::NuclearTarget_t

\brief     Target Nucleus enumeration

\author    s.r.dennis@liverpool.ac.uk
           University of Liverpool
          
\created   November 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_dune_nuclear_target_h_
#define _valor_dune_nuclear_target_h_

#include <string>

namespace valor {
namespace conv {

typedef enum ENuclearTarget
{ // Note that a specific name refers to only a single isotope, eg C12, Ar40 etc
  kntUndefined = -1, 
  kntInclusive,
  kntOther,
  kntHydrogen,
  kntCarbon,
  kntOxygen,
  kntArgon,
  kntCalcium,
  kntOnePastEnd
}
NuclearTarget_t;

  static const int kNNuclearTarget = static_cast<int>(kntOnePastEnd);

static const int kNuclearTargetPDG [conv::kNNuclearTarget] = {
  /* kntInclusive */ -2             ,
  /* kntOther     */ -3             ,
  /* kntHydrogen  */ 1000010010     ,
  /* kntCarbon    */ 1000060120     ,
  /* kntOxygen    */ 1000080160     ,
  /* kntArgon     */ 1000180400     ,
  /* kntCalcium   */ 1000200400     
};

} // conv


//
// Associated utilities
//
namespace util {

  std::string  AsString     (conv::NuclearTarget_t nt);
  std::string  AsStringTeX  (conv::NuclearTarget_t nt, bool root);
  
  conv::NuclearTarget_t NuclearTargetFromString(std::string nt);

} // util

} // valor
  
#endif 

