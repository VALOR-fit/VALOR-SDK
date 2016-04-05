//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#define __show_init_mesg_info__  
//~ #define __init_check_inverse__
//~ #define __show_init_mesg_debug__   
//~ #define __show_cholesky_mesg_info__
//~ #define __show_cholesky_mesg_debug__
//~ #define __show_tweak_mesg_debug__
//~ #define __show_update_correlated_mesg_debug__
//~ #define __show_chisqpenalty_mesg_debug__
//~ #define __show_chisqpenalty_mesg_debug2__

// system
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iomanip> 
#include <limits> 
// root
#include <TRandom3.h>
#include <TMath.h>
// fmwk
#include "Messenger/Messenger.h"
#include "Conventions/Build.h"
#include "Conventions/Constants.h"
#include "Utilities/NameRegistry.h"
#include "Utilities/UtlXML.h"
#include "Utilities/UtlNumStat.h"
#include "Utilities/UtlSystem.h"
#include "Utilities/UtlPrint.h"
#include "FitCore/FitParamList.h"
#include "FitCore/PhysParamList.h"
#include "FitCore/CorrelatedSystematics.h"

using std::cout;
using std::cerr;
using std::endl;

using namespace valor;
using namespace valor::conv;
using namespace valor::util;

#ifndef NSIGMA_LIMIT
#define NSIGMA_LIMIT 5
#endif

ClassImp(CorrelatedSystematics)

//________________________________________________________________________________________
CorrelatedSystematics::CorrelatedSystematics(unsigned int n_syst, const PhysParamId_t * syst_params, const string & xml_node_name):
//~ TObject(),
fUntunedValues    (NULL),
fInitialValues    (NULL),
fCurrentValues    (NULL),
fCovMtrx          (NULL),
fCovMtrxInv       (NULL),
fCovMtrxChoL      (NULL),
fSystParameters   (NULL),
fIsConfigured     (false)
{
  this->Init(n_syst,syst_params,xml_node_name);
}
//________________________________________________________________________________________
CorrelatedSystematics::CorrelatedSystematics() :
//~ TObject()
fUntunedValues    (NULL),
fInitialValues    (NULL),
fCurrentValues    (NULL),
fCovMtrx          (NULL),
fCovMtrxInv       (NULL),
fCovMtrxChoL      (NULL),
fSystParameters   (NULL),
fIsConfigured     (false)
{
  this->Init(0,nullptr);
}
//________________________________________________________________________________________
CorrelatedSystematics::~CorrelatedSystematics()
{
  this->CleanUp();
}
//________________________________________________________________________________________
void CorrelatedSystematics::Init(unsigned int n_syst, const PhysParamId_t * syst_params, const string & xml_node_name)
{
  this->CleanUp();
  
  fNSystParameters = n_syst;
  fSystParameters = new PhysParamId_t[fNSystParameters];
  for (unsigned int i = 0 ; i < n_syst ; i++) {
    fSystParameters[i] = syst_params[i];
  }
  
  fUntunedValues        = new TVectorD(fNSystParameters);
  fInitialValues        = new TVectorD(fNSystParameters);
  fCurrentValues        = new TVectorD(fNSystParameters);
  fCovMtrx              = 0;
  fCovMtrxInv           = 0;
  fCovMtrxChoL          = 0; 
  fIsTweaked            = false;
  
  fUntunedArray = fUntunedValues->GetMatrixArray();
  fInitialArray = fInitialValues->GetMatrixArray();
  fCurrentArray = fCurrentValues->GetMatrixArray();
  
  for (int ip = 0 ; ip < conv::knppmax; ip++) {
    fParam2Idx[ip] = -1;
  }
  
  for(int i = 0 ; i < fCurrentValues->GetNrows(); i++ ) {
    fUntunedArray[i] = 1.;
    fInitialArray[i] = 1.;
    fCurrentArray[i] = 1.;
  }
  
  this->SetName("Systematics");
  fXMLNodeName = xml_node_name;
  
  bool failure = false;
  // And do a check for duplicate or undefined parameters.
  for (unsigned int ii = 0 ; ii < n_syst ; ii++) {
    if (fSystParameters[ii] == conv::kPhypUndefined) {
      failure = true;
      LOG("CorrelatedSystematics",pFATAL) << "Parameter "<<ii<<" is undefined.";
      continue;
    }
    for (unsigned int jj = 0 ; jj < ii ; jj++) {
      if (fSystParameters[ii] == fSystParameters[jj]) {
        LOG("CorrelatedSystematics",pFATAL) << "Parameter "<<fSystParameters[ii]<<" is duplicated - appearing as both entry "<<ii<<" and "<<jj<<".";
        failure = true;
        break;
      }
    }
  }
  if (failure) {
    util::Die(-1);
  }
  return;
}
//________________________________________________________________________________________
void CorrelatedSystematics::FillParam2Idx(void)
{
  for (int ip = 0 ; ip < conv::knppmax; ip++) {
    fParam2Idx[ip] = -1;
    PhysParamId_t p = (PhysParamId_t)ip;
    for (unsigned int i = 0 ; i < fNSystParameters; i++) {
      if (p == fSystParameters[i]) {
        fParam2Idx[ip] = p;
        break;
      }
    }
  }
}
//________________________________________________________________________________________
void CorrelatedSystematics::CleanUp(void)
{
  if ( fUntunedValues ) delete fUntunedValues;
  if ( fInitialValues ) delete fInitialValues;
  if ( fCurrentValues ) delete fCurrentValues;
  if ( fCovMtrx       ) delete fCovMtrx;
  if ( fCovMtrxInv    ) delete fCovMtrxInv;
  if ( fCovMtrxChoL   ) delete fCovMtrxChoL;
  
  if ( fSystParameters) delete[] fSystParameters;
  
  fIsConfigured = false;
}
//________________________________________________________________________________________
bool CorrelatedSystematics::Read(TXMLEngine* xml, XMLNodePointer_t root_node)
{
  if (fIsConfigured) {
    PhysParamId_t tmp_par[fNSystParameters];
    for (unsigned int i = 0 ; i < fNSystParameters ; i++) {
      tmp_par[i] = fSystParameters[i];
    }
    this->CleanUp();
    this->Init(fNSystParameters,tmp_par);
  }
  
  // If specified, get the untuned parameter values. Otherwise make a vector with all values 1.
  TString untuned_values_path = TString::Format("syst/%s/untuned_values",fXMLNodeName.c_str());
  if (util::NodeExists(xml,root_node,untuned_values_path.Data())) {
    TVectorD * tmp_vec = util::XML2TVectorD(xml,root_node,untuned_values_path.Data());
    if(!tmp_vec) {
      LOG("CorrelatedSystematics",pFATAL) << "Failed to read "<<this->GetName()<<" untuned values vector from "<<untuned_values_path;
      util::Die(1); return false;
    }
    if (tmp_vec->GetNrows()!=fUntunedValues->GetNrows()) {
      LOG("CorrelatedSystematics",pFATAL) << this->GetName()<<" untuned values vector has wrong size "<<tmp_vec->GetNrows()<<" - expecting "<<fUntunedValues->GetNrows();
      util::Die(1); return false;
    }
    (*fUntunedValues) = (*tmp_vec);
    delete tmp_vec;
  }
  else {
    LOG("CorrelatedSystematics",pINFO) << "Initalising "<<this->GetName()<<" untuned values vector to one.";
    for (int i = 0 ; i < fUntunedValues->GetNrows(); i++) {
      (*fUntunedValues)[i] = 1.;
    }
  }
  
  // If specified, get the initial parameter values. Otherwise make a vector with all values 1.
  TString initial_values_path = TString::Format("syst/%s/initial_values",fXMLNodeName.c_str());
  if (util::NodeExists(xml,root_node,initial_values_path.Data())) {
    TVectorD * tmp_vec = util::XML2TVectorD(xml,root_node,initial_values_path.Data());
    if(!tmp_vec) {
      LOG("CorrelatedSystematics",pFATAL) << "Failed to read "<<this->GetName()<<" initial values vector from "<<initial_values_path;
      util::Die(1); return false;
    }
    if (tmp_vec->GetNrows()!=fInitialValues->GetNrows()) {
      LOG("CorrelatedSystematics",pFATAL) << this->GetName()<<" initial values vector has wrong size "<<tmp_vec->GetNrows()<<" - expecting "<<fInitialValues->GetNrows();
      util::Die(1); return false;
    }
    (*fInitialValues) = (*tmp_vec);
    delete tmp_vec;
  }
  else {
    LOG("CorrelatedSystematics",pINFO) << "Initalising "<<this->GetName()<<" initial values vector to one.";
    for (int i = 0 ; i < fInitialValues->GetNrows(); i++) {
      (*fInitialValues)[i] = 1.;
    }
  }
  
  // Get syst covariance matrix
  TString cov_mtrx_path = TString::Format("syst/%s/covariance_matrix",fXMLNodeName.c_str());
  fCovMtrx = util::XML2TMatrixD(xml,root_node,cov_mtrx_path.Data());  
  if(!fCovMtrx) {
    LOG("CorrelatedSystematics",pFATAL) << "No "<<this->GetName()<<" covariance matrix was found at "<<cov_mtrx_path;
    util::Die(1); return false;
  }
  
  // Check matrix size
  TMatrixD & cov = *fCovMtrx;
  const int np = fNSystParameters;  
  if(cov.GetNrows() != np || cov.GetNcols() != np) 
  {
     LOG("CorrelatedSystematics",pFATAL)
        << "\n Syst covariance matrix for "<<this->GetName()<<" has unexpected size!"
        << "\n Expecting: " <<  np << " x " << np << " matrix"
        << "\n Found: " << cov.GetNrows() << " x " <<  cov.GetNcols() << " matrix";
     util::Die(1); return false;
  }
  
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
  
  // Other matrix inversion methods - not used.
#ifdef __matrix_invert_direct__
  // Calculate matrix inversion directly
  fCovMtrxInv = new TMatrixD(cov);
  fCovMtrxInv->Invert(&det);
  LOG("CorrelatedSystematics",pINFO) << "Syst covariance matrix determinant (direct): " << det;
#elif __matrix_invert_svd__
  // Use scalar value decomposition for matrix inverse
  fCovMtrxInv = new TMatrixD(util::MatrixPseudoInverted(cov));
  LOG("CorrelatedSystematics",pINFO) << "Syst covariance matrix determinant (SVD): " << det;
#endif
  
  // Say we're configured
  fIsConfigured = true;

#ifdef __init_check_inverse__
  TMatrixD covTimesInverse = (cov * (*fCovMtrxInv));
  for (int ii = 0 ; ii < np ; ii++) {
    for (int jj = 0 ; jj < np ; jj++) {
      int target = (ii==jj);
      if (fabs(covTimesInverse[ii][jj]-target) > 1E-9) {
        LOG("CorrelatedSystematics",pERROR) 
          << "Bad inversion of syst matrix - element: "
          << ii << " , " << jj <<": " << covTimesInverse[ii][jj]-target << (ii==jj?"*":"");
        fIsConfigured = false;
      }
    }
  }
  if (!fIsConfigured) {util::Die(1); return fIsConfigured;}
#endif // __init_check_inverse__
  
#ifdef __show_init_mesg_debug__
  LOG("CorrelatedSystematics",pDEBUG) << "Input syst covariance matrix: "; 
  util::PrintMatrix(cov,7,5); // would be nice to use the same stream and threshold as mesg above
  LOG("CorrelatedSystematics",pDEBUG) << "Inverse syst covariance matrix: ";
  util::PrintMatrix(*fCovMtrxInv,7,5); // would be nice to use the same stream and threshold as mesg above
#endif
  
  return fIsConfigured;
}
//________________________________________________________________________________________
double CorrelatedSystematics::CurrentValue(PhysParamId_t p) const
{
  int idx = this->Param2Idx(p); 
  return fCurrentArray[idx];
}
//________________________________________________________________________________________
double CorrelatedSystematics::CurrentValueByIndex(int idx) const
{
  return fCurrentArray[idx];
}
//________________________________________________________________________________________
double CorrelatedSystematics::InitialValue(PhysParamId_t p) const
{
  int idx = this->Param2Idx(p); 
  return fInitialArray[idx];
}
//________________________________________________________________________________________
double CorrelatedSystematics::InitialValueByIndex(int idx) const
{
  return fInitialArray[idx];
}
//________________________________________________________________________________________
double CorrelatedSystematics::UntunedValue(PhysParamId_t p) const
{
  int idx = this->Param2Idx(p); 
  return fUntunedArray[idx];
}
//________________________________________________________________________________________
double CorrelatedSystematics::UntunedValueByIndex(int idx) const
{
  return fUntunedArray[idx];
}
//________________________________________________________________________________________
void CorrelatedSystematics::SetCurrentValue(PhysParamId_t p, double value)
{
  int idx = this->Param2Idx(p); 
  fCurrentArray[idx] = value;
}
//________________________________________________________________________________________
void CorrelatedSystematics::SetCurrentValueByIndex(int idx, double value)
{
  fCurrentArray[idx] = value;
}
//________________________________________________________________________________________
double CorrelatedSystematics::OneSigmaError(PhysParamId_t p) const
{
  const TMatrixD & cov = *fCovMtrx;
  int idx = this->Param2Idx(p); 
  return TMath::Sqrt(cov[idx][idx]);
}
//________________________________________________________________________________________
double CorrelatedSystematics::OneSigmaErrorByIndex(int idx) const
{
  const TMatrixD & cov = *fCovMtrx;
  return TMath::Sqrt(cov[idx][idx]);
}
//________________________________________________________________________________________
void CorrelatedSystematics::Tweak(
  bool       force_inputs, // force the input params or generate random ones internally  
  TVectorD * input_vec   , // input vector of syst params - Set to 0 to reset to nominal ones 
  Option_t * opt
)
{
  
#ifdef __show_tweak_mesg_debug__
  LOG("CorrelatedSystematics",pDEBUG)  << "Tweaking syst parameters";
#endif

  // Switch flags
  fIsTweaked         = true;
  
  bool reset_to_untuned = (opt && TString(opt).Contains("untuned"));
  if (reset_to_untuned) {
    (*fCurrentValues) = (*fUntunedValues);
    return;
  }

  //
  // Get best-fit and current syst params
  //
  
  TVectorD & initial_values = *fInitialValues;
  TVectorD & current_values = *fCurrentValues;

  //
  // Force the input parameters
  //
  if(force_inputs) 
  {
     // Input vector is NULL.
     // This resets the input params to their best-fit values.
     if(input_vec==0) {
#ifdef __show_tweak_mesg_debug__
        LOG("CorrelatedSystematics",pDEBUG) << "Reset input params to best-fit values.";
#endif
        fIsTweaked = false;
        current_values = initial_values;
        return;
     }
     else     
     {
#ifdef __show_tweak_mesg_debug__
        LOG("CorrelatedSystematics",pDEBUG) << "Forcing input syst parameters. ";
#endif
        const TVectorD & vec = *input_vec;
        assert(vec.GetNoElements() == (int)fNSystParameters);
        current_values = vec;
     }
  }//force_inputs?

  //
  // If the tweaking dials were not forced, generate correlated tweaking dials
  // using the Cholesky method
  //
  else
  {
#ifdef __show_tweak_mesg_debug__
     LOG("CorrelatedSystematics",pDEBUG) << "Generating random twk dials (correlated)! ";
#endif
     const TMatrixD & L = *fCovMtrxChoL;
     TVectorD vec = util::CholeskyGenerateCorrelatedParams(L,initial_values);
     current_values = vec;
  }
  return;
}
//________________________________________________________________________________________
void CorrelatedSystematics::CholeskyDecomposeMatrix(void)
{
#ifdef __show_cholesky_mesg_info__  
    LOG("CorrelatedSystematics",pDEBUG) 
      << "Running Cholesky's decomposition method on the syst covariance matrix.";
#endif

#ifdef __show_cholesky_mesg_debug__
    LOG("CorrelatedSystematics",pDEBUG) 
       << "Syst covariance matrix: "; 
    util::PrintMatrix(this->CovMatrix(),7,5); // would be nice to use the same stream and threshold as mesg above
#endif

    TMatrixD L = util::CholeskyDecomposition(this->CovMatrix());
    fCovMtrxChoL = new TMatrixD(L);
#ifdef __show_cholesky_mesg_debug__
    LOG("CorrelatedSystematics",pDEBUG) 
       << "Cholesky decomposition of syst covariance matrix: ";
    util::PrintMatrix(*fCovMtrxChoL,7,5); // would be nice to use the same stream and threshold as mesg above
#endif
}
//________________________________________________________________________________________
double CorrelatedSystematics::ChisqPenalty(bool /*all_syst*/) const
{
  //covariance matrix
  const TMatrixD & cov    (this->CovMatrix()   );
  const TMatrixD & covInv (this->CovMatrixInv());

  //Vector with deviations of current values from  best fit values
  const int np = cov.GetNrows();
  TMatrixD dev  (1,np); //vector of one column in matrix form for multiplication
  TMatrixD devT (np,1); //vector of one row in matrix form for multiplication
  for(int i = 0; i < np; i++){
    dev [0][i] = fCurrentArray[i]-fInitialArray[i];
    devT[i][0] = fCurrentArray[i]-fInitialArray[i];
  }
  
#ifdef __show_chisqpenalty_mesg_debug__
  LOG("CorrelatedSystematics",pDEBUG) << "Deviations in syst parameters: ";
  util::PrintVectorPair(*fCurrentValues,*fInitialValues,fSystParameters,7,5); // would be nice to use the same stream and threshold as mesg above
#endif

#ifdef __show_chisqpenalty_mesg_debug2__
    LOG("CorrelatedSystematics",pDEBUG) << "Syst covariance matrix: ";
    util::PrintMatrix(cov,7,5); // would be nice to use the same stream and threshold as mesg above
    LOG("CorrelatedSystematics",pDEBUG) << "Inverse syst covariance matrix: ";
    util::PrintMatrix(covInv,7,5); // would be nice to use the same stream and threshold as mesg above
#endif

    TMatrixD chisqpenalty =  dev * covInv * devT;
    double chisq_penalty = chisqpenalty[0][0]; //The final matrix is 1x1, just a number

#ifdef __show_chisqpenalty_mesg_debug__
    LOG("CorrelatedSystematics",pDEBUG) << "ChisqPenalty = " << chisq_penalty;
#endif

    if(chisq_penalty) return chisq_penalty;
    //}//all_syst
  return 0;
}
//________________________________________________________________________________________ 
void CorrelatedSystematics::PrintSystematicVariations(std::ostream & stream) const
{
  for (int i = 0 ; i < fCurrentValues->GetNoElements(); i++) {
    PhysParamId_t p = fSystParameters[i];
    stream << NameRegistry::Instance()->AsString(p) << "\t" << (*fCurrentValues)[i] << endl;
  }
}
//________________________________________________________________________________________
int CorrelatedSystematics::Param2Idx(PhysParamId_t p) const
{
  assert(p!=kPhypUndefined);
  return fParam2Idx[p];  
}
//________________________________________________________________________________________
conv::PhysParamId_t CorrelatedSystematics::Idx2Param (int index) const
{
  assert(index >= 0 && index <= (int)fNSystParameters);
  return fSystParameters[index];
}
//________________________________________________________________________________________
void CorrelatedSystematics::UpdateFrom(const valor::FitParamList  & fpl)
{
  for (int i = 0 ; i < fInitialValues->GetNoElements() ; i++) {
    PhysParamId_t p = fSystParameters[i];
    if (fpl.Exists(p)) {
      fCurrentArray[i] = fpl.CurrValue(p);
    }
    else {
      LOG("CorrelatedSystematics",pWARN) 
        << "Unable to update syst parameter " << NameRegistry::Instance()->AsString(p) << " from FPL.";
    }
  }
  fIsTweaked = true;
  return;
}
//________________________________________________________________________________________
void CorrelatedSystematics::UpdateFrom(const valor::PhysParamList  & ppl)
{
  for (int i = 0 ; i < fInitialValues->GetNoElements() ; i++) {
    PhysParamId_t p = fSystParameters[i];
    if (ppl.Contains(p)) {
      fCurrentArray[i] = ppl.Value(p);
    }
    else {
      LOG("CorrelatedSystematics",pWARN) 
        << "Unable to update syst parameter " << NameRegistry::Instance()->AsString(p) << " from PPL.";
    }
  }
  fIsTweaked = true;
  return;
}
//________________________________________________________________________________________
void CorrelatedSystematics::CopyTweaksTo(valor::PhysParamList & ppl) const
{
  for (int i = 0 ; i < fCurrentValues->GetNoElements() ; i++) {
    PhysParamId_t p = fSystParameters[i];
    ppl.SetValue(p,fCurrentArray[i]);
  }
  return;
}
//________________________________________________________________________________________
void CorrelatedSystematics::CopyTweaksTo(valor::FitParamList & fpl) const
{
  for (int i = 0 ; i < fInitialValues->GetNoElements() ; i++) {
    PhysParamId_t p = fSystParameters[i];
    if (fpl.Exists(p)) {
      fpl.SetCurrentValue(p,fCurrentArray[i]);
    }
    else {
      LOG("CorrelatedSystematics",pWARN) 
        << "FPL does not contain parameter " << NameRegistry::Instance()->AsString(p);
    }
  }
  return;
}
//________________________________________________________________________________________
void CorrelatedSystematics::CopyOneSigmaErrorsTo(valor::PhysParamList & ppl) const
{
  for (int i = 0 ; i < fCurrentValues->GetNoElements() ; i++) {
    PhysParamId_t p = fSystParameters[i];
    ppl.SetValue(p,this->OneSigmaError(p));
  }
  return;
}
//________________________________________________________________________________________
void CorrelatedSystematics::SetXMLNodeName(std::string xml_node_name)
{
  fXMLNodeName = xml_node_name;
}
//________________________________________________________________________________________
std::string CorrelatedSystematics::GetXMLNodeName(void) const
{
  return fXMLNodeName;
}
//________________________________________________________________________________________
unsigned int CorrelatedSystematics::Size(void) const
{
  return fNSystParameters;
}
//________________________________________________________________________________________
conv::PhysParamId_t CorrelatedSystematics::WhichParam(int idx) const
{
  return fSystParameters[idx];
}
//________________________________________________________________________________________
bool CorrelatedSystematics::AppendParamsTo(valor::FitParamList & fpl) const
{
  for (int i = 0 ; i < fInitialValues->GetNoElements() ; i++) {
    PhysParamId_t p = fSystParameters[i];
    double one_sig = this->OneSigmaError(p);
    double central = fInitialArray[i];
    double pmin    = central - NSIGMA_LIMIT*one_sig;
    double pmax    = central + NSIGMA_LIMIT*one_sig;
    double step    = 0.1 * one_sig;
    fpl.AddParam(-999,p,central,pmin,pmax,step,false,true);
  }
  return true;
}
//________________________________________________________________________________________
