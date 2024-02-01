//____________________________________________________________________________
/*!

  \class     valor::utils

  \brief     Utilities for macros

  \author    Tom Dealtry <t.dealtry \at lancaster.ac.uk>
             Lancaster University
          
  \created   Aug 12, 2015

  \copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_macros_utils_h_
#define _valor_macros_utils_h_

#include "valor/Oscillation/OscParams.h"

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

  // Remove from vector 'a' any items which also appear in vector 'b'
  template<class T> void RemoveItemsFromVector(std::vector<T>& a, std::vector<T>& b)
  // Implementation must stay in the .h file for the linker to find it...
  {
    // Use the erase-remove idiom
    a.erase(
      remove_if(
        a.begin(),
        a.end(),

        // This lambda returns true if current element 'e'
        // (from vector 'a') is in vector 'b'.
        [&b](const T& e)
        {
          auto it = find(b.begin(), b.end(), e);
          return (it != b.end());
        }
      ),

      a.end()
    );
  }
  
} // util
} // valor

#endif
