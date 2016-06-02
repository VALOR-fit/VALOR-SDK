//____________________________________________________________________________ 
/*

 \copyright (c) 2010-2013, VaLOR group.

*/ 
//____________________________________________________________________________

#define __show_fit_init_info__ 
//#define __show_fit_cov_matrix_debug__
#define __show_fit_result__ 
#define __show_fit_steps__ 
#define __show_fit_steps_debug__
#define __show_fit_cpu_time__
#define __give_fitter_covariance_matrix__

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

#include <Minuit2/MnUserParameters.h>
#include <Minuit2/MnUserCovariance.h>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnApplication.h>
#include <Minuit2/MnMigrad.h>
#include <Minuit2/MnHesse.h>
#include <Minuit2/MnMinos.h>
#include <Minuit2/MnPrint.h>

#include "valor/Conventions/Constants.h"
#include "valor/Conventions/Opt.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Messenger/Messenger.h"
#include "valor/FitCore/CompoundFitModel.h" 
#include "valor/FitCore/FitterMinuit2.h" 
#include "valor/FitCore/FitInputs.h" 
#include "valor/FitCore/FitParamList.h" 
#include "valor/FitCore/FitDataABC.h" 

using namespace valor; 
using namespace valor::conv;

using std::cout;
using std::endl;
   
//____________________________________________________________________________ 
FitterMinuit2::FitterMinuit2() :
FitterABC()
{
  this->Init();
}
//____________________________________________________________________________ 
FitterMinuit2::FitterMinuit2(FitInputs * fit_inputs, Option_t * option) :
FitterABC()
{
  this->Init(fit_inputs, option);
}
//____________________________________________________________________________ 
FitterMinuit2::~FitterMinuit2() 
{
}
//____________________________________________________________________________ 
void FitterMinuit2::Init(void) 
{
  fFitInputs           = 0;
  fOption              = "";
  fNFreeFitParams      = 0;
  fNFitBins            = 0;
  fCalcHesseErrors     = false;
  fCalcMinosErrors     = false;
  fTol                 = 0.1; // 0.01;
  fStr                 = 1;
  fMaxNumCalls         = 100000;
  fMigradMinStatusCode = -999;
  fMigradErrStatusCode = -999;      
  fHesseErrStatusCode  = -999; 
  fMinosErrStatusCode  = -999; 
  fFitTime             = 0;
  fFitTries            = 0;
}
//____________________________________________________________________________ 
void FitterMinuit2::Init(FitInputs * fit_inputs, Option_t * option) 
{
  LOG("Minuit2", pINFO) << "Fitter initialization...";

  this->Init();

  fFitInputs = fit_inputs;
  if(!fFitInputs) {
     LOG("Minuit2", pFATAL) << "** Null fit inputs!";
     exit(1);
  }

  FitParamList & fit_params = fFitInputs->FitParams();
  int param_list_size = fit_params.Size();

  LOG("Minuit2", pINFO) << "Fit parameter list size = " << param_list_size;

  fOption = option;

  // calc HESSE or MINOS errors after fit
  fCalcHesseErrors  = fOption.Contains("hesse");
  fCalcMinosErrors  = fOption.Contains("minos");

#ifdef __show_fit_init_info__
  cout << "FitterMinuit2/Init >> Fit parameter list size = " 
       << param_list_size << endl;
#endif
  
  // set params
  fit_params.SetTo(fUserParams);
  LOG("Minuit2", pINFO) << fUserParams;

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
      LOG("Minuit2", pINFO) << "Set tolerance to "<<tolerance;
      this->SetTol(tolerance);
    }
    else {
      LOG("Minuit2",pWARN) << "Failed to parse valid single tolerance from option string "<<fOption;
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
      LOG("Minuit2", pINFO) << "Set max number of calls to "<<ncalls;
      this->SetMaxCalls(ncalls);
    }
    else {
      LOG("Minuit2",pWARN) << "Failed to parse valid single max number of calls from option string "<<fOption;
    }
  }

  // calculate degrees of freedom  
  fNFitBins       = fit_inputs->Observation().NumOfBins();
  fNFreeFitParams = fit_params.GetNumberFreeParameters();
  
  assert(fNFitBins > 0);
  assert(fNFreeFitParams >= 0);

  LOG("Minuit2", pINFO) 
       << "NDoF = " << this->NDOF()
       << " (Number of bins = " << this->NBinsInFit()
       << ", Number of free parameters = " << this->NFitParams() << ")";
  
}
//____________________________________________________________________________
bool FitterMinuit2::Configure(const Registry & conf)
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
void FitterMinuit2::FillPriorCovMatrix(Option_t * /*opt*/)
{
  LOG("Minuit2",pDEBUG)<<"Constructing prior covariance matrix";
  
  FitParamList & fp = fFitInputs->FitParams();
  int num_of_params      = fp.GetNumberParameters();
  int num_of_free_params = fp.GetNumberFreeParameters();
  fCovMtrx = TMatrixD(num_of_free_params,num_of_free_params);
  const CompoundFitModel * cm = dynamic_cast<CompoundFitModel*>(&fFitInputs->Expectation());
  if (cm) {
    const TMatrixD & full_cov = cm->GetSystematics()->CovMatrix();
    int ifree = 0;
    for (int i = 0 ; i < num_of_params ; i++) {
      if (fp.IsFixedByIndex(i)) {continue;}
      int jfree = 0;
      for (int j = 0 ; j < num_of_params ; j++) {
        if (fp.IsFixedByIndex(j)) {continue;}
        fCovMtrx[ifree][jfree] = full_cov[i][j];
        jfree++;
      }
      ifree++;
    }
  }
  else {
    assert(!"Not supporting creating prior cov matrix without CFM model yet");
  }
  //~ LOG("Minuit2",pDEBUG)<<fCovMtrx;
}
//____________________________________________________________________________
bool FitterMinuit2::DoTheFit(void) 
{
#ifdef __show_fit_cpu_time__
  TStopwatch w;
  w.Start();
#endif
  fFitTime = 0;

  // Run the fitter once - we have the option here to check the status code
  // and if the fitter failed, re-seed the fitter and try again (not needed
  // so far).
  bool ok = this->RunOnce();

#ifdef __show_fit_cpu_time__
  w.Stop();
  cout << "FitterMinuit2/DoTheFit >> Fit performed in " 
       << w.CpuTime() << " sec." << endl;
  fFitTime = w.CpuTime();
#endif

  return ok;
}
//____________________________________________________________________________ 
bool FitterMinuit2::RunOnce(void) 
{
  // a few arrays to store parameters
  unsigned int nfreeparams = (unsigned int)fNFreeFitParams;
  unsigned int nparams     = (unsigned int)fFitInputs->FitParams().Size();
  vector<double> bestfit_free;
  vector<double> err_migrad_cov;
  vector<double> err_migrad_err;
#ifdef __get_hesse_errors__
  vector<double> err_hesse_cov;
  vector<double> err_hesse_err;
#endif
#ifdef __get_minos_errors__
  vector<double> err_minosplus;
  vector<double> err_minosminus;
#endif

  // Set fitter strategy
  // In general, low values of level mean fewer function calls and high values mean more reliable minimization.
  // Currently allowed values are 0 (low), 1 (default), and 2 (high).
  ROOT::Minuit2::MnStrategy strategy(1);

#ifdef __give_fitter_covariance_matrix__
  // We have a TMatrixD covariance matrix, need to translate it into
  // the Minuit2::MnUserCovariance
  this->FillPriorCovMatrix();
  ROOT::Minuit2::MnUserCovariance input_cov(nfreeparams);
  for(unsigned int i = 0; i < nfreeparams; i++) {
    for(unsigned int j = 0; j < nfreeparams; j++) {
      input_cov(i,j) = fCovMtrx(i,j);
    }
  }
#ifdef __show_fit_cov_matrix_debug__
  cout << "FitterMinuit2/RunOnce >> Input covariance matrix:" << endl;
  cout << input_cov << endl;
#endif
#else
  fCovMtrx = new TMatrixD(); // need to do this to make the destructor work. confusing...
#endif

  // Get the fitter & pass it the inputs
#ifdef __show_fit_steps__
  cout << "FitterMinuit2/RunOnce >> Getting the fitter & giving it inputs (data & pdf)..." << endl;
#endif

#ifdef __give_fitter_covariance_matrix__
  ROOT::Minuit2::MnMigrad migrad(*fFitInputs, fUserParams, input_cov);
#else
  ROOT::Minuit2::MnMigrad migrad(*fFitInputs, fUserParams);
#endif

  // Run MINUIT's minimization step
#ifdef __show_fit_steps__
  cout << "FitterMinuit2/RunOnce >> Running MIGRAD..." << endl;
#endif

  // max number of function calls
  // if zero, max_calls = 200 + 100 * npar + 5 * npar**2
  int max_calls = 0;

  // value used for terminating iteration procedure.
  // For example, MIGRAD will stop iterating when edm (expected distance from minimum) will be:
  // edm < tolerance * 10**-3
  double tolerance = 0.1;

  //Minimise!
  ROOT::Minuit2::FunctionMinimum bestfit = migrad(max_calls, tolerance);

#ifdef __show_fit_steps__
  cout << "FitterMinuit2/RunOnce >> MIGRAD minimisation complete" << endl;
#endif

#ifdef __show_fit_steps__
  cout << "FitterMinuit2/RunOnce >> Getting fit success status..." << endl;
#endif

  // Done! Copy current --> best-fit values
  fFitInputs->FitParams().CopyCurrToBestFitValues();
  
  if(fMigradMinStatusCode !=4 ) {
    LOG("Minuit2",pERROR) << "Fit failed. Returning...";
    return false;
  }

  //get the best fit vector, for use in this function
  std::vector<ROOT::Minuit2::MinuitParameter> bestfit_params = bestfit.UserParameters().Parameters();
  for(unsigned int ip = 0; ip < bestfit_params.size(); ip++) {
    if(!bestfit_params[ip].IsFixed()) {
      bestfit_free.push_back(bestfit_params[ip].Value());
    }
#ifdef __show_fit_steps_debug__
    cout << "FitterMinuit2/RunOnce >> Bestfit for par#" << ip << " = " << bestfit_params[ip].Value() << endl;
#endif
  }

  // Get errors
#ifdef __show_fit_steps__
  cout << endl << "FitterMinuit2/RunOnce >> Getting Migrad errors..." << endl;
#endif

  // migrad covariance
  if(bestfit.HasCovariance()) {
    ROOT::Minuit2::MnUserCovariance cov_migrad(bestfit.UserCovariance());
#ifdef __show_fit_steps_debug__
    cout << cov_migrad << endl;
#endif
    for(unsigned int ip = 0; ip < nfreeparams; ip++) {
      double tmp = sqrt(cov_migrad(ip,ip));
      err_migrad_cov.push_back(tmp);
#ifdef __show_fit_steps_debug__
      cout << "FitterMinuit2/RunOnce >> Migrad covariance for par#" << ip << " = " << bestfit_free[ip] << " +/- " << err_migrad_cov[ip] << endl;
#endif
    }
  }

  // migrad error
  err_migrad_err =  bestfit.UserParameters().Errors(); 
  for(unsigned int ip = 0; ip < nparams; ip++) {
#ifdef __show_fit_steps_debug__
    cout << "FitterMinuit2/RunOnce >> Migrad error for par#" << ip << " = " << bestfit_free[ip] << " +/- " << err_migrad_err[ip] << endl;
#endif
  }
  fFitInputs->FitParams().SetMigradErrors(err_migrad_err);

#ifdef __get_hesse_errors__
  //Get Hesse errors
#ifdef __show_fit_steps__
  cout << endl << "FitterMinuit2/RunOnce >> Getting Hesse errors..." << endl;
#endif
  // argument is strategy
  ROOT::Minuit2::MnHesse hesse(2);
  // perform hesse error calculation
  hesse(*fFitInputs, bestfit);
  //Check if fit succeeded
  GetStatusCode(bestfit);

  // hesse covariance
  if(bestfit.HasCovariance()) {
    ROOT::Minuit2::MnUserCovariance cov_hesse(bestfit.UserCovariance());
#ifdef __show_fit_steps_debug__
    cout << cov_hesse << endl;
#endif
    for(unsigned int ip = 0; ip < nfreeparams && ip < cov_hesse.Nrow(); ip++) {
      err_hesse_cov.push_back(sqrt(cov_hesse(ip,ip)));
#ifdef __show_fit_steps_debug__
      cout << "FitterMinuit2/RunOnce >> Hesse covariance for par#" << ip << " = " << bestfit_free[ip] << " +/- " << err_hesse_cov[ip] << endl;
#endif
    }
  }

  // hesse error
  err_hesse_err =  bestfit.UserParameters().Errors(); 
  fFitInputs->FitParams().SetHesseErrors(err_hesse_err);
  for(unsigned int ip = 0; ip < nparams; ip++) {
#ifdef __show_fit_steps_debug__
    cout << "FitterMinuit2/RunOnce >> Hesse error for par#" << ip << " = " << bestfit_free[ip] << " +/- " << err_hesse_err[ip] << endl;
#endif
  }
  GetStatusCode(bestfit);
#endif

#ifdef __get_minos_errors__
#ifdef __show_fit_steps__
  cout << endl << "FitterMinuit2/RunOnce >> Getting Minos errors..." << endl;
#endif

  //Get Minos Errors
  ROOT::Minuit2::MnMinos minos(*fFitInputs,bestfit);
  for(unsigned int ip = 0; ip < nparams; ip++) {
    if(bestfit_params[ip].IsFixed())
      continue;
    double up = minos.Upper(ip);
    double lo = minos.Lower(ip);
    err_minosminus.push_back(lo);
    err_minosplus. push_back(up);
#ifdef __show_fit_steps_debug__
    cout << "FitterMinuit2/RunOnce >> par#" << ip << " = " << bestfit_params[ip].Value() << " +" << up << " -" << lo << " (minos)" << endl;
#endif
  }
  fFitInputs->FitParams().SetMinosErrors(err_minosplus, err_minosminus);
#endif

  fFitInputs->FitParams().SetFitTime ( fFitTime );
  fFitInputs->FitParams().SetFitTries(fFitTries);
  fFitInputs->FitParams().SetBestFitStatistic(fFitInputs->FitParams().GetBestFitStatistic());


  // Print fit result
#ifdef __show_fit_result__
#ifdef __show_fit_steps__
  cout << endl << "FitterMinuit2/RunOnce >> Printing fit results..." << endl;
#endif
  cout << "FitterMinuit2/RunOnce >> fcn =  " << bestfit.Fval() << endl;
  cout << "FitterMinuit2/RunOnce >> chi2 = " << fFitInputs->Statistic() << endl;
  cout << bestfit << endl;
  cout << "FitterMinuit2/RunOnce >> migrad status code = " << fMigradMinStatusCode << endl;
#endif

#ifdef __show_fit_steps__
  cout << "FitterMinuit2/RunOnce >> Done" << endl;
#endif

  return (fMigradMinStatusCode == 0);
}
//____________________________________________________________________________ 
int FitterMinuit2::FitStatusCode(Option_t * option) 
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
