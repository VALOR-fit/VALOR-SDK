//____________________________________________________________________________ 
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/ 
//____________________________________________________________________________

#define __show_fit_result__ 
#define __show_fit_steps__ 

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

#include "valor/Conventions/Constants.h"
#include "valor/Conventions/Opt.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Messenger/Messenger.h"
#include "valor/FitCore/CompoundFitModel.h" 
#include "valor/FitCore/FitterMinuit.h" 
#include "valor/FitCore/FitInputs.h" 
#include "valor/FitCore/FitParamList.h" 
#include "valor/FitCore/FitDataABC.h"
#include "valor/Utilities/UtlSystem.h"

using namespace valor; 
using namespace valor::conv;

using std::vector;
   
//____________________________________________________________________________ 
FitterMinuit::FitterMinuit() :
FitterABC()
{
  this->Init();
}
//____________________________________________________________________________ 
FitterMinuit::FitterMinuit(FitInputs * fit_inputs, Option_t * option) :
FitterABC()
{
  this->Init(fit_inputs, option);
}
//____________________________________________________________________________ 
FitterMinuit::~FitterMinuit() 
{

}
//____________________________________________________________________________ 
void FitterMinuit::Init(void) 
{
  fFitInputs           = 0;
  fOption              = "";
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
void FitterMinuit::Init(FitInputs * fit_inputs, Option_t * option) 
{
  LOG("Minuit", pINFO) << "Fitter initialization...";

  this->Init();

  fFitInputs = fit_inputs;
  if(!fFitInputs) {
     LOG("Minuit", pFATAL) << "** Null fit inputs!";
     exit(1);
  }

  FitParamList & fit_params = fFitInputs->FitParams();
  int param_list_size = fit_params.Size();

  LOG("Minuit", pINFO) << "Fit parameter list size = " << param_list_size;

  fOption = option;

  // calc HESSE or MINOS errors after fit
  fCalcHesseErrors  = fOption.Contains("hesse");
  fCalcMinosErrors  = fOption.Contains("minos");

  if(fOption.Contains("minuit")) {
    // get fitter
    TVirtualFitter::SetDefaultFitter("Minuit");
    TVirtualFitter * fitter = TVirtualFitter::Fitter(0,param_list_size);
    
    // set print level
    Double_t arglist[5];
    arglist[0] = fPrintLevel;
    fitter->ExecuteCommand("SET PRINT",arglist,1);
    
    // set params
    fit_params.SetTo(fitter);
    
    // set minimization function
    fitter->SetFCN(FitFunc);            
  }
  
  // Parse a tolerance out of the fitter option
  if(fOption.Contains("tolerance",TString::kIgnoreCase)) {
    TPRegexp tol_re("(?<=tolerance\\=)-?[\\d.]+(?:e-?\\d+)?");
    /*               |Lookahead name |Generic number pattern| */
    TObjArray * matches = tol_re.MatchS(fOption,"i"); // i is case insensitive
    double tolerance = 0;
    if(matches->GetEntries()==1) {
      TObjString * tolerance_string = dynamic_cast<TObjString*>(matches->At(0));
      if (tolerance_string) {
        tolerance = strtod(tolerance_string->GetString().Data(),NULL);
      }
    }
      
    delete matches;
    
    if (tolerance > conv::kAVerySmallNum) {
      LOG("Minuit", pINFO) << "Set tolerance to "<<tolerance;
      this->SetTol(tolerance);
    }
    else {
      LOG("Minuit",pWARN) << "Failed to parse valid single tolerance from option string "<<fOption;
    }
  }
  
  // Parse a max number of calls out of the fitter option
  if(fOption.Contains("maxcalls",TString::kIgnoreCase)) {
    TPRegexp call_re("(?<=maxcalls\\=)-?[\\d.]+(?:e-?\\d+)?");
    /*               |Lookahead name |Generic number pattern| */
    TObjArray * matches = call_re.MatchS(fOption,"i"); // i is case insensitive
    double maxcalls = 0;
    if(matches->GetEntries()==1) {
      TObjString * maxcalls_string = dynamic_cast<TObjString*>(matches->At(0));
      if (maxcalls_string) {
        maxcalls = strtod(maxcalls_string->GetString().Data(),NULL);
      }
    }
      
    delete matches;
    
    if (maxcalls > conv::kAVerySmallNum) {
      int ncalls = TMath::Nint(maxcalls);
      LOG("Minuit", pINFO) << "Set max number of calls to "<<ncalls;
      this->SetMaxCalls(ncalls);
    }
    else {
      LOG("Minuit",pWARN) << "Failed to parse valid single max number of calls from option string "<<fOption;
    }
  }

/*
  if(fOption.Contains("simplex")) {
    TVirtualFitter::SetDefaultFitter("Simplex");
    TVirtualFitter * fitter = TVirtualFitter::Fitter(0,param_list_size);
    
    // set print level              
    Double_t arglist[5];
    arglist[0] = fPrintLevel;
    fitter->ExecuteCommand("SET PRINT",arglist,1);
    
    // set params                
    fit_params.SetTo(fitter);
    
    // set minimization function     
    fitter->SetFCN(FitFunc);
    fSimplex = true;
  }
*/

  // calculate degrees of freedom  
  fNFitBins       = fit_inputs->Observation().NumOfBins();
  fNFreeFitParams = fit_params.GetNumberFreeParameters();
  if(fNFitBins <= 0) {
      LOG("Minuit", pFATAL) << "Number of fit bins  = " << fNFitBins;
      util::Die(-1);
      return;
  }
  assert(fNFitBins > 0);
  assert(fNFreeFitParams >= 0);

  LOG("Minuit", pINFO) 
       << "NDoF = " << this->NDOF()
       << " (Number of bins = " << this->NBinsInFit()
       << ", Number of free parameters = " << this->NFitParams() << ")";
  
}
//____________________________________________________________________________ 
bool FitterMinuit::Configure(const Registry & conf)
{
  LOG("Minuit", pINFO) << "Fitter configuration: " << conf;

  //
  //
  // use this and deprecate the various Set(...) methods and some or all 
  // of the info passed via Option_t in the ctor
  //
  //

  return true;
}
//____________________________________________________________________________ 
bool FitterMinuit::DoTheFit(void) 
{
  // start clock
  TStopwatch w;
  w.Start();

  fFitTime  = 0;
  fFitTries = -1;
  
  // Initialise and get ready.
  // set params
  FitParamList & fit_params = fFitInputs->FitParams();
  int param_list_size = fit_params.Size();
  TVirtualFitter * fitter = TVirtualFitter::Fitter(0,param_list_size);
  // set print level
  Double_t arglist[5];
  arglist[0] = fPrintLevel;
  fitter->ExecuteCommand("SET PRINT",arglist,1);
  
  // set params
  fit_params.SetTo(fitter);
  // set minimization function
  fitter->SetFCN(FitFunc);         
  
  
  // And we're ready.   

  // reset fit time, bestfit statistic and status codes in
  // persistable FitParamList obj
  fit_params.SetFitTime(-999);
  fit_params.SetBestFitStatistic(-999);
  fit_params.ClearStatusCodes();

  // Run the fitter once - we have the option here to check the status code
  // and if the fitter failed, re-seed the fitter and try again
  bool ok = this->RunOnce(false);

  // 4: migrad not converged
  fFitTries = 1;

  if(fMigradMinStatusCode == 4) {
    LOG("Minuit", pWARN) << "Fit failed...";
    if( fOption.Contains("with_simplex_fallback") ) 
    {
       LOG("Minuit", pWARN) << "Trying SIMPLEX";
       this->RunOnce(true);
       fFitTries++;
    }
  }  

  // stop clock
  w.Stop();
  fFitTime = w.CpuTime();
  
  std::map<std::string, double> likelihood_contributions;

  LOG("Minuit", pINFO) 
    << "Fit performed in " << fFitTime 
    << " sec after " << fFitTries << " attempts."
    << " Used " <<gMinuit->fNfcn << " calls.";
  LOG("Minuit", pINFO) 
    << "Best-fit statistic: " << fit_params.GetBestFitStatistic();
    
  // copy fit time and bestfit statistic and status codes 
  // to persistable FitParamList obj

  fit_params.SetFitTime (fFitTime);
  fit_params.SetFitTries(fFitTries);

  fit_params.SetStatusCode("migrad_min", fMigradMinStatusCode);
  fit_params.SetStatusCode("migrad_err", fMigradErrStatusCode);

  if(fCalcHesseErrors) {
    fit_params.SetStatusCode("hesse_err", fHesseErrStatusCode);
  }
  if(fCalcMinosErrors) {
    fit_params.SetStatusCode("minos_err", fMinosErrStatusCode);
  }
  return ok;
}
//____________________________________________________________________________ 
bool FitterMinuit::RunOnce(bool use_simplex) 
{
  fHesseErrStatusCode = -1;
  double fcntemp;
  
  FitParamList & fp = fFitInputs->FitParams();

  // Get the fitter
#ifdef __show_fit_steps__
  LOG("Minuit", pINFO) << "Getting the fitter...";
#endif
  TVirtualFitter * fitter = TVirtualFitter::GetFitter();

  Double_t arg[5];
  arg[0] = fStr;
  fitter->ExecuteCommand("SET STRategy",arg,1);    

  // Pass fit inputs (data, pdf) to MINUIT
#ifdef __show_fit_steps__
  LOG("Minuit", pINFO) << "Passing fit inputs (data & pdf)...";
#endif
  gMinuit->SetObjectFit(fFitInputs);

  fitter->ExecuteCommand("SHO EPS", arg, 1);///

  // Run MINUIT's minimization step
#ifdef __show_fit_steps__
  LOG("Minuit", pINFO) << "Running " << ((use_simplex) ? "SIMPLEX..." : "MIGRAD... ") << TTimeStamp().AsString();
#endif
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
    fp.CopyCurrToBestFitValues();
    return false;
  }
  
  int num_of_params      = fp.GetNumberParameters();
  
  // Found best fit! Copy current --> best-fit values
  for(int ip = 0; ip < num_of_params; ip++) {
    double value, error;
    gMinuit->GetParameter(ip, value, error);
    PhysParamId_t p = fp.WhichParam(ip);
    fp.Update(p, value, true);
  }
  fp.CopyCurrToBestFitValues();
  
  // Get the contributions to the BF chisquare
  std::map<std::string,double> stat_contrib;
  (void)fFitInputs->Statistic("likelihood_ratio",&stat_contrib);
  fp.SetBestFitStatisticContributions(stat_contrib);

  // Print fit result
#ifdef __show_fit_result__
#ifdef __show_fit_steps__
  LOG("Minuit", pINFO) << "Printing MIGRAD fit results...";
#endif
  fitter->ExecuteCommand("SHOW FCNVALUE",&fcntemp,0);
  Double_t aminn = 0;
  fitter->PrintResults(3,aminn);
#endif

  // Copy errors from HESSE for free parameters
  TMatrixDSym cov(num_of_params);
  int ifree = 0;
  int jfree = 0;
  for(int i = 0; i<num_of_params; i++) {
    if (!fp.IsFixedByIndex(i)) {
      for(int j = 0; j<num_of_params; j++) {
        // Put unfixed errors in the matrix
        if (!fp.IsFixedByIndex(j)) {
          double scaling = fp.ParameterScalingByIndex(i)*fp.ParameterScalingByIndex(j);
          cov[i][j] = fitter->GetCovarianceMatrixElement(ifree,jfree)/scaling;
          jfree++;
        }
      }
      jfree = 0;
      ifree++;
    }
  }
  // Copy prior errors along diagonal for unfixed
  const CompoundFitModel * cm = dynamic_cast<CompoundFitModel*>(&fFitInputs->Expectation());
  if (cm) {
    for (int i = 0; i < num_of_params; i++) {
      if (!fp.IsFixedByIndex(i)) {continue;}
      PhysParamId_t p = fp.WhichParam(i);
      if (cm->GetSystematics()->Contains(p)) {
        cov[i][i] = cm->GetSystematics()->CovMatrixElement(p,p);
      }
    }
  }
  fp.SetCovMatrix(cov);

  double unfilled = -999;

  // Error vectors
  vector<double> migrad_err     (num_of_params, unfilled);
  vector<double> hesse_err      (num_of_params, unfilled);
  vector<double> minos_err_minus(num_of_params, unfilled);
  vector<double> minos_err_plus (num_of_params, unfilled);
  
  // Fill the MIGRAD errors
  for (int ip = 0 ; ip < num_of_params ; ip++) {
    migrad_err[ip] = fitter->GetParError(ip)/fp.ParameterScalingByIndex(ip);
  }
  fp.SetMigradErrors(migrad_err);

  // Get the error matrix status
  double ha(0.0);     // dummy vars
  double edm, errdef; // dummy vars
  int nvpar, nparx;   // dummy vars
  int errMatrixStatus = fitter->GetStats(ha, edm, errdef, nvpar, nparx);
  fMigradErrStatusCode = errMatrixStatus;

  // Calculate HESSE or MINOS errors
  if(fCalcHesseErrors) {
#ifdef __show_fit_steps__
    LOG("Minuit", pINFO) << "Running HESSE...";
#endif
    fitter->ExecuteCommand("HESSE",arglist, 1);
    
    errMatrixStatus = fitter->GetStats(ha, edm, errdef, nvpar, nparx);
    
    fHesseErrStatusCode = errMatrixStatus;
    
    // Fill the HESSE errors
    for (int ip = 0 ; ip < num_of_params ; ip++) {
      hesse_err[ip] = fitter->GetParError(ip)/fp.ParameterScalingByIndex(ip);
    }
    fp.SetHesseErrors (hesse_err);
  }
  
  //else 
  if(fCalcMinosErrors) {
#ifdef __show_fit_steps__
    LOG("Minuit", pINFO) << "Running MINOS...";
#endif
    fMinosErrStatusCode = fitter->ExecuteCommand("MINOS",arglist, 1);
  }
  // Get error matrix status
#ifdef __show_fit_steps__
  LOG("Minuit", pINFO) << "Getting error matrix status...";
#endif
  errMatrixStatus = fitter->GetStats(ha, edm, errdef, nvpar, nparx);

  // Get errors
#ifdef __show_fit_steps__
  LOG("Minuit", pINFO) << "Getting errors...";
#endif

  if(fCalcMinosErrors) {
    for(int ip = 0; ip < num_of_params; ip++) {
      double glob_corr, parerr; // dummy vars
        fitter->GetErrors(ip, minos_err_plus[ip], minos_err_minus[ip],parerr,glob_corr);
    }
    fp.SetMinosErrors (minos_err_plus, minos_err_minus);
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
#endif

#ifdef __show_fit_steps__
  LOG("Minuit", pINFO) << "Done";
#endif

  return true;//(fMigradErrStatusCode == 3);
}
//____________________________________________________________________________ 
int FitterMinuit::FitStatusCode (Option_t * option) const
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
