//____________________________________________________________________________
/*!

\enum      valor::conv::ConfLevel_t

\brief     Confidence level

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef conf_level_h
#define conf_level_h

#include <string>

namespace valor {

namespace conv {

typedef enum EConfLevel {
   kCLUndefined = -1, 
   kCL68, 
   kCL90, 
   kCL95, 
   kCL99,
   kCL1Sigma,
   kCL2Sigma,
   kCL3Sigma,
   kCL4Sigma,
   kCL5Sigma
} 
ConfLevel_t; 

} // conv

namespace util {
   
  std::string AsString(conv::ConfLevel_t cl);
  std::string ConfLevelAsString(conv::ConfLevel_t cl); //alias for above


} // util

} // valor
  
#endif 

