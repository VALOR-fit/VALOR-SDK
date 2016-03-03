//____________________________________________________________________________
/*!

  \class     valor::utils

  \brief     Utilities for macros

  \author    Tom Dealtry <t.dealtry \at lancaster.ac.uk>
             Lancaster University
          
  \created   Aug 12, 2015

  \copyright (c) 2010-2016, The VALOR group.

*/
//____________________________________________________________________________

#ifndef _valor_macros_utils_h_
#define _valor_macros_utils_h_

#include "Utilities/OscParams.h"

#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"

#include <string>

namespace valor {
  class PhysHypothesis;

namespace util {
  
  // Get the string related to the systematics which are being tweaked
  std::string SysThrowIdToString(unsigned int tweak_systematics);

  // Construct an absolute file path, relative to $T2KOSCANA
  std::string FormFilePath(const char * file, const char * dir);

  // Helper function. Get a TH2D from a file & check its valid
  TH2D * GetTH2DFromTFile(TFile * f, const char * name);
  
} // util
} // valor

#endif
