//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <TFile.h>
#include <TTree.h>
#include <TObjArray.h>

#include "valor/FitCore/FitParamList.h"
#include "valor/FitCore/PhysParamList.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Ntuple/StdFitNtp.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
StdFitNtp::StdFitNtp()
{
  this->Init();
}
//________________________________________________________________________________________
StdFitNtp::~StdFitNtp() 
{
  this->Clean();
}
//________________________________________________________________________________________
void StdFitNtp::Create(const char* file_name, const char* ntp_name, const char* ntp_label)
{
  LOG("Ntp", pDEBUG) << "Creating standard VALOR fit n-tuple...";

  if(file_name) {
    fOutFile = new TFile(file_name, "recreate");
  }

  fTrueParamArr    = new PhysParamList;
  fBestFitParamArr = new FitParamList;
  fBestFitModelArr = new TObjArray;
  fDataArr         = new TObjArray;

  fBestFitModelArr -> SetOwner(false);
  fDataArr         -> SetOwner(false);

//int migradmin, migraderr;

  fVTree = new TTree(ntp_name, ntp_label);

  fVTree->Branch("true_params",     "valor::PhysParamList", &fTrueParamArr,    1280000,0);
  fVTree->Branch("best_fit_params", "valor::FitParamList",  &fBestFitParamArr, 1280000,0);
  fVTree->Branch("best_fit_model",  "TObjArray",            &fBestFitModelArr, 1280000,0);
  fVTree->Branch("data",            "TObjArray",            &fDataArr,         1280000,0);

//fVTree->Branch("migradmin",                               &migradmin);
//fVTree->Branch("migraderr",                               &migraderr);
}
//________________________________________________________________________________________
void StdFitNtp::Reset(void)
{
  fTrueParamArr    -> Clear();
  fBestFitParamArr -> Clear();
  fBestFitModelArr -> Clear();
  fDataArr         -> Clear();
}
//________________________________________________________________________________________
void StdFitNtp::Fill(void)
{
  fVTree -> Fill();
}
//________________________________________________________________________________________
void StdFitNtp::Save(void)
{
  if(fOutFile) {
    fOutFile -> cd();
    fVTree   -> Write();
  }
}
//________________________________________________________________________________________
void StdFitNtp::Init(void)
{
  LOG("Ntp", pDEBUG) << "Initializing...";

  fOutFile         = 0;
  fVTree           = 0;
  fTrueParamArr    = 0;
  fBestFitParamArr = 0;
  fBestFitModelArr = 0;
  fDataArr         = 0;
}
//________________________________________________________________________________________
void StdFitNtp::Clean(void)
{
  LOG("Ntp", pDEBUG) << "Cleaning-up...";

  if (fOutFile) {
     fOutFile->Close();
     delete fOutFile;
  }

/*
  if (fVTree          ) delete fVTree;
  if (fTrueParamArr   ) delete fTrueParamArr;
  if (fBestFitParamArr) delete fBestFitParamArr;
  if (fBestFitModelArr) delete fBestFitModelArr;
  if (fDataArr        ) delete fDataArr;
*/
}
//________________________________________________________________________________________
