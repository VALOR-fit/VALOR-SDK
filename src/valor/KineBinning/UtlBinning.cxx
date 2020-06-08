//________________________________________________________________________________________
/*

   Copyright (c) 2010-2020, The VALOR group. All rights reserved

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
    const KBCubHcmbEdge2Edge * targetBinning,
    const KBCubHcmbEdge2Edge * currentRecoBinning,
    const KBCubHcmbEdge2Edge * currentTrueBinning);

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

   \param events The spectrum corresponding to the optimised binning.
   \param b The binning.
   \return A ROOT-compatible 2D histogram representing the simple binning.
*/
TH2D* Remap2D(const double* events, const VALORBinning* b);

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

  unsigned int oldNDim = 0;
  if (targetIsReco) {
    tmpb = use_simple ? b.RecoSmpBinning() : b.RecoOptBinning();

    const KBCubHcmbEdge2Edge * tmpb_reco = dynamic_cast<const KBCubHcmbEdge2Edge*>(tmpb);
    oldNDim += tmpb_reco->NDimensions();

    const KBCubHcmbEdge2Edge * tmpb_true = dynamic_cast<const KBCubHcmbEdge2Edge*>(b.TrueSmpBinning(m));
    if(tmpb_true) { oldNDim += tmpb_true->NDimensions(); }
  }
  else {
    tmpb = b.TrueSmpBinning(m);

    const KBCubHcmbEdge2Edge * tmpb_true = dynamic_cast<const KBCubHcmbEdge2Edge*>(tmpb);
    oldNDim += tmpb_true->NDimensions();

    const KBCubHcmbEdge2Edge * tmpb_reco = dynamic_cast<const KBCubHcmbEdge2Edge*>(use_simple ? b.RecoSmpBinning() : b.RecoOptBinning());
    if(tmpb_reco) { oldNDim += tmpb_reco->NDimensions(); }
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
  const KBCubHcmbEdge2Edge * newBinning = nullptr;
  if (oldNDim > 1) {
     newBinning = dynamic_cast<KBCubHcmbEdge2Edge*>(origBinning->Contract(ks));
     assert(newBinning);
    // Need to integrate out the reco variables here, before integrating over
    // truth
    const KBCubHcmbEdge2Edge * recoBinning = dynamic_cast<const KBCubHcmbEdge2Edge*>(b.RecoSmpBinning() );
    const KBCubHcmbEdge2Edge * trueBinning = dynamic_cast<const KBCubHcmbEdge2Edge*>(b.TrueSmpBinning(m));
    targetV = Contract(v, newBinning,recoBinning,trueBinning);
  }
  else {
    newBinning = origBinning;
    targetV = &v;
  }

  if(targetV->GetNDim() != 1) {
    LOG("UtlBinning",pFATAL) << "After contracting [" << v.GetName() << "] to kine space [" << util::AsString(ks)
                             << "], the target array had " << targetV->GetNDim() << " dimensions. Expected 1.";
    util::Die(-1);
  }

  if(targetV->GetSize() != (int)newBinning->NBins()) {
    LOG("UtlBinning",pFATAL) << "After contracting [" << v.GetName() << "] to kine space [" << util::AsString(ks)
                             << "], the target array had " << targetV->GetSize() << " bins. Expected "
                             << (int)newBinning->NBins();
    util::Die(-1);
  }

  const double * events = targetV->GetArray();

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
TH2D* Make2DHistogram(const VALORArray& v,
      const VALORBinning* recoBinning, const std::vector<conv::KineVar_t>& ks)
{
  if (!recoBinning) {
    LOG("UtlBinning", pERROR) << "The input VALORBinning was invalid.";
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
  TH2D* histo = Remap2D(events, recoBinning);
  
  if (std::fabs(v.Integral() - histo->Integral()) > conv::kASmallNum) {
    LOG("UtlBinning",pFATAL) << "Histogram contents don't match VALORArray contents ("<<v.GetName()<<")";
    util::Die(-1);
  }

  return histo;
}

VALORArray* Contract(const VALORArray& v,
    const KBCubHcmbEdge2Edge * targetBinning,
    const KBCubHcmbEdge2Edge * currentRecoBinning,
    const KBCubHcmbEdge2Edge * currentTrueBinning)
{

  if(!targetBinning) {
    LOG("UtlBinning", pFATAL) << "Input target binning was null";
    util::Die(-1); return nullptr;
  }

  bool has_reco = ( currentRecoBinning != nullptr ) ;
  bool has_true = ( currentTrueBinning != nullptr ) ;

//  // ASSUMPTION: The VALORArray has either one or two dimensions, where all reco binning is condensed
//  //             into one dimension and all true is condensed into the other.
//  if(has_reco && has_true && v.GetNDim() != 2) {
//    LOG("UtlBinning", pFATAL) << "Both reco and true binnings were input, but the input VALORArray ["
//                              << v.GetName()<<"] has "<<v.GetNDim()<<" dimensions - expected two, i.e. (reco,true)";
//    util::Die(-1); return nullptr;
//  }

  if (v.GetNDim() > 2) {
    LOG("UtlBinning", pFATAL) << v.GetName()<<" has "<<v.GetNDim()<<" dimensions - expected either (reco) or (reco,true)";
    util::Die(-1); return nullptr;
  }
  else {
    // Check reco and/or true size
    bool reco_matches = false;
    bool true_matches = false;

    if(v.GetNDim() <= 2) {
      bool this_dim_is_assigned = false;
      unsigned int v_size = (unsigned int)v.GetSize(0);
      if(v_size == currentRecoBinning->NBins()) {
        reco_matches = true;
        this_dim_is_assigned = true;
      }
      if(!this_dim_is_assigned && v_size == currentTrueBinning->NBins()) {
        // Check !this_dim_is_assigned because each binning must match a unique dimension
        true_matches = true;
        this_dim_is_assigned = true;
      }
    }

    if(v.GetNDim() == 2) {
      bool this_dim_is_assigned = false;
      unsigned int v_size = (unsigned int)v.GetSize(1);
      if(!reco_matches && v_size == currentRecoBinning->NBins()) {
        // Now also check !reco_matches, again because each binning must match a unique dimension
        reco_matches = true;
        this_dim_is_assigned = true;
      }
      if(!true_matches && !this_dim_is_assigned && v_size == currentTrueBinning->NBins()) {
        true_matches = true;
        this_dim_is_assigned = true;
      }
    }

    unsigned int n_matches = 0;
    if(reco_matches) { n_matches++; }
    if(true_matches) { n_matches++; }

    if(n_matches != (unsigned int)v.GetNDim()) {
      if(v.GetNDim() == 1) {
        LOG("UtlBinning", pFATAL) << v.GetName() << " has " << v.GetSize(0) << " dimensions - expecting "
                                  << currentRecoBinning->NBins() << " (from reco) or "
                                  << currentTrueBinning->NBins() << " (from true)";
      }
      else {
        LOG("UtlBinning", pFATAL) << v.GetName() << " has (" << v.GetSize(0) << ", "
                                  << v.GetSize(1) << ") dimensions - expecting "
                                  << currentRecoBinning->NBins() << " (from reco) and "
                                  << currentTrueBinning->NBins() << " (from true)";
      }
      util::Die(-1);
      return nullptr;
    }
  }
  
  // Are we targeting a reco or true variable?
  const vector<conv::KineVar_t> & target_kvs = targetBinning->KineVars();
  bool targetIsPureReco = true;
  bool targetIsPureTrue = true;
  // Record which dimension in the old binning corresponds to this entry in the new binning
  std::vector<unsigned int> kinevar_indices;
  for (conv::KineVar_t kv : target_kvs) {
    bool isTrue = util::IsTrue(kv);
    if (isTrue) {
      targetIsPureReco = false;
      if(!has_true) {
        LOG("UtlBinning",pFATAL) << "Tried to contract to a new binning containing true kine vars, but the original true binning does not exist.";
        util::Die(-1); return nullptr;
      }
      if (!currentTrueBinning->Contains(kv)) {
        LOG("UtlBinning",pFATAL) << currentTrueBinning->GetName() << " does not contain " << util::AsString(kv);
        util::Die(-1); return nullptr;
      }
      kinevar_indices.push_back(currentTrueBinning->Dimension(kv));
    }
    else {
      targetIsPureTrue = false;
      if(!has_reco) {
        LOG("UtlBinning",pFATAL) << "Tried to contract to a new binning containing reco kine vars, but the original reco binning does not exist.";
        util::Die(-1); return nullptr;
      }
      if (!currentRecoBinning->Contains(kv)) {
        LOG("UtlBinning",pFATAL) << currentRecoBinning->GetName() << " does not contain " << util::AsString(kv);
        util::Die(-1); return nullptr;
      }
      kinevar_indices.push_back(currentRecoBinning->Dimension(kv));
    }
  }
  
  if (!(targetIsPureTrue || targetIsPureReco)) {
    LOG("UtilBinning",pFATAL) << "Target binning must be pure true or reco.";
    util::Die(-1); return nullptr;
  }
  
  size_t n_old_true_bins   = (has_true ? currentTrueBinning->NBins() : 1);
  size_t n_old_reco_bins   = (has_reco ? currentRecoBinning->NBins() : 1);

  VALORArray* targetV = new VALORArray(targetBinning->NBins());
  if (targetIsPureReco) {
    // Integrate both over all true bins (if they exist), and any reco kine vars that exist in the old binning
    // but not the target binning
    for (size_t old_reco_bin = 0 ; old_reco_bin < n_old_reco_bins ; old_reco_bin++) {
      // Identify multidimensional bin centre
      std::vector<double> old_bin_centre = currentRecoBinning->BinCentre(old_reco_bin);

      // Reorder to get new bin centre
      std::vector<double> new_bin_centre;
      for (unsigned int kvidx : kinevar_indices) {
        new_bin_centre.push_back(old_bin_centre[kvidx]);
      }
      unsigned int new_reco_bin = targetBinning->FindBin(new_bin_centre);

      // Loop over true bins (if they exist, or a single dummy true bin if they don't) and fill
      for (size_t old_true_bin = 0 ; old_true_bin < n_old_true_bins ; old_true_bin++) {
        double bin_content = 0;
        if(has_true) { bin_content = v.GetEntry(old_reco_bin, old_true_bin); }
        else         { bin_content = v.GetEntry(old_reco_bin); }
        targetV->AddToEntry(bin_content, new_reco_bin);
      } // old_true_bin
    } // old_reco_bin
  }
  else {
    // Integrate both over all reco bins (if they exist), and any true kine vars that exist in the old binning
    // but not the target binning
    for (size_t old_true_bin = 0 ; old_true_bin < n_old_true_bins ; old_true_bin++) {
      // Identify multidimensional bin centre
      std::vector<double> old_bin_centre = currentTrueBinning->BinCentre(old_true_bin);

      // Reorder to get new bin centre
      std::vector<double> new_bin_centre;
      for (unsigned int kvidx : kinevar_indices) {
        new_bin_centre.push_back(old_bin_centre[kvidx]);
      }
      unsigned int new_true_bin = targetBinning->FindBin(new_bin_centre);

      // Loop over reco bins (if they exist, or a single dummy true bin if they don't) and fill
      for (size_t old_reco_bin = 0 ; old_reco_bin < n_old_reco_bins ; old_reco_bin++) {
        double bin_content = 0;
        if(has_reco) { bin_content = v.GetEntry(old_reco_bin, old_true_bin); }
        else         { bin_content = v.GetEntry(old_true_bin); }
        targetV->AddToEntry(bin_content, new_true_bin);
      } // old_reco_bin
    } // old_true_bin
  } // is pure reco?
  
  if (std::fabs(v.Integral() / targetV->Integral() - 1.0 ) > conv::kASmallNum) {
    LOG("UtlBinning",pFATAL) << "Contracted contents don't match input array contents ("<<v.GetName()<<")";
    LOG("UtlBinning",pFATAL) << std::fixed << std::setprecision(11)
                             << "Old = "<<v.Integral()<< " ; New = "<<targetV->Integral()
                             <<" ; Contracted kinespace = "<< util::AsString(util::KineSpaceFromKineVars(target_kvs));
    LOG("UtlBinning",pFATAL) << "\n\ncurrentRecoBinning:"; currentRecoBinning->Print(std::cout);
    LOG("UtlBinning",pFATAL) << "\n\ncurrentTrueBinning:"; currentTrueBinning->Print(std::cout);
    LOG("UtlBinning",pFATAL) << "\n\ntargetBinning:"; targetBinning->Print(std::cout);
    LOG("UtlBinning",pFATAL) << "\n\nold VArray dims:"; v.PrintDimensions(std::cout);
    LOG("UtlBinning",pFATAL) << "\n\nold VArray content:"; v.PrintEntries(std::cout << std::fixed << std::setprecision(11));
    LOG("UtlBinning",pFATAL) << "\n\nnew VArray dims:"; targetV->PrintDimensions(std::cout);
    LOG("UtlBinning",pFATAL) << "\n\nnew VArray content:"; targetV->PrintEntries(std::cout << std::fixed << std::setprecision(11));
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

TH2D* Remap2D(const double* events, const VALORBinning* b)
{
  // Find all bin edges in each dimension
  std::set<double> dim1Edges;
  std::set<double> dim2Edges;
  for(unsigned int i = 0; i < b->NBins(); ++i)
  {
     // Minimum edge of first dimension for bin i
     dim1Edges.insert(b->BinMinimum(i, 0));
     // Maximum edge of first dimension for bin i
     dim1Edges.insert(b->BinMaximum(i, 0));
     // Minimum edge of second dimension for bin i
     dim2Edges.insert(b->BinMinimum(i, 1));
     // Maximum edge of second dimension for bin i
     dim2Edges.insert(b->BinMaximum(i, 1));
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
        int index = b->FindBin(coords);
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

