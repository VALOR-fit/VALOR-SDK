//____________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//____________________________________________________________________________

#include <fstream>
#include <vector>
#include <string>
#include <map>

#include <TSystem.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TColor.h>
#include <TGraph.h>
#include <TMath.h>

#include "Utilities/UtlHist.h"
#include "Messenger/Messenger.h"
#include "Conventions/PhysParamId.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::ifstream;
using std::string;
using std::vector;
using std::map;

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
TObjArray * valor::util::MakeHistogramsOfParameterValues(int n_entries, const PhysParamList *const* pparms, int n_bins, const FitParamList * get_limits_from)
{
  if (n_entries <= 0) { return 0;}
  
  if (n_bins <= 0) {n_bins = TMath::CeilNint(sqrt((double)n_entries)/2);}
  
  const PhysParamList & pp = *(pparms[0]);
  int n_params  = pp.NParams();
  conv::PhysParamId_t params[n_params];
  double param_mins[n_params];
  double param_maxs[n_params];
  const map<int,double> & m = pp.Map();
  
  TObjArray * out_arr = new TObjArray(n_params);
  out_arr->SetOwner(true);
  
  // First loop over first param list to count the params and set the limits
  int ip=0;
  for (map<int,double>::const_iterator it=m.begin();it!=m.end();++it) {
    conv::PhysParamId_t p = (conv::PhysParamId_t)it->first;
    params[ip] = p;
    
    if (get_limits_from and get_limits_from->Exists(p)) { // Get parameter limits from fitparamlist if available
      param_mins[ip] = get_limits_from->MinValue(p);
      param_maxs[ip] = get_limits_from->MaxValue(p);
    }
    else { // otherwise just use the max and min values we have
      double min =  1E100;
      double max = -1E100;
      for (int il = 0 ; il < n_entries ; il++) {
        double v = pparms[il]->Value(p);
        if (v < min) min = v;
        if (v > max) max = v;
      }
      param_mins[ip] = min;
      param_maxs[ip] = max;
    }
    
    //~ std::cout<<util::RawParamAsString(p)<<" limits "<<param_mins[ip]<<"\t"<<param_maxs[ip]<<endl;
    ip++;
  }
  
  // Then loop over params again to make histograms
  for (ip = 0 ; ip < n_params ; ip++) {
    conv::PhysParamId_t p = params[ip];
    string hname (Form("param_values_%s",util::RawParamAsString(p).c_str()));
    string htitle(Form("%s Values",util::RawParamAsString(p).c_str()));
    
    out_arr->AddAt(new TH1D(hname.c_str(),htitle.c_str(),n_bins,param_mins[ip],param_maxs[ip]),ip);
  }
  
  // Now loop over the PhysParamLists to fill the histograms
  for (int il = 0 ; il < n_entries ; il++) {
    for (ip = 0 ; ip < n_params ; ip++) {
      conv::PhysParamId_t p = params[ip];
      ((TH1D*)out_arr->At(ip))->Fill(pparms[il]->Value(p));
    }
  }
  
  return out_arr;
}
//___________________________________________________________________________
TH1 * valor::util::MakeFittedHistogramShowingFittedValues(const TH1* hist, const TF1* func, int npoints)
{
  TH1 * fitted_hist = dynamic_cast<TH1*>(hist->Clone());
  assert(fitted_hist);
  
  int nbins_in = fitted_hist->GetXaxis()->GetNbins();
  
  double xmin = fitted_hist->GetXaxis()->GetBinLowEdge(1);
  double xmax = fitted_hist->GetXaxis()->GetBinUpEdge (nbins_in);
  fitted_hist->SetBins(npoints,xmin,xmax);
    
  for (int ibin = 1 ; ibin <= fitted_hist->GetXaxis()->GetNbins() ; ibin++) {
    double bin_width = fitted_hist->GetXaxis()->GetBinWidth(1);
    double bin_centre = fitted_hist->GetXaxis()->GetBinCenter(ibin);
    double func_val = func->Eval(bin_centre);
    fitted_hist->SetBinContent(ibin, func_val);
  }
  return fitted_hist;
}
//___________________________________________________________________________
