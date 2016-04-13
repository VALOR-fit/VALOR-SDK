//____________________________________________________________________________ 
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

*/ 
//____________________________________________________________________________

#include <cassert> 
#include <cstdlib> 
#include <vector> 

#include <TFile.h>
#include <TFitter.h> 
#include <TMath.h> 
#include <TMatrixDSym.h>
#include <TObjArray.h> 
#include <TPRegexp.h> 
#include <TRandom3.h> 
#include <TStopwatch.h>
#include <TString.h> 
#include <TSystem.h>
#include <TTimeStamp.h>
#include <TVirtualFitter.h> 

#include "Conventions/Constants.h"
#include "Conventions/Opt.h"
#include "Utilities/Rnd.h"
#include "Messenger/Messenger.h"
#include "Fitters/VMinuitFitter.h" 
#include "FitCore/FitInputs.h" 
#include "FitCore/FitParamList.h" 

using namespace valor; 
using namespace valor::conv;

using std::vector;
   
//____________________________________________________________________________ 
VMinuitFitter::VMinuitFitter() :
FitterABC_new()
{
  this->Init();
}
//____________________________________________________________________________ 
VMinuitFitter::~VMinuitFitter() 
{

}
//____________________________________________________________________________ 
void VMinuitFitter::Init(void) 
{
  fNFreeFitParams      = 0;
  fNFitBins            = 0;
  fCalcHesseErrors     = false;
  fCalcMinosErrors     = false;
//fSimplex             = false;
  fTol                 = 0.1; // 0.01;
  fStr                 = 1;
  fPrintLevel          = -1; // 3;
  fMaxNumCalls         = 100000;
  fMigradMinStatusCode = -999;
  fMigradErrStatusCode = -999;      
  fHesseErrStatusCode  = -999; 
  fMinosErrStatusCode  = -999; 
  fFitTime             = 0;
  fFitTries            = 0;
}
//____________________________________________________________________________ 
bool VMinuitFitter::Configure(const Registry & conf)
{
  LOG("Minuit", pINFO) << "Fitter configuration: " << conf;

/*
  fCalcHesseErrors =
  fCalcMinosErrors =
  fTol             = 
  fStr             =
  fPrintLevel      =
  fMaxNumCalls     =
*/
  //
  //
  // use this and deprecate the various Set(...) methods and some or all 
  // of the info passed via Option_t in the ctor
  //
  //

  return true;
}
//____________________________________________________________________________ 
bool VMinuitFitter::DoTheFit(FitInputs * inputs) 
{
  // Start clock
  TStopwatch w;
  w.Start();

  // Get fit parameters
  FitParamList & fit_params = inputs->FitParams();
  int param_list_size = fit_params.Size();

  // Calculate degrees of freedom  
  int nbins = inputs->Observation().NumOfBins();
  int nparams_free = fit_params.GetNumberFreeParameters();
  int ndof = nbins - nparams_free;

  assert(nbins > 0);
  assert(nparams_free >= 0);
//assert(ndof > 0);

  LOG("Minuit", pINFO) 
    << "NDoF = " << ndof
    << " (Number of bins = " << nbins
    << ", Number of free parameters = " << nparams_free << ")";
  
  // Get MINUIT
  TVirtualFitter::SetDefaultFitter("Minuit");
//TVirtualFitter::SetDefaultFitter("Simplex");
  TVirtualFitter * fitter = TVirtualFitter::Fitter(0,param_list_size);

  // Set print level
  Double_t arglist[5];
  arglist[0] = fPrintLevel;
  fitter->ExecuteCommand("SET PRINT",arglist,1);
  
  // Set params
  fit_params.SetTo(fitter);

  // Set minimization function
  fitter->SetFCN(FitFunc);         
   
  //-----------
  // reset fit time, bestfit statistic and status codes in
  // persistable FitParamList obj
  fit_params.SetFitTime(0);
  fit_params.SetBestFitStatistic(0);
  fit_params.ClearStatusCodes();
  //-----------

  // Run the fitter once - we have the option here to check the status code
  // and if the fitter failed, re-seed the fitter and try again
  bool ok = this->RunOnce(false);

  // 4: migrad not converged
  int ntries = 1;

  if(fMigradMinStatusCode == 4) {
    LOG("Minuit", pWARN) << "Fit failed...";
    if( fOption.Contains("with_simplex_fallback") ) 
    {
       LOG("Minuit", pWARN) << "Trying SIMPLEX";
       this->RunOnce(true);
       ntries++;
    }
  }  

  // stop clock
  w.Stop();
  double tfit = w.CpuTime();

  LOG("Minuit", pINFO) 
    << "Fit performed in " << tfit
    << " sec after " << ntries << " attempts";
  LOG("Minuit", pINFO) 
    << "Best-fit statistic: " << fit_params.GetBestFitStatistic();

  // -------------
  // copy fit time and bestfit statistic and status codes 
  // to persistable FitParamList obj
  fit_params.SetFitTime (tfit);
  fit_params.SetFitTries(ntries);
  fit_params.SetBestFitStatistic(fit_params.GetBestFitStatistic());
  fit_params.SetStatusCode("migrad_min", fMigradMinStatusCode);
  fit_params.SetStatusCode("migrad_err", fMigradErrStatusCode);
  if(fCalcHesseErrors) {
    fit_params.SetStatusCode("hesse_err", fHesseErrStatusCode);
  }
  if(fCalcMinosErrors) {
    fit_params.SetStatusCode("minos_err", fMinosErrStatusCode);
  }
  // -------------

  return ok;
}
//____________________________________________________________________________ 
bool VMinuitFitter::RunOnce(bool use_simplex) 
{
  fHesseErrStatusCode = -1;
  double fcntemp;

  // Get the fitter
  TVirtualFitter * fitter = TVirtualFitter::GetFitter();

  // Set strategy
  Double_t arg[5];
  arg[0] = fStr;
  fitter->ExecuteCommand("SET STRategy",arg,1);    

  // Pass fit inputs (data, pdf) to MINUIT
  LOG("Minuit", pINFO) << "Passing fit inputs (data & pdf)...";
  gMinuit->SetObjectFit(fFitInputs);

  //
  fitter->ExecuteCommand("SHO EPS", arg, 1);///

  // Run MINUIT's minimization step
  LOG("Minuit", pINFO) << "Running " << ((use_simplex) ? "SIMPLEX..." : "MIGRAD... ") << TTimeStamp().AsString();
  Double_t arglist[100];
  arglist[0] = fMaxNumCalls; 
  arglist[1] = fTol;  

  if(!use_simplex) {
    fMigradMinStatusCode = fitter->ExecuteCommand("MIGRAD",arglist, 2);    
  }//!_use_simples
  //SIMPLEX
  else {
    arglist[1] = 0.1; // higher tolerance
    fMigradMinStatusCode = fitter->ExecuteCommand("SIMPLEX",arglist, 2);
  }
  
  //return on not converged
  if(fMigradMinStatusCode == 4 && !use_simplex)
    return false;
  else if(fMigradMinStatusCode == 4 && use_simplex) {
    //copy current to best fit, to stop horrible crashes
    fFitInputs->FitParams().CopyCurrToBestFitValues();
    return false;
  }
  
  int num_of_params = fFitInputs->FitParams().Size();
  
  // Found best fit! Copy current --> best-fit values
  for(int ip = 0; ip < num_of_params; ip++) {
    double value, error;
    gMinuit->GetParameter(ip, value, error);
    PhysParamId_t p = fFitInputs->FitParams().WhichParam(ip);
    fFitInputs->FitParams().Update(p, value);
  }
  fFitInputs->FitParams().CopyCurrToBestFitValues();

  // Print fit result
  LOG("Minuit", pINFO) << "Printing MIGRAD fit results...";
  fitter->ExecuteCommand("SHOW FCNVALUE",&fcntemp,0);
  Double_t aminn = 0;
  fitter->PrintResults(3,aminn);

  // Get error matrix
  TMatrixDSym cov(num_of_params);
  for(int i=0; i<num_of_params; i++) {
    for(int j=0; j<num_of_params; j++) {
        cov[i][j] = fitter->GetCovarianceMatrixElement(i,j);
    }
  }
  fFitInputs->FitParams().SetCovMatrix(cov);

  //
  double unfilled = -999;

  // Error vectors
  vector<double> migrad_err     (num_of_params, unfilled);
  vector<double> hesse_err      (num_of_params, unfilled);
  vector<double> minos_err_minus(num_of_params, unfilled);
  vector<double> minos_err_plus (num_of_params, unfilled);
  
  // Fill the MIGRAD errors
  for (int ip = 0 ; ip < num_of_params ; ip++) {
    migrad_err[ip] = fitter->GetParError(ip);
  }
  fFitInputs->FitParams().SetMigradErrors(migrad_err);

  // Get the error matrix status
  double ha(0.0);     // dummy vars
  double edm, errdef; // dummy vars
  int nvpar, nparx;   // dummy vars
  int errMatrixStatus = fitter->GetStats(ha, edm, errdef, nvpar, nparx);
  fMigradErrStatusCode = errMatrixStatus;

  // Calculate HESSE or MINOS errors
  if(fCalcHesseErrors) {
    LOG("Minuit", pINFO) << "Running HESSE...";
    fitter->ExecuteCommand("HESSE",arglist, 1);
    
    errMatrixStatus = fitter->GetStats(ha, edm, errdef, nvpar, nparx);
    
    fHesseErrStatusCode = errMatrixStatus;
    
    // Fill the HESSE errors
    for (int ip = 0 ; ip < num_of_params ; ip++) {
      hesse_err[ip] = fitter->GetParError(ip);
    }
    fFitInputs->FitParams().SetHesseErrors (hesse_err);
  }
  
  //else 
  if(fCalcMinosErrors) {
    LOG("Minuit", pINFO) << "Running MINOS...";
    fMinosErrStatusCode = fitter->ExecuteCommand("MINOS",arglist, 1);
  }
  // Get error matrix status
  LOG("Minuit", pINFO) << "Getting error matrix status...";
  errMatrixStatus = fitter->GetStats(ha, edm, errdef, nvpar, nparx);

  // Get errors
  LOG("Minuit", pINFO) << "Getting errors...";

  if(fCalcMinosErrors) {
    for(int ip = 0; ip < num_of_params; ip++) {
      double glob_corr, parerr; // dummy vars
        fitter->GetErrors(ip, minos_err_plus[ip], minos_err_minus[ip],parerr,glob_corr);
    }
    fFitInputs->FitParams().SetMinosErrors (minos_err_plus, minos_err_minus);
  }
  
  // Print fit result
  if(fCalcHesseErrors) {
#ifdef __show_fit_result__
#ifdef __show_fit_steps__
    LOG("Minuit", pINFO) << "Printing HESSE fit results...";
#endif
    Double_t amin = 0;
    fitter->PrintResults(3,amin);
  //fitter->PrintResults(1,amin); // prints limits
  }
  LOG("Minuit", pINFO) << "Final error matrix status = " << errMatrixStatus; 

  LOG("Minuit", pINFO) << "Done";

  return true;//(fMigradErrStatusCode == 3);
}
//____________________________________________________________________________ 
int VMinuitFitter::FitStatusCode (Option_t * option) 
{ 
  if(!option) return 9999;

  TString opt(option);

  if ( opt.Contains("migrad_min") ) return fMigradMinStatusCode;
  if ( opt.Contains("migrad_err") ) return fMigradErrStatusCode;
  if ( opt.Contains("hesse_err")  ) return fHesseErrStatusCode;
  if ( opt.Contains("minos_err")  ) return fMinosErrStatusCode;
  
  return -999;
}
//____________________________________________________________________________ 
