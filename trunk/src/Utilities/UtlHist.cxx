//____________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//____________________________________________________________________________

#include <fstream>
#include <vector>

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
using std::vector;

using namespace valor;

//___________________________________________________________________________
TH1D util::Make1DHistogramFromArray(const VALORArray & va, const double * bin_edges, int dimension)
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
  h.SetEntries(h.Integral());
  
  return h;
}
//___________________________________________________________________________
TH2D util::Make2DHistogramFromArray(const VALORArray & va, double * const bin_edges_x, const double * bin_edges_y, int dimension_x, int dimension_y)
{
  if (va.GetNDim()<2) return TH2D();
  
  string name (va.GetName());
  string title(va.GetTitle());
  if (va.GetNDim() > 2) {
    name  += Form("_%d_%d",dimension_x, dimension_y);
    title += Form("_%d_%d",dimension_x, dimension_y);
  }
  
  int nbins_x = va.GetSize(dimension_x);
  int nbins_y = va.GetSize(dimension_y);
  
  TH2D h(name.c_str(),title.c_str(), nbins_x, bin_edges_x, nbins_y, bin_edges_y);
  h.GetXaxis()->SetTitle(va.GetDimensionName(dimension_x).c_str());
  h.GetYaxis()->SetTitle(va.GetDimensionName(dimension_y).c_str());
  
  const VALORArray * curr_va = &va;
  
  vector<VALORArray> vec;
  while (curr_va->GetNDim() > 2) {
    for (int i = 0 ; i < curr_va->GetNDim() ; i++) {
      if (i==dimension_x || i==dimension_y) continue;
      vec.push_back( curr_va->GetSlice(i,-1,-1) );
      curr_va = &(vec.back());
      if (i < dimension_x) dimension_x--;
      if (i < dimension_y) dimension_y--;
      break;
    }
  }
  
  for (int ibin_x = 0 ; ibin_x < nbins_x ; ibin_x++) {
    for (int ibin_y = 0 ; ibin_y < nbins_y ; ibin_y++) {
      int ibin = h.GetBin(ibin_x+1,ibin_y+1);
      if (dimension_x == 0 && dimension_y == 1) {
        h.AddBinContent(ibin,curr_va->GetEntry(ibin_x,ibin_y));
      }
      else if (dimension_x == 1 && dimension_y == 0) {
        h.AddBinContent(ibin,curr_va->GetEntry(ibin_y,ibin_x));
      }
      else {
        std::cerr<<"UtlHist/Make2DHistogramFromArray >> Failed to make histogram with dimensions "<<dimension_x<<","<<dimension_y<<endl;
        return h;
      }
    }
  }
  
  h.SetEntries(h.Integral());
  
  return h;
}
//___________________________________________________________________________

