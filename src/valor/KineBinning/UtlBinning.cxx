//________________________________________________________________________________________
/*

   Copyright (c) 2010-2019, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include "valor/KineBinning/UtlBinning.h"

#include <algorithm>
#include <numeric>
#include <cmath>

// valor
#include "valor/Conventions/Constants.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Messenger/Messenger.h"

using std::string;
using std::vector;

namespace valor
{
namespace util
{
namespace binning
{

/**
  Integrate a VALORArray with a specified binning such that only the
  specified kinematic variables remain.

  \param v The VALORArray containing the data to be integrated.
  \param targetBinning The target binning to be used by the new VALORArray.
  \param currentRecoBinning The current reco binning used by the VALORArray.
  \param currentTrueBinning The current true binning used by the VALORArray.
  \return The contracted VALORArray containing only the specified kinematic
     variables.
*/
VALORArray* Contract(const VALORArray& v,
    const KBCubHcmbEdge2Edge & targetBinning,
    const KBCubHcmbEdge2Edge & currentRecoBinning,
    const KBCubHcmbEdge2Edge & currentTrueBinning);

/**
  Maps the events associated with the specified binning to a 1D
  ROOT-compatible histogram.

  \param events The spectrum corresponding to the simple binning.
  \param b The simple binning.
  \return A ROOT-compatible 1D histogram representing the simple binning.
*/
TH1D* Remap1D(const double* events, const KBCubHcmbEdge2Edge& b);

/**
   Maps the events associated with the specified binning to a 2D
   ROOT-compatible histogram.

   Note that the ROOT histogram produced by this function should be used only
   for plotting purposes, as mapping from optimised to ROOT-compatible binning
   is not guaranteed to conserve total event counts.

   \param events The spectrum corresponding to the optimised binning.
   \param b The optimised binning.
   \return A ROOT-compatible 2D histogram representing the optimised binning.
*/
TH2D* RemapOptimised2D(const double* events,
      const KBCubHcmbMondrian& b);

/**
   Maps the events associated with the specified binning to a 2D
   ROOT-compatible histogram.

   \param events The spectrum corresponding to the optimised binning.
   \param b The optimised binning.
   \return A ROOT-compatible 2D histogram representing the simple binning.
*/
TH2D* RemapSimple2D(const double* events, const KBCubHcmbEdge2Edge& b);

/**
   Returns a permutation of all of the bin indices within the specified
   kinematic space.

   This method is a helper method for performing integration within
   multidimensional  kinematic variables, because VALORArray can only
   integrate over entire reconstructed or true space.

   \param binning The kinematic binning for which the permutation will be
      applied.
   \param ks The kinematic space for which bin indices are to be permuted.
   \return The permutation of all bins in the kinematic space.
*/
std::vector<std::vector<int>> BinPermutation(
      const KBCubHcmbEdge2Edge& binning, const conv::KineSpace_t ks);

/**
   Returns a permutation of all of the bin indices within the specified
   kinematic space.

   This method is a helper method for performing integration within
   multidimensional  kinematic variables, because VALORArray can only
   integrate over entire reconstructed or true space.

   \param binning The kinematic binning for which the permutation will be
      applied.
   \param kvs The kinematic variables for which bin indices are to be permuted.
   \return The permutation of all bins in the kinematic space.
*/
std::vector<std::vector<int>> BinPermutation(
      const KBCubHcmbEdge2Edge& binning,
      const std::vector<conv::KineVar_t>& kvs);
//________________________________________________________________________________________
TH1D* Make1DHistogram(const VALORArray& v,
      const SingleSampleKineBinning& b, conv::KineSpace_t ks,
      bool use_simple, conv::ReacMode_t m)
{
  const bool input_arr_is_1D = v.GetNDim()==1;
  if (util::NDim(ks)!=1) {
    LOG("UtlBinning",pWARN) << "Requested kinespace "<<util::AsString(ks)<<" is not 1D";
    return nullptr;
  }
  const conv::KineVar_t target_kv = util::KineVar(ks)[0];
  const bool targetIsReco = util::IsPureReco(ks);
  
  if (!targetIsReco && m==conv::kmUndefined) {
    LOG("UtlBinning",pWARN) << "Cannot contract to truth without a specified reaction mode.";
    return nullptr;
  }
  // Get the binning and associated kinematic variables for the VALORArray
  const VALORBinning * tmpb = nullptr;
  const KBCubHcmbEdge2Edge * origBinning = nullptr;
  
  if (targetIsReco) {
    tmpb = use_simple ? b.RecoSmpBinning() : b.RecoOptBinning();
  }
  else {
    tmpb = b.TrueSmpBinning(m);
  }
  origBinning = dynamic_cast<const KBCubHcmbEdge2Edge*>(tmpb);
  if (!origBinning) {
    LOG("UtlBinning", pWARN) << tmpb->GetName()<<" is not a KBCubHcmbEdge2Edge, so will skip this";
    return nullptr;
  }
  
  const std::vector<conv::KineVar_t>& myKv = origBinning->KineVars();
  
  if(std::find(myKv.begin(), myKv.end(), target_kv) == myKv.end()) { 
    // Mismatch in variables
    LOG("UtlBinning", pERROR) << "Kinematic variable " <<
       util::AsString(target_kv) << " not found in template kinematic space " <<
       util::AsString(origBinning->KineSpace());
    util::Die(-1);
  }
  
  // If necessary, contract the binning to 1D
  const VALORArray* targetV = nullptr;
  const unsigned int oldNDim = origBinning->NDimensions();
  const KBCubHcmbEdge2Edge * newBinning = nullptr;
  if (oldNDim > 1) {
     newBinning = dynamic_cast<KBCubHcmbEdge2Edge*>(origBinning->Contract(ks));
     assert(newBinning);
    // Need to integrate out the reco variables here, before integrating over
    // truth
    const KBCubHcmbEdge2Edge * recoBinning = dynamic_cast<const KBCubHcmbEdge2Edge*>(b.RecoSmpBinning() );
    const KBCubHcmbEdge2Edge * trueBinning = dynamic_cast<const KBCubHcmbEdge2Edge*>(b.TrueSmpBinning(m));
    targetV = Contract(v, *newBinning,*recoBinning,*trueBinning);
  }
  else {
    newBinning = origBinning;
    targetV = &v;
  }

  // Integrate over the unnecessary variables
  // get rid of the unnecessary truth or reco dimension
  VALORArray new_arr(input_arr_is_1D?v:v.GetSlice(targetIsReco?1:0));
  assert(new_arr.GetNDim()==1);
  assert(new_arr.GetSize() == (int)newBinning->NBins());
  double * events = new_arr.GetArray();

  // Map the events to a ROOT compatible histogram
  TH1D* histo = Remap1D(events, *newBinning);
  
  double arr_int = v.Integral();
  double histo_int = histo->Integral();
  double diff = std::abs(arr_int - histo_int);
  if (std::abs(arr_int)>conv::kASmallNum && diff/arr_int > conv::kASmallNum) {
    LOG("UtlBinning",pFATAL) << "Histogram contents don't match VALORArray contents ("
       <<v.GetName()<<" "<<util::AsString(ks)<<")\n"
       << "Array = "<<v.Integral()<< " ; Histogram = "<<histo->Integral()
       << "; Ratio = "<<histo->Integral() / v.Integral()
       << "; Difference = "<<diff;
    util::Die(-1);
  }
  
  if (targetV != &v) { delete targetV; }
  if (newBinning != origBinning) {
    delete const_cast<KBCubHcmbEdge2Edge*>(newBinning);
  }

  return histo;
}
//________________________________________________________________________________________
TH2D* Make2DOptimisedHistogram(const VALORArray& v,
      const SingleSampleKineBinning& b, const std::vector<conv::KineVar_t>& ks)
{
  // Get the reco binning and associated kinematic variables for the VALORArray
  const KBCubHcmbMondrian * recoBinning = dynamic_cast<const KBCubHcmbMondrian*>(b.RecoOptBinning());
  if (!recoBinning) {
    LOG("UtlBinning", pERROR) << b.RecoOptBinning()->GetName()<<" is not a KBCubHcmbMondrian";
     util::Die(-1);
  }
  const std::vector<conv::KineVar_t>& recoKv = recoBinning->KineVars();

  // Check that the request kinematic variables match those for the VALORArray
  // (This should use the contraction method in the future)
  if(ks.size() != recoKv.size())
  {
     LOG("UtlBinning", pERROR) << "Expected " << recoKv.size() <<
        " kinematic variables to plot, found " << ks.size();
     util::Die(-1);
  }
  for(auto kv : ks)
  {
     if(std::find(recoKv.begin(), recoKv.end(), kv) == recoKv.end())
     {  // Mismatch in variables
        LOG("UtlBinning", pERROR) << "Kinematic variable " <<
           util::AsString(kv) << " not found in template kinematic space " <<
           util::AsString(recoBinning->KineSpace());
        util::Die(-1);
     }
  }
  // Integrate over the truth variables
  unsigned int nbins = recoBinning->NBins();
  double events[nbins];
  for(unsigned int i = 0; i < nbins; ++i)
  {
     // Reco is always first dimension in VALORArray, so fix the bins in this
     // dimension and integrate over the rest
     events[i] = v.Integral(i, i);
  }
  
  // Map the events to a ROOT compatible histogram
  TH2D* histo = RemapOptimised2D(events, *recoBinning);
  
  if (std::fabs(v.Integral() - histo->Integral()) > conv::kASmallNum) {
    LOG("UtlBinning",pFATAL) << "Histogram contents don't match VALORArray contents ("<<v.GetName()<<")";
    util::Die(-1);
  }

  return histo;
}
//________________________________________________________________________________________
TH2D* Make2DSimpleHistogram(const VALORArray& v,
      const SingleSampleKineBinning& b, const std::vector<conv::KineVar_t>& ks)
{
  // Get the reco binning and associated kinematic variables for the VALORArray
  const KBCubHcmbEdge2Edge * recoBinning = dynamic_cast<const KBCubHcmbEdge2Edge*>(b.RecoSmpBinning());
  if (!recoBinning) {
    LOG("UtlBinning", pERROR) << b.RecoSmpBinning()->GetName()<<" is not a KBCubHcmbEdge2Edge";
    util::Die(-1);
  }
  const std::vector<conv::KineVar_t>& recoKv = recoBinning->KineVars();

  // Check that the request kinematic variables match those for the VALORArray
  // (This should use the contraction method in the future)
  if(ks.size() != recoKv.size())
  {
     LOG("UtlBinning", pERROR) << "Expected " << recoKv.size() <<
        " kinematic variables to plot, found " << ks.size();
     util::Die(-1);
  }
  for(auto kv : ks)
  {
     if(std::find(recoKv.begin(), recoKv.end(), kv) == recoKv.end())
     {  // Mismatch in variables
        LOG("UtlBinning", pERROR) << "Kinematic variable " <<
           util::AsString(kv) << " not found in template kinematic space " <<
           util::AsString(recoBinning->KineSpace());
        util::Die(-1);
     }
  }
  // Integrate over the truth variables
  unsigned int nbins = recoBinning->NBins();
  double events[nbins];
  for(unsigned int i = 0; i < nbins; ++i)
  {
     // Reco is always first dimension in VALORArray, so fix the bins in this
     // dimension and integrate over the rest
     events[i] = v.Integral(i, i);
  }
  
  // Map the events to a ROOT compatible histogram
  TH2D* histo = RemapSimple2D(events, *recoBinning);
  
  if (std::fabs(v.Integral() - histo->Integral()) > conv::kASmallNum) {
    LOG("UtlBinning",pFATAL) << "Histogram contents don't match VALORArray contents ("<<v.GetName()<<")";
    util::Die(-1);
  }

  return histo;
}

VALORArray* Contract(const VALORArray& v,
    const KBCubHcmbEdge2Edge & targetBinning,
    const KBCubHcmbEdge2Edge & currentRecoBinning,
    const KBCubHcmbEdge2Edge & currentTrueBinning)
{
  if (v.GetNDim() > 2) {
    LOG("UtlBinning", pFATAL) << v.GetName()<<" has "<<v.GetNDim()<<" dimensions - expected either (reco) or (reco,true)";
    util::Die(-1); return nullptr;
  }
  else {
    // Check reco * true size
    if ((unsigned int)v.GetSize(0) != currentRecoBinning.NBins()) {
      LOG("UtlBinning", pFATAL) << v.GetName()<<" has "<<v.GetSize(0)<<" reco dimensions - expecting "<<currentRecoBinning.NBins();
      util::Die(-1); return nullptr;
    }
    if ((unsigned int)v.GetSize(1) != currentTrueBinning.NBins()) {
      LOG("UtlBinning", pFATAL) << v.GetName()<<" has "<<v.GetSize(1)<<" true dimensions - expecting "<<currentTrueBinning.NBins();
      util::Die(-1); return nullptr;
    }
  }
  
  // Are we targeting a reco or true variable?
  const vector<conv::KineVar_t> & target_kvs = targetBinning.KineVars();
  bool targetIsPureReco = true;
  bool targetIsPureTrue = true;
  // Record which dimension in the old binning corresponds to this entry in the new binning
  std::vector<unsigned int> kinevar_indices;
  for (conv::KineVar_t kv : target_kvs) {
    bool isTrue = util::IsTrue(kv);
    if (isTrue) {
      targetIsPureReco = false;
      if (!currentRecoBinning.Contains(kv)) {
        LOG("UtlBinning",pFATAL) << currentRecoBinning.GetName() << " does not contain " << util::AsString(kv);
        util::Die(-1); return nullptr;
      }
    }
    else {
      targetIsPureTrue = false;
      if (!currentTrueBinning.Contains(kv)) {
        LOG("UtlBinning",pFATAL) << currentTrueBinning.GetName() << " does not contain " << util::AsString(kv);
        util::Die(-1); return nullptr;
      }
    }
    kinevar_indices.push_back(currentRecoBinning.Dimension(kv));
  }
  
  if (!(targetIsPureTrue || targetIsPureReco)) {
    LOG("UtilBinning",pFATAL) << "Target binning must be pure true or reco.";
    util::Die(-1); return nullptr;
  }
  
  size_t n_old_true_bins   = currentTrueBinning.NBins();
  size_t n_old_reco_bins   = currentRecoBinning.NBins();

  VALORArray* targetV = nullptr;
  if (targetIsPureReco) {
    targetV = new VALORArray(targetBinning.NBins(), currentTrueBinning.NBins());
    
    for (size_t old_reco_bin = 0 ; old_reco_bin < n_old_reco_bins ; old_reco_bin++) {
      // Identify bin multidimensional bin centre
      std::vector<double> old_bin_centre = currentRecoBinning.BinCentre(old_reco_bin);
      // Reorder to get new bin centre
      std::vector<double> new_bin_centre;
      for (unsigned int kvidx : kinevar_indices) {
        new_bin_centre.push_back(old_bin_centre[kvidx]);
      }
      unsigned int new_reco_bin = targetBinning.FindBin(new_bin_centre);
      // Loop over true bins and fill
      for (size_t true_bin = 0 ; true_bin < n_old_true_bins ; true_bin++) {
        double bin_content = v.GetEntry(old_reco_bin,true_bin);
        targetV->AddToEntry(bin_content,new_reco_bin,true_bin);
      }
    }
  }
  else {
    targetV = new VALORArray(currentRecoBinning.NBins(), targetBinning.NBins());
    
    for (size_t old_true_bin = 0 ; old_true_bin < n_old_true_bins ; old_true_bin++) {
      // Identify bin multidimensional bin centre
      std::vector<double> old_bin_centre = currentTrueBinning.BinCentre(old_true_bin);
      // Reorder to get new bin centre
      std::vector<double> new_bin_centre;
      for (unsigned int kvidx : kinevar_indices) {
        new_bin_centre.push_back(old_bin_centre[kvidx]);
      }
      unsigned int new_true_bin = targetBinning.FindBin(new_bin_centre);
      // Loop over reco bins and fill
      for (size_t reco_bin = 0 ; reco_bin < n_old_reco_bins ; reco_bin++) {
        double bin_content = v.GetEntry(reco_bin,old_true_bin);
        targetV->AddToEntry(bin_content,reco_bin,new_true_bin);
      }
    }
  }
  
  if (std::fabs(v.Integral() - targetV->Integral()) > conv::kASmallNum) {
    LOG("UtlBinning",pFATAL) << "Contracted contents don't match input array contents ("<<v.GetName()<<")";
    LOG("UtlBinning",pFATAL) << "Old = "<<v.Integral()<< " ; New = "<<targetV->Integral();
    util::Die(-1);
  }
    
  return targetV;
}

TH1D* Remap1D(const double* events, const KBCubHcmbEdge2Edge& b)
{
  // Find all bin edges
  std::set<double> dimEdges;
  for(unsigned int i = 0; i < b.NBins(); ++i)
  {
     // Minimum edge of first dimension for bin i
     dimEdges.insert(b.BinMinimum(i, 0));
     // Maximum edge of first dimension for bin i
     dimEdges.insert(b.BinMaximum(i, 0));
  }
  // No need to sort or remove duplicate edges as both properties are
  // guaranteed by set
  // Create the Root histogram
  int nDimBins = dimEdges.size() - 1;
  double dimBinEdges[nDimBins + 1];
  std::copy(dimEdges.begin(), dimEdges.end(), dimBinEdges);
  TH1D* histo = new TH1D("histo", "histo", nDimBins, dimBinEdges);

  // Get the number of bins from our ROOT histogram
  int xbins = histo->GetNbinsX();
  std::vector<double> coords = {0.0};
  for(int i = 1; i <= xbins; ++i)
  {
     coords[0] = histo->GetXaxis()->GetBinCenter(i);
     int index = b.FindBin(coords);
     histo->SetBinContent(i, events[index]);
  }

  return histo;
}

TH2D* RemapOptimised2D(const double* events,
      const KBCubHcmbMondrian& b)
{
  // Find all bin edges in each dimension
  std::set<double> dim1Edges;
  std::set<double> dim2Edges;
  for(unsigned int i = 0; i < b.NBins(); ++i)
  {
     // Minimum edge of first dimension for bin i
     dim1Edges.insert(b.BinMinimum(i, 0));
     // Maximum edge of first dimension for bin i
     dim1Edges.insert(b.BinMaximum(i, 0));
     // Minimum edge of second dimension for bin i
     dim2Edges.insert(b.BinMinimum(i, 1));
     // Maximum edge of second dimension for bin i
     dim2Edges.insert(b.BinMaximum(i, 1));
  }
  // No need to sort or remove duplicate edges as both properties are
  // guaranteed by set
  // Create the Root histogram
  int nDim1Bins = dim1Edges.size() - 1;
  double dim1BinEdges[nDim1Bins + 1];
  std::copy(dim1Edges.begin(), dim1Edges.end(), dim1BinEdges);
  int nDim2Bins = dim2Edges.size() - 1;
  double dim2BinEdges[nDim2Bins + 1];
  std::copy(dim2Edges.begin(), dim2Edges.end(), dim2BinEdges);
  TH2D* histo = new TH2D("histo", "histo", nDim1Bins, dim1BinEdges,
        nDim2Bins, dim2BinEdges);

  // Get the number of bins from our ROOT histogram
  int xbins = histo->GetNbinsX();
  int ybins = histo->GetNbinsY();
  std::vector<double> coords = {0.0, 0.0};
  for(int i = 1; i <= xbins; ++i)
  {
     coords[0] = histo->GetXaxis()->GetBinCenter(i);
     for(int j = 1; j <= ybins; ++j)
     {
        coords[1] = histo->GetYaxis()->GetBinCenter(j);
        int index = b.FindBin(coords);
        histo->SetBinContent(i, j, events[index]);
     }
  }

  return histo;
}

TH2D* RemapSimple2D(const double* events,
      const KBCubHcmbEdge2Edge& b)
{
  // Find all bin edges in each dimension
  std::set<double> dim1Edges;
  std::set<double> dim2Edges;
  for(unsigned int i = 0; i < b.NBins(); ++i)
  {
     // Minimum edge of first dimension for bin i
     dim1Edges.insert(b.BinMinimum(i, 0));
     // Maximum edge of first dimension for bin i
     dim1Edges.insert(b.BinMaximum(i, 0));
     // Minimum edge of second dimension for bin i
     dim2Edges.insert(b.BinMinimum(i, 1));
     // Maximum edge of second dimension for bin i
     dim2Edges.insert(b.BinMaximum(i, 1));
  }
  // No need to sort or remove duplicate edges as both properties are
  // guaranteed by set
  // Create the Root histogram
  int nDim1Bins = dim1Edges.size() - 1;
  double dim1BinEdges[nDim1Bins + 1];
  std::copy(dim1Edges.begin(), dim1Edges.end(), dim1BinEdges);
  int nDim2Bins = dim2Edges.size() - 1;
  double dim2BinEdges[nDim2Bins + 1];
  std::copy(dim2Edges.begin(), dim2Edges.end(), dim2BinEdges);
  TH2D* histo = new TH2D("histo", "histo", nDim1Bins, dim1BinEdges,
        nDim2Bins, dim2BinEdges);

  // Get the number of bins from our ROOT histogram
  int xbins = histo->GetNbinsX();
  int ybins = histo->GetNbinsY();
  std::vector<double> coords = {0.0, 0.0};
  for(int i = 1; i <= xbins; ++i)
  {
     coords[0] = histo->GetXaxis()->GetBinCenter(i);
     for(int j = 1; j <= ybins; ++j)
     {
        coords[1] = histo->GetYaxis()->GetBinCenter(j);
        int index = b.FindBin(coords);
        histo->SetBinContent(i, j, events[index]);
     }
  }

  return histo;
}

std::vector<std::vector<int>> BinPermutation(
      const KBCubHcmbEdge2Edge& binning, const conv::KineSpace_t ks)
{
  auto kvs = util::KineVar(ks);
  return BinPermutation(binning, kvs);
}

std::vector<std::vector<int>> BinPermutation(
      const KBCubHcmbEdge2Edge& binning,
      const std::vector<conv::KineVar_t>& kvs)
{
  std::vector<std::vector<int>> permutation;

  std::vector<int> binCounts;
  std::vector<int> repCounts;

  // Store the number of bins associatedwith each kinematic variable
  for(auto kv : kvs)
  {
     binCounts.push_back(binning.NBins(kv));
  }
  // Define a function to compute a running product
  auto lambda = [&](int product, int x) mutable { return product * x; };
  // Compute the product of the bin counts to determine the number of permutations
  auto count = std::accumulate(binCounts.begin(), binCounts.end(), 1, lambda);

  // Compute the number of repetitions of each bin index for the permutation
  for(unsigned int i = 0; i < kvs.size(); ++i)
  {
     repCounts.push_back(std::accumulate(binCounts.begin() + (i + 1),
              binCounts.end(), 1, lambda));
  }

  // Initialise the permutation vector
  for(int i = 0; i < count; ++i)
  {
     permutation.push_back(std::vector<int>());
  }

  // Fill the permutations vector
  for(unsigned int i = 0; i < kvs.size(); ++i)
  {
     int nreps = repCounts[i];
     int nbins = binCounts[i];
     int bin = 0;
     for(int j = 0; j < count; ++j)
     {
        // Each bin of each kinematic variable is repeated according to the
        // repetition count before moving to the next value
        permutation[j].push_back(bin);
        if(((j + 1) % nreps) == 0)
        {
           ++bin;
        }
        // Cycle back to the first bin of the kinematic variable once each bin
        // has been added to the permutation
        if(bin == nbins)
        {
           bin = 0;
        }
     }
  }

  return permutation;
}

}
}
}

