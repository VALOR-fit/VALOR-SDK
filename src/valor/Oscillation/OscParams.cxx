//____________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

 */
//____________________________________________________________________________

#include <cstdlib>

#include <TMath.h>

#include "valor/Conventions/Opt.h"
#include "valor/Conventions/PhysHypothesisId.h"
#include "valor/Conventions/Constants.h"
#include "valor/DerivedUtilities/UtlNumStat.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Oscillation/OscParams.h"

//#define __print_toys__

using std::cout;
using std::cerr;
using std::endl;
using std::string;

using namespace valor;
using namespace valor::conv;

//____________________________________________________________________________
namespace valor {
  namespace util {
    ostream & operator << (ostream & stream, const OscParams & oscparams)
    {
      oscparams.Print(stream);
      oscparams.Print_Toys(stream);
      return stream;
    }
  }
}
//____________________________________________________________________________
valor::util::OscParams * valor::util::OscParams::fInstance = 0;
//____________________________________________________________________________
valor::util::OscParams::OscParams()
{
  fInstance = 0;
  
  this->Init();
}
//____________________________________________________________________________
valor::util::OscParams::~OscParams()
{
  fInstance = 0;
  
  this->Cleanup();
}
//____________________________________________________________________________
valor::util::OscParams * valor::util::OscParams::Instance()
{
  if(fInstance == 0) {
    static valor::util::OscParams::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new OscParams;
  }
  return fInstance;
}
//____________________________________________________________________________
void valor::util::OscParams::Init()
{
  mixmode_mixing12 = "";
  mixmode_mixing13 = "";
  mixmode_mixing23 = "";
  
  priormode_mixing12 = "";
  priormode_mixing13 = "";
  priormode_mixing23 = "";
  priormode_dcp      = "";
  priormode_dm12sq   = "";
  priormode_dm23sq   = "";
  
  for(int i=0;i<2;i++){
    params_mixing12[i] = conv::kBadResult;
    params_mixing13[i] = conv::kBadResult;
    params_mixing23[i] = conv::kBadResult;
    params_dcp[i]      = conv::kBadResult;
    params_dm12sq[i]   = conv::kBadResult;
    params_dm23sq[i]   = conv::kBadResult;
  }
  
  hPrior_th13_dcp_nh = 0;
  hPrior_th13_dcp_ih = 0;
  hPrior_23_nh = 0;
  hPrior_23_ih = 0;
  
  hToyHisto_dcp_nh = new TH1D("hToyHisto_dcp_nh","hToyHisto_dcp_nh",100,-3.1416,+3.1416);
  hToyHisto_Mix13_nh = new TH1D("hToyHisto_Mix13_nh","hToyHisto_Mix13_nh",100,0.,0.04);
  hToyHisto_Mix23_nh = new TH1D("hToyHisto_Mix23_nh","hToyHisto_Mix23_nh",100,0.3,0.7);
  hToyHisto_Dm23_nh = new TH1D("hToyHisto_Dm23_nh","hToyHisto_Dm23_nh",100,2.3E-3,2.7E-3);
  hToyHisto_Mix12_nh = new TH1D("hToyHisto_Mix12_nh","hToyHisto_Mix12_nh",100,0.2,0.4);
  hToyHisto_Dm12_nh = new TH1D("hToyHisto_Dm12_nh","hToyHisto_Dm12_nh",100,6.E-5,9.E-5);
  
  hToyHisto_dcp_ih = new TH1D("hToyHisto_dcp_ih","hToyHisto_dcp_ih",100,-3.1416,+3.1416);
  hToyHisto_Mix13_ih = new TH1D("hToyHisto_Mix13_ih","hToyHisto_Mix13_ih",100,0.,0.04);
  hToyHisto_Mix23_ih = new TH1D("hToyHisto_Mix23_ih","hToyHisto_Mix23_ih",100,0.3,0.7);
  hToyHisto_Dm23_ih = new TH1D("hToyHisto_Dm23_ih","hToyHisto_Dm23_ih",100,2.3E-3,2.7E-3);
  hToyHisto_Mix12_ih = new TH1D("hToyHisto_Mix12_ih","hToyHisto_Mix12_ih",100,0.2,0.4);
  hToyHisto_Dm12_ih = new TH1D("hToyHisto_Dm12_ih","hToyHisto_Dm12_ih",100,6.E-5,9.E-5);
  
  hToyHisto_dcpBar_nh = new TH1D("hToyHisto_dcpBar_nh","hToyHisto_dcpBar_nh",100,-3.1416,+3.1416);
  hToyHisto_Mix13Bar_nh = new TH1D("hToyHisto_Mix13Bar_nh","hToyHisto_Mix13Bar_nh",100,0.,0.04);
  hToyHisto_Mix23Bar_nh = new TH1D("hToyHisto_Mix23Bar_nh","hToyHisto_Mix23Bar_nh",100,0.3,0.7);
  hToyHisto_Dm23Bar_nh = new TH1D("hToyHisto_Dm23Bar_nh","hToyHisto_Dm23Bar_nh",100,2.3E-3,2.7E-3);
  hToyHisto_Mix12Bar_nh = new TH1D("hToyHisto_Mix12Bar_nh","hToyHisto_Mix12Bar_nh",100,0.2,0.4);
  hToyHisto_Dm12Bar_nh = new TH1D("hToyHisto_Dm12Bar_nh","hToyHisto_Dm12Bar_nh",100,6.E-5,9.E-5);
  
  hToyHisto_dcpBar_ih = new TH1D("hToyHisto_dcpBar_ih","hToyHisto_dcpBar_ih",100,-3.1416,+3.1416);
  hToyHisto_Mix13Bar_ih = new TH1D("hToyHisto_Mix13Bar_ih","hToyHisto_Mix13Bar_ih",100,0.,0.04);
  hToyHisto_Mix23Bar_ih = new TH1D("hToyHisto_Mix23Bar_ih","hToyHisto_Mix23Bar_ih",100,0.3,0.7);
  hToyHisto_Dm23Bar_ih = new TH1D("hToyHisto_Dm23Bar_ih","hToyHisto_Dm23Bar_ih",100,2.3E-3,2.7E-3);
  hToyHisto_Mix12Bar_ih = new TH1D("hToyHisto_Mix12Bar_ih","hToyHisto_Mix12Bar_ih",100,0.2,0.4);
  hToyHisto_Dm12Bar_ih = new TH1D("hToyHisto_Dm12Bar_ih","hToyHisto_Dm12Bar_ih",100,6.E-5,9.E-5);
  
  hToyHisto_MH = new TH1D("hToyHisto_MH","hToyHisto_MH",2,0,2);
  
  mixing12_nh = conv::kBadResult;
  mixing13_nh = conv::kBadResult;
  mixing23_nh = conv::kBadResult;
  dcp_nh = conv::kBadResult;
  dm12sq_nh = conv::kBadResult;
  dm23sq_nh = conv::kBadResult;
  mixing12_ih = conv::kBadResult;
  mixing13_ih = conv::kBadResult;
  mixing23_ih = conv::kBadResult;
  dcp_ih = conv::kBadResult;
  dm12sq_ih = conv::kBadResult;
  dm23sq_ih = conv::kBadResult;
  mixing12bar_nh = conv::kBadResult;
  mixing13bar_nh = conv::kBadResult;
  mixing23bar_nh = conv::kBadResult;
  dcpbar_nh = conv::kBadResult;
  dm12sqbar_nh = conv::kBadResult;
  dm23sqbar_nh = conv::kBadResult;
  mixing12bar_ih = conv::kBadResult;
  mixing13bar_ih = conv::kBadResult;
  mixing23bar_ih = conv::kBadResult;
  dcpbar_ih = conv::kBadResult;
  dm12sqbar_ih = conv::kBadResult;
  dm23sqbar_ih = conv::kBadResult;
  beta_bar = conv::kBadResult;
  
}
//____________________________________________________________________________
void valor::util::OscParams::Cleanup()
{
  mixmode_mixing12 = "";
  mixmode_mixing13 = "";
  mixmode_mixing23 = "";
  
  priormode_mixing12 = "";
  priormode_mixing13 = "";
  priormode_mixing23 = "";
  priormode_dcp      = "";
  priormode_dm12sq   = "";
  priormode_dm23sq   = "";
  
  for(int i=0;i<2;i++){
    params_mixing12[i] = conv::kBadResult;
    params_mixing13[i] = conv::kBadResult;
    params_mixing23[i] = conv::kBadResult;
    params_dcp[i]      = conv::kBadResult;
    params_dm12sq[i]   = conv::kBadResult;
    params_dm23sq[i]   = conv::kBadResult;
  }
  
  if(hPrior_th13_dcp_nh) delete hPrior_th13_dcp_nh;
  if(hPrior_th13_dcp_ih) delete hPrior_th13_dcp_ih;
  if(hPrior_23_nh) delete hPrior_23_nh;
  if(hPrior_23_ih) delete hPrior_23_ih;
  
  if(hToyHisto_dcp_nh) delete hToyHisto_dcp_nh;
  if(hToyHisto_Mix13_nh) delete hToyHisto_Mix13_nh;
  if(hToyHisto_Mix23_nh) delete hToyHisto_Mix23_nh;
  if(hToyHisto_Dm23_nh) delete hToyHisto_Dm23_nh;
  if(hToyHisto_Mix12_nh) delete hToyHisto_Mix12_nh;
  if(hToyHisto_Dm12_nh) delete hToyHisto_Dm12_nh;
  if(hToyHisto_dcp_ih) delete hToyHisto_dcp_ih;
  if(hToyHisto_Mix13_ih) delete hToyHisto_Mix13_ih;
  if(hToyHisto_Mix23_ih) delete hToyHisto_Mix23_ih;
  if(hToyHisto_Dm23_ih) delete hToyHisto_Dm23_ih;
  if(hToyHisto_Mix12_ih) delete hToyHisto_Mix12_ih;
  if(hToyHisto_Dm12_ih) delete hToyHisto_Dm12_ih;
  if(hToyHisto_MH) delete hToyHisto_MH;
  
  //NuBar
  if(hToyHisto_dcpBar_nh) delete hToyHisto_dcpBar_nh;
  if(hToyHisto_Mix13Bar_nh) delete hToyHisto_Mix13Bar_nh;
  if(hToyHisto_Mix23Bar_nh) delete hToyHisto_Mix23Bar_nh;
  if(hToyHisto_Dm23Bar_nh) delete hToyHisto_Dm23Bar_nh;
  if(hToyHisto_Mix12Bar_nh) delete hToyHisto_Mix12Bar_nh;
  if(hToyHisto_Dm12Bar_nh) delete hToyHisto_Dm12Bar_nh;
  if(hToyHisto_dcpBar_ih) delete hToyHisto_dcpBar_ih;
  if(hToyHisto_Mix13Bar_ih) delete hToyHisto_Mix13Bar_ih;
  if(hToyHisto_Mix23Bar_ih) delete hToyHisto_Mix23Bar_ih;
  if(hToyHisto_Dm23Bar_ih) delete hToyHisto_Dm23Bar_ih;
  if(hToyHisto_Mix12Bar_ih) delete hToyHisto_Mix12Bar_ih;
  if(hToyHisto_Dm12Bar_ih) delete hToyHisto_Dm12Bar_ih;
  
  
  mixing12_nh = conv::kBadResult;
  mixing13_nh = conv::kBadResult;
  mixing23_nh = conv::kBadResult;
  dcp_nh = conv::kBadResult;
  dm12sq_nh = conv::kBadResult;
  dm23sq_nh = conv::kBadResult;
  mixing12_ih = conv::kBadResult;
  mixing13_ih = conv::kBadResult;
  mixing23_ih = conv::kBadResult;
  dcp_ih = conv::kBadResult;
  dm12sq_ih = conv::kBadResult;
  dm23sq_ih = conv::kBadResult;
  mixing12bar_nh = conv::kBadResult;
  mixing13bar_nh = conv::kBadResult;
  mixing23bar_nh = conv::kBadResult;
  dcpbar_nh = conv::kBadResult;
  dm12sqbar_nh = conv::kBadResult;
  dm23sqbar_nh = conv::kBadResult;
  mixing12bar_ih = conv::kBadResult;
  mixing13bar_ih = conv::kBadResult;
  mixing23bar_ih = conv::kBadResult;
  dcpbar_ih = conv::kBadResult;
  dm12sqbar_ih = conv::kBadResult;
  dm23sqbar_ih = conv::kBadResult;
  beta_bar = conv::kBadResult;
  
}
//____________________________________________________________________________

void valor::util::OscParams::SetPrior(conv::PhysParamId_t physpar,string mode,double par1,double par2,string mixmode)
{
  if(physpar==conv::kPhypMixing13){
    priormode_mixing13 = mode;
    mixmode_mixing13   = mixmode;
    params_mixing13[0] = par1;
    params_mixing13[1] = par2;
  }
  else if(physpar==conv::kPhypMixing23){
    priormode_mixing23 = mode;
    mixmode_mixing23   = mixmode;
    params_mixing23[0] = par1;
    params_mixing23[1] = par2;
  }
  else if(physpar==conv::kPhypMixing12){
    priormode_mixing12 = mode;
    mixmode_mixing12   = mixmode;
    params_mixing12[0] = par1;
    params_mixing12[1] = par2;
  }
  else if(physpar==conv::kPhypCPV){
    priormode_dcp = mode;
    params_dcp[0] = par1;
    params_dcp[1] = par2;
  }
  else if(physpar==conv::kPhypDm23sq){
    priormode_dm23sq = mode;
    params_dm23sq[0] = par1;
    params_dm23sq[1] = par2;
  }
  else if(physpar==conv::kPhypDm12sq){
    priormode_dm12sq = mode;
    params_dm12sq[0] = par1;
    params_dm12sq[1] = par2;
  }
  else{ //kPhypUndefined
    cerr << endl;
    cerr << "The physics " << util::RawParamAsString(physpar) << " parameter is not defined" << endl;
    cerr << endl;
    exit(1);
  }
  
  cout << endl;
  cout << "Prior for " << util::RawParamAsString(physpar) << " : " << endl;
  if(mode == "gaus"){
    cout << " - gaus : mean = " << par1 << " - sigma = " << par2 << endl;
  }
  else if(mode == "flat"){
    cout << " - flat : low = " << par1 << " - up = " << par2 << endl;
  }
  else if(mode == "user"){
    cout << " - user " << endl;
  }
  else{
    cerr << endl;
    cerr << "Prior mode " << mode << "is not defined for " << util::RawParamAsString(physpar) << endl;
    cerr << endl;
    exit(1);
  }
  
  if(IsMixingAngle(physpar)){
    if( Opt::Instance()->gOscMixingParamChoice == kNuMixSinsqTheta){
      if(mixmode == "sinsq2th"){
        cout << " - Convert extracted value from sinsq2th to sinsqth" << endl;
      }
    }
    else if( Opt::Instance()->gOscMixingParamChoice == kNuMixSinsq2Theta){
      if(mixmode == "sinsqth"){
        cout << " - Convert extracted value from sinsqth to sinsq2th" << endl;
      }
    }
    if(mixmode != "sinsq2th" && mixmode != "sinsqth"){
      cerr << endl;
      cerr << "The mixing mode " << mixmode << " is not used" << endl;
      cerr << endl;
      exit(1);
    }
  }
  
}
//____________________________________________________________________________
void valor::util::OscParams::SetValues(double input_mixing12_nh, double input_mixing13_nh, double input_mixing23_nh,
                                       double input_dcp_nh,      double input_dm12sq_nh,   double input_dm23sq_nh,
                                       double input_beta_bar,
                                       double input_mixing12_ih, double input_mixing13_ih, double input_mixing23_ih,
                                       double input_dcp_ih,      double input_dm12sq_ih,   double input_dm23sq_ih,
                                       double input_mh_nh_prior, bool marginalize_MH,
                                       double input_mixing12bar_nh, double input_mixing13bar_nh, double input_mixing23bar_nh,
                                       double input_dcpbar_nh,      double input_dm12sqbar_nh,   double input_dm23sqbar_nh,
                                       double input_mixing12bar_ih, double input_mixing13bar_ih, double input_mixing23bar_ih,
                                       double input_dcpbar_ih,      double input_dm12sqbar_ih,   double input_dm23sqbar_ih
                                       )
{
  // Check whether some osc. params need to be randomized (i.e. if == -999)
  //NH
  randomize_mixing12_nh = (TMath::Abs(input_mixing12_nh + 999) < kASmallNum);
  randomize_mixing13_nh = (TMath::Abs(input_mixing13_nh + 999) < kASmallNum);
  randomize_mixing23_nh = (TMath::Abs(input_mixing23_nh + 999) < kASmallNum);
  randomize_dcp_nh      = (TMath::Abs(input_dcp_nh      + 999) < kASmallNum);
  randomize_dm12sq_nh   = (TMath::Abs(input_dm12sq_nh   + 999) < kASmallNum);
  randomize_dm23sq_nh   = (TMath::Abs(input_dm23sq_nh   + 999) < kASmallNum);
  //IH
  randomize_mixing12_ih = (TMath::Abs(input_mixing12_ih + 999) < kASmallNum);
  randomize_mixing13_ih = (TMath::Abs(input_mixing13_ih + 999) < kASmallNum);
  randomize_mixing23_ih = (TMath::Abs(input_mixing23_ih + 999) < kASmallNum);
  randomize_dcp_ih      = (TMath::Abs(input_dcp_ih      + 999) < kASmallNum);
  randomize_dm12sq_ih   = (TMath::Abs(input_dm12sq_ih   + 999) < kASmallNum);
  randomize_dm23sq_ih   = (TMath::Abs(input_dm23sq_ih   + 999) < kASmallNum);
  
  //NH
  randomize_mixing12bar_nh = (TMath::Abs(input_mixing12bar_nh + 999) < kASmallNum);
  randomize_mixing13bar_nh = (TMath::Abs(input_mixing13bar_nh + 999) < kASmallNum);
  randomize_mixing23bar_nh = (TMath::Abs(input_mixing23bar_nh + 999) < kASmallNum);
  randomize_dcpbar_nh      = (TMath::Abs(input_dcpbar_nh      + 999) < kASmallNum);
  randomize_dm12sqbar_nh   = (TMath::Abs(input_dm12sqbar_nh   + 999) < kASmallNum);
  randomize_dm23sqbar_nh   = (TMath::Abs(input_dm23sqbar_nh   + 999) < kASmallNum);
  //IH
  randomize_mixing12bar_ih = (TMath::Abs(input_mixing12bar_ih + 999) < kASmallNum);
  randomize_mixing13bar_ih = (TMath::Abs(input_mixing13bar_ih + 999) < kASmallNum);
  randomize_mixing23bar_ih = (TMath::Abs(input_mixing23bar_ih + 999) < kASmallNum);
  randomize_dcpbar_ih      = (TMath::Abs(input_dcpbar_ih      + 999) < kASmallNum);
  randomize_dm12sqbar_ih   = (TMath::Abs(input_dm12sqbar_ih   + 999) < kASmallNum);
  randomize_dm23sqbar_ih   = (TMath::Abs(input_dm23sqbar_ih   + 999) < kASmallNum);
  
  //Beta bar
  randomize_beta_bar    = (TMath::Abs(input_beta_bar    + 999) < kASmallNum);
  
  //Set to defaults parameters that are randomized
  
  //NH
  if(Opt::Instance()->gOscMixingParamChoice == conv::kNuMixSinsq2Theta) {
    if(randomize_mixing13_nh) input_mixing13_nh = conv::kUsed_Sinsq2Theta13;
    if(randomize_mixing23_nh) input_mixing23_nh = conv::kUsed_Sinsq2Theta23;
    if(randomize_mixing12_nh) input_mixing12_nh = conv::kUsed_Sinsq2Theta12;
  }
  else {
    if(randomize_mixing13_nh) input_mixing13_nh = conv::kUsed_SinsqTheta13;
    if(randomize_mixing23_nh) input_mixing23_nh = conv::kUsed_SinsqTheta23;
    if(randomize_mixing12_nh) input_mixing12_nh = conv::kUsed_SinsqTheta12;
  }
  if(randomize_dm23sq_nh) input_dm23sq_nh = conv::kUsed_Dm23sq;
  if(randomize_dm12sq_nh) input_dm12sq_nh = conv::kUsed_Dm12sq;
  if(randomize_dcp_nh)    input_dcp_nh    = conv::kUsed_DeltaCP;
  //IH
  if(Opt::Instance()->gOscMixingParamChoice == conv::kNuMixSinsq2Theta) {
    if(randomize_mixing13_ih) input_mixing13_ih = conv::kUsed_Sinsq2Theta13;
    if(randomize_mixing23_ih) input_mixing23_ih = conv::kUsed_Sinsq2Theta23;
    if(randomize_mixing12_ih) input_mixing12_ih = conv::kUsed_Sinsq2Theta12;
  }
  else {
    if(randomize_mixing13_ih) input_mixing13_ih = conv::kUsed_SinsqTheta13;
    if(randomize_mixing23_ih) input_mixing23_ih = conv::kUsed_SinsqTheta23;
    if(randomize_mixing12_ih) input_mixing12_ih = conv::kUsed_SinsqTheta12;
  }
  if(randomize_dm23sq_ih) input_dm23sq_ih = conv::kUsed_Dm23sq;
  if(randomize_dm12sq_ih) input_dm12sq_ih = conv::kUsed_Dm12sq;
  if(randomize_dcp_ih)    input_dcp_ih    = conv::kUsed_DeltaCP;
  
  //NH
  if(Opt::Instance()->gOscMixingParamChoice == conv::kNuMixSinsq2Theta) {
    if(randomize_mixing13bar_nh) input_mixing13bar_nh = conv::kUsed_Sinsq2Theta13;
    if(randomize_mixing23bar_nh) input_mixing23bar_nh = conv::kUsed_Sinsq2Theta23;
    if(randomize_mixing12bar_nh) input_mixing12bar_nh = conv::kUsed_Sinsq2Theta12;
  }
  else {
    if(randomize_mixing13bar_nh) input_mixing13bar_nh = conv::kUsed_SinsqTheta13;
    if(randomize_mixing23bar_nh) input_mixing23bar_nh = conv::kUsed_SinsqTheta23;
    if(randomize_mixing12bar_nh) input_mixing12bar_nh = conv::kUsed_SinsqTheta12;
  }
  if(randomize_dm23sqbar_nh) input_dm23sqbar_nh = conv::kUsed_Dm23sq;
  if(randomize_dm12sqbar_nh) input_dm12sqbar_nh = conv::kUsed_Dm12sq;
  if(randomize_dcpbar_nh)    input_dcpbar_nh    = conv::kUsed_DeltaCP;
  //IH
  if(Opt::Instance()->gOscMixingParamChoice == conv::kNuMixSinsq2Theta) {
    if(randomize_mixing13bar_ih) input_mixing13bar_ih = conv::kUsed_Sinsq2Theta13;
    if(randomize_mixing23bar_ih) input_mixing23bar_ih = conv::kUsed_Sinsq2Theta23;
    if(randomize_mixing12bar_ih) input_mixing12bar_ih = conv::kUsed_Sinsq2Theta12;
  }
  else {
    if(randomize_mixing13bar_ih) input_mixing13bar_ih = conv::kUsed_SinsqTheta13;
    if(randomize_mixing23bar_ih) input_mixing23bar_ih = conv::kUsed_SinsqTheta23;
    if(randomize_mixing12bar_ih) input_mixing12bar_ih = conv::kUsed_SinsqTheta12;
  }
  if(randomize_dm23sqbar_ih) input_dm23sqbar_ih = conv::kUsed_Dm23sq;
  if(randomize_dm12sqbar_ih) input_dm12sqbar_ih = conv::kUsed_Dm12sq;
  if(randomize_dcpbar_ih)    input_dcpbar_ih    = conv::kUsed_DeltaCP;
  //Beta bar
  if(randomize_beta_bar) input_beta_bar = conv::kUsed_BetaBar;
  
  
  //Set to defaults (only if enough negative (standard value is -99)
  //NH
  if(Opt::Instance()->gOscMixingParamChoice == conv::kNuMixSinsq2Theta) {
    if(input_mixing13_nh < -1 && input_mixing13_nh > -900) input_mixing13_nh = conv::kUsed_Sinsq2Theta13;
    if(input_mixing23_nh < -1 && input_mixing23_nh > -900) input_mixing23_nh = conv::kUsed_Sinsq2Theta23;
    if(input_mixing12_nh < -1 && input_mixing12_nh > -900) input_mixing12_nh = conv::kUsed_Sinsq2Theta12;
  }
  else {
    if(input_mixing13_nh < -1 && input_mixing13_nh > -900) input_mixing13_nh = conv::kUsed_SinsqTheta13;
    if(input_mixing23_nh < -1 && input_mixing23_nh > -900) input_mixing23_nh = conv::kUsed_SinsqTheta23;
    if(input_mixing12_nh < -1 && input_mixing12_nh > -900) input_mixing12_nh = conv::kUsed_SinsqTheta12;
  }
  if(input_dm23sq_nh < -1  && input_dm23sq_nh > -900) input_dm23sq_nh = conv::kUsed_Dm23sq;
  if(input_dm12sq_nh < -1  && input_dm12sq_nh > -900) input_dm12sq_nh = conv::kUsed_Dm12sq;
  if(input_dcp_nh    < -90 && input_dcp_nh    > -900) input_dcp_nh    = conv::kUsed_DeltaCP;
  //IH
  if(Opt::Instance()->gOscMixingParamChoice == conv::kNuMixSinsq2Theta) {
    if(input_mixing13_ih < -1 && input_mixing13_ih > -900) input_mixing13_ih = conv::kUsed_Sinsq2Theta13;
    if(input_mixing23_ih < -1 && input_mixing23_ih > -900) input_mixing23_ih = conv::kUsed_Sinsq2Theta23;
    if(input_mixing12_ih < -1 && input_mixing12_ih > -900) input_mixing12_ih = conv::kUsed_Sinsq2Theta12;
  }
  else {
    if(input_mixing13_ih < -1 && input_mixing13_ih > -900) input_mixing13_ih = conv::kUsed_SinsqTheta13;
    if(input_mixing23_ih < -1 && input_mixing23_ih > -900) input_mixing23_ih = conv::kUsed_SinsqTheta23;
    if(input_mixing12_ih < -1 && input_mixing12_ih > -900) input_mixing12_ih = conv::kUsed_SinsqTheta12;
  }
  if(input_dm23sq_ih < -1  && input_dm23sq_ih > -900) input_dm23sq_ih = conv::kUsed_Dm23sq;
  if(input_dm12sq_ih < -1  && input_dm12sq_ih > -900) input_dm12sq_ih = conv::kUsed_Dm12sq;
  if(input_dcp_ih    < -90 && input_dcp_ih    > -900) input_dcp_ih = conv::kUsed_DeltaCP;
  //Beta bar
  if(input_beta_bar < -90 && input_beta_bar > -900) input_beta_bar = conv::kUsed_BetaBar;
  
  //NHBar
  if(Opt::Instance()->gOscMixingParamChoice == conv::kNuMixSinsq2Theta) {
    if(input_mixing13bar_nh < -1 && input_mixing13bar_nh > -900) input_mixing13bar_nh = conv::kUsed_Sinsq2Theta13;
    if(input_mixing23bar_nh < -1 && input_mixing23bar_nh > -900) input_mixing23bar_nh = conv::kUsed_Sinsq2Theta23;
    if(input_mixing12bar_nh < -1 && input_mixing12bar_nh > -900) input_mixing12bar_nh = conv::kUsed_Sinsq2Theta12;
  }
  else {
    if(input_mixing13bar_nh < -1 && input_mixing13bar_nh > -900) input_mixing13bar_nh = conv::kUsed_SinsqTheta13;
    if(input_mixing23bar_nh < -1 && input_mixing23bar_nh > -900) input_mixing23bar_nh = conv::kUsed_SinsqTheta23;
    if(input_mixing12bar_nh < -1 && input_mixing12bar_nh > -900) input_mixing12bar_nh = conv::kUsed_SinsqTheta12;
  }
  if(input_dm23sqbar_nh < -1  && input_dm23sqbar_nh > -900) input_dm23sqbar_nh = conv::kUsed_Dm23sq;
  if(input_dm12sqbar_nh < -1  && input_dm12sqbar_nh > -900) input_dm12sqbar_nh = conv::kUsed_Dm12sq;
  if(input_dcpbar_nh    < -90 && input_dcpbar_nh    > -900) input_dcpbar_nh    = conv::kUsed_DeltaCP;
  //IHBar
  if(Opt::Instance()->gOscMixingParamChoice == conv::kNuMixSinsq2Theta) {
    if(input_mixing13bar_ih < -1 && input_mixing13bar_ih > -900) input_mixing13bar_ih = conv::kUsed_Sinsq2Theta13;
    if(input_mixing23bar_ih < -1 && input_mixing23bar_ih > -900) input_mixing23bar_ih = conv::kUsed_Sinsq2Theta23;
    if(input_mixing12bar_ih < -1 && input_mixing12bar_ih > -900) input_mixing12bar_ih = conv::kUsed_Sinsq2Theta12;
  }
  else {
    if(input_mixing13bar_ih < -1 && input_mixing13bar_ih > -900) input_mixing13bar_ih = conv::kUsed_SinsqTheta13;
    if(input_mixing23bar_ih < -1 && input_mixing23bar_ih > -900) input_mixing23bar_ih = conv::kUsed_SinsqTheta23;
    if(input_mixing12bar_ih < -1 && input_mixing12bar_ih > -900) input_mixing12bar_ih = conv::kUsed_SinsqTheta12;
  }
  if(input_dm23sqbar_ih < -1  && input_dm23sqbar_ih > -900) input_dm23sqbar_ih = conv::kUsed_Dm23sq;
  if(input_dm12sqbar_ih < -1  && input_dm12sqbar_ih > -900) input_dm12sqbar_ih = conv::kUsed_Dm12sq;
  if(input_dcpbar_ih    < -90 && input_dcpbar_ih    > -900) input_dcpbar_ih = conv::kUsed_DeltaCP;
  
  
  //MH
  if(input_mh_nh_prior < -1 && input_mh_nh_prior > -900) input_mh_nh_prior = conv::kPrior_MH;
  
  //cerr << "Default value for input_mh_nh_prior is hard coded. This should be moved into the relevant Conventions file" << endl;
  
  //Set the input values in the OscParams
  //NH
  mixing12_nh = input_mixing12_nh;
  mixing13_nh = input_mixing13_nh;
  mixing23_nh = input_mixing23_nh;
  dcp_nh      = input_dcp_nh;
  dm12sq_nh   = input_dm12sq_nh;
  dm23sq_nh   = input_dm23sq_nh;
  //IH
  mixing12_ih = input_mixing12_ih;
  mixing13_ih = input_mixing13_ih;
  mixing23_ih = input_mixing23_ih;
  dcp_ih      = input_dcp_ih;
  dm12sq_ih   = input_dm12sq_ih;
  dm23sq_ih   = input_dm23sq_ih;
  
  //NH
  mixing12bar_nh = input_mixing12bar_nh;
  mixing13bar_nh = input_mixing13bar_nh;
  mixing23bar_nh = input_mixing23bar_nh;
  dcpbar_nh      = input_dcpbar_nh;
  dm12sqbar_nh   = input_dm12sqbar_nh;
  dm23sqbar_nh   = input_dm23sqbar_nh;
  //IH
  mixing12bar_ih = input_mixing12bar_ih;
  mixing13bar_ih = input_mixing13bar_ih;
  mixing23bar_ih = input_mixing23bar_ih;
  dcpbar_ih      = input_dcpbar_ih;
  dm12sqbar_ih   = input_dm12sqbar_ih;
  dm23sqbar_ih   = input_dm23sqbar_ih;
  
  
  //Other
  beta_bar       = input_beta_bar;
  
  marginalize_mh = marginalize_MH;
  mh_nh_prior    = input_mh_nh_prior;
  MH             = Opt::Instance()->gMassHierarchyAssumed;
  iMH            = (MH == kNmhNormal) ? 0 : 1;
}
//____________________________________________________________________________
void valor::util::OscParams::Randomize()
{
  // Mass Hierarchy
  
#ifdef __print_toys__
  cout << endl;
  cout << "Before toys: " << endl
  << "Mass Hierarchy (local) = " << iMH << endl
  << " (global) = " << Opt::Instance()->gMassHierarchyAssumed
  << endl;
#endif
  
  if(marginalize_mh){
    
    const double min = 0.;
    const double max = 1.;
    double Pnh = mh_nh_prior;
    double Pih = 1 - Pnh;
    
    if(Pnh < 0. || Pnh > 1.){
      cerr << endl;
      cerr << "Randomize" << endl;
      cerr << "The input probability for NH is not possible" << endl;
      cerr << "P(NH) = " << Pnh << endl;
      cerr << endl;
      exit(1);
    }
    if(Pih < 0. || Pih > 1.){
      cerr << endl;
      cerr << "Randomize" << endl;
      cerr << "The input probability for IH is not possible" << endl;
      cerr << "P(IH) = " << Pih << endl;
      cerr << endl;
      exit(1);
    }
    
    double rndm = Rnd::Instance()->RndToyMCosc().Uniform(min,max);
    
    iMH = conv::kBadResult;
    if(rndm <= Pnh) iMH = 0; // Normal Hierarchy
    else            iMH = 1; // Inverted Hierarchy
    
    if( iMH==0 ){
      Opt::Instance()->gMassHierarchyAssumed = util::HierarchyFromString("normal");
    }
    else if( iMH==1 ){
      Opt::Instance()->gMassHierarchyAssumed = util::HierarchyFromString("inverted");
    }
    else{
      cerr << endl;
      cerr << "Mass Hierarchy " << iMH << " is not defined" << endl;
      cerr << endl;
      exit(1);
    }
    
  } // Mass Hierarchy
  
  double par1 = conv::kBadResult;
  double par2 = conv::kBadResult;
  string mixmode = "";
  string priormode = "";
  
  //Used to check if osc paramters have been generated for normal osillactions.
  //If so the NuBar oscillation parameters will be equal, if not marginalised with
  // Nu oscillation priors
  bool rand_mix23_nh 		= false;
  bool rand_mix13_nh 		= false;
  bool rand_mix12_nh 		= false;
  bool rand_dcp_nh 	 	= false;
  bool rand_dm23sq_nh   	= false;
  bool rand_dm12sq_nh 		= false;
  bool rand_mix23_ih 		= false;
  bool rand_mix13_ih 		= false;
  bool rand_mix12_ih 		= false;
  bool rand_dcp_ih 	 	= false;
  bool rand_dm23sq_ih   	= false;
  bool rand_dm12sq_ih 		= false;

  bool rand_mix23bar_nh 	= false;
  bool rand_mix13bar_nh 	= false;
  bool rand_mix12bar_nh 	= false;
  bool rand_dcpbar_nh 	 	= false;
  bool rand_dm23sqbar_nh        = false;
  bool rand_dm12sqbar_nh 	= false;
  bool rand_mix23bar_ih 	= false;
  bool rand_mix13bar_ih 	= false;
  bool rand_mix12bar_ih 	= false;
  bool rand_dcpbar_ih 	 	= false;
  bool rand_dm23sqbar_ih        = false;
  bool rand_dm12sqbar_ih 	= false;
  
  // Normal Hierarchy
  if(iMH==0){
    
    // mixing23
    par1 = params_mixing23[0];
    par2 = params_mixing23[1];
    priormode = priormode_mixing23;
    mixmode = mixmode_mixing23;
    if(randomize_mixing23_nh){
      mixing23_nh = this->ExtractParam(conv::kPhypMixing23,priormode,par1,par2,mixmode);
      rand_mix23_nh = true;
    }
    
    // mixing13
    par1 = params_mixing13[0];
    par2 = params_mixing13[1];
    priormode = priormode_mixing13;
    mixmode = mixmode_mixing13;
    if(randomize_mixing13_nh){
      mixing13_nh = this->ExtractParam(conv::kPhypMixing13,priormode,par1,par2,mixmode);
      rand_mix13_nh	= true;
    }
    
    // deltaCP
    par1 = params_dcp[0];
    par2 = params_dcp[1];
    priormode = priormode_dcp;
    mixmode = "";
    if(randomize_dcp_nh){
      dcp_nh = this->ExtractParam(conv::kPhypCPV,priormode,par1,par2,mixmode);
      rand_dcp_nh = true;
    }
    
    // dmsq23
    par1 = params_dm23sq[0];
    par2 = params_dm23sq[1];
    priormode = priormode_dm23sq;
    mixmode = "";
    if(randomize_dm23sq_nh){
      dm23sq_nh = this->ExtractParam(conv::kPhypDm23sq,priormode,par1,par2,mixmode);
      rand_dm23sq_nh = true;
    }
    
    // dmsq12
    par1 = params_dm12sq[0];
    par2 = params_dm12sq[1];
    priormode = priormode_dm12sq;
    mixmode = "";
    if(randomize_dm12sq_nh){
      dm12sq_nh = this->ExtractParam(conv::kPhypDm12sq,priormode,par1,par2,mixmode);
      rand_dm12sq_nh = true;
    }
    
    // mixing12
    par1 = params_mixing12[0];
    par2 = params_mixing12[1];
    priormode = priormode_mixing12;
    mixmode = mixmode_mixing12;
    if(randomize_mixing12_nh){
      mixing12_nh = this->ExtractParam(conv::kPhypMixing12,priormode,par1,par2,mixmode);
      rand_mix12_nh = true;
    }    
        
    //NuBar Parameters!

    // mixing23
    par1 = params_mixing23[0];
    par2 = params_mixing23[1];
    priormode = priormode_mixing23;
    mixmode = mixmode_mixing23;
    if(randomize_mixing23bar_nh){
      mixing23bar_nh = this->ExtractParam(conv::kPhypMixing23Bar,priormode,par1,par2,mixmode);
      rand_mix23bar_nh = true;
    }
    
    // mixing13
    par1 = params_mixing13[0];
    par2 = params_mixing13[1];
    priormode = priormode_mixing13;
    mixmode = mixmode_mixing13;
    if(randomize_mixing13bar_nh){
      mixing13bar_nh = this->ExtractParam(conv::kPhypMixing13Bar,priormode,par1,par2,mixmode);
      rand_mix13bar_nh	= true;
    }
    
    // deltaCP
    par1 = params_dcp[0];
    par2 = params_dcp[1];
    priormode = priormode_dcp;
    mixmode = "";
    if(randomize_dcpbar_nh){
      dcpbar_nh = this->ExtractParam(conv::kPhypCPVBar,priormode,par1,par2,mixmode);
      rand_dcpbar_nh = true;
    }
    
    // dmsq23
    par1 = params_dm23sq[0];
    par2 = params_dm23sq[1];
    priormode = priormode_dm23sq;
    mixmode = "";
    if(randomize_dm23sqbar_nh){
      dm23sqbar_nh = this->ExtractParam(conv::kPhypDm23sqBar,priormode,par1,par2,mixmode);
      rand_dm23sqbar_nh = true;
    }
    
    // dmsq12
    par1 = params_dm12sq[0];
    par2 = params_dm12sq[1];
    priormode = priormode_dm12sq;
    mixmode = "";
    if(randomize_dm12sqbar_nh){
      dm12sqbar_nh = this->ExtractParam(conv::kPhypDm12sqBar,priormode,par1,par2,mixmode);
      rand_dm12sqbar_nh = true;
    }
 
    // mixing12
    par1 = params_mixing12[0];
    par2 = params_mixing12[1];
    priormode = priormode_mixing12;
    mixmode = mixmode_mixing12;
    if(randomize_mixing12bar_nh){
      mixing12bar_nh = this->ExtractParam(conv::kPhypMixing12Bar,priormode,par1,par2,mixmode);
      rand_mix12bar_nh = true;
    }
    
  }
  
  // Inverted Hierarchy
  
  else if(iMH==1){
    
    // mixing23
    par1 = params_mixing23[0];
    par2 = params_mixing23[1];
    priormode = priormode_mixing23;
    mixmode = mixmode_mixing23;
    if(randomize_mixing23_ih){
      mixing23_ih = this->ExtractParam(conv::kPhypMixing23,priormode,par1,par2,mixmode);
      rand_mix23_ih = true;
    }
    
    // mixing13
    par1 = params_mixing13[0];
    par2 = params_mixing13[1];
    priormode = priormode_mixing13;
    mixmode = mixmode_mixing13;
    if(randomize_mixing13_ih){
      mixing13_ih = this->ExtractParam(conv::kPhypMixing13,priormode,par1,par2,mixmode);
      rand_mix13_ih = true;
    }
    
    // deltaCP
    par1 = params_dcp[0];
    par2 = params_dcp[1];
    priormode = priormode_dcp;
    mixmode = "";
    if(randomize_dcp_ih){
      dcp_ih = this->ExtractParam(conv::kPhypCPV,priormode,par1,par2,mixmode);
      rand_dcp_ih = true;
    }
    
    // dmsq23
    par1 = params_dm23sq[0];
    par2 = params_dm23sq[1];
    priormode = priormode_dm23sq;
    mixmode = "";
    if(randomize_dm23sq_ih){
      dm23sq_ih = this->ExtractParam(conv::kPhypDm23sq,priormode,par1,par2,mixmode);
      rand_dm23sq_ih = true;
    }
    
    // dmsq12
    par1 = params_dm12sq[0];
    par2 = params_dm12sq[1];
    priormode = priormode_dm12sq;
    mixmode = "";
    if(randomize_dm12sq_ih){
      dm12sq_ih = this->ExtractParam(conv::kPhypDm12sq,priormode,par1,par2,mixmode);
      rand_dm12sq_ih = true;
    }
     
    // mixing12
    par1 = params_mixing12[0];
    par2 = params_mixing12[1];
    priormode = priormode_mixing12;
    mixmode = mixmode_mixing12;
    if(randomize_mixing12_ih){
      mixing12_ih = this->ExtractParam(conv::kPhypMixing12,priormode,par1,par2,mixmode);
      rand_mix12_ih = true;
    }
    
    // mixing23
    par1 = params_mixing23[0];
    par2 = params_mixing23[1];
    priormode = priormode_mixing23;
    mixmode = mixmode_mixing23;
    if(randomize_mixing23bar_ih){
      mixing23bar_ih = this->ExtractParam(conv::kPhypMixing23Bar,priormode,par1,par2,mixmode);
      rand_mix23bar_ih = true;
    }
    
    // mixing13
    par1 = params_mixing13[0];
    par2 = params_mixing13[1];
    priormode = priormode_mixing13;
    mixmode = mixmode_mixing13;
    if(randomize_mixing13bar_ih){
      mixing13bar_ih = this->ExtractParam(conv::kPhypMixing13Bar,priormode,par1,par2,mixmode);
      rand_mix13bar_ih	= true;
    }
    
    // deltaCP
    par1 = params_dcp[0];
    par2 = params_dcp[1];
    priormode = priormode_dcp;
    mixmode = "";
    if(randomize_dcpbar_ih){
      dcpbar_ih = this->ExtractParam(conv::kPhypCPVBar,priormode,par1,par2,mixmode);
      rand_dcpbar_ih = true;
    }
    
    // dmsq23
    par1 = params_dm23sq[0];
    par2 = params_dm23sq[1];
    priormode = priormode_dm23sq;
    mixmode = "";
    if(randomize_dm23sqbar_ih){
      dm23sqbar_ih = this->ExtractParam(conv::kPhypDm23sqBar,priormode,par1,par2,mixmode);
      rand_dm23sqbar_ih = true;
    }
    
    // dmsq12
    par1 = params_dm12sq[0];
    par2 = params_dm12sq[1];
    priormode = priormode_dm12sq;
    mixmode = "";
    if(randomize_dm12sqbar_ih){
      dm12sqbar_ih = this->ExtractParam(conv::kPhypDm12sqBar,priormode,par1,par2,mixmode);
      rand_dm12sqbar_ih = true;
    }
    
    // mixing12
    par1 = params_mixing12[0];
    par2 = params_mixing12[1];
    priormode = priormode_mixing12;
    mixmode = mixmode_mixing12;
    if(randomize_mixing12bar_ih){
      mixing12bar_ih = this->ExtractParam(conv::kPhypMixing12Bar,priormode,par1,par2,mixmode);
      rand_mix12bar_ih = true;
    }

  }
  
  else{
    cerr << endl;
    cerr << "Mass Hierarchy is not defined" << endl;
    cerr << endl;
    exit(1);
  }
  
#ifdef __print_toys__
  cout << endl;
  cout << "After toys: " << endl;
  cout << "Mass hierarchy = " << iMH << endl;
  cout << "Mass hierarchy global = " << Opt::Instance()->gMassHierarchyAssumed << endl;
  cout << endl;
  cout << "mixing12_nh = " << mixing12_nh << endl;
  cout << "mixing13_nh = " << mixing13_nh << endl;
  cout << "mixing23_nh = " << mixing23_nh << endl;
  cout << "dcp_nh = " << dcp_nh << endl;
  cout << "dm12sq_nh = " << dm12sq_nh << endl;
  cout << "dm23sq_nh = " << dm23sq_nh << endl;
  cout << endl;
  cout << "mixing12_ih = " << mixing12_ih << endl;
  cout << "mixing13_ih = " << mixing13_ih << endl;
  cout << "mixing23_ih = " << mixing23_ih << endl;
  cout << "dcp_ih = " << dcp_ih << endl;
  cout << "dm12sq_ih = " << dm12sq_ih << endl;
  cout << "dm23sq_ih = " << dm23sq_ih << endl;
  cout << endl;
#endif
  
}
//____________________________________________________________________________
void valor::util::OscParams::FillOscToyHisto(PhysHypothesis &ph)
{
  
  // Fill toy osc params histograms

  double mix12 = conv::kBadResult;
  double mix13 = conv::kBadResult;
  double mix23 = conv::kBadResult;
  double dm12  = conv::kBadResult;
  double dm23  = conv::kBadResult;
  double dcp   = conv::kBadResult;
  double mix12bar = conv::kBadResult;
  double mix13bar = conv::kBadResult;
  double mix23bar = conv::kBadResult;
  double dm12bar  = conv::kBadResult;
  double dm23bar  = conv::kBadResult;
  double dcpbar   = conv::kBadResult;
  
  if(ph.Contains(kPhypMixing12)) mix12 = ph.CurrValue(kPhypMixing12);
  if(ph.Contains(kPhypMixing13)) mix13 = ph.CurrValue(kPhypMixing13);
  if(ph.Contains(kPhypMixing23)) mix23 = ph.CurrValue(kPhypMixing23);
  if(ph.Contains(kPhypDm12sq))   dm12 = ph.CurrValue(kPhypDm12sq);
  if(ph.Contains(kPhypDm23sq))   dm23 = ph.CurrValue(kPhypDm23sq);
  if(ph.Contains(kPhypCPV))      dcp = ph.CurrValue(kPhypCPV);
  
  if(ph.Contains(kPhypMixing12Bar)) mix12bar = ph.CurrValue(kPhypMixing12Bar);
  if(ph.Contains(kPhypMixing13Bar)) mix13bar = ph.CurrValue(kPhypMixing13Bar);
  if(ph.Contains(kPhypMixing23Bar)) mix23bar = ph.CurrValue(kPhypMixing23Bar);
  if(ph.Contains(kPhypDm12sqBar))   dm12bar = ph.CurrValue(kPhypDm12sqBar);
  if(ph.Contains(kPhypDm23sqBar))   dm23bar = ph.CurrValue(kPhypDm23sqBar);
  if(ph.Contains(kPhypCPVBar))      dcpbar = ph.CurrValue(kPhypCPVBar);
  
  int mh = Opt::Instance()->gMassHierarchyAssumed;
  
  if(mh==conv::kNmhNormal){
    hToyHisto_Mix12_nh->Fill(mix12);
    hToyHisto_Mix13_nh->Fill(mix13);
    hToyHisto_Mix23_nh->Fill(mix23);
    hToyHisto_Dm12_nh->Fill(dm12);
    hToyHisto_Dm23_nh->Fill(dm23);
    hToyHisto_dcp_nh->Fill(dcp);
    hToyHisto_Mix12Bar_nh->Fill(mix12bar);
    hToyHisto_Mix13Bar_nh->Fill(mix13bar);
    hToyHisto_Mix23Bar_nh->Fill(mix23bar);
    hToyHisto_Dm12Bar_nh->Fill(dm12bar);
    hToyHisto_Dm23Bar_nh->Fill(dm23bar);
    hToyHisto_dcpBar_nh->Fill(dcpbar);
  }
  else if(mh==conv::kNmhInverted){
    hToyHisto_Mix12_ih->Fill(mix12);
    hToyHisto_Mix13_ih->Fill(mix13);
    hToyHisto_Mix23_ih->Fill(mix23);
    hToyHisto_Dm12_ih->Fill(dm12);
    hToyHisto_Dm23_ih->Fill(dm23);
    hToyHisto_dcp_ih->Fill(dcp);
    hToyHisto_Mix12Bar_ih->Fill(mix12bar);
    hToyHisto_Mix13Bar_ih->Fill(mix13bar);
    hToyHisto_Mix23Bar_ih->Fill(mix23bar);
    hToyHisto_Dm12Bar_ih->Fill(dm12bar);
    hToyHisto_Dm23Bar_ih->Fill(dm23bar);
    hToyHisto_dcpBar_ih->Fill(dcpbar);
  }
  hToyHisto_MH->Fill(mh);
  //cout << "mh = " << mh << endl;
}
//____________________________________________________________________________
TH1D *valor::util::OscParams::GetToyHisto(string oscparam, string MH_str)
{
  if(oscparam=="hierarchy"){
    return hToyHisto_MH;
  }
  
  if(MH_str=="normal"){
    if     (oscparam=="deltaCP")      return hToyHisto_dcp_nh;
    else if(oscparam=="mixing23")     return hToyHisto_Mix23_nh;
    else if(oscparam=="mixing13")     return hToyHisto_Mix13_nh;
    else if(oscparam=="dmsq23")       return hToyHisto_Dm23_nh;
    else if(oscparam=="mixing12")     return hToyHisto_Mix12_nh;
    else if(oscparam=="dmsq12")       return hToyHisto_Dm12_nh;
    else if(oscparam=="deltaCPBar")   return hToyHisto_dcpBar_nh;
    else if(oscparam=="mixing23Bar")  return hToyHisto_Mix23Bar_nh;
    else if(oscparam=="mixing13Bar")  return hToyHisto_Mix13Bar_nh;
    else if(oscparam=="dmsq23Bar")    return hToyHisto_Dm23Bar_nh;
    else if(oscparam=="mixing12Bar")  return hToyHisto_Mix12Bar_nh;
    else if(oscparam=="dmsq12Bar")    return hToyHisto_Dm12Bar_nh;
    else {
      cerr << endl;
      cerr << "The oscillation parameters is not defined: " << oscparam << endl;
      cerr << "Following oscillation parameters are shown:" << endl;
      cerr << " - deltaCP, mixing23, mixing13, dmsq23, mixing12, dmsq12" << endl;
      cerr << endl;
      exit(1);
    }
  }
  else if(MH_str=="inverted"){
    if     (oscparam=="deltaCP")      return hToyHisto_dcp_ih;
    else if(oscparam=="mixing23")     return hToyHisto_Mix23_ih;
    else if(oscparam=="mixing13")     return hToyHisto_Mix13_ih;
    else if(oscparam=="dmsq23")       return hToyHisto_Dm23_ih;
    else if(oscparam=="mixing12")     return hToyHisto_Mix12_ih;
    else if(oscparam=="dmsq12")       return hToyHisto_Dm12_ih;
    else if(oscparam=="deltaCPBar")   return hToyHisto_dcpBar_ih;
    else if(oscparam=="mixing23Bar")  return hToyHisto_Mix23Bar_ih;
    else if(oscparam=="mixing13Bar")  return hToyHisto_Mix13Bar_ih;
    else if(oscparam=="dmsq23Bar")    return hToyHisto_Dm23Bar_ih;
    else if(oscparam=="mixing12Bar")  return hToyHisto_Mix12Bar_ih;
    else if(oscparam=="dmsq12Bar")    return hToyHisto_Dm12Bar_ih;
    else {
      cerr << endl;
      cerr << "The oscillation parameters is not defined: " << oscparam << endl;
      cerr << "Following oscillation parameters are shown:" << endl;
      cerr << " - deltaCP, mixing23, mixing13, dmsq23, mixing12, dmsq12" << endl;
      cerr << endl;
      exit(1);
    }
  }
  else {
    cerr << endl;
    cerr << "Mass Hierarchy set to " << MH_str << " is not defined" << endl;
    cerr << "Following options are available: normal or inverted" << endl;
    cerr << endl;
    exit(1);
  }
}
//____________________________________________________________________________
double valor::util::OscParams::ExtractParam(conv::PhysParamId_t physpar,string mode,double par1,double par2,string mixmode)
{
  double valrndm = conv::kBadResult;
  
  //cout << util::RawParamAsString(physpar) << endl;

  if(mode=="gaus")
    valrndm = Rnd::Instance()->RndToyMCosc().Gaus(par1,par2);
  else if(mode=="flat"){
    if(par1>=par2){
      cerr << endl;
      cerr << "valor::util::OscParams::ExtractParam" << endl;
      cerr << "Parameters for " << mode << " prior are not set correctly" << endl;
      cerr << "par1 = " << par1 << " - par2 = " << par2 << endl;
      cerr << endl;
      exit(1);
    }
    valrndm = Rnd::Instance()->RndToyMCosc().Uniform(par1,par2);
  }
  else{
    cerr << endl;
    cerr << "valor::util::OscParams::ExtractParam" << endl;
    cerr << "Prior is not defined: " << mode << endl;
    cerr << endl;
    exit(1);
  }
  
  if(IsMixingAngle(physpar)){
    if(Opt::Instance()->gOscMixingParamChoice == kNuMixSinsqTheta){
      if(mixmode == "sinsqth")       return valrndm;
      else if(mixmode == "sinsq2th") return util::FromSinsq2thToSinsqth(valrndm);
      else {
        cerr << endl;
	cerr << "valor::util::OscParams::ExtractParam" << endl;
        cerr << "mixmode is not defined: " << mixmode << endl;
        cerr << endl;
        exit(1);
      }
    }
    else if(Opt::Instance()->gOscMixingParamChoice == kNuMixSinsq2Theta){
      if(mixmode == "sinsqth")       return util::FromSinsqthToSinsq2th(valrndm);
      else if(mixmode == "sinsq2th") return valrndm;
      else {
        cerr << endl;
        cerr << "valor::util::OscParams::ExtractParam" << endl;
	cerr << "mixmode is not defined: " << mixmode << endl;
        cerr << endl;
        exit(1);
      }
    }
  }

  return valrndm;
}
//____________________________________________________________________________
void valor::util::OscParams::UpdatePhysHypothesis(PhysHypothesis& ph, bool print)
{
  double input_mixing12_used, input_mixing13_used, input_mixing23_used;
  double input_dcp_used,      input_dm12sq_used,   input_dm23sq_used;
  double input_beta_bar_used;
  
  double input_mixing12bar_used, input_mixing13bar_used, input_mixing23bar_used;
  double input_dcpbar_used,      input_dm12sqbar_used,   input_dm23sqbar_used;

  //Get the values to use (based on MH)
  if(iMH == 0) {
    //NH
    input_mixing12_used = mixing12_nh;
    input_mixing13_used = mixing13_nh;
    input_mixing23_used = mixing23_nh;
    input_dcp_used      = dcp_nh;
    input_dm12sq_used   = dm12sq_nh;
    input_dm23sq_used   = dm23sq_nh;
    input_mixing12bar_used = mixing12bar_nh;
    input_mixing13bar_used = mixing13bar_nh;
    input_mixing23bar_used = mixing23bar_nh;
    input_dcpbar_used      = dcpbar_nh;
    input_dm12sqbar_used   = dm12sqbar_nh;
    input_dm23sqbar_used   = dm23sqbar_nh;
  }
  else if(iMH == 1) {
    //IH
    input_mixing12_used = mixing12_ih;
    input_mixing13_used = mixing13_ih;
    input_mixing23_used = mixing23_ih;
    input_dcp_used      = dcp_ih;
    input_dm12sq_used   = dm12sq_ih;
    input_dm23sq_used   = dm23sq_ih;
    input_mixing12bar_used = mixing12bar_ih;
    input_mixing13bar_used = mixing13bar_ih;
    input_mixing23bar_used = mixing23bar_ih;
    input_dcpbar_used      = dcpbar_ih;
    input_dm12sqbar_used   = dm12sqbar_ih;
    input_dm23sqbar_used   = dm23sqbar_ih;
  }
  else {
    cerr << "SetOscParams has unknown value of iMH: " << iMH << endl;
    exit(1);
  }
  //Other
  input_beta_bar_used = beta_bar;  // It's never randomized
  
#ifdef __print_toys__
  cout << endl;
  cout << "Used values:" << endl;
  cout << endl;
  cout << "input_mixing12_used = " << input_mixing12_used << endl;
  cout << "input_mixing13_used = " << input_mixing13_used << endl;
  cout << "input_mixing23_used = " << input_mixing23_used << endl;
  cout << "input_dcp_used = " << input_dcp_used << endl;
  cout << "input_dm12sq_used = " << input_dm12sq_used << endl;
  cout << "input_dm23sq_used = " << input_dm23sq_used << endl;
  cout << "input_mixing12bar_used = " << input_mixing12bar_used << endl;
  cout << "input_mixing13bar_used = " << input_mixing13bar_used << endl;
  cout << "input_mixing23bar_used = " << input_mixing23bar_used << endl;
  cout << "input_dcpbar_used = " << input_dcpbar_used << endl;
  cout << "input_dm12sqbar_used = " << input_dm12sqbar_used << endl;
  cout << "input_dm23sqbar_used = " << input_dm23sqbar_used << endl;
  cout << endl;
#endif
  
  //Update the PhysHypothesis
  if(ph.Hypothesis() == kHypNuMuDisapp2F) {
    ph.SetCurrValue(kPhypMixing23,  input_mixing23_used);
    ph.SetCurrValue(kPhypDm23sq,    input_dm23sq_used);
  }//kHypNuMuDisapp2F
  else if(ph.Hypothesis() == kHypOsc3F) {
    ph.SetCurrValue(kPhypMixing12,  input_mixing12_used);
    ph.SetCurrValue(kPhypMixing13,  input_mixing13_used);
    ph.SetCurrValue(kPhypMixing23,  input_mixing23_used);
    ph.SetCurrValue(kPhypCPV    ,   input_dcp_used);
    ph.SetCurrValue(kPhypDm12sq,    input_dm12sq_used);
    ph.SetCurrValue(kPhypDm23sq,    input_dm23sq_used);
  }//kHypOsc3F
  else if(ph.Hypothesis() == kHypOsc3FBar) {
    ph.SetCurrValue(kPhypMixing12,  input_mixing12_used);
    ph.SetCurrValue(kPhypMixing13,  input_mixing13_used);
    ph.SetCurrValue(kPhypMixing23,  input_mixing23_used);
    ph.SetCurrValue(kPhypCPV    ,   input_dcp_used);
    ph.SetCurrValue(kPhypDm12sq,    input_dm12sq_used);
    ph.SetCurrValue(kPhypDm23sq,    input_dm23sq_used);
    
    ph.SetCurrValue(kPhypMixing12Bar,  input_mixing12bar_used);
    ph.SetCurrValue(kPhypMixing13Bar,  input_mixing13bar_used);
    ph.SetCurrValue(kPhypMixing23Bar,  input_mixing23bar_used);
    ph.SetCurrValue(kPhypCPVBar    ,   input_dcpbar_used);
    ph.SetCurrValue(kPhypDm12sqBar,    input_dm12sqbar_used);
    ph.SetCurrValue(kPhypDm23sqBar,    input_dm23sqbar_used);
  }//kHypOsc3FBar
  else if(ph.Hypothesis() == kHypOsc3FBarDissap) {
    ph.SetCurrValue(kPhypMixing12,  input_mixing12_used);
    ph.SetCurrValue(kPhypMixing13,  input_mixing13_used);
    ph.SetCurrValue(kPhypMixing23,  input_mixing23_used);
    ph.SetCurrValue(kPhypCPV    ,   input_dcp_used);
    ph.SetCurrValue(kPhypDm12sq,    input_dm12sq_used);
    ph.SetCurrValue(kPhypDm23sq,    input_dm23sq_used);
    
    ph.SetCurrValue(kPhypMixing23Bar,  input_mixing23bar_used);
    ph.SetCurrValue(kPhypDm23sqBar,    input_dm23sqbar_used);
  }//kHypOsc3FBar
  else if(ph.Hypothesis() == kHypOsc3FNormBar) {
    ph.SetCurrValue(kPhypMixing12,  input_mixing12_used);
    ph.SetCurrValue(kPhypMixing13,  input_mixing13_used);
    ph.SetCurrValue(kPhypMixing23,  input_mixing23_used);
    ph.SetCurrValue(kPhypCPV    ,   input_dcp_used);
    ph.SetCurrValue(kPhypDm12sq,    input_dm12sq_used);
    ph.SetCurrValue(kPhypDm23sq,    input_dm23sq_used);
    
    ph.SetCurrValue(kPhypBetaBar,    input_beta_bar_used);
  }//kHypOsc3FNormBar
  else if(ph.Hypothesis() == kHypOsc3FBarNormBar) {
    ph.SetCurrValue(kPhypMixing12,  input_mixing12_used);
    ph.SetCurrValue(kPhypMixing13,  input_mixing13_used);
    ph.SetCurrValue(kPhypMixing23,  input_mixing23_used);
    ph.SetCurrValue(kPhypCPV    ,   input_dcp_used);
    ph.SetCurrValue(kPhypDm12sq,    input_dm12sq_used);
    ph.SetCurrValue(kPhypDm23sq,    input_dm23sq_used);
    
    ph.SetCurrValue(kPhypMixing12Bar,  input_mixing12_used);
    ph.SetCurrValue(kPhypMixing13Bar,  input_mixing13_used);
    ph.SetCurrValue(kPhypMixing23Bar,  input_mixing23_used);
    ph.SetCurrValue(kPhypCPVBar    ,   input_dcp_used);
    ph.SetCurrValue(kPhypDm12sqBar,    input_dm12sq_used);
    ph.SetCurrValue(kPhypDm23sqBar,    input_dm23sq_used);
    
    ph.SetCurrValue(kPhypBetaBar,    input_beta_bar_used);
  }//kHypOsc3FBarNormBar
  else {
    cout << "SetOscParams() " << util::AsString(ph.Hypothesis()) << " not implemented" << endl;
    exit(1);
  }
  
  if(print)
    cout << "- Physics hypothesis set: " << ph << endl;
  
}
//____________________________________________________________________________
void valor::util::OscParams::UpdateFitParamList(FitParamList* fparm, bool set_init, bool print)
{
  double input_mixing12_used, input_mixing13_used, input_mixing23_used;
  double input_dcp_used,      input_dm12sq_used,   input_dm23sq_used;
  double input_beta_bar_used;
  
  //Updated so that NuBar parameters can be set independantly of Nu osc params
  //If marginalised, NuBar and Nu are set equal to each other for now
  //BetaBar fit param lists still use old convention (NuBar osc param = Nu osc param)
  
  double input_mixing12bar_used, input_mixing13bar_used, input_mixing23bar_used;
  double input_dcpbar_used,      input_dm12sqbar_used,   input_dm23sqbar_used;
  
  //Get the values to use (based on MH)
  if(iMH == 0) {
    //NH
    input_mixing12_used = mixing12_nh;
    input_mixing13_used = mixing13_nh;
    input_mixing23_used = mixing23_nh;
    input_dcp_used      = dcp_nh;
    input_dm12sq_used   = dm12sq_nh;
    input_dm23sq_used   = dm23sq_nh;
    input_mixing12bar_used = mixing12bar_nh;
    input_mixing13bar_used = mixing13bar_nh;
    input_mixing23bar_used = mixing23bar_nh;
    input_dcpbar_used      = dcpbar_nh;
    input_dm12sqbar_used   = dm12sqbar_nh;
    input_dm23sqbar_used   = dm23sqbar_nh;
  }
  else if(iMH == 1) {
    //IH
    input_mixing12_used = mixing12_ih;
    input_mixing13_used = mixing13_ih;
    input_mixing23_used = mixing23_ih;
    input_dcp_used      = dcp_ih;
    input_dm12sq_used   = dm12sq_ih;
    input_dm23sq_used   = dm23sq_ih;
    input_mixing12bar_used = mixing12bar_ih;
    input_mixing13bar_used = mixing13bar_ih;
    input_mixing23bar_used = mixing23bar_ih;
    input_dcpbar_used      = dcpbar_ih;
    input_dm12sqbar_used   = dm12sqbar_ih;
    input_dm23sqbar_used   = dm23sqbar_ih;
  }
  else {
    cerr << "SetOscParams has unknown value of iMH: " << iMH << endl;
    exit(1);
  }
  //Other
  input_beta_bar_used = beta_bar;
  
  //Update the FitParamList
  if(fparm->PhysicsHypothesis() == kHypNuMuDisapp2F) {
    if(set_init) {
      fparm->SetInitValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetInitValue(kPhypDm23sq,    input_dm23sq_used);
    }
    else {
      fparm->SetCurrentValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetCurrentValue(kPhypDm23sq,    input_dm23sq_used);
    }
  }//kHypNuMuDisapp2F
  else if(fparm->PhysicsHypothesis() == kHypOsc3F) {
    if(set_init) {
      fparm->SetInitValue(kPhypMixing12,  input_mixing12_used);
      fparm->SetInitValue(kPhypMixing13,  input_mixing13_used);
      fparm->SetInitValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetInitValue(kPhypCPV    ,   input_dcp_used);
      fparm->SetInitValue(kPhypDm12sq,    input_dm12sq_used);
      fparm->SetInitValue(kPhypDm23sq,    input_dm23sq_used);
    }
    else {
      fparm->SetCurrentValue(kPhypMixing12,  input_mixing12_used);
      fparm->SetCurrentValue(kPhypMixing13,  input_mixing13_used);
      fparm->SetCurrentValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetCurrentValue(kPhypCPV    ,   input_dcp_used);
      fparm->SetCurrentValue(kPhypDm12sq,    input_dm12sq_used);
      fparm->SetCurrentValue(kPhypDm23sq,    input_dm23sq_used);
    }
  }//kHypOsc3F
  else if(fparm->PhysicsHypothesis() == kHypOsc3FBar) {
    if(set_init) {
      fparm->SetInitValue(kPhypMixing12,  input_mixing12_used);
      fparm->SetInitValue(kPhypMixing13,  input_mixing13_used);
      fparm->SetInitValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetInitValue(kPhypCPV    ,   input_dcp_used);
      fparm->SetInitValue(kPhypDm12sq,    input_dm12sq_used);
      fparm->SetInitValue(kPhypDm23sq,    input_dm23sq_used);
      
      fparm->SetInitValue(kPhypMixing12Bar,  input_mixing12bar_used);
      fparm->SetInitValue(kPhypMixing13Bar,  input_mixing13bar_used);
      fparm->SetInitValue(kPhypMixing23Bar,  input_mixing23bar_used);
      fparm->SetInitValue(kPhypCPVBar    ,   input_dcpbar_used);
      fparm->SetInitValue(kPhypDm12sqBar,    input_dm12sqbar_used);
      fparm->SetInitValue(kPhypDm23sqBar,    input_dm23sqbar_used);
    }
    else {
      fparm->SetCurrentValue(kPhypMixing12,  input_mixing12_used);
      fparm->SetCurrentValue(kPhypMixing13,  input_mixing13_used);
      fparm->SetCurrentValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetCurrentValue(kPhypCPV    ,   input_dcp_used);
      fparm->SetCurrentValue(kPhypDm12sq,    input_dm12sq_used);
      fparm->SetCurrentValue(kPhypDm23sq,    input_dm23sq_used);
      
      fparm->SetCurrentValue(kPhypMixing12Bar,  input_mixing12bar_used);
      fparm->SetCurrentValue(kPhypMixing13Bar,  input_mixing13bar_used);
      fparm->SetCurrentValue(kPhypMixing23Bar,  input_mixing23bar_used);
      fparm->SetCurrentValue(kPhypCPVBar    ,   input_dcpbar_used);
      fparm->SetCurrentValue(kPhypDm12sqBar,    input_dm12sqbar_used);
      fparm->SetCurrentValue(kPhypDm23sqBar,    input_dm23sqbar_used);
    }
  }//kHypOsc3FBar
  else if(fparm->PhysicsHypothesis() == kHypOsc3FBarDissap) {
    if(set_init) {
      fparm->SetInitValue(kPhypMixing12,  input_mixing12_used);
      fparm->SetInitValue(kPhypMixing13,  input_mixing13_used);
      fparm->SetInitValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetInitValue(kPhypCPV    ,   input_dcp_used);
      fparm->SetInitValue(kPhypDm12sq,    input_dm12sq_used);
      fparm->SetInitValue(kPhypDm23sq,    input_dm23sq_used);
      fparm->SetInitValue(kPhypMixing23Bar,  input_mixing23bar_used);
      fparm->SetInitValue(kPhypDm23sqBar,    input_dm23sqbar_used);
    }
    else {
      fparm->SetCurrentValue(kPhypMixing12,  input_mixing12_used);
      fparm->SetCurrentValue(kPhypMixing13,  input_mixing13_used);
      fparm->SetCurrentValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetCurrentValue(kPhypCPV    ,   input_dcp_used);
      fparm->SetCurrentValue(kPhypDm12sq,    input_dm12sq_used);
      fparm->SetCurrentValue(kPhypDm23sq,    input_dm23sq_used);
      fparm->SetCurrentValue(kPhypMixing23Bar,  input_mixing23bar_used);
      fparm->SetCurrentValue(kPhypDm23sqBar,    input_dm23sqbar_used);
    }
  }//kHypOsc3FBarDissap
  else if(fparm->PhysicsHypothesis() == kHypOsc3FNormBar) {
    if(set_init) {
      fparm->SetInitValue(kPhypMixing12,  input_mixing12_used);
      fparm->SetInitValue(kPhypMixing13,  input_mixing13_used);
      fparm->SetInitValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetInitValue(kPhypCPV    ,   input_dcp_used);
      fparm->SetInitValue(kPhypDm12sq,    input_dm12sq_used);
      fparm->SetInitValue(kPhypDm23sq,    input_dm23sq_used);
      
      fparm->SetInitValue(kPhypBetaBar,    input_beta_bar_used);
    }
    else {
      fparm->SetCurrentValue(kPhypMixing12,  input_mixing12_used);
      fparm->SetCurrentValue(kPhypMixing13,  input_mixing13_used);
      fparm->SetCurrentValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetCurrentValue(kPhypCPV    ,   input_dcp_used);
      fparm->SetCurrentValue(kPhypDm12sq,    input_dm12sq_used);
      fparm->SetCurrentValue(kPhypDm23sq,    input_dm23sq_used);
      
      fparm->SetCurrentValue(kPhypBetaBar,    input_beta_bar_used);
    }
  }//kHypOsc3FNormBar
  else if(fparm->PhysicsHypothesis() == kHypOsc3FBarNormBar) {
    if(set_init) {
      fparm->SetInitValue(kPhypMixing12,  input_mixing12_used);
      fparm->SetInitValue(kPhypMixing13,  input_mixing13_used);
      fparm->SetInitValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetInitValue(kPhypCPV    ,   input_dcp_used);
      fparm->SetInitValue(kPhypDm12sq,    input_dm12sq_used);
      fparm->SetInitValue(kPhypDm23sq,    input_dm23sq_used);
      
      fparm->SetInitValue(kPhypMixing12Bar,  input_mixing12_used);
      fparm->SetInitValue(kPhypMixing13Bar,  input_mixing13_used);
      fparm->SetInitValue(kPhypMixing23Bar,  input_mixing23_used);
      fparm->SetInitValue(kPhypCPVBar    ,   input_dcp_used);
      fparm->SetInitValue(kPhypDm12sqBar,    input_dm12sq_used);
      fparm->SetInitValue(kPhypDm23sqBar,    input_dm23sq_used);
      
      fparm->SetInitValue(kPhypBetaBar,    input_beta_bar_used);
    }
    else {
      fparm->SetCurrentValue(kPhypMixing12,  input_mixing12_used);
      fparm->SetCurrentValue(kPhypMixing13,  input_mixing13_used);
      fparm->SetCurrentValue(kPhypMixing23,  input_mixing23_used);
      fparm->SetCurrentValue(kPhypCPV    ,   input_dcp_used);
      fparm->SetCurrentValue(kPhypDm12sq,    input_dm12sq_used);
      fparm->SetCurrentValue(kPhypDm23sq,    input_dm23sq_used);
      
      fparm->SetCurrentValue(kPhypMixing12Bar,  input_mixing12_used);
      fparm->SetCurrentValue(kPhypMixing13Bar,  input_mixing13_used);
      fparm->SetCurrentValue(kPhypMixing23Bar,  input_mixing23_used);
      fparm->SetCurrentValue(kPhypCPVBar    ,   input_dcp_used);
      fparm->SetCurrentValue(kPhypDm12sqBar,    input_dm12sq_used);
      fparm->SetCurrentValue(kPhypDm23sqBar,    input_dm23sq_used);
      
      fparm->SetCurrentValue(kPhypBetaBar,    input_beta_bar_used);
    }
  }//kHypOsc3FBarNormBar
  else {
    cout << "SetOscParams() " << util::AsString(fparm->PhysicsHypothesis()) << " not implemented" << endl;
    exit(1);
  }
  
  if(print)
    cout << "- Oscillation parameters in physics param list updated: " << fparm << endl;
}
//____________________________________________________________________________
void valor::util::OscParams::Print(ostream & stream) const
{
  stream << "   - True oscillation parameters: " << endl
  << "     - Normal ordering: " << endl
  << "       > " << util::RawParamAsString(kPhypMixing12) << " = "; ((randomize_mixing12_nh) ? cout << "* randomized *" : cout << mixing12_nh) << endl
  << "       > " << util::RawParamAsString(kPhypMixing13) << " = "; ((randomize_mixing13_nh) ? cout << "* randomized *" : cout << mixing13_nh) << endl
  << "       > " << util::RawParamAsString(kPhypMixing23) << " = "; ((randomize_mixing23_nh) ? cout << "* randomized *" : cout << mixing23_nh) << endl
  << "       > " << util::RawParamAsString(kPhypCPV    )  << " = "; ((randomize_dcp_nh)      ? cout << "* randomized *" : cout << dcp_nh) << endl
  << "       > " << util::RawParamAsString(kPhypDm12sq)   << " = "; ((randomize_dm12sq_nh)   ? cout << "* randomized *" : cout << dm12sq_nh )  << " eV^2/c^4" << endl
  << "       > " << util::RawParamAsString(kPhypDm23sq)   << " = "; ((randomize_dm23sq_nh)   ? cout << "* randomized *" : cout << dm23sq_nh )  << " eV^2/c^4" << endl
  << "     - Inverted ordering: " << endl
  << "       > " << util::RawParamAsString(kPhypMixing12) << " = "; ((randomize_mixing12_ih) ? cout << "* randomized *" : cout << mixing12_ih) << endl
  << "       > " << util::RawParamAsString(kPhypMixing13) << " = "; ((randomize_mixing13_ih) ? cout << "* randomized *" : cout << mixing13_ih) << endl
  << "       > " << util::RawParamAsString(kPhypMixing23) << " = "; ((randomize_mixing23_ih) ? cout << "* randomized *" : cout << mixing23_ih) << endl
  << "       > " << util::RawParamAsString(kPhypCPV    )  << " = "; ((randomize_dcp_ih)      ? cout << "* randomized *" : cout << dcp_ih) << endl
  << "       > " << util::RawParamAsString(kPhypDm12sq)   << " = "; ((randomize_dm12sq_ih)   ? cout << "* randomized *" : cout << dm12sq_ih )  << " eV^2/c^4" << endl
  << "       > " << util::RawParamAsString(kPhypDm23sq)   << " = "; ((randomize_dm23sq_ih)   ? cout << "* randomized *" : cout << dm23sq_ih )  << " eV^2/c^4" << endl
  << "     - Normal ordering Bar: " << endl
  << "       > " << util::RawParamAsString(kPhypMixing12Bar) << " = "; ((randomize_mixing12bar_nh) ? cout << "* randomized *" : cout << mixing12bar_nh) << endl
  << "       > " << util::RawParamAsString(kPhypMixing13Bar) << " = "; ((randomize_mixing13bar_nh) ? cout << "* randomized *" : cout << mixing13bar_nh) << endl
  << "       > " << util::RawParamAsString(kPhypMixing23Bar) << " = "; ((randomize_mixing23bar_nh) ? cout << "* randomized *" : cout << mixing23bar_nh) << endl
  << "       > " << util::RawParamAsString(kPhypCPVBar    )  << " = "; ((randomize_dcpbar_nh)      ? cout << "* randomized *" : cout << dcpbar_nh) << endl
  << "       > " << util::RawParamAsString(kPhypDm12sqBar)   << " = "; ((randomize_dm12sqbar_nh)   ? cout << "* randomized *" : cout << dm12sqbar_nh )  << " eV^2/c^4" << endl
  << "       > " << util::RawParamAsString(kPhypDm23sqBar)   << " = "; ((randomize_dm23sqbar_nh)   ? cout << "* randomized *" : cout << dm23sqbar_nh )  << " eV^2/c^4" << endl
  << "     - Inverted ordering Bar: " << endl
  << "       > " << util::RawParamAsString(kPhypMixing12Bar) << " = "; ((randomize_mixing12bar_ih) ? cout << "* randomized *" : cout << mixing12bar_ih) << endl
  << "       > " << util::RawParamAsString(kPhypMixing13Bar) << " = "; ((randomize_mixing13bar_ih) ? cout << "* randomized *" : cout << mixing13bar_ih) << endl
  << "       > " << util::RawParamAsString(kPhypMixing23Bar) << " = "; ((randomize_mixing23bar_ih) ? cout << "* randomized *" : cout << mixing23bar_ih) << endl
  << "       > " << util::RawParamAsString(kPhypCPVBar    )  << " = "; ((randomize_dcpbar_ih)      ? cout << "* randomized *" : cout << dcpbar_ih) << endl
  << "       > " << util::RawParamAsString(kPhypDm12sqBar)   << " = "; ((randomize_dm12sqbar_ih)   ? cout << "* randomized *" : cout << dm12sqbar_ih )  << " eV^2/c^4" << endl
  << "       > " << util::RawParamAsString(kPhypDm23sqBar)   << " = "; ((randomize_dm23sqbar_ih)   ? cout << "* randomized *" : cout << dm23sqbar_ih )  << " eV^2/c^4" << endl

  << "     - Other: " << endl
  << "       > " << util::RawParamAsString(kPhypBetaBar)  << " = "; ((randomize_beta_bar)    ? cout << "* randomized *" : cout << beta_bar) << endl
  << "       > Mass hierarchy = "; ((marginalize_mh) ? cout << "Margninalized (NH prior = " << mh_nh_prior << ")" : cout << util::AsString(MH)) << endl;
  if(!priors_file.empty()) {
    if((randomize_mixing13_nh && randomize_dcp_nh)
       || (randomize_mixing13_ih && randomize_dcp_ih)
       || (randomize_mixing23_nh && randomize_dm23sq_nh)
       || (randomize_mixing23_ih && randomize_dm23sq_ih)
       || (randomize_mixing13bar_nh && randomize_dcpbar_nh)
       || (randomize_mixing13bar_ih && randomize_dcpbar_ih)
       || (randomize_mixing23bar_nh && randomize_dm23sqbar_nh)
       || (randomize_mixing23bar_ih && randomize_dm23sqbar_ih)
       ) {
      stream << "       > Using priors from file: " << priors_file << " for parameters:" << endl;
      if(randomize_mixing13_nh && randomize_dcp_nh)
        stream << "          - " << util::RawParamAsString(kPhypMixing13) << " vs " << util::RawParamAsString(kPhypCPV) << " (NH)" << endl;
      if(randomize_mixing13_ih && randomize_dcp_ih)
        stream << "          - " << util::RawParamAsString(kPhypMixing13) << " vs " << util::RawParamAsString(kPhypCPV) << " (IH)" << endl;
      if(randomize_mixing23_nh && randomize_dm23sq_nh)
        stream << "          - " << util::RawParamAsString(kPhypMixing23) << " vs " << util::RawParamAsString(kPhypDm23sq) << " (NH)" << endl;
      if(randomize_mixing23_ih && randomize_dm23sq_ih)
        stream << "          - " << util::RawParamAsString(kPhypMixing23) << " vs " << util::RawParamAsString(kPhypDm23sq) << " (IH)" << endl;
    }
  }
  stream << endl;
  
}
//____________________________________________________________________________
void valor::util::OscParams::Print_Toys(ostream & stream) const
{
  // Print out mean and rms of toy osc param distributions
  
  stream << endl;
  stream << "deltaCP (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_dcp_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_dcp_nh->GetRMS()  << endl;
  stream << "deltaCP (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_dcp_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_dcp_ih->GetRMS()  << endl;
  stream << "Mixing13 (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix13_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix13_nh->GetRMS()  << endl;
  stream << "Mixing13 (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix13_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix13_ih->GetRMS()  << endl;
  stream << "Mixing23 (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix23_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix23_nh->GetRMS()  << endl;
  stream << "Mixing23 (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix23_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix23_ih->GetRMS()  << endl;
  stream << "Dm23 (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_Dm23_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Dm23_nh->GetRMS()  << endl;
  stream << "Dm23 (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_Dm23_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Dm23_ih->GetRMS()  << endl;
  stream << "Mixing12 (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix12_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix12_nh->GetRMS()  << endl;
  stream << "Mixing12 (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix12_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix12_ih->GetRMS()  << endl;
  stream << "Dm12 (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_Dm12_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Dm12_nh->GetRMS()  << endl;
  stream << "Dm12 (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_Dm12_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Dm12_ih->GetRMS()  << endl;
  stream << "deltaCPBar (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_dcpBar_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_dcpBar_nh->GetRMS()  << endl;
  stream << "deltaCPBar (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_dcpBar_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_dcpBar_ih->GetRMS()  << endl;
  stream << "Mixing13Bar (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix13Bar_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix13Bar_nh->GetRMS()  << endl;
  stream << "Mixing13Bar (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix13Bar_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix13Bar_ih->GetRMS()  << endl;
  stream << "Mixing23Bar (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix23Bar_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix23Bar_nh->GetRMS()  << endl;
  stream << "Mixing23Bar (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix23Bar_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix23Bar_ih->GetRMS()  << endl;
  stream << "Dm23Bar (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_Dm23Bar_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Dm23Bar_nh->GetRMS()  << endl;
  stream << "Dm23Bar (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_Dm23Bar_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Dm23Bar_ih->GetRMS()  << endl;
  stream << "Mixing12Bar (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix12Bar_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix12Bar_nh->GetRMS()  << endl;
  stream << "Mixing12Bar (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_Mix12Bar_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Mix12Bar_ih->GetRMS()  << endl;
  stream << "Dm12Bar (NH):" << endl;
  stream << "  - Mean = " << hToyHisto_Dm12Bar_nh->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Dm12Bar_nh->GetRMS()  << endl;
  stream << "Dm12Bar (IH):" << endl;
  stream << "  - Mean = " << hToyHisto_Dm12Bar_ih->GetMean() << endl;
  stream << "  - RMS  = " << hToyHisto_Dm12Bar_ih->GetRMS()  << endl;
  stream << endl;
}
//____________________________________________________________________________
void valor::util::OscParams::Write(TFile * f)
{
  f->cd();
  
  //priors
  if(hPrior_th13_dcp_nh) {
    hPrior_th13_dcp_nh->SetDirectory(0);
    hPrior_th13_dcp_nh->Write();
  }
  if(hPrior_th13_dcp_ih) {
    hPrior_th13_dcp_ih->SetDirectory(0);
    hPrior_th13_dcp_ih->Write();
  }
  if(hPrior_23_nh) {
    hPrior_23_nh->SetDirectory(0);
    hPrior_23_nh->Write();
  }
  if(hPrior_23_ih) {
    hPrior_23_ih->SetDirectory(0);
    hPrior_23_ih->Write();
  }
  //toys used
  if(hToyHisto_dcp_nh) {
    hToyHisto_dcp_nh->SetDirectory(0);
    hToyHisto_dcp_nh->Write();
  }
  if(hToyHisto_Mix13_nh) {
    hToyHisto_Mix13_nh->SetDirectory(0);
    hToyHisto_Mix13_nh->Write();
  }
  if(hToyHisto_Mix23_nh) {
    hToyHisto_Mix23_nh->SetDirectory(0);
    hToyHisto_Mix23_nh->Write();
  }
  if(hToyHisto_Dm23_nh) {
    hToyHisto_Dm23_nh->SetDirectory(0);
    hToyHisto_Dm23_nh->Write();
  }
  if(hToyHisto_Mix12_nh) {
    hToyHisto_Mix12_nh->SetDirectory(0);
    hToyHisto_Mix12_nh->Write();
  }
  if(hToyHisto_Dm12_nh) {
    hToyHisto_Dm12_nh->SetDirectory(0);
    hToyHisto_Dm12_nh->Write();
  }
  if(hToyHisto_dcp_ih) {
    hToyHisto_dcp_ih->SetDirectory(0);
    hToyHisto_dcp_ih->Write();
  }
  if(hToyHisto_Mix13_ih) {
    hToyHisto_Mix13_ih->SetDirectory(0);
    hToyHisto_Mix13_ih->Write();
  }
  if(hToyHisto_Mix23_ih) {
    hToyHisto_Mix23_ih->SetDirectory(0);
    hToyHisto_Mix23_ih->Write();
  }
  if(hToyHisto_Dm23_ih) {
    hToyHisto_Dm23_ih->SetDirectory(0);
    hToyHisto_Dm23_ih->Write();
  }
  if(hToyHisto_Mix12_ih) {
    hToyHisto_Mix12_ih->SetDirectory(0);
    hToyHisto_Mix12_ih->Write();
  }
  if(hToyHisto_Dm12_ih) {
    hToyHisto_Dm12_ih->SetDirectory(0);
    hToyHisto_Dm12_ih->Write();
  }
  if(hToyHisto_MH) {
    hToyHisto_MH->SetDirectory(0);
    hToyHisto_MH->Write();
  }
  if(hToyHisto_dcpBar_nh) {
    hToyHisto_dcpBar_nh->SetDirectory(0);
    hToyHisto_dcpBar_nh->Write();
  }
  if(hToyHisto_Mix13Bar_nh) {
    hToyHisto_Mix13Bar_nh->SetDirectory(0);
    hToyHisto_Mix13Bar_nh->Write();
  }
  if(hToyHisto_Mix23Bar_nh) {
    hToyHisto_Mix23Bar_nh->SetDirectory(0);
    hToyHisto_Mix23Bar_nh->Write();
  }
  if(hToyHisto_Dm23Bar_nh) {
    hToyHisto_Dm23Bar_nh->SetDirectory(0);
    hToyHisto_Dm23Bar_nh->Write();
  }
  if(hToyHisto_Mix12Bar_nh) {
    hToyHisto_Mix12Bar_nh->SetDirectory(0);
    hToyHisto_Mix12Bar_nh->Write();
  }
  if(hToyHisto_Dm12Bar_nh) {
    hToyHisto_Dm12Bar_nh->SetDirectory(0);
    hToyHisto_Dm12Bar_nh->Write();
  }
  if(hToyHisto_dcpBar_ih) {
    hToyHisto_dcpBar_ih->SetDirectory(0);
    hToyHisto_dcpBar_ih->Write();
  }
  if(hToyHisto_Mix13Bar_ih) {
    hToyHisto_Mix13Bar_ih->SetDirectory(0);
    hToyHisto_Mix13Bar_ih->Write();
  }
  if(hToyHisto_Mix23Bar_ih) {
    hToyHisto_Mix23Bar_ih->SetDirectory(0);
    hToyHisto_Mix23Bar_ih->Write();
  }
  if(hToyHisto_Dm23Bar_ih) {
    hToyHisto_Dm23Bar_ih->SetDirectory(0);
    hToyHisto_Dm23Bar_ih->Write();
  }
  if(hToyHisto_Mix12Bar_ih) {
    hToyHisto_Mix12Bar_ih->SetDirectory(0);
    hToyHisto_Mix12Bar_ih->Write();
  }
  if(hToyHisto_Dm12Bar_ih) {
    hToyHisto_Dm12Bar_ih->SetDirectory(0);
    hToyHisto_Dm12Bar_ih->Write();
  }
  
}
//____________________________________________________________________________
//____________________________________________________________________________
void valor::util::OscParams::SetRandomize(conv::PhysParamId_t p, bool random)
{
  switch(p){
    case(kPhypBetaBar) :
      randomize_beta_bar = random;
      break;
    case(kPhypMixing12) :
      randomize_mixing12_ih = random;
      randomize_mixing12_nh = random;
      break;
    case(kPhypMixing12Bar) :
      randomize_mixing12bar_ih = random;
      randomize_mixing12bar_nh = random;
      break;
    case(kPhypMixing13) :
      randomize_mixing13_ih = random;
      randomize_mixing13_nh = random;
      break;
    case(kPhypMixing13Bar) :
      randomize_mixing13bar_ih = random;
      randomize_mixing13bar_nh = random;
      break;
    case(kPhypMixing23) :
      randomize_mixing23_ih = random;
      randomize_mixing23_nh = random;
      break;
    case(kPhypMixing23Bar) :
      randomize_mixing23bar_ih = random;
      randomize_mixing23bar_nh = random;
      break;
    case(kPhypDm23sq) :
      randomize_dm23sq_nh = random;
      randomize_dm23sq_ih = random;
      break;
    case(kPhypDm23sqBar) :
      randomize_dm23sqbar_nh = random;
      randomize_dm23sqbar_ih = random;
      break;
    case(kPhypDm12sq) :
      randomize_dm12sq_nh = random;
      randomize_dm12sq_ih = random;
      break;
    case(kPhypDm12sqBar) :
      randomize_dm12sqbar_nh = random;
      randomize_dm12sqbar_ih = random;
      break;
    case(kPhypCPV) :
      randomize_dcp_nh = random;
      randomize_dcp_ih = random;
      break;
    case(kPhypCPVBar) :
      randomize_dcpbar_nh = random;
      randomize_dcpbar_ih = random;
      break;
    default:
      std::cerr << "OscParams/SetRandomize >> Parameter: " << util::RawParamAsString(p)
      << " not handled in function." << std::endl;
      exit(0);
      break;
  }
}

void valor::util::OscParams::Override(conv::PhysParamId_t p, double value)
{
   if(iMH == 0)
   {  // NH
      switch(p)
      {
         case kPhypMixing23:
            mixing23_nh = value;
            break;
         case kPhypMixing13:
            mixing13_nh = value;
            break;
         case kPhypMixing12:
            mixing12_nh = value;
            break;
         case kPhypCPV:
            dcp_nh = value;
            break;
         case kPhypDm23sq:
            dm23sq_nh = value;
            break;
         case kPhypDm12sq:
            dm12sq_nh = value;
            break;
         case kPhypMixing23Bar:
            mixing23bar_nh = value;
            break;
         case kPhypMixing13Bar:
            mixing13bar_nh = value;
            break;
         case kPhypMixing12Bar:
            mixing12bar_nh = value;
            break;
         case kPhypCPVBar:
            dcpbar_nh = value;
            break;
         case kPhypDm23sqBar:
            dm23sqbar_nh = value;
            break;
         case kPhypDm12sqBar:
            dm12sqbar_nh = value;
            break;
         default:
            std::cout << "OscParams Error: Unrecognised PhysParam_t: " << p << std::endl;
            exit(1);
      }
   }
   else
   {  // IH
      switch(p)
      {
         case kPhypMixing23:
            mixing23_ih = value;
            break;
         case kPhypMixing13:
            mixing13_ih = value;
            break;
         case kPhypMixing12:
            mixing12_ih = value;
            break;
         case kPhypCPV:
            dcp_ih = value;
            break;
         case kPhypDm23sq:
            dm23sq_ih = value;
            break;
         case kPhypDm12sq:
            dm12sq_ih = value;
            break;
         case kPhypMixing23Bar:
            mixing23bar_ih = value;
            break;
         case kPhypMixing13Bar:
            mixing13bar_ih = value;
            break;
         case kPhypMixing12Bar:
            mixing12bar_ih = value;
            break;
         case kPhypCPVBar:
            dcpbar_ih = value;
            break;
         case kPhypDm23sqBar:
            dm23sqbar_ih = value;
            break;
         case kPhypDm12sqBar:
            dm12sqbar_ih = value;
            break;
         default:
            std::cout << "OscParams Error: Unrecognised PhysParam_t: " << p << std::endl;
            exit(1);
      }
   }
}

//____________________________________________________________________________
//____________________________________________________________________________
void valor::util::OscParams::SetRandomize(bool random)
{

      randomize_beta_bar = random;
      randomize_mixing12_ih = random;
      randomize_mixing12_nh = random;
      randomize_mixing12bar_ih = random;
      randomize_mixing12bar_nh = random;
      randomize_mixing13_ih = random;
      randomize_mixing13_nh = random;
      randomize_mixing13bar_ih = random;
      randomize_mixing13bar_nh = random;
      randomize_mixing23_ih = random;
      randomize_mixing23_nh = random;
      randomize_mixing23bar_ih = random;
      randomize_mixing23bar_nh = random;
      randomize_dm23sq_nh = random;
      randomize_dm23sq_ih = random;
      randomize_dm23sqbar_nh = random;
      randomize_dm23sqbar_ih = random;
      randomize_dm12sq_nh = random;
      randomize_dm12sq_ih = random;
      randomize_dm12sqbar_nh = random;
      randomize_dm12sqbar_ih = random;
      randomize_dcp_nh = random;
      randomize_dcp_ih = random;
      randomize_dcpbar_nh = random;
      randomize_dcpbar_ih = random;

}
//____________________________________________________________________________
//____________________________________________________________________________
//____________________________________________________________________________
//____________________________________________________________________________
//____________________________________________________________________________
//____________________________________________________________________________
