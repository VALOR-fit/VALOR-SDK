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

#include "NDFit/NDData.h"
#include "FitCore/FitModelABC.h"
#include "FitCore/FitParamList.h"
#include "FitCore/PhysHypothesis.h"
#include "Utilities/Rnd.h"
#include "Utilities/UtlStyle.h"
#include "Utilities/UtlNumStat.h"
#include "Messenger/Messenger.h"

using namespace t2kana;
using namespace t2kana::conv;

//________________________________________________________________________________________
NDData::NDData() :
FitDataABC()
{

}
//________________________________________________________________________________________
NDData::~NDData() 
{

}
//________________________________________________________________________________________
string NDData::Name(void)
{
  return "LBNxND";
}
//________________________________________________________________________________________
bool NDData::Read(string /*file*/, Option_t * /*opt*/)
{
  return true;
}
//________________________________________________________________________________________
bool NDData::Write(TObjArray * /*array*/, const char * /*tag*/)
{
  return true;
}
//________________________________________________________________________________________
bool NDData::Write(const char * /*filename*/, const char * /*tag*/, const char * /*opt_file*/)
{
  return true;
}
//________________________________________________________________________________________
bool NDData::Generate(
    FitModelABC * model_abc, const FitParamList & fp, bool fluctuate)
{
  LOG("NDData",pINFO) << "Generating a toy-MC expt with the following params: " << fp;
  LOG("NDData",pINFO) << "Stat. fluctuations are turned-" << ((fluctuate)? "ON" : "OFF");

  bool ok = model_abc->Generate(fp);
  if(!ok) {
    LOG("NDData",pERROR) 
       << "Could not generate predictions with model: " << model_abc->Name()
       << " and input params: " << fp;
    return false;
  }

  return this->Generate(model_abc,fp,fluctuate);
}
//________________________________________________________________________________________
bool NDData::Generate(
   FitModelABC * model_abc, const PhysHypothesis & ph, bool fluctuate)
{
  LOG("NDData",pINFO) << "Generating a toy-MC expt with the following params: " << ph;
  LOG("NDData",pINFO) << "Stat. fluctuations are turned-" << ((fluctuate)? "ON" : "OFF");

  bool ok = model_abc->Generate(ph);
  if(!ok) {
    LOG("NDData",pERROR) 
       << "Could not generate predictions with model: " << model_abc->Name()
       << " with systematic tweaks: " << model_abc->Tweaks()
       << " and input params: " << ph;
    return false;
  }

  return this->Generate(model_abc,fluctuate);
}
//________________________________________________________________________________________
bool NDData::Generate(FitModelABC * /*model_abc*/, bool /*fluctuate*/)
{

  return true;
}
//________________________________________________________________________________________
int NDData::NumOfBins(void)
{
  return 0;
}
//________________________________________________________________________________________
double NDData::Statistic(FitModelABC* model_abc, const FitParamList & fp, Option_t * opt)
{
  bool ok = model_abc->Generate(fp);
  assert(ok);

  return this->Statistic(model_abc,opt);
}
//________________________________________________________________________________________
double NDData::Statistic(FitModelABC * model_abc, const PhysHypothesis & ph, Option_t * opt)
{
  bool ok = model_abc->Generate(ph);
  assert(ok);

  return this->Statistic(model_abc,opt);  
}
//________________________________________________________________________________________
double NDData::Statistic(FitModelABC * model_abc, Option_t * /*opt*/) 
{
  TObjArray * array = new TObjArray();
  model_abc->Write(array, "model");

  // Get predictions from array

  // Calculate statistic
  double S = 0;

  return S;
}
//________________________________________________________________________________________

