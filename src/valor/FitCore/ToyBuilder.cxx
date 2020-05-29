#include "valor/FitCore/ToyBuilder.h"

#include <limits>
#include <sstream>
#include <string>
#include <cmath>
#include "TMath.h"

#include "valor/Conventions/Constants.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/ToyStream.h"
#include "valor/FitCore/UtlFitParamList.h"
#include "valor/FitCore/UtlOscillation.h"

//#define VALOR_EXPT_LOW_LEVEL_MESG_ENABLED

namespace valor
{

ToyBuilder::ToyBuilder(CompoundFitElements& elements,
                       bool use_simple_binning, bool store_mode_arrays,
                       bool toys_are_marginalisation, bool save_throws_to_tree):
  _model(elements.FitModel()),
  _data(elements.FitData()),
  _toys_are_marginalisation(toys_are_marginalisation),
  _use_simple_binning(use_simple_binning),
  _store_mode_arrays(store_mode_arrays),
  _save_throws_to_tree(save_throws_to_tree),
  _tree_pars(nullptr),
  _file_pars(nullptr),
  _read_params(new double[conv::knppmax]),
  _use_read_params(new bool[conv::knppmax]),
  _read_from_tree(false),
  _step_size(-999),
  _toy_throw_file(nullptr),
  _toy_stream_persistent(nullptr)
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

   delete this->_toy_stream_persistent;
}
//______________________________________________________________________________
void ToyBuilder::BuildToys(const std::string filename, const int Ntoys, FitParamList& fpl,
      bool randomizeSystematics, bool fluctuateStatistics, const char* options, bool rethrowUnphysical)
{
   // Create a file to store this batch of toys
   if(filename.empty()) {
      LOG("ToyBuilder", pFATAL) << "Tried to generate toys, but the name of the output file was not specified!";
      util::Die(-1);
   }
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

   if(_toys_are_marginalisation && fluctuateStatistics){
      LOG("ToyBuilder", pWARN) << "The option to fluctuate statistics was requested when building "
                               << "marginalisation toys. This has been ignored, as it is not yet supported.";
   }

   unsigned int n_rethrows = 0;
   int throw_tree_ientry = 0;   // When reading in parameter values from an input tree, we may be only reading the nth entry, or randomly sampling entries, so we keep track of the current entry number.
   bool last_throw_was_physical = true;
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
            PhysParamList tweaks = this->_model->Tweaks();
            fpl.FromPhysParamList(tweaks, "current silent");
         }
         if(_read_from_tree) {
            // Will we read throws from a pre-existing file instead of generating them? (This is used e.g. to convert
            // T2K legacy toy throws into SDK marginalisation toys, or to read MCMC steps).
            // This can be used in conjunction with the above randomisation, as this can overwrite some of the thrown
            // params, or all, depending on the contents of the parameter name map passed to SetReadFromThrowTree().
            this->ReadParamsFromThrowFile(fpl, options, throw_tree_ientry, Ntoys, last_throw_was_physical);
         }

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
         LOG("ToyBuilder", pINFO) << fpl;
#endif

         // Check parameter name, maybe rethrow
         if(rethrowUnphysical && !util::AllWithinLimits(fpl))
         {
            n_rethrows++;
            last_throw_was_physical = false;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
            LOG("ToyBuilder", pDEBUG) << "At least one parameter thrown " <<
               "outside expected limits. Rethrowing.";
#endif
         }
         else
         {
            last_throw_was_physical = true;
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
bool ToyBuilder::BuildSingleToy(const std::string filename, FitParamList& fpl,
                                bool isLastToy, bool fluctuateStatistics, bool allowUnphysical)
{
  if(_toys_are_marginalisation && fluctuateStatistics){
    LOG("ToyBuilder", pWARN) << "The option to fluctuate statistics was requested when building "
                             << "marginalisation toys. This has been ignored, as it is not yet supported.";
  }

  // Create a file and ToyStream to store this toy, if it doesn't already exist.
  if(!_toy_stream_persistent) {

    if(filename.empty()) {
      LOG("ToyBuilder", pFATAL) << "Tried to generate toys, but the name of the output file was not specified!";
      util::Die(-1);
    }

    if(_toys_are_marginalisation) {
      // Build marginalisation toys
      std::string write_opt_str = "pdft_vf";
      if(_store_mode_arrays ) { write_opt_str += "_pdfc_vf"; }
      if(_use_simple_binning) { write_opt_str += "+SB";      }

      // First generate a dummy toy to be overwritten (so that the model/data arrays can be initialised)
      FitParamList* fpl_void = util::GetFitParameterList("void", "");
      this->_model->Generate(*fpl_void);

      _toy_stream_persistent = this->_model->InitToyStream(filename, "write", write_opt_str);
    }
    else {
      // Build fake data toys
      std::string write_opt_str = "data_vf";
      if(_use_simple_binning) { write_opt_str += "+SB"; }

      // First generate a dummy toy to be overwritten (so that the model/data arrays can be initialised)
      FitParamList* fpl_void = util::GetFitParameterList("void", "");
      this->_data->Generate(this->_model, *fpl_void, false);

      _toy_stream_persistent = this->_data->InitToyStream(filename, "write", write_opt_str);
    }
  }

  // Check that the parameter values in the input FitParamList are valid
  if(!allowUnphysical && !util::AllWithinLimits(fpl))
  {
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
    LOG("ToyBuilder", pDEBUG) << "At least one parameter thrown " <<
               "outside expected limits. Rethrowing.";
#endif
    return false;
  }

  // Generate a toy-MC experiment and write to the output binary file
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
  PhysParamList true_params = fpl.AsPhysParamList("current");
  LOG("ToyBuilder", pDEBUG) << "Generating toy-MC data";
  LOG("ToyBuilder", pDEBUG) << "Toy-MC generation uses true parameters: " << true_params;
#endif
  this->_model->Generate(fpl, _toy_stream_persistent->GetVALORArrayFormat());
  if(_toys_are_marginalisation) {
    this->_model->Write(_toy_stream_persistent);
  }
  else {
    this->_data->Generate(this->_model, fluctuateStatistics, _toy_stream_persistent->GetVALORArrayFormat());
    this->_data->Write(_toy_stream_persistent);
  }
  this->SaveParamThrows(fpl, isLastToy, _toy_stream_persistent->GetFilename());

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
  if(_toys_are_marginalisation) {
    LOG("ToyBuilder", pNOTICE) << "Marg toy generated. Event rates:\n" << this->_model->GetPDFIntegralAsString();
  }
  else {
    LOG("ToyBuilder", pNOTICE) << "Fake data toy generated. Event rates:\n" << this->_data->GetPDFIntegralAsString();
  }
#endif

  if(isLastToy) {
    delete _toy_stream_persistent;
  }
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
int ToyBuilder::GetThrowTreeEntryNum(const int& current_throw_tree_ientry, const int& ntoys, const int& nentries,
                                     const bool& last_throw_was_physical, const char* toy_throw_tree_opt)
{
  int next_throw_tree_ientry = -999;

  TString options(toy_throw_tree_opt);
  if(options.Contains("const_step")) {
    if(!last_throw_was_physical) {
      // If the last attempt at generating a toy resulted in something unphysical, just try to read the next entry from
      // the tree. This should be okay, as if we're extracting every nth entry from the tree and one fails, we shouldn't
      // need to skip n entries to get a toy that's uncorrelated with the last successfully built toy.
      next_throw_tree_ientry = current_throw_tree_ientry + 1;
    }
    else {
      // We're trying to extract every nth toy, but if the previous toy had to be rethrown any number of times, the
      // number of rethrows could be > n, so for the next toy, we can't just assume that the entry at n*stepsize will
      // not have already been used. Additionally, if we rethrow R toys on average to successfully generate a toy,
      // then just stepping by a constant amount of entries will mean that the input tree must have
      // (stepsize + R) * ntoys entries, which may be out of bounds if R is non-zero. So this should help to avoid that
      // while accounting for rethrows and still keeping a good step size. I'd still expect to get an out of bounds
      // error if the step size is small and R and ntoys are both large.
      double ratio = ((double)current_throw_tree_ientry) / ((double)_step_size);
      if(fmod(ratio , 1.0 ) < conv::kASmallNum) {   // i.e. if the current entry lies exactly on a step
        next_throw_tree_ientry = current_throw_tree_ientry + _step_size;
      }
      else {
        next_throw_tree_ientry = TMath::Ceil(ratio) * _step_size;
      }
    }
  }
  else if(options.Contains("random_sample")) {
    // TODO: Implement: See Git issue #22
    LOG("ToyBuilder", pFATAL) << "Option \'random_sample\' not yet implemented.";
    util::Die(-1);
  }
  else {
    LOG("ToyBuilder", pFATAL) << "Option \'" << toy_throw_tree_opt << "\' not yet implemented.";
    util::Die(-1);
  }

  if(next_throw_tree_ientry < 0) {
    LOG("ToyBuilder", pFATAL) << "The next entry number to be read from the input tree was set to "
                              << next_throw_tree_ientry << ", so something's gone wrong!";
    util::Die(-1);
  }
  if(next_throw_tree_ientry >= nentries) {
    LOG("ToyBuilder", pFATAL) << "When extracting toy throws from the input tree, the current entry number is "
                              << next_throw_tree_ientry << ", but the tree only contains " << nentries
                              << " entries. This can happen if many of the entries lead to unphysical toys being "
                              << "generated, so that more entries are skipped than there are entries in the tree - "
                              << "the number of requested toys";
    util::Die(-1);
  }

  return next_throw_tree_ientry;
}
//______________________________________________________________________________
void ToyBuilder::SetReadFromThrowTree(const FitParamList& fpl, std::string toy_throw_file_to_read, std::string tree_name,
                                      std::string param_name_map_file, int step_size, const char* options)
{
  TString ts(options);

  _read_from_tree = (toy_throw_file_to_read != "" && toy_throw_file_to_read != "NONE" &&
                     param_name_map_file    != "" && param_name_map_file    != "NONE" &&
                     tree_name              != "" && tree_name              != "NONE");

  if(_read_from_tree) {
    LOG("valor_dune", pNOTICE) << "Will read parameter values from an input tree instead of (or in conjunction with) "
                               << "randomly throwing parameter values.";
  }
  else {
    LOG("valor_dune", pNOTICE) << "Stopping reading parameter values from the input tree.";
    return;
  }

  // If we already have a toy throw tree open, close it so we can open a new one
  if(_tree_pars) {
    _toy_throw_file->Close();
    _tree_pars = nullptr;
  }

  // Open the tree containing thrown parameter values
  if(toy_throw_file_to_read == "" || param_name_map_file == "") {
    LOG("valor_dune", pERROR) << "No input toy throw file or parameter name map file was specified.";
    util::Die(-1);
  }

  _toy_throw_file = new TFile(toy_throw_file_to_read.c_str(), "READ");
  if(!_toy_throw_file->IsOpen()) {
    LOG("valor_dune", pERROR) << "Could not open file [" << toy_throw_file_to_read << "]";
    util::Die(-1);
  }
  _toy_throw_file->cd();
  _tree_pars = (TTree *) _toy_throw_file->Get(tree_name.c_str());
  if(!_tree_pars) {
    LOG("valor_dune", pERROR) << "Could not get tree named [" << tree_name
                              << "] from toy throw file named [" << toy_throw_file_to_read << "]";
    util::Die(-1);
  }

  // Set up an array to hold all of the parameter values that we'll read
  for(int ip = 0; ip < conv::knppmax; ip++){
    _read_params   [ip] = -999;
    _use_read_params[ip] = false;
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

    bool found = false;
    std::string arg_file_line = "";
    while (std::getline(map, arg_file_line))
    {
      // Find the read parameter name that corresponds to this parameter
      if(util::RemoveAllWhitespace(arg_file_line) == "") { continue; }
      vector<std::string> tokens = util::Tokens(arg_file_line, ",");
      std::string search_sdk_name    = util::GetStringBetweenDelimiters(util::RemoveAllWhitespace(tokens[0]), "'", "'");
      std::string search_read_name = util::GetStringBetweenDelimiters(util::RemoveAllWhitespace(tokens[1]), "'", "'");
      if (search_sdk_name == sdk_param_name) {
        read_param_name = search_read_name;
        _use_read_params[ip] = true;
        found = true;
        break;
      }
    }
    if(!found) { continue; }

    int branch_status = _tree_pars->SetBranchAddress(read_param_name.c_str(), &(_read_params[ip]));
    if(branch_status != 0) {
      LOG("valor_dune", pERROR) << "Could not assign parameter [" << read_param_name
                                << "] (ip = " << ip << ") to a branch in the input thrown tree.";
      util::Die(-1);
    }
  }

  _step_size = step_size;
}
//______________________________________________________________________________
void ToyBuilder::ReadParamsFromThrowFile(FitParamList& fpl, const char* options, int& throw_tree_ientry,
                                         const int& Ntoys, const bool& last_throw_was_physical)
{
   TString ts(options);

  FitParamList * free_par_list = fpl.ParameterOfInterestSubset();

  // Read the toy from the tree and fill the FPL
  throw_tree_ientry = this->GetThrowTreeEntryNum(throw_tree_ientry, Ntoys, _tree_pars->GetEntries(), last_throw_was_physical, options);
  _tree_pars->GetEntry(throw_tree_ientry);
  for (int ip = 0 ; ip < conv::knppmax ; ip++) {
     conv::PhysParamId_t p = (conv::PhysParamId_t)ip;
     if (p == conv::kPhypUndefined) { continue; }
     if (!fpl.Exists(p))            { continue; }
     if (!_use_read_params[ip])     { continue; }

     // Keep the parameter(s) of interest at their initial values
     if(free_par_list->Exists(p)) {
        fpl.SetCurrentValue(p, free_par_list->InitValue(p));
        continue;
     }

     if (_read_params[ip] + 999 < conv::kASmallNum) {
        LOG("valor_dune", pERROR) << "The thrown value for param ["
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
  LOG("valor_dune", pDEBUG) << "Reading toy from input throw file with FPL = " << fpl;
#endif
}

} // valor

