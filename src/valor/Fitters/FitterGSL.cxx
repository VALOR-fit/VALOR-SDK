//____________________________________________________________________________ 
/*

 >> ChangeLog: 3/Sept/2024: Add modern Likelihood dependencies

*/ 
//____________________________________________________________________________

#define __show_fit_init_info__ 
#define __show_fit_result__ 
#define __show_fit_steps__ 
#define __show_fit_cpu_time__

#include "valor/Fitters/FitterGSL.h" 

using std::cerr; 
using std::cout; 
using std::endl; 
using std::ostringstream;

using namespace valor; 
using namespace valor::conv;

   
//____________________________________________________________________________ 
FitterGSL::FitterGSL() :
FitterABC()
{
  this->Init();
}
//____________________________________________________________________________ 
FitterGSL::FitterGSL(LikelihoodGSL * lkhd, Option_t * option) :
FitterABC()
{
  this->Init(lkhd, option);
}
//____________________________________________________________________________ 
FitterGSL::~FitterGSL() 
{

}
//____________________________________________________________________________ 
void FitterGSL::Init(void) 
{
  fLikelihood     = 0;
  fOption         = "";
  fNFreeFitParams = 0;
  fNFitBins       = 0;
  fFitTries       = 0;
#ifdef __VALOR_FMWK_GSL_ENABLED__
  fFunc           = 0;
  fMinimizer      = 0;
#endif
}
//____________________________________________________________________________ 
void FitterGSL::Init(LikelihoodGSL * lkhd, Option_t * option) 
{
#ifdef __show_fit_init_info__
  cout << "FitterGSL/Init >> Fitter initialization..." << endl;
#endif

  this->Init();

  fLikelihood = lkhd;
  if(!fLikelihood) {
     cerr << "FitterGSL/Init >> ** Null input likelihood!" << endl;
     exit(1);
  }

  fOption = option;

  FitParamList & fit_params = fLikelihood->Kernel()->FitParams();
  int param_list_size = fit_params.Size();

#ifdef __show_fit_init_info__
  cout << "FitterGSL/Init >> Fit parameter list size = " 
       << param_list_size << endl;
#endif

#ifdef __VALOR_FMWK_GSL_ENABLED__
  if(fOption.Contains("SimAnneal")) {
    fMinimizer = new ROOT::Math::GSLSimAnMinimizer;
  }
  else
  if(fOption.Contains("LevMarq")) {
    fMinimizer = new ROOT::Math::GSLNLSMinimizer;
  }
  else
  if(fOption.Contains("ConjugateFR")) {
    fMinimizer = new ROOT::Math::GSLMinimizer(ROOT::Math::kConjugateFR);
  }
  else
  if(fOption.Contains("ConjugatePR")) {
    fMinimizer = new ROOT::Math::GSLMinimizer(ROOT::Math::kConjugatePR);
  }
  else
  if(fOption.Contains("VectorBFGS")) {
    fMinimizer = new ROOT::Math::GSLMinimizer(ROOT::Math::kVectorBFGS2);
  }
  else
  if(fOption.Contains("SteepestDescent")) {
    fMinimizer = new ROOT::Math::GSLMinimizer(ROOT::Math::kSteepestDescent);
  }
  else {
     cout << "FitterGSL/Init >> Can not handle option: " << option << endl;
     exit(1);
  }

  fMinimizer->SetMaxFunctionCalls(1000000);
  fMinimizer->SetMaxIterations(100000);
  fMinimizer->SetTolerance(0.001);

  // calculate degrees of freedom
  fNFitBins       = fLikelihood->Kernel()->Observation().NumOfBins();
  fNFreeFitParams = fit_params.GetNumberFreeParameters();

  if (fOption.Contains("LevMarq") ){ 
    fFitMethodFunc = fLikelihood->AsFitMethodFunction();
  }
  else{
    fFunc = fLikelihood->AsFunctor(); 
    fMinimizer->SetFunction(*fFunc);
  }
  fit_params.SetTo(fMinimizer);  
#ifdef __show_fit_init_info__
  cout << "FitterGSL/Init >> NDoF = " << this->NDOF()
       << " (Number of bins = " << this->NBinsInFit()
       << ", Number of free parameters = " << this->NFitParams() << ")"
       << endl;
#endif

#endif // GSL
}
//____________________________________________________________________________ 
bool FitterGSL::DoTheFit(void) 
{
#ifdef __VALOR_FMWK_GSL_ENABLED__

#ifdef __show_fit_cpu_time__
  TStopwatch w;
  w.Start();
#endif
  fFitTime = 0;

  if (fOption.Contains("LevMarq") ){
    ROOT::Fit::Fitter fitter;
    fLikelihood->Kernel()->FitParams().SetTo(&fitter);
    fitter.Config().MinimizerOptions().SetMinimizerType("GSLMultiFit");
    
    fitter.FitFCN(*fFitMethodFunc,0,fLikelihood->Kernel()->GetNPoints(),true);
    ROOT::Fit::FitResult result = fitter.Result();
    result.Print(std::cout);
  }
  else{
    if(!fMinimizer) return false;
    // Run the fitter once 
    fMinimizer->Minimize();
  }
  
#ifdef __show_fit_cpu_time__
  w.Stop();
  cout << "FitterGSL/DoTheFit >> Fit performed in " 
       << w.CpuTime() << " sec." << endl;
  fFitTime = w.CpuTime();
#endif
  
  fLikelihood->Kernel()->FitParams().CopyCurrToBestFitValues();
  
  fLikelihood->Kernel()->FitParams().SetFitTime ( fFitTime );
  fLikelihood->Kernel()->FitParams().SetBestFitStatistic ( fLikelihood->Kernel()->Statistic() );
  
  if(fMinimizer->ProvidesError()) {

    TMatrixD cov(fNFreeFitParams,fNFreeFitParams);
    fMinimizer->GetCovMatrix(cov.GetMatrixArray());
    // Put in FPL
  }
  
  return true;
  

#endif // GSL

  cout << "FitterGSL/DoTheFit >> GSL not enabed." << endl;
  return false;
}
//____________________________________________________________________________ 
