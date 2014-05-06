//________________________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <cassert>

#include <TFile.h>
#include <TObjArray.h>
#include <TH1D.h>

#include "Examples/X01/X01Data.h"
#include "Examples/X01/X01Const.h"
#include "Examples/X01/X01Model.h"
#include "FitCore/FitParamList.h"
#include "FitCore/PhysHypothesis.h"
#include "Utilities/Rnd.h"
#include "Utilities/UtlStyle.h"
#include "Utilities/UtlNumStat.h"
#include "Messenger/Messenger.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
X01Data::X01Data() :
FitDataABC()
{
  const int    n   = example01::kNEbins;
  const double min = example01::kEmin;
  const double max = example01::kEmax;

  fTotSpectrumData = new TH1D("data_tot", "", n, min, max);
}
//________________________________________________________________________________________
X01Data::~X01Data() 
{
  delete fTotSpectrumData;
}
//________________________________________________________________________________________
string X01Data::Name(void)
{
  return "X01";
}
//________________________________________________________________________________________
bool X01Data::Read(string /*file*/, Option_t * /*opt*/)
{
  return true;
}
//________________________________________________________________________________________
bool X01Data::Write(
  TObjArray * array, const char * tag, Option_t * /*write_opt*/)
{
  if(!array) return false;
  util::SetDefaultStyle();
  TH1D * tot = new TH1D(*fTotSpectrumData);
  tot->SetName( Form("%s_%s_%s", this->Name().c_str(), "tot", tag) );
  array->Add(tot);
  return true;
}
//________________________________________________________________________________________
bool X01Data::Write(
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
bool X01Data::Generate(
   FitModelABC * model_abc, const FitParamList & fp, bool fluctuate)
{
  LOG("X01",pINFO) << "Generating a toy-MC expt with the following params: " << fp;
  LOG("X01",pINFO) << "Stat. fluctuations are turned-" << ((fluctuate)? "ON" : "OFF");

  bool ok = model_abc->Generate(fp);
  if(!ok) {
    LOG("X01",pERROR) 
       << "Could not generate predictions with model: " << model_abc->Name()
       << " and input params: " << fp;
    return false;
  }

  return this->Generate(model_abc,fp,fluctuate);
}
//________________________________________________________________________________________
bool X01Data::Generate(
   FitModelABC * model_abc, const PhysHypothesis & ph, bool fluctuate)
{
  LOG("X01",pINFO) << "Generating a toy-MC expt with the following params: " << ph;
  LOG("X01",pINFO) << "Stat. fluctuations are turned-" << ((fluctuate)? "ON" : "OFF");

  //X01Model * model = dynamic_cast<X01Model * > (model_abc);
  //assert(model);

  bool ok = model_abc->Generate(ph);
  if(!ok) {
    LOG("X01",pERROR) 
       << "Could not generate predictions with model: " << model_abc->Name()
       << " with systematic tweaks: " << model_abc->Tweaks()
       << " and input params: " << ph;
    return false;
  }

  return this->Generate(model_abc,fluctuate);
}
//________________________________________________________________________________________
bool X01Data::Generate(FitModelABC * model_abc, bool fluctuate)
{
  fTotSpectrumData->Reset();

  TObjArray * array = new TObjArray();
  model_abc->Write(array, "model");

  const char * name = Form("%s_%s",model_abc->Name().c_str(), "tot_model");
  TH1D * mc_tot = (TH1D*) array->FindObject(name);
  assert(mc_tot);

  for(int i = 1; i <= fTotSpectrumData->GetNbinsX(); i++)
  {
     double nmc    = mc_tot->GetBinContent(i);
     double ndata  = (fluctuate) ? Rnd::Instance()->RndToyMCstat().Poisson(nmc) : nmc;
     fTotSpectrumData->SetBinContent(i,ndata);
  }

  return true;
}
//________________________________________________________________________________________
int X01Data::NumOfBins(void)
{
  return example01::kNEbins;
}
//________________________________________________________________________________________
double X01Data::Statistic(FitModelABC* model_abc, const FitParamList & fp, Option_t * opt)
{
  //X01Model * model = dynamic_cast<X01Model * > (model_abc);
  //assert(model);

  bool ok = model_abc->Generate(fp);
  assert(ok);

  return this->Statistic(model_abc,opt);
}
//________________________________________________________________________________________
double X01Data::Statistic(FitModelABC * model_abc, const PhysHypothesis & ph, Option_t * opt)
{
  bool ok = model_abc->Generate(ph);
  assert(ok);

  return this->Statistic(model_abc,opt);  
}
//________________________________________________________________________________________
double X01Data::Statistic(FitModelABC * model_abc, Option_t * /*opt*/) 
{
  TObjArray * array = new TObjArray();
  model_abc->Write(array, "model");

  const char * name = Form("%s_%s",model_abc->Name().c_str(), "tot_model");
  TH1D * mc_tot = (TH1D*) array->FindObject(name);
  assert(mc_tot);

  double S = 0;

  for(int i = 1; i <= fTotSpectrumData->GetNbinsX(); i++)
  {
     double ndata  = fTotSpectrumData->GetBinContent(i);
     double nmc    = mc_tot->GetBinContent(i);
     double dS     = util::ChisqPoisson(ndata, nmc);
     S += dS;
  }

  return S;
}
//________________________________________________________________________________________

