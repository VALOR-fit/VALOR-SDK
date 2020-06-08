#include "valor/FitCore/ToyBuilder.h"

#include <limits>
#include <sstream>
#include <string>
#include "TMath.h"

#include "valor/Conventions/Constants.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/ToyStream.h"
#include "valor/FitCore/UtlFitParamList.h"
#include "valor/FitCore/UtlOscillation.h"

//#define VALOR_EXPT_LOW_LEVEL_MESG_ENABLED

namespace valor
{

ToyBuilder::ToyBuilder(CompoundFitElements& elements, string filePrefix,
                       bool use_simple_binning, bool store_mode_arrays,
                       bool toys_are_marginalisation, bool save_throws_to_tree):
  _filePrefix(filePrefix),
  _model(elements.FitModel()),
  _data(elements.FitData()),
  _use_simple_binning(use_simple_binning),
  _store_mode_arrays(store_mode_arrays),
  _toys_are_marginalisation(toys_are_marginalisation),
  _save_throws_to_tree(save_throws_to_tree),
  _tree_pars(nullptr),
  _file_pars(nullptr),
  _read_params(new double[conv::knppmax]),
  _toy_throw_file(nullptr)
{
}
//______________________________________________________________________________
ToyBuilder::~ToyBuilder()
{
   this->_model = nullptr;
   this->_data  = nullptr;
   delete this->_tree_pars;

   if(this->_file_pars) {
      this->_file_pars->Close();
   }
   delete this->_file_pars;

   if(this->_toy_throw_file) {
      this->_toy_throw_file->Close();
   }
   delete this->_toy_throw_file;
}
//______________________________________________________________________________
void ToyBuilder::BuildToys(const int fitParamBin, const int batch,
      const int Ntoys, FitParamList& fpl, bool randomizeSystematics,
      bool fluctuateStatistics, const char* options, bool rethrowUnphysical,
      std::string toy_throw_file_to_read, std::string param_name_map_file)
{
   this->BuildToys(fitParamBin, -1, batch, Ntoys, fpl, randomizeSystematics, fluctuateStatistics,
         options, rethrowUnphysical, toy_throw_file_to_read, param_name_map_file);
}
//______________________________________________________________________________
void ToyBuilder::BuildToys(const int fitParam1Bin, const int fitParam2Bin,
      const int batch, const int Ntoys, FitParamList& fpl, bool randomizeSystematics,
      bool fluctuateStatistics, const char* options, bool rethrowUnphysical,
      std::string toy_throw_file_to_read, std::string param_name_map_file)
{
   std::ostringstream oss;
   oss << this->_filePrefix << "_" << fitParam1Bin;
   if(fitParam2Bin > -1)
   {  // Filename for 2D fit
      oss << "_v_" << fitParam2Bin;
   }
   oss << "_" << batch << "_" << Ntoys << ".bin";

   std::string filename = oss.str();

   // Create a file to store this batch of toys
   util::ToyStream* toy_stream = nullptr;
   if(_toys_are_marginalisation) {
      // Build marginalisation toys
      std::string write_opt_str = "pdft_vf";
      if(_store_mode_arrays ) { write_opt_str += "_pdfc_vf"; }
      if(_use_simple_binning) { write_opt_str += "+SB";      }

      // First generate a dummy toy to be overwritten (so that the model/data arrays can be initialised)
      FitParamList* fpl_void = util::GetFitParameterList("void", "");
      this->_model->Generate(*fpl_void);

      toy_stream = this->_model->InitToyStream(filename, "write", write_opt_str);
   }
   else {
      // Build fake data toys
      std::string write_opt_str = "data_vf";
      if(_use_simple_binning) { write_opt_str += "+SB"; }

      // First generate a dummy toy to be overwritten (so that the model/data arrays can be initialised)
      FitParamList* fpl_void = util::GetFitParameterList("void", "");
      this->_data->Generate(this->_model, *fpl_void, false);

      toy_stream = this->_data->InitToyStream(filename, "write", write_opt_str);
   }

   // Will we read throws from a pre-existing file instead of generating them? (This is used e.g. to convert
   // T2K legacy toy throws into SDK marginalisation toys)
   bool read_systematics (toy_throw_file_to_read != "" && toy_throw_file_to_read != "NONE" &&
                          param_name_map_file    != "" && param_name_map_file    != "NONE");

   if(_toys_are_marginalisation && fluctuateStatistics){
      LOG("ToyBuilder", pWARN) << "The option to fluctuate statistics was requested when building "
                               << "marginalisation toys. This has been ignored, as it is not yet supported.";
   }

   unsigned int n_rethrows = 0;
   for(int t = 0; t < Ntoys; ++t)
   {
      for( ; ; )
      {
         this->_model->ResetSystematics("all");
         // Randomise the systematics in the model
         if(randomizeSystematics)
         {
            this->_model->RandomizeSystematics(options);
            util::oscillation::ThrowOscillationParameters(fpl);
         }
         else if(read_systematics) {
            this->ReadParamsFromThrowFile(fpl, toy_throw_file_to_read, param_name_map_file, options, t);
            break;
         }

         if(!read_systematics) {
            PhysParamList tweaks = this->_model->Tweaks();
            fpl.FromPhysParamList(tweaks, "current silent");
         }

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
         LOG("ToyBuilder", pINFO) << fpl;
#endif

         // Check parameter name, maybe rethrow
         if(rethrowUnphysical && !util::AllWithinLimits(fpl))
         {
            n_rethrows++;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
            LOG("ToyBuilder", pDEBUG) << "At least one parameter thrown " <<
               "outside expected limits. Rethrowing.";
#endif
         }
         else
         {
            break;
         }
      }
      PhysParamList true_params = fpl.AsPhysParamList("current");

      // Generate a toy-MC experiment
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
      LOG("ToyBuilder", pDEBUG) << "Generating toy-MC data";
      LOG("ToyBuilder", pDEBUG) << "Toy-MC generation uses true parameters: " << true_params;
#endif
      if(t % 100 == 0) { LOG("ToyBuilder", pNOTICE) << "Generating toy-MC #" << t; }
      this->_model->Generate(fpl, toy_stream->GetVALORArrayFormat());
      if(_toys_are_marginalisation) {
         this->_model->Write(toy_stream);
      }
      else {
        this->_data->Generate(this->_model, fluctuateStatistics, toy_stream->GetVALORArrayFormat());
        this->_data->Write(toy_stream);
      }
      this->SaveParamThrows(fpl, t == Ntoys-1, filename);

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
     if(_toys_are_marginalisation) {
       LOG("ToyBuilder", pNOTICE) << "Marg toy #" << t << " event rates:\n" << this->_model->GetPDFIntegralAsString();
     }
     else {
       LOG("ToyBuilder", pNOTICE) << "Fake data toy #" << t << " event rates:\n" << this->_data->GetPDFIntegralAsString();
     }
#endif

   }
   delete toy_stream;
   LOG("ToyBuilder", pNOTICE) << "Finished building toys. Generated " << Ntoys
                              << ", re-threw " << n_rethrows;
}
//______________________________________________________________________________
void ToyBuilder::SaveParamThrows(FitParamList& fpl, bool write_tree, std::string binary_file_name)
{
   if(!_save_throws_to_tree) { return; }

   // Set up an array to hold all of the parameter values that we'll save to the output tree
   double param_values[conv::knppmax];
   for(int ip = 0; ip < conv::knppmax; ip++){
      param_values[ip] = -999;
   }

   // Set up the output file, if we haven't done so already
   if(!_file_pars) {
      // Construct the output file name
      std::string file_name = binary_file_name;
      std::string to_replace = ".bin";
      size_t start_pos = file_name.find(to_replace);
      if(start_pos == std::string::npos) {
         LOG("ToyBuilder", pERROR) << "Could not construct output file name.";
         util::Die(-1);
      }
      file_name.replace(start_pos, to_replace.length(), "_toy_throws.root");

      _file_pars = new TFile(file_name.c_str(), "RECREATE");
   }

   // Set up the tree, if we haven't done so already
   if(!_tree_pars) {
      _tree_pars = new TTree("marg_parameter_values","Input parameters used for toys");
      for (int ip = 0 ; ip < conv::knppmax ; ip++) {
         conv::PhysParamId_t p = (conv::PhysParamId_t)ip;
         if(p == conv::kPhypUndefined) { continue; }
         std::string param_name = util::NameRegistry::Instance()->ParamAsStringVerySimple(p);
         std::string branch_info = param_name + "/D";
         if (fpl.Exists(p)) {
            _tree_pars->Branch(param_name.c_str(), &(param_values[ip]), branch_info.c_str() );
         }
      }
   }

   // Save this throw to the tree
   for (int ip = 0 ; ip < conv::knppmax ; ip++) {
      conv::PhysParamId_t p = (conv::PhysParamId_t)ip;
      if (fpl.Exists(p)) {
         param_values[ip] = fpl.CurrValue(p);
      }
   }
   _tree_pars->Fill();

   // If this is the final toy, write the tree to a new file
   if(write_tree) {
      _file_pars->cd();
      _tree_pars->Write();
   }
}
//______________________________________________________________________________
void ToyBuilder::ReadParamsFromThrowFile(FitParamList& fpl, std::string toy_throw_file_to_read,
                                              std::string param_name_map_file, const char* options, int toy_number)
{
   TString ts(options);

   // Open the tree containing thrown parameter values, if we don't have it already
   if(!_tree_pars) {
     if(toy_throw_file_to_read == "" || param_name_map_file == "") {
        LOG("ToyBuilder", pERROR) << "No input toy throw file or parameter name map file was specified.";
        util::Die(-1);
     }

     _toy_throw_file = new TFile(toy_throw_file_to_read.c_str(), "READ");
     if(!_toy_throw_file->IsOpen()) {
        LOG("ToyBuilder", pERROR) << "Could not open file [" << toy_throw_file_to_read << "]";
        util::Die(-1);
     }
     _toy_throw_file->cd();
     _tree_pars = (TTree *) _toy_throw_file->Get("marg_parameter_values");

     // Set up an array to hold all of the parameter values that we'll read
     for(int ip = 0; ip < conv::knppmax; ip++){
        _read_params[ip] = -999;
     }

     // Set up the branches to read
     for (int ip = 0 ; ip < conv::knppmax ; ip++) {
        conv::PhysParamId_t p = (conv::PhysParamId_t)ip;
        if (p == conv::kPhypUndefined) { continue; }
        if (!fpl.Exists(p)) { continue; }
        std::string sdk_param_name = util::NameRegistry::Instance()->ParamAsStringVerySimple(p);
        std::string read_param_name = "";

        // Put in special cases here, where we may need to skip params
        if (ts.Contains("compare-t2k-legacy") &&
            (p == util::NameRegistry::Instance()->PhysParamFromString("SCCA") ||
             p == util::NameRegistry::Instance()->PhysParamFromString("SCCV"))) {
           _read_params[ip] = 1.0;
           continue;
        }

        // Open the parameter map file
        std::ifstream map(param_name_map_file);
        if (map.fail()) {
           LOG("ToyBuilder",pFATAL) << "Failed to open file "<<param_name_map_file;
           util::Die(-1) ; return;
        }
        std::string arg_file_line = "";
        while (std::getline(map, arg_file_line))
        {
           // Find the read parameter name that corresponds to this parameter
           vector<std::string> tokens = util::Tokens(arg_file_line, ",");
           std::string search_sdk_name    = util::GetStringBetweenDelimiters(util::RemoveAllWhitespace(tokens[0]), "'", "'");
           std::string search_read_name = util::GetStringBetweenDelimiters(util::RemoveAllWhitespace(tokens[1]), "'", "'");
           if (search_sdk_name == sdk_param_name) {
              read_param_name = search_read_name;
              break;
           }
        }
        int branch_status = _tree_pars->SetBranchAddress(read_param_name.c_str(), &(_read_params[ip]));
        if(branch_status != 0) {
           LOG("ToyBuilder", pERROR) << "Could not assign parameter [" << read_param_name
                                     << "] (ip = " << ip << ") to a branch in the input thrown tree.";
           util::Die(-1);
        }
     }
  }   // setup the input tree

  FitParamList * free_par_list = fpl.ParameterOfInterestSubset();

  // Read the toy from the tree and fill the FPL
  _tree_pars->GetEntry(toy_number);
  for (int ip = 0 ; ip < conv::knppmax ; ip++) {
     conv::PhysParamId_t p = (conv::PhysParamId_t)ip;
     if (p == conv::kPhypUndefined) { continue; }
     if (!fpl.Exists(p)) { continue; }

     // Keep the parameter(s) of interest at their initial values
     if(free_par_list->Exists(p)) {
        fpl.SetCurrentValue(p, free_par_list->InitValue(p));
        continue;
     }

     if (_read_params[ip] + 999 < conv::kASmallNum) {
        LOG("ToyBuilder", pERROR) << "The thrown value for param ["
                                  << util::NameRegistry::Instance()->ParamAsStringVerySimple(p)
                                  << "] could not be found.";
        util::Die(-1);
     }

     double param_value = _read_params[ip];

     // Put in special cases here, where the param value needs changing too
     if (ts.Contains("compare-t2k-legacy") && p == util::NameRegistry::Instance()->PhysParamFromString("sk_e_scale")) {
        param_value += 1;
     }

     fpl.SetCurrentValue(p, param_value);
  }

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
  LOG("ToyBuilder", pDEBUG) << "Reading toy from input throw file with FPL = " << fpl;
#endif
}

} // valor

