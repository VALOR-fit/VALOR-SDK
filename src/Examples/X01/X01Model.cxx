//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <ostream>

#include <TFile.h>
#include <TObjArray.h>
#include <TH1D.h>

#include "Conventions/PhysHypothesisId.h"
#include "Conventions/PhysParamId.h"
#include "Conventions/NuOscMixParam.h"
#include "Conventions/NuOscDmsqScheme.h"
#include "Conventions/Opt.h"
#include "Conventions/PDG.h"
#include "Utilities/UtlStyle.h"
#include "Messenger/Messenger.h"
#include "Examples/X01/X01Model.h"
#include "Examples/X01/X01Const.h"
#include "Examples/X01/X01Utils.h"

using std::endl;

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
X01Model::X01Model() :
FitModelABC()
{
  const int    n   = example01::kNEbins;
  const double min = example01::kEmin;
  const double max = example01::kEmax;

  fTotSpectrumPdf = new TH1D("tot", "", n, min, max);
  fSigSpectrumPdf = new TH1D("sig", "", n, min, max);
  fBkgSpectrumPdf = new TH1D("bkg", "", n, min, max);

  fSigSpectrumPdf->SetLineColor(kGreen);
  fBkgSpectrumPdf->SetLineColor(kRed);
}
//________________________________________________________________________________________
X01Model::~X01Model() 
{
  if (fTotSpectrumPdf) delete fTotSpectrumPdf;
  if (fSigSpectrumPdf) delete fSigSpectrumPdf;
  if (fBkgSpectrumPdf) delete fBkgSpectrumPdf;
}
//________________________________________________________________________________________
string X01Model::Name(void)
{
  return "X01";
}
//________________________________________________________________________________________
bool X01Model::Read(string /*file*/, Option_t * /*opt*/)
{
  // No model configuration to read from file in this example
  // This model is just a simple analytical function defined in util::example01 namespace

  return true;
}
//________________________________________________________________________________________
bool X01Model::Generate(const PhysHypothesis & ph, Option_t * /*opt*/)
{
  fPhysHypothesis.Copy(ph);
  PhysHypothesis_t hyp = fPhysHypothesis.Hypothesis();

  LOG("X01",pINFO)  
     << "Generating model predictions for the input hypothesis: " 
     << fPhysHypothesis;

  //
  // 3-flavour oscillations
  //
  if(hyp == kHypOsc3F) 
  {
    // set oscillation parameters to the oscillation probability calculation code
    double mixing12 = fPhysHypothesis.CurrValue(kPhypMixing12);
    double mixing13 = fPhysHypothesis.CurrValue(kPhypMixing13);
    double mixing23 = fPhysHypothesis.CurrValue(kPhypMixing23);
    double dmsq     = fPhysHypothesis.CurrValue(kPhypDm12sq);
    double Dmsq     = fPhysHypothesis.CurrValue(kPhypDm23sq);
    double cpv      = fPhysHypothesis.CurrValue(kPhypCPV);

    NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
    NuOscDmsqScheme_t  dmsqscheme = Opt::Instance()->gOscDmsqScheme;

    // mixing params are: sin^2(2*theta)
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |Dm^2_{23}|                                                                                             
    if(dmsqscheme == kNuDmsq12andAbs23 && omp == kNuMixSinsq2Theta)
    {
        fNuOsc3Flavour.SetOscParameters(mixing12,mixing13,mixing23,dmsq,Dmsq,cpv);
    }
    // mixing params are: sin^2(theta)                                                                                                                                         
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |Dm^2_{23}|                                                                                              
    else if(dmsqscheme == kNuDmsq12andAbs23 && omp == kNuMixSinsqTheta)
    {
        fNuOsc3Flavour.SetOscParametersSinSqTAll(mixing12,mixing13,mixing23,dmsq,Dmsq,cpv);
    }
    // mixing params are: sin^2(2*theta)                                                                                                                                        
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |m^2_{3} - (m^2_{1} + m^2_{1})/2|                                                                        
    else if(dmsqscheme == kNuDmsqFogliLisi && omp == kNuMixSinsq2Theta)
    {
        fNuOsc3Flavour.SetOscParametersSSq2TDmAtmSol(mixing12,mixing13,mixing23,dmsq,Dmsq,cpv);
    }
    // mixing params are: sin^2(theta)                                                                                                                                             
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |m^2_{3} - (m^2_{1} + m^2_{1})/2|                                                                         
    else if(dmsqscheme == kNuDmsqFogliLisi && omp == kNuMixSinsqTheta)
    {
        fNuOsc3Flavour.SetOscParametersDmsqAtmSol(mixing12,mixing13,mixing23,dmsq,Dmsq,cpv);
    }
    // mixing paras are: sin^2(2*theta)
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |Dm^2_{31}|
    else if(dmsqscheme == kNuDmsq12andAbs31 && omp == kNuMixSinsq2Theta)
    {
        fNuOsc3Flavour.SetOscParametersSSq2TDmsq13(mixing12,mixing13,mixing23,dmsq,Dmsq,cpv);
    }
    // mixing paras are: sin^2(theta)
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |Dm^2_{31}|
    else if(dmsqscheme == kNuDmsq12andAbs31 && omp == kNuMixSinsqTheta)
    {
        fNuOsc3Flavour.SetOscParametersDmsq13(mixing12,mixing13,mixing23,dmsq,Dmsq,cpv);
    }
    //unknown
    else {
       LOG("X01",pFATAL) << "Can't handle choice of "
           << "\n oscillation mixing params: " << util::AsString(omp)
           << "\n and independent squared mass-splittings: " << util::AsString(dmsqscheme)
           << "\n in the context of 3-flavour oscillations";
      exit(1);
    }

    for(int i = 1; i <= fTotSpectrumPdf->GetNbinsX(); i++) 
    {
      double Etrue = fTotSpectrumPdf->GetBinCenter(i);
      double p_numu_numu = fNuOsc3Flavour.CalcMatterOscProb(kPdgNuMu, kPdgNuMu, Etrue);
      double fsig0 = util::example01::SigSpectrumUnosc(Etrue);
      double fsig  = fsig0 * p_numu_numu;
      double fbkg  = util::example01::BkgSpectrum(Etrue);
      double ftot  = fsig + fbkg;
      fSigSpectrumPdf->SetBinContent(i, fsig);
      fBkgSpectrumPdf->SetBinContent(i, fbkg);
      fTotSpectrumPdf->SetBinContent(i, ftot);
    }

  } else {
    //
    // in this example handle only 3-flavour oscillations
    //
    LOG("X01",pERROR) << "Can not handle hypothesis: " << fPhysHypothesis;
    return false;
  }

  LOG("X01",pDEBUG) << "Done generating predictions";

  return true;
}
//________________________________________________________________________________________
void X01Model::RandomizeSystematics(void)
{
  // No systematics in this example
}
//________________________________________________________________________________________
void X01Model::TweakSystematic(
  string /*syst_name*/, double /*twkdial*/, Option_t * /*opt*/)
{
  // No systematics in this example
}
//________________________________________________________________________________________
void X01Model::TweakSystematic(
  PhysParamId_t /*syst*/, double /*twkdial*/, Option_t * /*opt*/)
{
  // No systematics in this example
}
//________________________________________________________________________________________
PhysParamList X01Model::Tweaks(void)
{
  PhysParamList list;
  // No systematics in this example
  return list;
}
//________________________________________________________________________________________
void X01Model::ResetSystematics(void)
{
  // No systematics in this example
}
//________________________________________________________________________________________
void X01Model::PrintSystematics(ostream & stream) const
{
  // No systematics in this example  
  stream << endl;
}
//________________________________________________________________________________________
bool X01Model::Generate (const FitParamList & fp, Option_t * opt)
{
  // extract the values of params driving the oscillation, decoherence or decay model
  PhysHypothesis ph(fp);

  // call the main Generate() method
  return this->Generate(ph,opt);
}
//________________________________________________________________________________________
bool X01Model::Write(TObjArray * array, const char * tag, Option_t * /*write_opt*/)
{
  if(!array) return false;

  util::SetDefaultStyle();

  TH1D * tot = new TH1D(*fTotSpectrumPdf);
  TH1D * sig = new TH1D(*fSigSpectrumPdf);
  TH1D * bkg = new TH1D(*fBkgSpectrumPdf);

  tot->SetName( Form("%s_%s_%s", this->Name().c_str(), "tot", tag) );
  sig->SetName( Form("%s_%s_%s", this->Name().c_str(), "sig", tag) );
  bkg->SetName( Form("%s_%s_%s", this->Name().c_str(), "bkg", tag) );

  array->Add(tot);
  array->Add(sig);
  array->Add(bkg);

  return true;
}
//________________________________________________________________________________________
bool X01Model::Write(
  const char * filename, const char * tag, Option_t * write_opt, Option_t * file_opt)
{
  TObjArray * array = new TObjArray();
  this->Write(array,tag,write_opt);

  Option_t * used_file_opt = (file_opt == 0) ? "recreate" : file_opt;
  TFile f(filename, used_file_opt);
  array->Write();
  f.Close();

  return true;
}
//________________________________________________________________________________________
double X01Model::GetFitStatisticPenalty (Option_t * /*opt*/) const
{
  // No systematics in this example

  return 0;
}
//________________________________________________________________________________________

