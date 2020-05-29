//____________________________________________________________________________
/*!

\file      ModeList

\brief     Stores a list of modes read from XML

\note      Guaranteed to not contain any undefined modes or coarse modes.


\author    Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

           Francis Bench <f.bench \at liverpool.ac.uk>
           University of Liverpool

\created   November 2019

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_modelist_h_
#define _valor_modelist_h_

#include <vector>
#include <map>
#include <string>

#include <TXMLEngine.h>

#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/ReacModeCoarse.h"

namespace valor {
  class ModeList {
    public:
      ModeList(TXMLEngine* xml_model, XMLNodePointer_t root_node);
      ModeList(std::string mode_xml_file, std::string coarse_xml_file = "");
      const std::vector<conv::ReacMode_t>       & Modes      () const;
      const std::vector<conv::ReacModeCoarse_t> & CoarseModes() const;

      // Plotting conventions used in plot_pdf.C
      int GetCoarseModeLineCol(conv::ReacModeCoarse_t mc) const;
      int GetCoarseModeFillCol(conv::ReacModeCoarse_t mc) const;
      int GetCoarseModeFillSty(conv::ReacModeCoarse_t mc) const;

    private:
      void Init(std::string mode_xml_file, std::string coarse_xml_file = "");
      std::vector<conv::ReacMode_t>         fModes;
      std::vector<conv::ReacModeCoarse_t>   fModesCoarse;
      std::map<conv::ReacModeCoarse_t, int> fModesCoarseLineCol;
      std::map<conv::ReacModeCoarse_t, int> fModesCoarseFillCol;
      std::map<conv::ReacModeCoarse_t, int> fModesCoarseFillSty;

      // Default plotting info
      const int fDefLineCol = (int)kBlack;
      const int fDefFillCol = (int)kGray;   // This is incremented by one for each coarse mode present, so at least the fill colours will be slightly different
      const int fDefFillSty = 1001;         // Solid fill
  };
} // valor


#endif
