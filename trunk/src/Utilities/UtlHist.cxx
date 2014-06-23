//____________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//____________________________________________________________________________

#include <fstream>

#include <TSystem.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TColor.h>
#include <TGraph.h>
#include <string>

#include "Utilities/UtlHist.h"
#include "Messenger/Messenger.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::ifstream;
using std::string;

using namespace valor;

//___________________________________________________________________________
TH1D util::Make1DHistogramFromArray(const VALORArray & va, double * bin_edges, int dimension)
{
  string name (va.GetName());
  string title(va.GetTitle());
  if (va.GetNDim() > 1) {
    name  += Form("_%d",dimension);
    title += Form("_%d",dimension);
  }
  
  int nbins = va.GetSize(dimension);
  
  TH1D h(name.c_str(),title.c_str(),nbins,bin_edges);
  h.GetXaxis()->SetTitle(va.GetDimensionName(dimension).c_str());
  
  if (va.GetNDim()==1) {
    for (int ibin = 0 ; ibin < nbins ; ibin++) {
      h.AddBinContent(ibin+1,va.GetEntry(ibin));
    }
  }
  else {
    const int * array_dimensions = va.GetDimensions();
    int dimcounter[6] = {};
    for (int ientry = 0 ; ientry < va.GetSize(); ientry++) {
      h.AddBinContent(dimcounter[dimension]+1, va.GetArray()[ientry]);
      for (int idim = va.GetNDim()-1; idim>=0 ; idim--) {
        if (dimcounter[idim] < array_dimensions[idim]-1) {
          dimcounter[idim]++;
          break;
        }
        else dimcounter[idim] = 0;
      }
    }
  }
  
  return h;
}
//___________________________________________________________________________

