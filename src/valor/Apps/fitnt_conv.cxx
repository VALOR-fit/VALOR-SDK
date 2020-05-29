//____________________________________________________________________________
/*!

\app       valor-fitnt_conv

\brief     Picks some info out ofan input VALOR fit n-tuple and converts it
           to a bare ROOT format (for comparison with other fitters

\syntax    valor-fitnt_conv
              --input /path/input_file.root
             [--output /path/output_file.root]
              --format output_format

           --format options:
               * standard

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   March 27, 2014

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

// System
#include <string>
#include <sstream>
#include <vector>
#include <utility>
// ROOT
#include <TSystem.h>
#include <TFile.h>
#include <TChain.h>
#include <TLeaf.h>
#include <TVector.h>
#include <TMatrixD.h>
#include <TMatrixDSym.h>
// VALOR/SDK
#include "valor/CmdLine/CmdLnArgParser.h"
#include "valor/Messenger/Messenger.h"
#include "valor/FitCore/PhysParamList.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/FitCore/FitInputs.h"
#include "valor/FitCore/UtlFitParamList.h"
#include "valor/FitCore/CompoundFitElements.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Conventions/ParameterPool.h"

using std::string;
using std::ostringstream;
using namespace valor;

void GetCommandLineArgs (int argc, char ** argv);

string gOptInpFileName;
string gOptOutFileName;
string gOptOutFormat;
string gOptConfig;

int ConversionToStandardBareRootFormat(void);

//___________________________________________________________________
int main(int argc, char ** argv)
{
  GetCommandLineArgs (argc, argv);

  if(gOptOutFormat == "standard") {
    return ConversionToStandardBareRootFormat();
  }
  else {
    LOG("valor",pFATAL)
      << "No output format selected. Exiting.";
  }

  return 0;
}
//___________________________________________________________________
int ConversionToStandardBareRootFormat(void)
{
  //
  // Read the config file to populate the ParameterPool
  //

  std::string config_path = util::CompleteConfigPath(gOptConfig);
  CompoundFitElements * elements = new CompoundFitElements();
  elements->Read(config_path.c_str());

  const int buffer_size = 100;

  //
  // Create the output n-tuple and output meta-data
  //

  TTree * out_metadata = new TTree (
     "valor_fit_metadata",
     "Selected metadata for VALOR fit");

  int    brPId   = 0;
  char   brPName [buffer_size] = {};

  out_metadata -> Branch("parameter_id",   &brPId,         "parameter_id/I");
  out_metadata -> Branch("parameter_name", (void*)brPName, "parameter_name/C", buffer_size);

  TTree * out_fitnt = new TTree(
     "valor_fit_ntp_conversion",
      "Selected info extracted from the VALOR fit n-tuple and converted to bare ROOT format");

  int           brItoy      = 0;
  TMatrixDSym * brCovMtrx  = 0;
  TVectorD *    brBestFitP = 0;

  out_fitnt->Branch("itoymc",   &brItoy,      "itoymc/I");
  out_fitnt->Branch("covmtrx",  "TMatrixDSym", &brCovMtrx,   1280000,0);
  out_fitnt->Branch("bestfitp", "TVectorD",    &brBestFitP,  1280000,0);

  //
  // Open the input file, get the VALOR fit n-tuple and prepare to read it
  //

  TChain * fitnt = new TChain("valor_fit_ntp");
  fitnt->Add(gOptInpFileName.c_str());
  assert(fitnt->GetEntries());

  PhysParamList * true_params          = 0; // input true params
  FitParamList *  best_fit_params      = 0; // best-fit params
  TObjArray *     best_fit_model_array = 0; // an array of various best-fit model predictions for the current toy-MC experiment
  TObjArray *     data_array           = 0; // an array of all fitted data distributions for the current toy-MC experiment

  fitnt->SetBranchAddress("true_params",        &true_params);
  fitnt->SetBranchAddress("best_fit_params",    &best_fit_params);
  fitnt->SetBranchAddress("best_fit_model",     &best_fit_model_array);
  fitnt->SetBranchAddress("data",               &data_array);

  // Make a list of parameter names using first FPL
  fitnt->GetEntry(0);
  int n_params_input = best_fit_params->Size();
  //find number of parameters after marginalisation
  std::vector<conv::PhysParamId_t> unmarginalised_parameters;
  int n_final_params = 0;
  const util::NameRegistry * nr = util::NameRegistry::Instance();
  for (int ip = 0 ; ip < n_params_input ; ip++) {
    conv::PhysParamId_t p = nr->PhysParamFromString(best_fit_params->NameByIndex(ip));
    if (!best_fit_params->IsMarginalized(p)) {
      unmarginalised_parameters.push_back(p);
      n_final_params++;
    }
  }
  // Fill the output tree
  for(int ip = 0; ip < n_final_params; ip++) {
    //~ conv::PhysParamId_t p = best_fit_params->WhichParam(ip); // Should work but something weird is happening
    conv::PhysParamId_t p = nr->PhysParamFromString(best_fit_params->NameByIndex(ip));
    brPId = ip;
    strcpy(brPName, best_fit_params->Name(p).c_str());
    // std::pair<double,double> erange  = ParameterPool::Instance()->GetParam(p).GetKineRange(conv::kvEtrue);
    // std::pair<double,double> q2range = ParameterPool::Instance()->GetParam(p).GetKineRange(conv::kvQ2true);
    // brPEmin  = erange.first;
    // brPEmax  = erange.second;
    // brPQ2min = q2range.first;
    // brPQ2max = q2range.second;
    out_metadata->Fill();
  }

  //
  // Loop over toy-MC fits and extract info
  //

  Long_t ntoymc = fitnt->GetEntries();

  for(Long_t itoymc = 0; itoymc < ntoymc; itoymc++) {

    fitnt->GetEntry(itoymc);

    LOG("valor", pNOTICE)
       << "******************************************************* \n"
       << "Toy-MC experiment: # " << itoymc << "\n"
       << "******************************************************* \n";

    LOG("valor", pNOTICE)
      << "\n >> Input parameter list: \n "
      << *true_params;

    LOG("valor", pNOTICE)
      << "\n >> Best-fit parameter list: \n "
      << *best_fit_params;

    if (n_params_input!=best_fit_params->Size()) {
      LOG("valor", pFATAL)
        << "All FPLs given to this program must have the same parameters defined. Exiting.";
      exit(-1);
    }

    brItoy = itoymc;

    // Marginalise matrix
    TMatrixDSym * cov = best_fit_params->GetCovMatrix();
    brCovMtrx = new TMatrixDSym(n_final_params);

    for(int ipar1 = 0; ipar1 < n_final_params; ipar1++)
    {
      conv::PhysParamId_t p1 = unmarginalised_parameters.at(ipar1);
      assert(p1!=conv::kPhypUndefined);
      int idx_in_orig_matrix1 = best_fit_params->Id(p1);
      for (int ipar2 = 0 ; ipar2 < n_final_params; ipar2++) {
        conv::PhysParamId_t p2 = unmarginalised_parameters.at(ipar2);
        assert(p2!=conv::kPhypUndefined);
        int idx_in_orig_matrix2 = best_fit_params->Id(p2);
        (*brCovMtrx)[ipar1][ipar2] = (*cov)[idx_in_orig_matrix1][idx_in_orig_matrix2];
      } //ipar2
    } //ipar1



    brBestFitP = new TVectorD(n_params_input);
    for(int ip = 0; ip < n_final_params; ip++) {
      conv::PhysParamId_t p = unmarginalised_parameters.at(ip);
      (*brBestFitP)[ip] = best_fit_params->BestFitValue(p);
    }

    out_fitnt->Fill();
  }

  LOG("valor", pNOTICE)
      << "*** Saving VALOR fit n-tuple conversion to: "
      << gOptOutFileName;

  TFile fout(gOptOutFileName.c_str(),"recreate");
  out_fitnt->Write();
  out_metadata->Write();
  fout.Close();

  LOG("valor", pNOTICE)  << "Done!";
  return 0;
}
//___________________________________________________________________
void GetCommandLineArgs(int argc, char ** argv)
{
  LOG("valor", pINFO) << "*** Parsing command line arguments";

  // Parse run options for this app
  CmdLnArgParser parser(argc,argv);

  // get output format
  if ( parser.OptionExists("format") ) {
    LOG("valor", pINFO) << "Reading output format";
    gOptOutFormat = parser.ArgAsString("format");
  } else {
    LOG("valor", pFATAL)
       << "Unspecified output format - Exiting";
    exit(1);
  }

  // get input VALOR fit file name
  if ( parser.OptionExists("input") ) {
    LOG("valor", pINFO) << "Reading input file";
    gOptInpFileName = parser.ArgAsString("input");
  } else {
    LOG("valor", pFATAL)
       << "Unspecified input filename - Exiting";
    exit(1);
  }

  // get output file name (or set default)
  if ( parser.OptionExists("output") ) {
    LOG("valor", pINFO) << "Reading output file";
    gOptOutFileName = parser.ArgAsString("output");
  } else {

    // set default
    string inpname = gOptInpFileName;
    unsigned int L = inpname.length();
    // if the last 4 characters are "root" (file extension) then remove them
    if(inpname.substr(L-4, L).find("root") != string::npos) {
        inpname.erase(L-4, L);
    }

    ostringstream name;
    name << inpname << "vfnt_extracts.root";

    gOptOutFileName = gSystem->BaseName(name.str().c_str());
  }

  // get config file name
  if ( parser.OptionExists("config") ) {
    LOG("valor", pINFO) << "Reading config file";
    gOptConfig = parser.ArgAsString("config");
  } else {
    LOG("valor", pFATAL)
      << "Unspecified config file name - Exiting";
    exit(1);
  }

}
//_________________________________________________________________________________
