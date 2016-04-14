//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

// system
#include <cassert>
#include <cstdlib>
#include <iostream>
// fmwk
#include "Conventions/Constants.h"
#include "Messenger/Messenger.h"
#include "Utilities/UtlNumStat.h"
#include "Utilities/UtlSystem.h"

#include "FitCore/CompoundCorrelatedSystematics.h"

using namespace valor;
using namespace valor::conv;
using namespace valor::util;

using namespace std;

ClassImp(CompoundCorrelatedSystematics)

//________________________________________________________________________________________
CompoundCorrelatedSystematics::CompoundCorrelatedSystematics(const vector<const CorrelatedSystematics*> & vec, string xml_node_name)
: CorrelatedSystematics()
{
  
  this->CompoundInit(vec.size(),vec.data(),xml_node_name);
  return;
}
//________________________________________________________________________________________
CompoundCorrelatedSystematics::CompoundCorrelatedSystematics(unsigned int n_cov_matrices, const CorrelatedSystematics * const * syst_objs, string xml_node_name)
: CorrelatedSystematics()
{
  this->CompoundInit(n_cov_matrices,syst_objs,xml_node_name);
  return;
}
//________________________________________________________________________________________
void CompoundCorrelatedSystematics::CompoundInit(unsigned int n_cov_matrices, const CorrelatedSystematics * const * syst_objs, string xml_node_name)
{
  // Count how many total elements we have
  LOG("CompoundCorrelatedSystematics",pDEBUG) << "Initialising a CompoundCorrelatedSystematics from "<<n_cov_matrices<<" smaller matrices";
  unsigned int total_elements = 0;
  for (unsigned int icov = 0 ; icov < n_cov_matrices ; icov++) {
    const CorrelatedSystematics * corr = syst_objs[icov];
    if (!corr) {
      LOG("CompoundCorrelatedSystematics",pFATAL) << "Failed to read CorrelatedSystematics object "<<icov;
      util::Die(-1);return;
    }
    if (!(corr->IsConfigured())) {
      LOG("CompoundCorrelatedSystematics",pFATAL) << "CorrelatedSystematics object is not configured properly - "<<corr->GetName()<<"("<<icov<<")";
      util::Die(-1);return;
    }
    total_elements += corr->Size();
  } // icov
  
  LOG("CompoundCorrelatedSystematics",pDEBUG) << "New CompoundCorrelatedSystematics contains "<<total_elements<<" parameters.";
  
  std::vector<const TMatrixD *> all_cov_matrices;
  std::vector<const TVectorD *> all_untuned_vectors;
  std::vector<const TVectorD *> all_initial_vectors;
  std::vector<const TVectorD *> all_current_vectors;
  
  // Get list of systematics and vectors/matrices
  PhysParamId_t * params = new PhysParamId_t[total_elements];
  int iparam = 0;
  for (unsigned int icov = 0 ; icov < n_cov_matrices ; icov++) {
    const CorrelatedSystematics * corr = syst_objs[icov];
    for (unsigned int ip = 0 ; ip < corr->Size(); ip++) {
      params[iparam++] = corr->WhichParam(ip);
    }
    all_cov_matrices   .push_back(&(corr->CovMatrix    ()));
    all_untuned_vectors.push_back(&(corr->UntunedValues()));
    all_initial_vectors.push_back(&(corr->InitialValues()));
    all_current_vectors.push_back(&(corr->CurrentValues()));
  }
  
  // Build this object's supermatrix.
  this->Init(total_elements, params, xml_node_name);
  
  // Copy vectors and matrices
  fCovMtrx = new TMatrixD(util::ConcatenateTMatrices(all_cov_matrices));
  (*fUntunedValues) = util::ConcatenateTVectors(all_untuned_vectors);
  (*fInitialValues) = util::ConcatenateTVectors(all_initial_vectors);
  (*fCurrentValues) = util::ConcatenateTVectors(all_current_vectors);
  
  
  // Do Cholesky decomposition
  LOG("CorrelatedSystematics",pNOTICE) << "Running Cholesky decomposition of syst covariance matrix ";
  this->CholeskyDecomposeMatrix();
  double det;
  
  // Calculate matrix inversion using Cholesky triangular matrices - should be most accurate.
  TMatrixD cholInv(*fCovMtrxChoL);
  cholInv.Invert(&det);
  LOG("CorrelatedSystematics",pINFO) << "Syst covariance matrix decomposition determinant: " << det;
  TMatrixD cholInvT(TMatrixD::kTransposed,cholInv);
  fCovMtrxInv = new TMatrixD(cholInvT * cholInv);
  
  // Set the names
  fXMLNodeName = xml_node_name;
  fName = "CompoundSyst";
  
  // And we're configured
  fIsConfigured = true;
  
  return;
}
//________________________________________________________________________________________
CompoundCorrelatedSystematics::~CompoundCorrelatedSystematics()
{
  
}
//________________________________________________________________________________________
