//____________________________________________________________________________
/*!

\file      ModeList

\brief     Stores a list of modes read from XML

\note      


\author    Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

\created   November 2019

\copyright (c) 2010-2019, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_modelist_h_
#define _valor_modelist_h_

#include <vector>
#include <string>

#include "valor/Conventions/ReacMode.h"

namespace valor {
  class ModeList {
    public:
      ModeList(std::string xml_file);
      const std::vector<conv::ReacMode_t> & Modes() const;
    private:
      std::vector<conv::ReacMode_t> fModes;
  };
} // valor


#endif
