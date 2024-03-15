//____________________________________________________________________________
/*!

\class     valor::utils

\brief     Utilities for printing ROOT objects in a neat way

\author    Steve Dennis <stephen.dennis \at warwick.ac.uk>
           University of Warwick
          
\created   Feb 01, 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_print_utils_h_
#define _valor_print_utils_h_

#include <iostream>
#include <string>

#include "TVector.h"
#include "TMatrixD.h"
#include "TString.h"

#include "valor/Conventions/PhysParamId.h"

namespace valor {
namespace util {

  void PrintVector    (const TVectorD & v, int w=15, int prec=9, Option_t * opt="", std::ostream & stream = std::cout);
  void PrintVectorPair(const TVectorD & current, const TVectorD & nominal, int w=15, int prec=9, Option_t * option="", std::ostream & stream = std::cout);
  void (PrintMatrix)  (const TMatrixD & m, int w=15, int prec=9, std::ostream & stream = std::cout);
  //~ void PrintMatrixTeX (const TMatrixD & m, int w=20, int prec=9, std::ostream & stream = std::cout, TString opt = "");
  
  // As above, but taking a list of parameters so the names can be printed
          
  void PrintVector     (const TVectorD & v, const conv::PhysParamId_t * params, int w=15, int prec=9, Option_t * opt="", std::ostream & stream = std::cout);
  void PrintVectorPair (const TVectorD & current, const TVectorD & nominal, const conv::PhysParamId_t * params,
                        int w, int prec, Option_t * option = "", std::ostream & stream = std::cout);
  
  // Writes to the VALOR slack channel (if enabled)
  void NotifySlack (std::string message, std::string channel, std::string username);
  
} // util
} // valor
  
#endif


