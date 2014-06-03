//____________________________________________________________________________ 
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/ 
//____________________________________________________________________________

#define __show_fit_result__ 
#define __show_fit_steps__ 

#include <cassert> 
#include <cstdlib> 
#include <vector> 

#include <TMath.h> 
#include <TRandom3.h> 
#include <TVirtualFitter.h> 
#include <TString.h> 
#include <TFile.h>
#include <TSystem.h>
#include <TStopwatch.h>

#include "Conventions/Opt.h"
#include "Utilities/Rnd.h"
#include "Messenger/Messenger.h"
#include "FitCore/FitterMinuit.h" 
#include "FitCore/FitInputs.h" 
#include "FitCore/FitParamList.h" 
#include "FitCore/FitDataABC.h" 

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
  fTol                 = 0.01;
  fStr                 = 1;
  fMigradMinStatusCode = -999;
  fMigradErrStatusCode = -999;      
  fHesseErrStatusCode  = -999; 
  fMinosErrStatusCode  = -999; 
  fFitTime             = 0;
  fFitTries            = 0;
}
//____________________________________________________________________________ 
int FitterMinuit::FitStatusCode (Option_t * option) 
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
    arglist[0] = -1;
    fitter->ExecuteCommand("SET PRINT",arglist,1);
    
    // set params
    fit_params.SetTo(fitter);
    
    // set minimization function
    fitter->SetFCN(FitFunc);            
  }

/*
  if(fOption.Contains("simplex")) {
    TVirtualFitter::SetDefaultFitter("Simplex");
    TVirtualFitter * fitter = TVirtualFitter::Fitter(0,param_list_size);
    
    // set print level              
    Double_t arglist[5];
    arglist[0] = -1;
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
  
  assert(fNFitBins > 0);
  assert(fNFreeFitParams >= 0);

  LOG("Minuit", pINFO) 
       << "NDoF = " << this->NDOF()
       << " (Number of bins = " << this->NBinsInFit()
       << ", Number of free parameters = " << this->NFitParams() << ")";
  
}
//____________________________________________________________________________ 
bool FitterMinuit::DoTheFit(void) 
{
  // start clock
  TStopwatch w;
  w.Start();

  fFitTime  = 0;
  fFitTries = -1;

  // reset fit time, bestfit statistic and status codes in
  // persistable FitParamList obj
  fFitInputs->FitParams().SetFitTime(0);
  fFitInputs->FitParams().SetBestFitStatistic(0);
  fFitInputs->FitParams().ClearStatusCodes();

  // Run the fitter once - we have the option here to check the status code
  // and if the fitter failed, re-seed the fitter and try again
  bool ok = this->RunOnce(false);

  // 4: migrad not converged
  int maxntries = 10;
  fFitTries = 1;

/* 
  HERE: This bit of code has a considerable amount of ugliness and it can not be eaily ported,
  although perhaps it contains some wisdom of how to resolve pathological fit cases.
  Comment-out but keep so as to review later

  OscAnaType_t ana = Opt::Instance()->gOscAnaTypeChoice;
  if(ana != kAnaNuMuDisappOnly)
    maxntries = 25;
  while(fMigradMinStatusCode == 4 && fFitTries < maxntries) {
    cout << "Original fit failed; reseeding & trying again. Attempt: " << fFitTries << endl;
    //randomise oscillation parameters
    NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
    switch(omp) {
    case(conv::kNuMixSinsq2Theta):
      if((ana == kAnaNuMuDisappOnly || ana == kAnaCombined) && fFitInputs->FitParams().Exists(conv::kPhypMixing23) && !fFitInputs->FitParams().IsFixed(conv::kPhypMixing23))
	fFitInputs->FitParams().SetInitValue(conv::kPhypMixing23, Rnd::Instance()->RndFitter().Uniform(0.80,0.98));
      if((ana == kAnaNuEAppOnly     || ana == kAnaCombined) && fFitInputs->FitParams().Exists(conv::kPhypMixing13) && !fFitInputs->FitParams().IsFixed(conv::kPhypMixing13))
	fFitInputs->FitParams().SetInitValue(conv::kPhypMixing13, Rnd::Instance()->RndFitter().Uniform(0.05,0.15));
      break;
    case(conv::kNuMixSinsqTheta):
      if((ana == kAnaNuMuDisappOnly || ana == kAnaCombined) && fFitInputs->FitParams().Exists(conv::kPhypMixing23) && !fFitInputs->FitParams().IsFixed(conv::kPhypMixing23)) {
	if (fFitInputs->FitParams().InitValue(conv::kPhypMixing23)< 0.515)
	  fFitInputs->FitParams().SetInitValue(conv::kPhypMixing23, Rnd::Instance()->RndFitter().Uniform(0.35,0.50));
	else
	  fFitInputs->FitParams().SetInitValue(conv::kPhypMixing23, Rnd::Instance()->RndFitter().Uniform(0.53,0.68));
      }
      if((ana == kAnaNuEAppOnly || ana == kAnaCombined) && fFitInputs->FitParams().Exists(conv::kPhypMixing13) && !fFitInputs->FitParams().IsFixed(conv::kPhypMixing13))
	fFitInputs->FitParams().SetInitValue(conv::kPhypMixing13, Rnd::Instance()->RndFitter().Uniform(0.015,0.035));
      break;
    case(conv::kNuMixUndefined):
      cerr << "Mixing parameter type undefinded. Exiting..." << endl;
      exit(1);
      break;
    }
    if(fFitInputs->FitParams().Exists(conv::kPhypDm23sq) && !fFitInputs->FitParams().IsFixed(conv::kPhypDm23sq))
      fFitInputs->FitParams().SetInitValue(conv::kPhypDm23sq, Rnd::Instance()->RndFitter().Uniform(0.0020,0.0030));
    if(fFitInputs->FitParams().Exists(conv::kPhypDm12sq) && !fFitInputs->FitParams().IsFixed(conv::kPhypDm12sq))
      fFitInputs->FitParams().SetInitValue(conv::kPhypDm12sq, util::RandomizeDm12sq());
    if(fFitInputs->FitParams().Exists(conv::kPhypMixing12) && !fFitInputs->FitParams().IsFixed(conv::kPhypMixing12))
      fFitInputs->FitParams().SetInitValue(conv::kPhypMixing12, util::RandomizeMixing12());
    if(fFitInputs->FitParams().Exists(conv::kPhypDeltaCP) && !fFitInputs->FitParams().IsFixed(conv::kPhypDeltaCP))
      fFitInputs->FitParams().SetInitValue(conv::kPhypDeltaCP, util::RandomizeDeltaCP());
    //randomise systematics
    //first check if there is a floating systematic in the fit (only need to look at one, since we're only ever fitting either 0 or all systematics)
    if(fFitInputs->FitParams().Exists(conv::kNuisXSecNormNuEToNuMu) && !fFitInputs->FitParams().IsFixed(conv::kNuisXSecNormNuEToNuMu)) {
      //tweak everything
      fFitInputs->Expectation().TweakAllSystematics();
      //get the tweaks
      PhysParamList tweaks = fFitInputs->Expectation().Tweaks();
      //initialise the FitParamList with these random tweaks
      fFitInputs->FitParams().FromPhysParamList(tweaks,"initial",true);
      fFitInputs->FitParams().FromPhysParamList(tweaks,"current",true);
    }//systematics exist in fit
    //cout << fFitInputs->FitParams() << endl;
    this->RunOnce(false);
    fFitTries++;
  }
*/

  if(fMigradMinStatusCode == 4) {
    LOG("Minuit", pWARN) << "Fits still failed; trying SIMPLEX";
    this->RunOnce(true);
    fFitTries = 99;
  }  

  // stop clock
  w.Stop();
  fFitTime = w.CpuTime();

  LOG("Minuit", pINFO) 
    << "Fit performed in " << fFitTime 
    << " sec after " << fFitTries << " attempts";
  LOG("Minuit", pINFO) 
    << "Best-fit statistic: " << fFitInputs->FitParams().GetBestFitStatistic();

  // copy fit time and bestfit statistic and status codes 
  // to persistable FitParamList obj

  fFitInputs->FitParams().SetFitTime (fFitTime);
  fFitInputs->FitParams().SetFitTries(fFitTries);
  fFitInputs->FitParams().SetBestFitStatistic(fFitInputs->FitParams().GetBestFitStatistic());

  fFitInputs->FitParams().SetStatusCode("migrad_min", fMigradMinStatusCode);
  fFitInputs->FitParams().SetStatusCode("migrad_err", fMigradErrStatusCode);

  if(fCalcHesseErrors) {
    fFitInputs->FitParams().SetStatusCode("hesse_err", fHesseErrStatusCode);
  }
  if(fCalcMinosErrors) {
    fFitInputs->FitParams().SetStatusCode("minos_err", fMinosErrStatusCode);
  }
  return ok;
}
//____________________________________________________________________________ 
bool FitterMinuit::RunOnce(bool use_simplex) 
{

  fHesseErrStatusCode = -1;
  double fcntemp;

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

  // Run MINUIT's minimization step
#ifdef __show_fit_steps__
  LOG("Minuit", pINFO) << "Running MIGRAD...";
#endif
  Double_t arglist[100];
  arglist[0] = 50000; // num of func calls
  arglist[1] = fTol;  // tolerance
  if(!use_simplex) {
    /*
    //in disappearance mode, when fitting for deltacp, do two fits - fit everything but deltacp, then fit only deltacp
    OscAnaType_t ana = Opt::Instance()->gOscAnaTypeChoice;
    if(ana == kAnaNuMuDisappOnly && fFitInputs->FitParams().GetParListName() == "nuosc3f_numudisapp_nuis4+nuisance") {
#ifdef __show_fit_steps__
      LOG("Minuit", pINFO) << "Performing two step fit. Fit everything but dcp, fit only dcp...";
#endif
      //fix dcp
      fFitInputs->FitParams().FixParameter(conv::kPhypDeltaCP);
      double dcp_slot = (double)(fFitInputs->FitParams().Id(conv::kPhypDeltaCP) + 1); //FitParamList Id runs from 0. MINUIT Id runs from 1.
      fitter->ExecuteCommand("FIX",&dcp_slot,1);
      //fit
      fMigradMinStatusCode = fitter->ExecuteCommand("MIGRAD",arglist, 2);
      //return if the not-dcp fit failed
      if(fMigradMinStatusCode == 4) {
	fFitInputs->FitParams().FreeParameter(conv::kPhypDeltaCP);
	fitter->ExecuteCommand("RELEASE",&dcp_slot,1);
	LOG("Minuit", pINFO) << "Fit did not converge when fixing dcp";
	return false;
      }
      //print
#ifdef __show_fit_result__
#ifdef __show_fit_steps__
      LOG("Minuit", pINFO) << "Printing MIGRAD intermediate fit results...";
#endif
      fitter->ExecuteCommand("SHOW FCNVALUE",&fcntemp,0); //print the bestfit statistic
      Double_t aminn = 0;
      fitter->PrintResults(3,aminn);
#endif
      //fix everything else
      double fixed_slot[100] = {0};
      int nfix = 0;
      for(int ip = 0; ip < fFitInputs->FitParams().Size(); ip++) {
	PhysParamId_t p = fFitInputs->FitParams().WhichParam(ip);
	if(!fFitInputs->FitParams().IsFixed(p)) {
	  fFitInputs->FitParams().FixParameter(ip);
 	  fixed_slot[nfix] = ip+1; //FitParamList Id runs from 0. MINUIT Id runs from 1.
	  nfix++;
	}
      }//ip
      //free dcp
      fFitInputs->FitParams().FreeParameter(conv::kPhypDeltaCP);
      fitter->ExecuteCommand("RELEASE",&dcp_slot,1);
      //fit
      fMigradMinStatusCode = fitter->ExecuteCommand("MIGRAD",arglist, 2);
      //free everything that I just fixed
      for(int ip = 0; ip <= nfix; ip++) {
	fFitInputs->FitParams().FreeParameter(ip);
      }
      fitter->ExecuteCommand("RELEASE",fixed_slot,nfix);
    }//disapp & free dcp
    else
    */
    
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
#ifdef __show_fit_result__
#ifdef __show_fit_steps__
  LOG("Minuit", pINFO) << "Printing MIGRAD fit results...";
#endif
  fitter->ExecuteCommand("SHOW FCNVALUE",&fcntemp,0);
  Double_t aminn = 0;
  fitter->PrintResults(3,aminn);
#endif

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
#ifdef __show_fit_steps__
    LOG("Minuit", pINFO) << "Running HESSE...";
#endif
    fHesseErrStatusCode = fitter->ExecuteCommand("HESSE",arglist, 1);
    
    // Fill the HESSE errors
    for (int ip = 0 ; ip < num_of_params ; ip++) {
      hesse_err[ip] = fitter->GetParError(ip);
    }
    fFitInputs->FitParams().SetHesseErrors (hesse_err);
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
  LOG("Minuit", pINFO) << "Final error matrix status = " << fMigradErrStatusCode; 
#endif

#ifdef __show_fit_steps__
  LOG("Minuit", pINFO) << "Done";
#endif

  return true;//(fMigradErrStatusCode == 3);
}
//____________________________________________________________________________ 
