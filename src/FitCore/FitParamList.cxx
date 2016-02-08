//________________________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <cassert>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>

#include <TSystem.h>

#include "Conventions/Constants.h"
#include "Conventions/PhysParamId.h"
#include "Conventions/Opt.h"
#include "FitCore/PhysParamList.h"
#include "FitCore/FitParamList.h"

#include "Utilities/NameRegistry.h"
#include "Utilities/UtlSystem.h"

using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::setw;
using std::setprecision;

using namespace valor;
using namespace valor::conv;

ClassImp(FitParamList)

//________________________________________________________________________________________
namespace valor {
  ostream & operator << (ostream & stream, const FitParamList & plst)
  {
    plst.Print(stream);
    return stream;
  }
}
//________________________________________________________________________________________
FitParamList::FitParamList() :
TNamed()
{
  this->Init(0);
}
//________________________________________________________________________________________
FitParamList::FitParamList(int nparams) :
TNamed()
{
  this->Init(nparams);
}
//________________________________________________________________________________________
FitParamList::FitParamList(const FitParamList & plst) :
TNamed()
{
  this->Init(plst.fNParams);
  this->Copy(plst);
}
//________________________________________________________________________________________
FitParamList::~FitParamList()
{
  this->CleanUp();
}

//________________________________________________________________________________________
bool FitParamList::AddParam (int ip, PhysParamId_t p, 
  double init, double min, double max, double step, bool fix, bool nuis, string name)
{
  if (ip < 0 || ip >= fNParams) {
    this->Print();
    cerr<<"FitParamList/AddParam >> Failed to add parameter "<<util::NameRegistry::Instance()->AsString(p) <<" ("<<name
             <<") to slot "<<ip<<" (max params "<<fNParams<<") "<<endl;
    exit(-1);
  }
  
  if (this->Exists(p)) {
    this->Print();
    cerr<<"FitParamList/AddParam >> Failed to add parameter "<<util::NameRegistry::Instance()->AsString(p) <<" ("<<name
             <<"): parameter already exists."<<endl;
    exit(-1);
  }

  fIdMap.insert(map<PhysParamId_t, int>::value_type(p,ip));

  if(name.size()==0) {
    name = util::NameRegistry::Instance()->AsString(p);
  }

  fName         [ip].SetString(name.c_str());
  fBestFitValue [ip] = -99999999;
  fCurrValue    [ip] = init;
  fInitValue    [ip] = init;
  fMinValue     [ip] = min; 
  fMaxValue     [ip] = max; 
  fStepSize     [ip] = step; 
  fIsFixed      [ip] = fix;  
  fIsNuisance   [ip] = nuis;  

  return true;
}
//________________________________________________________________________________________
bool FitParamList::Exists(PhysParamId_t p) const
{
  map<PhysParamId_t, int>::const_iterator iter = fIdMap.find(p);
  if(iter != fIdMap.end()) return true;
  return false;
}
//________________________________________________________________________________________
void FitParamList::Update(PhysParamId_t p, double v)
{
  fCurrValue [this->Id(p)] = v;
}
//________________________________________________________________________________________
void FitParamList::SetInitValue(PhysParamId_t p, double v)
{
  fInitValue [this->Id(p)] = v;
  fCurrValue [this->Id(p)] = v;
}                  

//________________________________________________________________________________________
void FitParamList::SetCurrentValue(PhysParamId_t p, double v)
{
  fCurrValue [this->Id(p)] = v;
}                  
//________________________________________________________________________________________
void FitParamList::ResetCurrToInitValues(void)
{
  for(int ip = 0; ip < fNParams; ip++) {
     fCurrValue[ip] = fInitValue[ip];
  }
}
//________________________________________________________________________________________
void FitParamList::CopyCurrToBestFitValues(void)
{
  for(int ip = 0; ip < fNParams; ip++) {
     fBestFitValue[ip] = fCurrValue[ip];
  }
  fBestFitStatistic = fCurrStatistic;
}
//________________________________________________________________________________________
void FitParamList::CopyBestFitToCurrValues(void)
{
  for(int ip = 0; ip < fNParams; ip++) {
     fCurrValue[ip] = fBestFitValue[ip];
  }
  fCurrStatistic = fBestFitStatistic;

}
//________________________________________________________________________________________
void FitParamList::CopyBestFitToInitValues(void)
{
  for(int ip = 0; ip < fNParams; ip++) {
     fInitValue[ip] = fBestFitValue[ip];
  }
}
//________________________________________________________________________________________
void FitParamList::CopyBestFitToInitValues(const FitParamList & plst)
{
  if(plst.GetParListName() != fParListName) {
    cerr << "Cannot copy BestFit values in FitParamList of type " << plst.GetParListName()
	 << " to InitValues in FitParamList of type " << fParListName << endl;
    exit(1);
  }
  for(int ip = 0; ip < fNParams; ip++) {
    fInitValue[ip] = plst.BestFitValueByIndex(ip);
  }
}
//________________________________________________________________________________________
void FitParamList::SetCovMatrix(TMatrixDSym & covmatrix)
{
  TMatrixDSym & m = *fCovMatrix;
  for(int ip = 0; ip < fNParams; ip++) {
    for(int jp = 0; jp < fNParams; jp++) {
       m[ip][jp] = covmatrix[ip][jp];
    }
  }
}
//________________________________________________________________________________________
void FitParamList::SetErrors(double * errplus, double * errminus, double * errmigrad)
{
  for(int ip = 0; ip < fNParams; ip++) {
     fPlusError  [ip] = errplus  [ip];
     fMinusError [ip] = errminus [ip];
     fMigradError[ip] = errmigrad[ip];
     if(errplus[ip] > kASmallNum || errminus[ip] > kASmallNum)
       fMinosErrorsSet = true;
  }
  fMigradErrorsSet = true;
}
//________________________________________________________________________________________
void FitParamList::SetMigradErrors(vector<double> err)
{
  if(err.size() != (unsigned int)this->Size()) {
    cerr << "FitterKernelMinuit2/SetMigradErrors >> Error vector size: " << err.size()
	 << " is not the same size as the number of free parameters: "
	 << this->GetNumberFreeParameters() << endl;
    exit(1);
  }
  
  for(int ip = 0; ip < fNParams; ip++) {
    fMigradError [ip] = err[ip];
  }
  fMigradErrorsSet = true;
}
//________________________________________________________________________________________
void FitParamList::SetHesseErrors(vector<double> err)
{
  if(err.size() != (unsigned int)this->Size()) {
    cerr << "FitterKernelMinuit2/SetHesseErrors >> Error vector size: " << err.size()
	 << " is not the same size as the number of free parameters: "
	 << this->GetNumberFreeParameters() << endl;
    exit(1);
  }
  
  for(int ip = 0; ip < fNParams; ip++) {
    fHesseError [ip] = err[ip];
  }
  fHesseErrorsSet = true;
}
//________________________________________________________________________________________
void FitParamList::SetMinosErrors(vector<double> plus, vector<double> minus)
{
  if((int)plus.size() != this->Size()) {
    cerr << "FitterKernelMinuit2/SetMinosErrors >> Error vector size: " << plus.size()
	 << " is not the same size as the number of parameters: "
	 << this->GetNumberFreeParameters() << endl;
    exit(1);
  }
  if(plus.size() != minus.size()) {
    cerr << "FitterKernelMinuit2/SetMinosErrors >> Error vector size (plus): " << plus.size()
	 << " is not the same size as error vector size (minus): " << minus.size() << endl;
    exit(1);
  }

  for(int ip = 0; ip < fNParams; ip++) {
    fPlusError [ip] = plus [ip];
    fMinusError[ip] = minus[ip];
  }
  
  fMinosErrorsSet = true;
}
//________________________________________________________________________________________
void FitParamList::FixParameter(PhysParamId_t p)
{
  fIsFixed [this->Id(p)] = true;
}
//________________________________________________________________________________________
void FitParamList::FixParameterByIndex(int ip)
{
  fIsFixed [ip] = true;
}
//________________________________________________________________________________________
void FitParamList::FreeParameter(PhysParamId_t p)
{
  fIsFixed [this->Id(p)] = false;
}
//________________________________________________________________________________________
void FitParamList::FreeParameterByIndex(int ip)
{
  fIsFixed [ip] = false;
}
//________________________________________________________________________________________
void FitParamList::SetNuisance(PhysParamId_t p, bool tf)
{
  fIsNuisance [this->Id(p)] = tf;
}
//________________________________________________________________________________________
FitParamList * FitParamList::FreeParameterSubset(void)
{
  int nfree = this->GetNumberFreeParameters();

  FitParamList * fparm = new FitParamList(nfree);

  int ipc = 0;
  for(int ip = 0; ip < fNParams; ip++) {
    if(fIsFixed[ip]) continue;
    PhysParamId_t p = this->WhichParam(ip);
    double init = fInitValue[ip];
    double min  = fMinValue[ip];
    double max  = fMaxValue[ip];
    double step = fStepSize[ip];
    bool   nuis = fIsNuisance[ip];
    fparm->AddParam(ipc, p, init, min, max, step, false, nuis);
    ipc++;
  }

  return fparm;
}
//________________________________________________________________________________________
void FitParamList::SetTo(TVirtualFitter * fitter)
{
  if(!fitter) return;
  fitter->Clear();
  map<valor::conv::PhysParamId_t, int>::const_iterator iter = fIdMap.begin();
  for ( ; iter != fIdMap.end(); ++iter) {
    PhysParamId_t p = iter->first;
    int iparam = iter->second;
    fitter->SetParameter(
      iparam,
      this->Name(p).c_str(),
      this->InitValue(p),
      this->StepSize(p),
      this->MinValue(p),
      this->MaxValue(p) );

    if(this->IsFixed(p)) {
     fitter->FixParameter(iparam);
    }
  }
}
//________________________________________________________________________________________
void FitParamList::SetTo(ROOT::Minuit2::MnUserParameters & params)
{
  //if(!params) return;
  //params.Clear();
  for(int ip = 0; ip < fNParams; ip++) {
    valor::conv::PhysParamId_t p = WhichParam(ip);
    //keep mixing angles in physical region
    if(util::IsMixingAngle(p)) {
      params.Add(this->Name(p).c_str(), this->InitValue(p), this->StepSize(p), 0, 1);
    }
    /*
    //stop splines going too far from nominal
    //(normalisations will eventually make poisson term grow too, not necessarily true for splines)
    else if(util::ArrIdx::Instance()->XSecSystIdx(p) >= 0) {
      if(kXSecSystHasWghtSplines[util::ArrIdx::Instance()->XSecSystIdx(p)])
	params.Add(this->Name(p).c_str(), this->InitValue(p), this->StepSize(p), this->MinValue(p), this->MaxValue(p));
      else
	params.Add(this->Name(p).c_str(), this->InitValue(p), this->StepSize(p));
    }
    //sk energy scale is also special
    if(p == kNuisSuperkEnergyScale)
	params.Add(this->Name(p).c_str(), this->InitValue(p), this->StepSize(p), this->MinValue(p), this->MaxValue(p));
    */
    // confine everything else
    else {
      params.Add(this->Name(p).c_str(), this->InitValue(p), this->StepSize(p), this->MinValue(p), this->MaxValue(p));
    }
    /*
    //everything else is free
    else {
      params.Add(this->Name(p).c_str(), this->InitValue(p), this->StepSize(p));
    }
    */
    if(this->IsFixed(p)) {
     params.Fix(ip);
    }
  }//loop over params
}
//________________________________________________________________________________________
void FitParamList::SetTo(ROOT::Math::Minimizer * fitter)
{
  if(!fitter) return;
  fitter->Clear();
  // map<valor::conv::PhysParamId_t, int>::const_iterator iter = fIdMap.begin();
  // for ( ; iter != fIdMap.end(); ++iter) {
  for(int ip = 0; ip < fNParams; ip++) {  
    //    PhysParamId_t p = iter->first;
    PhysParamId_t p = WhichParam(ip);
    //    int iparam = iter->second;
    int iparam = ip;

    if(this->IsFixed(p)) {
      fitter->SetFixedVariable(
       iparam,
       this->Name(p).c_str(), 
       this->InitValue(p));
    }
    else{
      fitter->SetLimitedVariable(
	  iparam, 
	  this->Name(p).c_str(), 
	  this->InitValue(p), 
	  this->StepSize(p),
	  this->MinValue(p),
	  this->MaxValue(p) );
    }
  }
}
//________________________________________________________________________________________
void FitParamList::SetTo(ROOT::Fit::Fitter * fitter)
{
  if(!fitter) return;
  // map<valor::conv::PhysParamId_t, int>::const_iterator iter = fIdMap.begin();
  // for ( ; iter != fIdMap.end(); ++iter) {
  double par0[fNParams];
  for(int ip = 0; ip < fNParams; ip++) {
    PhysParamId_t p = WhichParam(ip);
    par0[ip] = this->InitValue(p);
  }
  (fitter->Config()).SetParamsSettings(fNParams,par0);

  for(int ip = 0; ip < fNParams; ip++) {  
    //    PhysParamId_t p = iter->first;
    PhysParamId_t p = WhichParam(ip);
    //    int iparam = iter->second;
    int iparam = ip;

    if(this->IsFixed(p)) {
      (fitter->Config()).ParSettings(iparam).Set(this->Name(p).c_str(),this->InitValue(p));
    }
    else{
      (fitter->Config()).ParSettings(iparam).Set(this->Name(p).c_str(),this->InitValue(p),this->StepSize(p),this->MinValue(p),this->MaxValue(p) );
    }
  }
}
//________________________________________________________________________________________
void FitParamList::Sync(std::vector<double> par)
{
  if(par.size() <= 0) return;
  for(int ip = 0; ip < fNParams; ip++) {
     fCurrValue[ip] = par[ip];
  }
}
//________________________________________________________________________________________
void FitParamList::Sync(const double * par)
{
  if(!par) return;
  for(int ip = 0; ip < fNParams; ip++) {
     fCurrValue[ip] = par[ip];
  }
}
//________________________________________________________________________________________
PhysParamList FitParamList::AsPhysParamList(Option_t * option) const
{
  PhysParamList pl;
  TString opt(option);
  map<PhysParamId_t, int>::const_iterator iter = fIdMap.begin();
  for( ; iter != fIdMap.end(); ++iter)
  {
    PhysParamId_t p = iter->first;
    int ip = iter->second;
    double v = 0;
    if      ( opt.Contains("initial")  ) v = fInitValue[ip];
    else if ( opt.Contains("current")  ) v = fCurrValue[ip];
    else if ( opt.Contains("best-fit") ) v = fBestFitValue[ip];
    pl.SetValue(p,v);
  }
  return pl;
}
//________________________________________________________________________________________
void FitParamList::FromPhysParamList(valor::PhysParamList pl, Option_t * option, bool skip_osc_params)
{
  TString opt(option);
  if (opt.Contains("skiposc")) {skip_osc_params = true;}
  
  bool silently_pass_missing_parameters = opt.Contains("silent");
  bool type_to_update_not_found = false;
  
  map<PhysParamId_t, int>::const_iterator iter = fIdMap.begin();
  for( ; iter != fIdMap.end(); ++iter)
  {
    PhysParamId_t p = iter->first;
    if(skip_osc_params && !util::IsSystematic(p)) {continue;}
    
    int ip = iter->second;
    if (pl.Contains(p)) {
      double input = pl.Value(p);
      if      ( opt.Contains("initial")  ) {fInitValue[ip]    = input;}
      else if ( opt.Contains("current")  ) {fCurrValue[ip]    = input;}
      else if ( opt.Contains("best-fit") ) {fBestFitValue[ip] = input;}
      else {
        type_to_update_not_found = true;
      }
    }
    else {
      if (!silently_pass_missing_parameters) {
        cout << "FitParamList/FromPhysParamList >> Skipping parameter not found in PhysParamList: "
             << util::NameRegistry::Instance()->AsString(p) <<endl;
      }
    }
  } //iter
  
  if (type_to_update_not_found) {
    cerr<<"FitParamList/FromPhysParamList >> Could not find appropriate parameter type to update in option string "
        <<option<<". Please specify one of 'initial', 'current' or 'best-fit'"<<endl;
        util::Die(-1);
  }
  
  return;
}
//________________________________________________________________________________________
int FitParamList::GetNumberFreeParameters(void)
{
  int nparams = 0;

  for(int ip=0; ip<fNParams;  ip++) {
    bool is_fixed = fIsFixed[ip];
    if(!is_fixed) {
      nparams++;
    }
  }
  return nparams;
}
//________________________________________________________________________________________
int FitParamList::GetNumberParameters(void)
{
  int nparams = 0;

  for(int ip=0; ip<fNParams;  ip++) {
    nparams++;
  }
  return nparams;
}
//________________________________________________________________________________________
void FitParamList::ClearStatusCodes(void)
{
  fStatusCodes.clear();
}
//________________________________________________________________________________________
void FitParamList::SetStatusCode(string st, int code)
{
  fStatusCodes[st] = code;
}
//________________________________________________________________________________________
int FitParamList::GetStatusCode(string st) const
{
  map<string,int>::const_iterator sciter = fStatusCodes.find(st);
  if(sciter != fStatusCodes.end()) {
    return sciter->second;
  }
  return -9876;
}
//________________________________________________________________________________________
void FitParamList::Print(ostream & stream) const
{
  if(fShowBestFitValuesOnly) {
     fShowBestFitValuesOnly = false; // has one-off effect
     stream << endl
            << setw(6)  << "id"
            << setw(80) << "name"
            << setw(15) << "best-fit"
            << endl;
     for(int ip=0; ip<fNParams;  ip++) {
       stream << setw(6)  << ip 
              << setw(80) << fName[ip].GetString().Data()
              << setprecision(6)
              << setw(15) << fBestFitValue[ip]
              << endl;
     }
  } 
  else
  if(fShowCurrValuesOnly) {
     fShowCurrValuesOnly = false; // has one-off effect
     stream << endl
            << setw(6)  << "id"
            << setw(80) << "name"
            << setw(15) << "curr"
            << endl;
     for(int ip=0; ip<fNParams;  ip++) {
       stream << setw(6)  << ip 
              << setw(80) << fName[ip].GetString().Data()
              << setprecision(6)
              << setw(15) << fCurrValue[ip]
              << endl;
     }
  } 
  else 
  {
     stream << endl
            << setw(6)  << "id"
            << setw(35) << "name"
            << setw(15) << "curr"
            << setw(15) << "init"
            << setw(15) << "min"
            << setw(15) << "max"
            << setw(15) << "step"
            << setw(8)  << "fixed?"
            << setw(10) << "nuisance?"
            << endl;
     for(int ip=0; ip<fNParams;  ip++) {
       stream << setw(6)  << ip 
              << setw(35) << fName[ip].GetString().Data()
              << setprecision(6)
              << setw(15) << fCurrValue[ip]
              << setw(15) << fInitValue[ip]
              << setw(15) << fMinValue[ip]
              << setw(15) << fMaxValue[ip]
              << setw(15) << fStepSize[ip]
              << setw(8) << ((fIsFixed[ip])    ? "yes" : "no")
              << setw(8) << ((fIsNuisance[ip]) ? "yes" : "no")
              << endl;
     }
  }

  stream << "Status codes: " << endl;  
  map<string,int>::const_iterator sciter = fStatusCodes.begin();
  for( ; sciter != fStatusCodes.end(); ++sciter) {
    string name = sciter->first;
    int code = sciter->second;
    stream << setw(20) << name << setw(10) << code << endl;  
  }
  stream << "Fit time: " << fFitTime  << " sec" << ", tries: " << fFitTries << endl;
  stream << "Best-fit statistic: " << fBestFitStatistic << endl;  
}
//________________________________________________________________________________________
void FitParamList::Print(Option_t *) const
{
  this->Print(cout);
}
//________________________________________________________________________________________
FitParamList * FitParamList::NamedParameterList(string name)
{
  //
  // 3-flavour oscillation fit with/without systematics included 
  //

  if(name == "nuosc3f") {
    FitParamList * fp = FitParamList::NuOsc3FlavourFit();
    fp -> SetParListName(name);
    fp -> SetSystMode(0);
    return fp;
  }
  //~ if(name == "nuosc3f+nuisance") {
    //~ FitParamList * fp = FitParamList::NuOsc3FlavourFitWithAllSyst();
    //~ fp -> SetParListName(name);
    //~ fp -> SetSystMode(1);
    //~ return fp;
  //~ }

  //
  // 3-flavour oscillation fit with various oscillation param subsets either fixed or
  // treated as nuisance paramers.
  //

  if(name == "nuosc3f_12constrain") {
    FitParamList * fp = FitParamList::NuOsc3FlavourFit();
    fp -> SetNuisance (kPhypMixing12, true);
    fp -> SetNuisance (kPhypDm12sq,   true);
    fp -> SetParListName(name);
    fp -> SetSystMode(0);
    return fp;
  }
  //~ if(name == "nuosc3f_12constrain+nuisance") {
    //~ FitParamList * fp = FitParamList::NuOsc3FlavourFitWithAllSyst();
    //~ fp -> SetNuisance (kPhypMixing12, true);
    //~ fp -> SetNuisance (kPhypDm12sq,   true);
    //~ fp -> SetParListName(name);
    //~ fp -> SetSystMode(1);
    //~ return fp;
  //~ }
  if(name == "nuosc3f_12fix") {
    FitParamList * fp = FitParamList::NuOsc3FlavourFit();
    fp -> FixParameter (kPhypMixing12);
    fp -> FixParameter (kPhypDm12sq  );
    fp -> SetParListName(name);
    fp -> SetSystMode(0);
    return fp;
  }
  //~ if(name == "nuosc3f_12fix+nuisance") {
    //~ FitParamList * fp = FitParamList::NuOsc3FlavourFitWithAllSyst();
    //~ fp -> FixParameter (kPhypMixing12);
    //~ fp -> FixParameter (kPhypDm12sq  );
    //~ fp -> SetParListName(name);
    //~ fp -> SetSystMode(1);
    //~ return fp;
  //~ }

/*
  if(name == "nuosc3f_numudisapp_fix4") {
    FitParamList * fp = FitParamList::NuOsc3FlavourFit();
    fp -> FixParameter (kPhypMixing13);
    fp -> FixParameter (kPhypMixing12);
    fp -> FixParameter (kPhypCPV);
    fp -> FixParameter (kPhypDm12sq);
    fp -> SetParListName(name);
    fp -> SetSystMode(0);
    return fp;
  }
  if(name == "nuosc3f_numudisapp_fix4+nuisance") {
    FitParamList * fp = FitParamList::NuOsc3FlavourFitWithAllSystDisap();
    fp -> FixParameter (kPhypMixing13);
    fp -> FixParameter (kPhypMixing12);
    fp -> FixParameter (kPhypCPV);
    fp -> FixParameter (kPhypDm12sq);
    fp -> SetParListName(name);
    fp -> SetSystMode(1);
    return fp;
  }

  if(name == "nuosc3f_nueapp_fix4") {
    FitParamList * fp = FitParamList::NuOsc3FlavourFit();
    fp -> FixParameter (kPhypMixing23);
    fp -> FixParameter (kPhypMixing12);
    fp -> FixParameter (kPhypDm23sq);
    fp -> FixParameter (kPhypDm12sq);
    fp -> SetParListName(name);
    fp -> SetSystMode(0);
    return fp;
  }
  if(name == "nuosc3f_nueapp_fix4+nuisance") {
    FitParamList * fp = FitParamList::NuOsc3FlavourFitWithAllSystApp();
    fp -> FixParameter (kPhypMixing23);
    fp -> FixParameter (kPhypMixing12);
    fp -> FixParameter (kPhypDm23sq);
    fp -> FixParameter (kPhypDm12sq);
    fp -> SetParListName(name);
    fp -> SetSystMode(1);
    return fp;
  }
*/

  if(name == "nuosc3f_dcpsensitivity_fix5") {
    FitParamList * fp = FitParamList::NuOsc3FlavourFit();
    fp -> FixParameter (kPhypMixing13);
    fp -> FixParameter (kPhypMixing12);
    fp -> FixParameter (kPhypMixing23);
    fp -> FixParameter (kPhypDm12sq);
    fp -> FixParameter (kPhypDm23sq);
    fp -> SetParListName(name);
    fp -> SetSystMode(0);
    return fp;
  }

  if(name == "nuosc3f_dcpsensitivity_fix3") {
    FitParamList * fp = FitParamList::NuOsc3FlavourFit();
    fp -> SetNuisance  (kPhypMixing23, true);
    fp -> SetNuisance  (kPhypDm23sq,   true);
    fp -> FixParameter (kPhypMixing13);
    fp -> FixParameter (kPhypMixing12);
    fp -> FixParameter (kPhypDm12sq);
    fp -> SetParListName(name);
    fp -> SetSystMode(4);//use future osc errors
    return fp;
  }

  if(name == "nuosc3f_dcpsensitivity") {
    FitParamList * fp = FitParamList::NuOsc3FlavourFit();
    fp -> SetNuisance  (kPhypMixing23, true);
    fp -> SetNuisance  (kPhypDm23sq,   true);
    fp -> SetNuisance  (kPhypMixing13, true);
    fp -> SetNuisance  (kPhypMixing12, true);
    fp -> SetNuisance  (kPhypDm12sq,   true);
    fp -> SetParListName(name);
    fp -> SetSystMode(3);//use current osc errors
    return fp;
  }

  //~ if(name == "nuosc3f_dcpsensitivity_fix3+nuisance") {
    //~ FitParamList * fp = FitParamList::NuOsc3FlavourFitWithAllSyst();
    //~ fp -> SetNuisance  (kPhypMixing23, true);
    //~ fp -> SetNuisance  (kPhypDm23sq,   true);
    //~ fp -> FixParameter (kPhypMixing13);
    //~ fp -> FixParameter (kPhypMixing12);
    //~ fp -> FixParameter (kPhypDm12sq);
    //~ fp -> SetParListName(name);
    //~ fp -> SetSystMode(5);//use future osc errors
    //~ return fp;
  //~ }

  //~ if(name == "nuosc3f_dcpsensitivity+nuisance") {
    //~ FitParamList * fp = FitParamList::NuOsc3FlavourFitWithAllSyst();
    //~ fp -> SetNuisance  (kPhypMixing23, true);
    //~ fp -> SetNuisance  (kPhypDm23sq,   true);
    //~ fp -> SetNuisance  (kPhypMixing13, true);
    //~ fp -> SetNuisance  (kPhypMixing12, true);
    //~ fp -> SetNuisance  (kPhypDm12sq,   true);
    //~ fp -> SetParListName(name);
    //~ fp -> SetSystMode(6);//use current osc errors
    //~ return fp;
  //~ }


/*
  //
  // 2-flavour numu-disappearance oscillation fit with/without systematics included 
  //

  if(name == "numudisapp2f") {
    FitParamList * fp = FitParamList::NumuDisapFit();
    fp -> SetParListName(name);
    fp -> SetSystMode(0);
    return fp;
  }
  if(name == "numudisapp2f+nuisance") {
    FitParamList * fp = FitParamList::NumuDisapFitWithAllSyst();
    fp -> SetParListName(name);
    fp -> SetSystMode(1);
    return fp;
  }

  //
  // numu decay fit
  //

  if(name == "decay") {
    FitParamList * fp = FitParamList::NumuDecayFit();
    fp -> SetParListName(name);
    fp -> SetSystMode(0);
    return fp;
  }

  //
  // numu decoherence fit
  //

  if(name == "decoh") {
    FitParamList * fp = FitParamList::NumuDecohFit();
    fp -> SetParListName(name);
    fp -> SetSystMode(0);
    return fp;
  }
  
  //
  // sterile neutrino fitting
  //
  if(name == "nuoscsterile4f") {
    FitParamList * fp = FitParamList::NuOscSterile4FlavourFit();
    fp -> SetParListName(name);
    fp -> SetSystMode(0);
    return fp;
  }
  if(name == "nuoscsterile4f+nuisance") {
    FitParamList * fp = FitParamList::NuOscSterile4FlavourFitWithAllSyst();
    fp -> SetParListName(name);
    fp -> SetSystMode(1);
    return fp;
  }
  if(name == "nuoscsterile5f") {
    FitParamList * fp = FitParamList::NuOscSterile5FlavourFit();
    fp -> SetParListName(name);
    fp -> SetSystMode(0);
    return fp;
  }
  if(name == "nuoscsterile5f+nuisance") {
    FitParamList * fp = FitParamList::NuOscSterile5FlavourFitWithAllSyst();
    fp -> SetParListName(name);
    fp -> SetSystMode(1);
    return fp;
  }
*/

  return 0;
}
//________________________________________________________________________________________
FitParamList * FitParamList::NuOsc3FlavourFit(int nuisance)
{
  FitParamList * fparm = new FitParamList(6+nuisance);
  FitParamList::Append3FlavourOscillation(fparm, 0);
  return fparm;
}
//________________________________________________________________________________________
FitParamList * FitParamList::NuOsc3FlavourBarFit(int nuisance)
{
  FitParamList * fparm = new FitParamList(12+nuisance);
  FitParamList::Append3FlavourOscillation(fparm, 0);
  FitParamList::Append3FlavourOscillationBar(fparm, 6);
  
  // Set initial values for the non-barred fit params
  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  switch(omp) {
    case(kNuMixSinsq2Theta):
      fparm->SetInitValue(kPhypMixing12   , conv::kGlobFitSinsq2Theta12);
      fparm->SetInitValue(kPhypMixing13   , conv::kGlobFitSinsq2Theta13);
      fparm->SetInitValue(kPhypMixing23   , conv::kGlobFitSinsq2Theta23);
      break;
    case(kNuMixSinsqTheta):
      fparm->SetInitValue(kPhypMixing12   , conv::kGlobFitSinsqTheta12);
      fparm->SetInitValue(kPhypMixing13   , conv::kGlobFitSinsqTheta13);
      fparm->SetInitValue(kPhypMixing23   , conv::kGlobFitSinsqTheta23);
      break;
    default:
      std::cerr<<"Unknown oscillation mixing parameter choice: "<<omp<<std::endl;
      exit(-1);
  }
  
  fparm->SetInitValue(kPhypCPV        , conv::kGlobFitDeltaCP);
  NuOscDmsqScheme_t odmsq = Opt::Instance()->GetEffectiveDmsqScheme();
  
  switch (odmsq) {
    case(kNuDmsqFogliLisi):
      fparm->SetInitValue(kPhypDm23sq,    conv::kGlobFitDmsqFL);
      fparm->SetInitValue(kPhypDm12sq,    conv::kGlobFitDm12sq);
      break;
    case(kNuDmsq12andAbs23):
      fparm->SetInitValue(kPhypDm23sq,    conv::kGlobFitDm23sq);
      fparm->SetInitValue(kPhypDm12sq,    conv::kGlobFitDm12sq);
      break;
    case(kNuDmsq12andAbs31):
      fparm->SetInitValue(kPhypDm12sq,    conv::kGlobFitDm12sq);
      if(Opt::Instance()->gMassHierarchyAssumed == kNmhNormal)
        fparm->SetInitValue(kPhypDm23sq,    conv::kGlobFitDm23sq + conv::kGlobFitDm12sq);
      else if(Opt::Instance()->gMassHierarchyAssumed == kNmhInverted)
        fparm->SetInitValue(kPhypDm23sq,    conv::kGlobFitDm23sq + conv::kGlobFitDm12sq);
      break;
    default:
      std::cerr<<"Unknown mass scheme: "<<odmsq<<std::endl;
      exit(-1);
  }
       
  return fparm;
}







// NEW BETABAR

//________________________________________________________________________________________
FitParamList * FitParamList::NuOsc3FlavourBarNormBarFit(int nuisance)
{
  FitParamList * fparm = new FitParamList(13+nuisance);  // 13 instead of 12
  FitParamList::Append3FlavourOscillation(fparm, 0);
  FitParamList::Append3FlavourOscillationBar(fparm, 6);
  FitParamList::AppendNormBar(fparm, 12);
  
  // Set initial values for the non-barred fit params
  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  switch(omp) {
    case(kNuMixSinsq2Theta):
      fparm->SetInitValue(kPhypMixing12   , conv::kGlobFitSinsq2Theta12);
      fparm->SetInitValue(kPhypMixing13   , conv::kGlobFitSinsq2Theta13);
      fparm->SetInitValue(kPhypMixing23   , conv::kGlobFitSinsq2Theta23);
      break;
    case(kNuMixSinsqTheta):
      fparm->SetInitValue(kPhypMixing12   , conv::kGlobFitSinsqTheta12);
      fparm->SetInitValue(kPhypMixing13   , conv::kGlobFitSinsqTheta13);
      fparm->SetInitValue(kPhypMixing23   , conv::kGlobFitSinsqTheta23);
      break;
    default:
      std::cerr<<"Unknown oscillation mixing parameter choice: "<<omp<<std::endl;
      exit(-1);
  }
  
  fparm->SetInitValue(kPhypCPV        , conv::kGlobFitDeltaCP);
  NuOscDmsqScheme_t odmsq = Opt::Instance()->GetEffectiveDmsqScheme();
  
  switch (odmsq) {
    case(kNuDmsqFogliLisi):
      fparm->SetInitValue(kPhypDm23sq,    conv::kGlobFitDmsqFL);
      fparm->SetInitValue(kPhypDm12sq,    conv::kGlobFitDm12sq);
      break;
    case(kNuDmsq12andAbs23):
      fparm->SetInitValue(kPhypDm23sq,    conv::kGlobFitDm23sq);
      fparm->SetInitValue(kPhypDm12sq,    conv::kGlobFitDm12sq);
      break;
    case(kNuDmsq12andAbs31):
      fparm->SetInitValue(kPhypDm12sq,    conv::kGlobFitDm12sq);
      if(Opt::Instance()->gMassHierarchyAssumed == kNmhNormal)
        fparm->SetInitValue(kPhypDm23sq,    conv::kGlobFitDm23sq + conv::kGlobFitDm12sq);
      else if(Opt::Instance()->gMassHierarchyAssumed == kNmhInverted)
        fparm->SetInitValue(kPhypDm23sq,    conv::kGlobFitDm23sq + conv::kGlobFitDm12sq);
      break;
    default:
      std::cerr<<"Unknown mass scheme: "<<odmsq<<std::endl;
      exit(-1);
  }
       
  fparm->SetInitValue(kPhypBetaBar, conv::kGlobFitBetaBar); // set init value

  return fparm;
}

// NEW BETABAR 2
//________________________________________________________________________________________
FitParamList * FitParamList::NuOsc3FlavourNormBarFit(int nuisance)
{
  FitParamList * fparm = new FitParamList(7+nuisance);  // 7 instead of 6
  FitParamList::Append3FlavourOscillation(fparm, 0);
  FitParamList::AppendNormBar(fparm, 6);
  return fparm;
}

//







//________________________________________________________________________________________
FitParamList * FitParamList::NuOscSterile4FlavourFit(int nuisance)
{
  FitParamList * fparm = new FitParamList(12+nuisance);
  FitParamList::Append4FlavourSterileOscillation(fparm, 0);

  return fparm;
}
//________________________________________________________________________________________
FitParamList * FitParamList::NuOscSterile5FlavourFit(int nuisance)
{
  FitParamList * fparm = new FitParamList(18+nuisance);
  FitParamList::Append4FlavourSterileOscillation(fparm, 0);
  return fparm;
}
//________________________________________________________________________________________
FitParamList * FitParamList::NumuDisapFit(int nuisance)
{
  FitParamList * fparm = new FitParamList(2+nuisance);
  FitParamList::Append2FlavourOscillation(fparm, 0);

  return fparm;
}
//________________________________________________________________________________________
FitParamList * FitParamList::NumuDecayFit(int nuisance)
{
  FitParamList * fparm = new FitParamList(2+nuisance);

  //                                         init    min      max    step   fix?   nuis?
  fparm->AddParam(0, kPhypSinsq2ThetaDecay,  1.0,    0.0,     1.0,   0.02,  false, false);
  fparm->AddParam(1, kPhypAlphaDecay,        0.01,   1E-4,   1E-3,   1E-5,  false, false);

  return fparm;
}
//________________________________________________________________________________________
FitParamList * FitParamList::NumuDecohFit(int nuisance)
{
  FitParamList * fparm = new FitParamList(2+nuisance);

  //                                          init    min     max     step    fix?   nuis?  
  fparm->AddParam(0, kPhypSinsq2ThetaDecoh,   1.0,    0.0,    1.0,    0.02,   false, false);
  fparm->AddParam(1, kPhypMusqDecoh,          0.01,   0.001,  0.04,   0.001,  false, false);

  return fparm;
}
//________________________________________________________________________________________
void FitParamList::Append3FlavourOscillation(FitParamList * fparm, int start)
{
  int ip = start; // first unallocated element in the param list

  //
  // add osc params
  //
  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  switch(omp) {
  case(kNuMixSinsq2Theta):
     fparm->AddParam( ip++, kPhypMixing23,  kInitFitSinsq2Theta23,  kMinFitSinsq2Theta23,  kMaxFitSinsq2Theta23,  kStepSinsq2Theta23,  false, false);
     fparm->AddParam( ip++, kPhypMixing13,  kInitFitSinsq2Theta13,  kMinFitSinsq2Theta13,  kMaxFitSinsq2Theta13,  kStepSinsq2Theta13,  false, false);
     fparm->AddParam( ip++, kPhypMixing12,  kInitFitSinsq2Theta12,  kMinFitSinsq2Theta12,  kMaxFitSinsq2Theta12,  kStepSinsq2Theta12,  false, false);
     break;
  case(kNuMixSinsqTheta):
     fparm->AddParam( ip++, kPhypMixing23,  kInitFitSinsqTheta23,   kMinFitSinsqTheta23,   kMaxFitSinsqTheta23,   kStepSinsqTheta23,  false, false);
     fparm->AddParam( ip++, kPhypMixing13,  kInitFitSinsqTheta13,   kMinFitSinsqTheta13,   kMaxFitSinsqTheta13,   kStepSinsqTheta13,  false, false);
     fparm->AddParam( ip++, kPhypMixing12,  kInitFitSinsqTheta12,   kMinFitSinsqTheta12,   kMaxFitSinsqTheta12,   kStepSinsqTheta12,  false, false);
     break;
  default:
     delete fparm;
     return;
  }
  
  NuOscDeltaCP_t dcp = Opt::Instance()->gOscDeltaCP;
  switch(dcp){
  case(kNuDeltaCPPhase):
  fparm->AddParam( ip++, kPhypCPV,   0,                -kpi,          kpi,           1.0E-2,  false, false);
  break;
  case(kNuDeltaCPSin):
  fparm->AddParam( ip++, kPhypCPV,   0,                  -1,            1,           1.0E-2,  false, false);
  break;
  default:
    delete fparm;
    return;
  }

  //  fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq,   kMinFitDm23sq, kMaxFitDm23sq, 1.0E-4,  false, false);
  NuOscDmsqScheme_t odmsq = Opt::Instance()->GetEffectiveDmsqScheme();
  switch (odmsq) {
  case(kNuDmsqFogliLisi):
    fparm->AddParam( ip++, kPhypDm23sq,    kGlobFitDmsqFL,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    break;
  case(kNuDmsq12andAbs23):
    fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    break;
  case(kNuDmsq12andAbs31):
    if(Opt::Instance()->gMassHierarchyAssumed == kNmhNormal)
      fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq + kInitFitDm12sq,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    else if(Opt::Instance()->gMassHierarchyAssumed == kNmhInverted)
      fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq - kInitFitDm12sq,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    break;
  default:
    delete fparm;
    return;
  }
  
  fparm->AddParam( ip++, kPhypDm12sq,    kInitFitDm12sq,   kMinFitDm12sq, kMaxFitDm12sq, kStepDm12sq,  false, false);

}
//________________________________________________________________________________________
void FitParamList::Append3FlavourOscillationBar(FitParamList * fparm, int start)
{
  int ip = start; // first unallocated element in the param list

  //
  // add osc params
  //
  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  switch(omp) {
  case(kNuMixSinsq2Theta):
     fparm->AddParam( ip++, kPhypMixing23Bar,  kInitFitSinsq2Theta23Bar,  kMinFitSinsq2Theta23Bar,  kMaxFitSinsq2Theta23Bar,  kStepSinsq2Theta23Bar,  false, false);
     fparm->AddParam( ip++, kPhypMixing13Bar,  kInitFitSinsq2Theta13Bar,  kMinFitSinsq2Theta13Bar,  kMaxFitSinsq2Theta13Bar,  kStepSinsq2Theta13Bar,  false, false);
     fparm->AddParam( ip++, kPhypMixing12Bar,  kInitFitSinsq2Theta12Bar,  kMinFitSinsq2Theta12Bar,  kMaxFitSinsq2Theta12Bar,  kStepSinsq2Theta12Bar,  false, false);
     break;
  case(kNuMixSinsqTheta):
     fparm->AddParam( ip++, kPhypMixing23Bar,  kInitFitSinsqTheta23Bar,   kMinFitSinsqTheta23Bar,   kMaxFitSinsqTheta23Bar,   kStepSinsqTheta23Bar,  false, false);
     fparm->AddParam( ip++, kPhypMixing13Bar,  kInitFitSinsqTheta13Bar,   kMinFitSinsqTheta13Bar,   kMaxFitSinsqTheta13Bar,   kStepSinsqTheta13Bar,  false, false);
     fparm->AddParam( ip++, kPhypMixing12Bar,  kInitFitSinsqTheta12Bar,   kMinFitSinsqTheta12Bar,   kMaxFitSinsqTheta12Bar,   kStepSinsqTheta12Bar,  false, false);
     break;
  default:
     delete fparm;
     return;
  }
  
  NuOscDeltaCP_t dcp = Opt::Instance()->gOscDeltaCP;
  switch(dcp){
  case(kNuDeltaCPPhase):
  fparm->AddParam( ip++, kPhypCPVBar,   0,                -kpi,          kpi,           1.0E-2,  false, false);
  break;
  case(kNuDeltaCPSin):
  fparm->AddParam( ip++, kPhypCPVBar,   0,                  -1,            1,           1.0E-2,  false, false);
  break;
  default:
    delete fparm;
    return;
  }

  //  fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq,   kMinFitDm23sq, kMaxFitDm23sq, 1.0E-4,  false, false);
  NuOscDmsqScheme_t odmsq = Opt::Instance()->GetEffectiveDmsqScheme();
  switch (odmsq) {
  case(kNuDmsqFogliLisi):
    fparm->AddParam( ip++, kPhypDm23sqBar,    kGlobFitDmsqFL,   kMinFitDm23sqBar, kMaxFitDm23sqBar, kStepDm23sqBar,  false, false);
    break;
  case(kNuDmsq12andAbs23):
    fparm->AddParam( ip++, kPhypDm23sqBar,    kInitFitDm23sqBar,   kMinFitDm23sqBar, kMaxFitDm23sqBar, kStepDm23sqBar,  false, false);
    break;
  case(kNuDmsq12andAbs31):
    if(Opt::Instance()->gMassHierarchyAssumed == kNmhNormal)
      fparm->AddParam( ip++, kPhypDm23sqBar,    kInitFitDm23sqBar + kInitFitDm12sqBar,   kMinFitDm23sqBar, kMaxFitDm23sqBar, kStepDm23sqBar,  false, false);
    else if(Opt::Instance()->gMassHierarchyAssumed == kNmhInverted)
      fparm->AddParam( ip++, kPhypDm23sqBar,    kInitFitDm23sqBar - kInitFitDm12sqBar,   kMinFitDm23sqBar, kMaxFitDm23sqBar, kStepDm23sqBar,  false, false);
    break;
  default:
    delete fparm;
    return;
  }
   
  fparm->AddParam( ip++, kPhypDm12sqBar,    kInitFitDm12sqBar,   kMinFitDm12sqBar, kMaxFitDm12sqBar, kStepDm12sqBar,  false, false);

}

// NEW BETABAR

//________________________________________________________________________________________
void FitParamList::AppendNormBar(FitParamList * fparm, int start)
{
  int ip = start; // first unallocated element in the param list
  fparm->AddParam( ip++, kPhypBetaBar,    kInitFitBetaBar,   kMinFitBetaBar, kMaxFitBetaBar, kStepBetaBar,  false, false);  // add parameter
}

//

//________________________________________________________________________________________
void FitParamList::Append4FlavourSterileOscillation(FitParamList * fparm, int start)
{
  int ip = start; // first unallocated element in the param list

  //
  // add osc params
  //
  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  switch(omp) {
  case(kNuMixSinsq2Theta):
    fparm->AddParam( ip++, kPhypMixing12,  kInitFitSinsq2Theta12,  kMinFitSinsq2Theta12,  kMaxFitSinsq2Theta12,  kStepSinsq2Theta12,  false, false);
    fparm->AddParam( ip++, kPhypMixing13,  kInitFitSinsq2Theta13,  kMinFitSinsq2Theta13,  kMaxFitSinsq2Theta13,  kStepSinsq2Theta13,  false, false);
    fparm->AddParam( ip++, kPhypMixing14,  kInitFitSinsq2Theta14,  kMinFitSinsq2Theta14,  kMaxFitSinsq2Theta14,  kStepSinsq2Theta14,  false, false);
    fparm->AddParam( ip++, kPhypMixing23,  kInitFitSinsq2Theta23,  kMinFitSinsq2Theta23,  kMaxFitSinsq2Theta23,  kStepSinsq2Theta23,  false, false);
    fparm->AddParam( ip++, kPhypMixing24,  kInitFitSinsq2Theta24,  kMinFitSinsq2Theta24,  kMaxFitSinsq2Theta24,  kStepSinsq2Theta24,  false, false);
    fparm->AddParam( ip++, kPhypMixing34,  kInitFitSinsq2Theta34,  kMinFitSinsq2Theta34,  kMaxFitSinsq2Theta34,  kStepSinsq2Theta34,  false, false);
    break;
  case(kNuMixSinsqTheta):
    fparm->AddParam( ip++, kPhypMixing12,  kInitFitSinsqTheta12,  kMinFitSinsqTheta12,  kMaxFitSinsqTheta12,  kStepSinsqTheta12,  false, false);
    fparm->AddParam( ip++, kPhypMixing13,  kInitFitSinsqTheta13,  kMinFitSinsqTheta13,  kMaxFitSinsqTheta13,  kStepSinsqTheta13,  false, false);
    fparm->AddParam( ip++, kPhypMixing14,  kInitFitSinsq2Theta14,  kMinFitSinsq2Theta14,  kMaxFitSinsq2Theta14,  kStepSinsq2Theta14,  false, false);
    fparm->AddParam( ip++, kPhypMixing23,  kInitFitSinsqTheta23,  kMinFitSinsqTheta23,  kMaxFitSinsqTheta23,  kStepSinsqTheta23,  false, false);
    fparm->AddParam( ip++, kPhypMixing24,  kInitFitSinsqTheta24,  kMinFitSinsqTheta24,  kMaxFitSinsqTheta24,  kStepSinsqTheta24,  false, false);
    fparm->AddParam( ip++, kPhypMixing34,  kInitFitSinsqTheta34,  kMinFitSinsqTheta34,  kMaxFitSinsqTheta34,  kStepSinsqTheta34,  false, false);
     break;
  default:
     delete fparm;
     return;
  }
  
  NuOscDeltaCP_t dcp = Opt::Instance()->gOscDeltaCP;
  switch(dcp){
  case(kNuDeltaCPPhase):
    fparm->AddParam( ip++, kPhypDelta12,   0,                -kpi,          kpi,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta13,   0,                -kpi,          kpi,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta34,   0,                -kpi,          kpi,           1.0E-2,  false, false);
  break;
  case(kNuDeltaCPSin):
    fparm->AddParam( ip++, kPhypDelta12,   0,                  -1,            1,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta13,   0,                  -1,            1,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta34,   0,                  -1,            1,           1.0E-2,  false, false);
  break;
  default:
    delete fparm;
    return;
  }

  fparm->AddParam( ip++, kPhypDm12sq,    kInitFitDm12sq,   kMinFitDm12sq, kMaxFitDm12sq, kStepDm12sq,  false, false);
  //  fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq,   kMinFitDm23sq, kMaxFitDm23sq, 1.0E-4,  false, false);
  NuOscDmsqScheme_t odmsq = Opt::Instance()->GetEffectiveDmsqScheme();
  switch (odmsq) {
  case(kNuDmsqFogliLisi):
    fparm->AddParam( ip++, kPhypDm23sq,    kGlobFitDmsqFL,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    break;
  case(kNuDmsq12andAbs23):
    fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    break;
  case(kNuDmsq12andAbs31):
    if(Opt::Instance()->gMassHierarchyAssumed == kNmhNormal)
      fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq + kInitFitDm12sq,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    else if(Opt::Instance()->gMassHierarchyAssumed == kNmhInverted)
      fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq - kInitFitDm12sq,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    break;
  default:
    delete fparm;
    return;
  }
  
  fparm->AddParam( ip++, kPhypDm41sq,    kInitFitDm41sq,   kMinFitDm41sq, kMaxFitDm41sq, kStepDm41sq,  false, false);
}
//________________________________________________________________________________________
void FitParamList::Append5FlavourSterileOscillation(FitParamList * fparm, int start)
{

  int ip = start; // first unallocated element in the param list

  //
  // add osc params
  //
  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  switch(omp) {
  case(kNuMixSinsq2Theta):
    fparm->AddParam( ip++, kPhypMixing12,  kInitFitSinsq2Theta12,  kMinFitSinsq2Theta12,  kMaxFitSinsq2Theta12,  kStepSinsq2Theta12,  false, false);
    fparm->AddParam( ip++, kPhypMixing13,  kInitFitSinsq2Theta13,  kMinFitSinsq2Theta13,  kMaxFitSinsq2Theta13,  kStepSinsq2Theta13,  false, false);
    fparm->AddParam( ip++, kPhypMixing14,  kInitFitSinsq2Theta14,  kMinFitSinsq2Theta14,  kMaxFitSinsq2Theta14,  kStepSinsq2Theta14,  false, false);
    fparm->AddParam( ip++, kPhypMixing15,  kInitFitSinsq2Theta15,  kMinFitSinsq2Theta15,  kMaxFitSinsq2Theta15,  kStepSinsq2Theta15,  false, false);
    fparm->AddParam( ip++, kPhypMixing23,  kInitFitSinsq2Theta23,  kMinFitSinsq2Theta23,  kMaxFitSinsq2Theta23,  kStepSinsq2Theta23,  false, false);
    fparm->AddParam( ip++, kPhypMixing24,  kInitFitSinsq2Theta24,  kMinFitSinsq2Theta24,  kMaxFitSinsq2Theta24,  kStepSinsq2Theta24,  false, false);
    fparm->AddParam( ip++, kPhypMixing25,  kInitFitSinsq2Theta25,  kMinFitSinsq2Theta25,  kMaxFitSinsq2Theta25,  kStepSinsq2Theta25,  false, false);
    fparm->AddParam( ip++, kPhypMixing34,  kInitFitSinsq2Theta34,  kMinFitSinsq2Theta34,  kMaxFitSinsq2Theta34,  kStepSinsq2Theta34,  false, false);
    fparm->AddParam( ip++, kPhypMixing35,  kInitFitSinsq2Theta35,  kMinFitSinsq2Theta35,  kMaxFitSinsq2Theta35,  kStepSinsq2Theta35,  false, false);
    break;
  case(kNuMixSinsqTheta):
    fparm->AddParam( ip++, kPhypMixing12,  kInitFitSinsqTheta12,  kMinFitSinsqTheta12,  kMaxFitSinsqTheta12,  kStepSinsqTheta12,  false, false);
    fparm->AddParam( ip++, kPhypMixing13,  kInitFitSinsqTheta13,  kMinFitSinsqTheta13,  kMaxFitSinsqTheta13,  kStepSinsqTheta13,  false, false);
    fparm->AddParam( ip++, kPhypMixing14,  kInitFitSinsqTheta14,  kMinFitSinsqTheta14,  kMaxFitSinsqTheta14,  kStepSinsqTheta14,  false, false);
    fparm->AddParam( ip++, kPhypMixing15,  kInitFitSinsqTheta15,  kMinFitSinsqTheta15,  kMaxFitSinsqTheta15,  kStepSinsqTheta15,  false, false);
    fparm->AddParam( ip++, kPhypMixing23,  kInitFitSinsqTheta23,  kMinFitSinsqTheta23,  kMaxFitSinsqTheta23,  kStepSinsqTheta23,  false, false);
    fparm->AddParam( ip++, kPhypMixing24,  kInitFitSinsqTheta24,  kMinFitSinsqTheta24,  kMaxFitSinsqTheta24,  kStepSinsqTheta24,  false, false);
    fparm->AddParam( ip++, kPhypMixing25,  kInitFitSinsqTheta25,  kMinFitSinsqTheta25,  kMaxFitSinsqTheta25,  kStepSinsqTheta25,  false, false);
    fparm->AddParam( ip++, kPhypMixing34,  kInitFitSinsqTheta34,  kMinFitSinsqTheta34,  kMaxFitSinsqTheta34,  kStepSinsqTheta34,  false, false);
    fparm->AddParam( ip++, kPhypMixing35,  kInitFitSinsqTheta35,  kMinFitSinsqTheta35,  kMaxFitSinsqTheta35,  kStepSinsqTheta35,  false, false);
     break;
  default:
     delete fparm;
     return;
  }
  
  NuOscDeltaCP_t dcp = Opt::Instance()->gOscDeltaCP;
  switch(dcp){
  case(kNuDeltaCPPhase):
    fparm->AddParam( ip++, kPhypDelta12,   0,                -kpi,          kpi,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta13,   0,                -kpi,          kpi,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta15,   0,                -kpi,          kpi,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta34,   0,                -kpi,          kpi,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta35,   0,                -kpi,          kpi,           1.0E-2,  false, false);
  break;
  case(kNuDeltaCPSin):
    fparm->AddParam( ip++, kPhypDelta12,   0,                  -1,            1,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta13,   0,                  -1,            1,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta15,   0,                  -1,            1,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta34,   0,                  -1,            1,           1.0E-2,  false, false);
    fparm->AddParam( ip++, kPhypDelta35,   0,                  -1,            1,           1.0E-2,  false, false);
  break;
  default:
    delete fparm;
    return;
  }

  fparm->AddParam( ip++, kPhypDm12sq,    kInitFitDm12sq,   kMinFitDm12sq, kMaxFitDm12sq, kStepDm12sq,  false, false);
  //  fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq,   kMinFitDm23sq, kMaxFitDm23sq, 1.0E-4,  false, false);
  NuOscDmsqScheme_t odmsq = Opt::Instance()->GetEffectiveDmsqScheme();
  switch (odmsq) {
  case(kNuDmsqFogliLisi):
    fparm->AddParam( ip++, kPhypDm23sq,    kGlobFitDmsqFL,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    break;
  case(kNuDmsq12andAbs23):
    fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    break;
  case(kNuDmsq12andAbs31):
    if(Opt::Instance()->gMassHierarchyAssumed == kNmhNormal)
      fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq + kInitFitDm12sq,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    else if(Opt::Instance()->gMassHierarchyAssumed == kNmhInverted)
      fparm->AddParam( ip++, kPhypDm23sq,    kInitFitDm23sq - kInitFitDm12sq,   kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq,  false, false);
    break;
  default:
    delete fparm;
    return;
  }
  
  fparm->AddParam( ip++, kPhypDm41sq,    kInitFitDm41sq,   kMinFitDm41sq, kMaxFitDm41sq, kStepDm41sq,  false, false);
  fparm->AddParam( ip++, kPhypDm51sq,    kInitFitDm51sq,   kMinFitDm51sq, kMaxFitDm51sq, kStepDm51sq,  false, false);
}
//________________________________________________________________________________________
void FitParamList::Append2FlavourOscillation(FitParamList * fparm, int start)
{

  int ip = start; // first unallocated element in the param list

  //
  // add osc params
  //
  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  switch(omp) {
  case(kNuMixSinsq2Theta):
    fparm->AddParam( ip++, kPhypMixing23, kInitFitSinsq2Theta23, kMinFitSinsq2Theta23,  kMaxFitSinsq2Theta23,  kStepSinsq2Theta23,  false, false);
    break;
  case(kNuMixSinsqTheta):
    fparm->AddParam( ip++, kPhypMixing23, kInitFitSinsqTheta23,  kMinFitSinsqTheta23,   kMaxFitSinsqTheta23,   kStepSinsq2Theta23,  false, false);
    break;
  default:
    delete fparm;
    return;
  }
  fparm->AddParam( ip++, kPhypDm23sq, kInitFitDm23sq, kMinFitDm23sq, kMaxFitDm23sq, kStepDm23sq, false, false);

}
//________________________________________________________________________________________
void FitParamList::AppendAllSystematics(FitParamList * /*fparm*/, int /*start*/) // '/* */' use to supress unused parameter warnings
{
// Add nuisance params
//

}
//________________________________________________________________________________________
PhysHypothesis_t FitParamList::PhysicsHypothesis(void) const
{
  // Steriles 3+2,3+1
  if( this->Exists(kPhypDm51sq) ) {
    return kHypOscSterile5F;
  }
  if(this->Exists(kPhypDm41sq) ) {
    return kHypOscSterile4F;
  }


  // NEW BETABAR

  // neutrino oscillations (3-flavour framework) with separate nu/nubar parameters and normalization
  if( this->Exists(kPhypDm12sqBar)   &&
      this->Exists(kPhypDm23sqBar)   &&
      this->Exists(kPhypMixing13Bar) &&
      this->Exists(kPhypMixing12Bar) &&
      this->Exists(kPhypMixing23Bar) &&
      this->Exists(kPhypCPVBar)      &&
      this->Exists(kPhypDm12sq)      &&
      this->Exists(kPhypDm23sq)      &&
      this->Exists(kPhypMixing13)    &&
      this->Exists(kPhypMixing12)    &&
      this->Exists(kPhypMixing23)    &&
      this->Exists(kPhypCPV)         &&
      this->Exists(kPhypBetaBar) )
  {
    return kHypOsc3FBarNormBar;
  }
  //

  // NEW BETABAR 2

  // neutrino oscillations (3-flavour framework) with separate nu/nubar parameters and normalization
  if( this->Exists(kPhypDm12sq)      &&
      this->Exists(kPhypDm23sq)      &&
      this->Exists(kPhypMixing13)    &&
      this->Exists(kPhypMixing12)    &&
      this->Exists(kPhypMixing23)    &&
      this->Exists(kPhypCPV)         &&
      this->Exists(kPhypBetaBar) )
  {
    return kHypOsc3FNormBar;
  }
  //

  
  // neutrino oscillations (3-flavour framework) with separate nu/nubar parameters
  if( this->Exists(kPhypDm12sqBar)   &&
      this->Exists(kPhypDm23sqBar)   &&
      this->Exists(kPhypMixing13Bar) &&
      this->Exists(kPhypMixing12Bar) &&
      this->Exists(kPhypMixing23Bar) &&
      this->Exists(kPhypCPVBar)      &&
      this->Exists(kPhypDm12sq)      &&
      this->Exists(kPhypDm23sq)      &&
      this->Exists(kPhypMixing13)    &&
      this->Exists(kPhypMixing12)    &&
      this->Exists(kPhypMixing23)    &&
      this->Exists(kPhypCPV) )
  {
    return kHypOsc3FBar;
  }
  //


  // neutrino oscillations (3-flavour framework)
  if( this->Exists(kPhypDm12sq)   &&
      this->Exists(kPhypDm23sq)   &&
      this->Exists(kPhypMixing13) &&
      this->Exists(kPhypMixing12) &&
      this->Exists(kPhypMixing23) &&
      this->Exists(kPhypCPV) )
  {
    return kHypOsc3F;
  }

  // numu disappearance, 2-flavour neutrino oscillation framework
  else
  if( this->Exists(kPhypDm23sq) &&
      this->Exists(kPhypMixing23) )
  {
    return kHypNuMuDisapp2F;
  }
  
  // neutrino decay   
  else
  if( this->Exists(kPhypAlphaDecay) &&
      this->Exists(kPhypSinsq2ThetaDecay) )
  {
    return kHypDecay;
  }
   
  // neutrino decoherence
  else
  if( this->Exists(kPhypMusqDecoh) &&
      this->Exists(kPhypSinsq2ThetaDecoh) )
  {
    return kHypDecoh;
  }

  // there are fit params, but they are all systematics
  else {
    if(this->Size() > 0) {
      bool all_systematics = true;
      map<PhysParamId_t, int>::const_iterator it = fIdMap.begin();
      for( ; it != fIdMap.begin(); ++it) {
        PhysParamId_t p = it->first;
        if(p < kNuis000) {
          all_systematics = false;
          break;
        }
      }
      if(all_systematics) return kHypVoid;
    }
  }

  // undefined
  return kHypUndefined;
}
//________________________________________________________________________________________
void FitParamList::Copy(const FitParamList & plst)
{
  if(this->fNParams != plst.fNParams)
  {
     this->CleanUp();
     this->Init(plst.fNParams);
  }

  this->fParListName = plst.fParListName;
  this->fSystMode    = plst.fSystMode;
  this->fIdMap.insert(plst.fIdMap.begin(),  plst.fIdMap.end());

  for(int i = 0; i < plst.fNParams; i++) {
     this->fName[i].SetString(plst.fName[i].GetString().Data());
     this->fBestFitValue[i] = plst.fBestFitValue[i];
     this->fCurrValue[i]    = plst.fCurrValue[i];
     this->fInitValue[i]    = plst.fInitValue[i];
     this->fMinValue[i]     = plst.fMinValue[i];
     this->fMaxValue[i]     = plst.fMaxValue[i];
     this->fStepSize[i]     = plst.fStepSize[i];
     this->fIsFixed[i]      = plst.fIsFixed[i];
     this->fIsNuisance[i]   = plst.fIsNuisance[i];
     this->fMigradError[i]  = plst.fMigradError[i];
     this->fHesseError[i]   = plst.fHesseError[i];
     this->fPlusError[i]    = plst.fPlusError[i];
     this->fMinusError[i]   = plst.fMinusError[i];

     // MARGINALIZATION
     this->fDoMarg[i]        = plst.fDoMarg[i];
     this->fPriorMode[i]     = plst.fPriorMode[i];
     this->fPriorModeName[i] = TObjString(plst.fPriorModeName[i]);
     this->fMeanGaus[i]      = plst.fMeanGaus[i];
     this->fSigmaGaus[i]     = plst.fSigmaGaus[i];
     this->fMinValueFlat[i]  = plst.fMinValueFlat[i];
     this->fMaxValueFlat[i]  = plst.fMaxValueFlat[i];

     this->fIsPrior2D[i]        = plst.fIsPrior2D[i];

     if(plst.fPriorHisto[i]!=0) this->fPriorHisto[i]      = (TH1D*) plst.fPriorHisto[i]->Clone("");
     else this->fPriorHisto[i] = 0;
     
     if(plst.fToyHisto[i]!=0)   this->fToyHisto[i]        = (TH1D*) plst.fToyHisto[i]->Clone("");
     else this->fToyHisto[i] = 0;
       
     if(plst.fPriorHisto2D[i]!=0) this->fPriorHisto2D[i]  = (TH2D*) plst.fPriorHisto2D[i]->Clone("");
     else this->fPriorHisto2D[i] = 0;
     
     if(plst.fToyHisto2D[i]!=0) this->fToyHisto2D[i]      = (TH2D*) plst.fToyHisto2D[i]->Clone("");
     else this->fToyHisto2D[i] = 0;
     
     //this->fPriorHisto[i]    = new TH1F(plst.fPriorHisto[i]);
     //this->fPriorHisto[i]->Copy(plst.fPriorHisto[i]);
     //
  } 

  for(int i = 0; i < plst.fNParams; i++) {
    for(int j = 0; j < plst.fNParams; j++) {
      (*this->fCovMatrix)[i][j] =  (*plst.fCovMatrix)[i][j];
    }
  }
    
  this->fBestFitStatistic = plst.fBestFitStatistic;
  this->fCurrStatistic    = plst.fCurrStatistic;
  this->fFitTime          = plst.fFitTime;
  this->fFitTries         = plst.fFitTries;
  
  this->fStatusCodes.insert(plst.fStatusCodes.begin(),  plst.fStatusCodes.end());
  
  this->fMigradErrorsSet = plst.fMigradErrorsSet;
  this->fHesseErrorsSet  = plst.fHesseErrorsSet;
  this->fMinosErrorsSet  = plst.fMinosErrorsSet;
  
  this->fShowCurrValuesOnly    = plst.fShowCurrValuesOnly;
  this->fShowBestFitValuesOnly = plst.fShowBestFitValuesOnly;
}
//________________________________________________________________________________________
void FitParamList::Init(int nparams)
{
   fIdMap.clear();

   fParListName = "";
   fSystMode = -1;

   fNParams = nparams; 

   if(nparams>0) {
     fName         = new TObjString[fNParams];
     fBestFitValue = new double[fNParams];
     fCurrValue    = new double[fNParams];
     fInitValue    = new double[fNParams];
     fMinValue     = new double[fNParams];
     fMaxValue     = new double[fNParams];
     fStepSize     = new double[fNParams];
     fIsFixed      = new bool  [fNParams];
     fIsNuisance   = new bool  [fNParams];
     fMigradError  = new double[fNParams];
     fHesseError   = new double[fNParams];
     fPlusError    = new double[fNParams];
     fMinusError   = new double[fNParams];
     fCovMatrix    = new TMatrixDSym(fNParams);


     // MARGINALIZATION
     fDoMarg        = new bool      [fNParams];
     fPriorMode     = new int       [fNParams];
     fPriorModeName = new TObjString[fNParams];
     fMeanGaus      = new double    [fNParams];
     fSigmaGaus     = new double    [fNParams];
     fMinValueFlat  = new double    [fNParams];
     fMaxValueFlat  = new double    [fNParams];    
     fPriorHisto    = new TH1D *    [fNParams];
     fToyHisto      = new TH1D *    [fNParams];

     fIsPrior2D     = new bool      [fNParams];

     fPriorHisto2D  = new TH2D *    [fNParams];
     fToyHisto2D    = new TH2D *    [fNParams];
     //

   } else {
     fName         = 0;
     fBestFitValue = 0;
     fCurrValue    = 0;
     fInitValue    = 0;
     fMinValue     = 0;
     fMaxValue     = 0;
     fStepSize     = 0;
     fIsFixed      = 0;
     fIsNuisance   = 0;
     fMigradError  = 0;
     fHesseError   = 0;
     fPlusError    = 0;
     fMinusError   = 0;
     fCovMatrix    = 0;

     // MARGINALIZATION
     fDoMarg        = 0;
     fPriorMode     = 0;
     fPriorModeName = 0;
     fMeanGaus      = 0;
     fSigmaGaus     = 0;
     fMinValueFlat  = 0;
     fMaxValueFlat  = 0;
     fIsPrior2D     = 0;
     fPriorHisto    = 0;
     fToyHisto      = 0;
     fPriorHisto2D  = 0;
     fToyHisto2D    = 0;
     //

   }
   
   for (int i = 0 ; i < fNParams ; i++) {
     fName         [i].SetString("Uninitialised");
     fBestFitValue [i] = 0;
     fCurrValue    [i] = 0;
     fInitValue    [i] = 0;
     fMinValue     [i] = 0;
     fMaxValue     [i] = 0;
     fStepSize     [i] = 0;
     fIsFixed      [i] = 0;
     fIsNuisance   [i] = 0;
     fMigradError  [i] = 0;
     fHesseError   [i] = 0;
     fPlusError    [i] = 0;
     fMinusError   [i] = 0;

     // MARGINALIZATION
     fDoMarg       [i] = 0;
     fPriorMode    [i] = 0;
     fPriorModeName[i].SetString("Uninitialised");
     fMeanGaus     [i] = 0;
     fSigmaGaus    [i] = 0;
     fMinValueFlat [i] = 0;
     fMaxValueFlat [i] = 0;
     fIsPrior2D    [i] = 0;
     fPriorHisto   [i] = 0;
     fToyHisto     [i] = 0;
     fPriorHisto2D [i] = 0;
     fToyHisto2D   [i] = 0;
     //


   }
     

   fShowCurrValuesOnly    = false;
   fShowBestFitValuesOnly = false;

   fBestFitStatistic = 0.;
   fCurrStatistic    = 0.;
   fFitTime          = -1;
   fFitTries         = 0;

   fStatusCodes.clear();

   fMigradErrorsSet = false;
   fHesseErrorsSet  = false;
   fMinosErrorsSet  = false;
}
//________________________________________________________________________________________
void FitParamList::CleanUp(void)
{
   fIdMap.clear();
   fStatusCodes.clear();

   if( fName        ) { delete [] fName        ; fName         = 0; }
   if( fBestFitValue) { delete [] fBestFitValue; fBestFitValue = 0; }
   if( fCurrValue   ) { delete [] fCurrValue   ; fCurrValue    = 0; }
   if( fInitValue   ) { delete [] fInitValue   ; fInitValue    = 0; }
   if( fMinValue    ) { delete [] fMinValue    ; fMinValue     = 0; }
   if( fMaxValue    ) { delete [] fMaxValue    ; fMaxValue     = 0; }
   if( fStepSize    ) { delete [] fStepSize    ; fStepSize     = 0; }
   if( fIsFixed     ) { delete [] fIsFixed     ; fIsFixed      = 0; }
   if( fIsNuisance  ) { delete [] fIsNuisance  ; fIsNuisance   = 0; }
   if( fMigradError ) { delete [] fMigradError ; fMigradError  = 0; }
   if( fHesseError  ) { delete [] fHesseError  ; fHesseError   = 0; }
   if( fPlusError   ) { delete [] fPlusError   ; fPlusError    = 0; }
   if( fMinusError  ) { delete [] fMinusError  ; fMinusError   = 0; }
   if( fCovMatrix   ) { delete    fCovMatrix   ; fCovMatrix    = 0; }

   // MARGINALIZATION
   if (fDoMarg        ) { delete [] fDoMarg       ; fDoMarg        = 0 ; }
   if (fPriorMode     ) { delete [] fPriorMode    ; fPriorMode     = 0 ; }
   if (fPriorModeName ) { delete [] fPriorModeName; fPriorModeName = 0 ; }
   if (fMeanGaus      ) { delete [] fMeanGaus     ; fMeanGaus      = 0 ; }
   if (fSigmaGaus     ) { delete [] fSigmaGaus    ; fSigmaGaus     = 0 ; }
   if (fMinValueFlat  ) { delete [] fMinValueFlat ; fMinValueFlat  = 0 ; }
   if (fMaxValueFlat  ) { delete [] fMaxValueFlat ; fMaxValueFlat  = 0 ; }

   if (fIsPrior2D     ) { delete [] fIsPrior2D    ; fIsPrior2D     = 0 ; }

   if (fPriorHisto    ) { delete [] fPriorHisto   ; fPriorHisto    = 0 ; }
   if (fToyHisto      ) { delete [] fToyHisto     ; fToyHisto      = 0 ; }
   if (fPriorHisto2D  ) { delete [] fPriorHisto2D ; fPriorHisto2D  = 0 ; }
   if (fToyHisto2D    ) { delete [] fToyHisto2D   ; fToyHisto2D    = 0 ; }
   //
   
   fBestFitStatistic = 0.;
   fCurrStatistic    = 0.;
   fFitTime          = 0.;
   fFitTries         = -1;

   fParListName = "";
   fSystMode    = -1;
}
//________________________________________________________________________________________
int FitParamList::Id(PhysParamId_t p) const
{
   map<PhysParamId_t, int>::const_iterator it = fIdMap.find(p);
   if(it == fIdMap.end()) {
     cerr << "FitParamList::Id() >> Parameter: " << util::NameRegistry::Instance()->AsString(p) << " PhysParamId_t p = " << p 
          << " was not included in current fit parameter list" << endl;
     exit(1);
   }
   int id = it->second;
   return id;
}
//________________________________________________________________________________________
valor::conv::PhysParamId_t FitParamList::WhichParam(int id) const
{
   map<PhysParamId_t, int>::const_iterator it = fIdMap.begin();
   for( ; it != fIdMap.end(); ++it) {
      int curr_id = it->second;
      if(curr_id == id) return it->first;
   }
   return kPhypUndefined;
}
//________________________________________________________________________________________
double FitParamList::HypothesisFitStatisticPenalty(Option_t * /*opt*/) const
{
  double chisq = 0;
  double center, error, current, tweak;
  for (int ip = kPhypMixing23 ; ip < kNuis000 ; ip++) {
    PhysParamId_t p = (PhysParamId_t) ip;
    if (not (this->Exists(p) && this->IsNuisance(p))) continue;
    current = this->CurrValue(p);
    
    center = Opt::Instance()->GetGlobalBestFit(p);
    error  = Opt::Instance()->GetGlobalFitError(p)*center;

    NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
    if(p==conv::kPhypMixing13 && omp==conv::kNuMixSinsqTheta){ // Use the reactor constrain with sinsq2theta13 (not sinsqtheta13)
      double current_sinsq2th = 1 - (1-2*current)*(1-2*current);   
      current = current_sinsq2th;
      center = conv::kGlobFitSinsq2Theta13;
      error = conv::kGlobFitErrSinsq2Theta13 * center;
    }

    if (center == -999) {
      std::cerr<<"FitParamList/HypothesisFitStatisticPenalty >> Using zero penalty for parameter "
               << fName[ip].GetString() << std::endl;
      continue;
    }
  
    tweak = (current-center)/error;
    
    chisq += tweak*tweak;
  }
  
  return chisq;
}





// MARGINALIZATION
//________________________________________________________________________________________
void FitParamList::SetDoMarg(valor::conv::PhysParamId_t p,bool domarg)
{
  fDoMarg [this->Id(p)] = domarg;
}
//________________________________________________________________________________________
void FitParamList::SetPriorMode(valor::conv::PhysParamId_t p,int priormode)
{
  fPriorMode [this->Id(p)] = priormode;
  
  if      (priormode==0) fPriorModeName [this->Id(p)].SetString("flat");
  else if (priormode==1) fPriorModeName [this->Id(p)].SetString("gaus");
  else if (priormode==2) fPriorModeName [this->Id(p)].SetString("user");
  else{
    cerr << "Chosen marginalization prior mode " << priormode << " is not available" << endl;
    cerr << "Possible choices are:" << endl;
    cerr << "  0 --> flat prior" << endl;
    cerr << "  1 --> gaus prior" << endl;
    cerr << "  2 --> prior from user distribution" << endl;
    exit(1);
  }
}
//________________________________________________________________________________________
void FitParamList::SetPriorMode(valor::conv::PhysParamId_t p,string priormodename)
{
  if(priormodename=="flat"){
    fPriorModeName [this->Id(p)].SetString("flat");
    fPriorMode [this->Id(p)] = 0;
  }
  else if (priormodename=="gaus"){
    fPriorModeName [this->Id(p)].SetString("gaus");
    fPriorMode [this->Id(p)] = 1;
  }
  else if (priormodename=="user"){
    fPriorModeName [this->Id(p)].SetString("user");
    fPriorMode [this->Id(p)] = 2;
  }
  else{
    cerr << "Chosen marginalization prior mode " << priormodename << " is not available" << endl;
    cerr << "Possible choices are:" << endl;
    cerr << "  flat --> flat prior" << endl;
    cerr << "  gaus --> gaus prior" << endl;
    cerr << "  user --> prior from user distribution" << endl;
    exit(1);
  }
}
//________________________________________________________________________________________
void FitParamList::SetMeanGaus(valor::conv::PhysParamId_t p,double mean)
{
  fMeanGaus [this->Id(p)] = mean;
}
//________________________________________________________________________________________
void FitParamList::SetSigmaGaus(valor::conv::PhysParamId_t p,double sigma)
{
  fSigmaGaus [this->Id(p)] = sigma;
}
//________________________________________________________________________________________
void FitParamList::SetMinValueFlat(valor::conv::PhysParamId_t p,double minvalmarg)
{
  fMinValueFlat [this->Id(p)] = minvalmarg;
}
//________________________________________________________________________________________
void FitParamList::SetMaxValueFlat(valor::conv::PhysParamId_t p,double maxvalmarg)
{
  fMaxValueFlat [this->Id(p)] = maxvalmarg;
}
//________________________________________________________________________________________
void FitParamList::SetPriorHisto(valor::conv::PhysParamId_t p,TH1D *hprior)
{
  char name[200];
  sprintf(name,"%s_%s","hPrior",util::RawParamAsStringRaw(p).c_str());
  fPriorHisto [this->Id(p)] = (TH1D*) hprior->Clone(name);
  
  this->SetPriorDimension2D(p,false); // The prior is 1D
}

//________________________________________________________________________________________
void FitParamList::SetPriorHisto2D(valor::conv::PhysParamId_t p,TH2D *hprior)
{
  char name[200];
  sprintf(name,"%s_%s","hPrior",util::RawParamAsStringRaw(p).c_str());
  fPriorHisto2D [this->Id(p)] = (TH2D*) hprior->Clone(name);

  this->SetPriorDimension2D(p,true); // The prior is 2D
}


//________________________________________________________________________________________
void FitParamList::SetToyHisto(valor::conv::PhysParamId_t p,TH1D *hprior)
{
  char name[200];
  sprintf(name,"%s_%s","hToy",util::RawParamAsStringRaw(p).c_str());
  fToyHisto [this->Id(p)] = (TH1D*) hprior->Clone(name);
  fToyHisto [this->Id(p)]->Reset();
}

//________________________________________________________________________________________
void FitParamList::SetToyHisto2D(valor::conv::PhysParamId_t p,TH2D *hprior)
{
  char name[200];
  sprintf(name,"%s_%s","hToy2D",util::RawParamAsStringRaw(p).c_str());
  fToyHisto2D [this->Id(p)] = (TH2D*) hprior->Clone(name);
  fToyHisto2D [this->Id(p)]->Reset();
}


//________________________________________________________________________________________
string FitParamList::GetPriorModeName (valor::conv::PhysParamId_t p) const
{
  const char * data = fPriorModeName[this->Id(p)].GetString().Data();
  return string(data);
}
//________________________________________________________________________________________

void FitParamList::SetPriorDimension2D(valor::conv::PhysParamId_t p, bool isprior2d){
  fIsPrior2D [this->Id(p)] = isprior2d;
}
