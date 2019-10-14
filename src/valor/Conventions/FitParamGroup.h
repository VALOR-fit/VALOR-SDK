//____________________________________________________________________________
/*!

\class     valor::conv::FitParamGroup_t

\brief     Enumeration of fit parameter groups.

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   December 21, 2015

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_dune_fit_param_group_h
#define _valor_dune_fit_param_group_h

#include <string>

using std::string;

namespace valor {
namespace conv {

typedef enum EFitParamGroup
{
   kpgUndefined = -1,  //
   kpgFHCNuMuFlux,     //
   kpgFHCNuMuBarFlux,  //
   kpgFHCNuEFlux,      //
   kpgFHCNuEBarFlux,   //
   kpgRHCNuMuFlux,     //
   kpgRHCNuMuBarFlux,  //
   kpgRHCNuEFlux,      //
   kpgRHCNuEBarFlux,   //
   kpgXSec,            //
   kpgHadronization,   //
   kpgFSI,             //
   kpgDetector,        //
   kpgOscillations
} 
FitParamGroup_t; 

} // conv


//
// Associated utilities
//
namespace util {


} // util

} // valor
  
#endif 

