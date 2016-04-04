//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

//#include <cassert>

#include <TFile.h>
#include <TTree.h>
#include <TObjArray.h>

#include "NDFit/NDNtp.h"

#include "FitCore/FitParamList.h"
#include "FitCore/PhysParamList.h"
#include "Messenger/Messenger.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
NDNtp::NDNtp()
{
  this->Init();
}
//________________________________________________________________________________________
NDNtp::~NDNtp() 
{
  this->Clean();
}
//________________________________________________________________________________________
void NDNtp::Create(const char * output_filename)
{
  LOG("NDNtp", pDEBUG) << "Booking VALOR/DUNE ND fit tree...";

  if(output_filename) {
    fOutFile = new TFile(output_filename, "recreate");
  }

  fTrueParamArr    = new PhysParamList;
  fBestFitParamArr = new FitParamList;
  fBestFitModelArr = new TObjArray;
  fDataArr         = new TObjArray;

  fBestFitModelArr -> SetOwner(false);
  fDataArr         -> SetOwner(false);

//int migradmin, migraderr;

  fVTree = new TTree("valor_dune_nd_ntp","VALOR/DUNE ND fit results");

  fVTree->Branch("true_params",     "valor::PhysParamList", &fTrueParamArr,    1280000,0);
  fVTree->Branch("best_fit_params", "valor::FitParamList",  &fBestFitParamArr, 1280000,0);
  fVTree->Branch("best_fit_model",  "TObjArray",            &fBestFitModelArr, 1280000,0);
  fVTree->Branch("data",            "TObjArray",            &fDataArr,         1280000,0);

//fVTree->Branch("migradmin",                               &migradmin);
//fVTree->Branch("migraderr",                               &migraderr);

}
//________________________________________________________________________________________
void NDNtp::Reset(void)
{
  fTrueParamArr    -> Clear();
  fBestFitParamArr -> Clear();
  fBestFitModelArr -> Clear();
  fDataArr         -> Clear();
}
//________________________________________________________________________________________
void NDNtp::Fill(void)
{
  fVTree -> Fill();
}
//________________________________________________________________________________________
void NDNtp::Save(void)
{
  if(fOutFile) {
    fOutFile -> cd();
    fVTree   -> Write();
  }
}
//________________________________________________________________________________________
void NDNtp::Init(void)
{
  LOG("NDNtp", pDEBUG) << "Initializing...";

  fOutFile         = 0;
  fVTree           = 0;
  fTrueParamArr    = 0;
  fBestFitParamArr = 0;
  fBestFitModelArr = 0;
  fDataArr         = 0;
}
//________________________________________________________________________________________
void NDNtp::Clean(void)
{
  LOG("NDNtp", pDEBUG) << "Cleaning-up...";

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
