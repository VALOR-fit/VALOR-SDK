#include "valor/FitCore/UtlOscillation.h"

#include <algorithm>

#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/NuOscDmsqScheme.h"
#include "valor/Conventions/NuOscDeltaCP.h"
#include "valor/Conventions/NuOscDmsqScheme.h"
#include "valor/Conventions/NuMassHierarchy.h"
#include "valor/Conventions/Opt.h"
#include "valor/Conventions/PhysParamId.h"
#include "valor/Conventions/PhysHypothesisId.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/Utilities/Env.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlNumStat.h"
#include "valor/Utilities/Rnd.h"
#include "valor/FitCore/PhysHypothesis.h"

#include <TSystem.h>
#include <TMath.h>

using namespace valor::conv;

namespace valor
{
namespace util
{
namespace oscillation
{
void SetOscillationParameters(CmdLnArgList& arg, FitParamList& fpl)
{
   const conv::PhysHypothesis_t hyp_id = fpl.PhysicsHypothesis();
   if(hyp_id == conv::kHypUndefined) {
      LOG("UtlOscillation", pFATAL) << "Could not get a physics hypothesis from the input FitParamList";
      util::Die(-1);
   }
   const PhysHypothesis hyp(hyp_id);
   const std::vector<conv::PhysParamId_t> osc_parameters = hyp.GetParams();

   string free_param1 = arg.GetString("fit-param1");
   string free_param2 = arg.Exists("fit-param2") ? arg.GetString("fit-param2") : "";

   for(valor::conv::PhysParamId_t p : osc_parameters)
   {
      std::string simple_name = util::NameRegistry::Instance()->ParamAsStringVerySimple(p);
      if(fpl.Exists(p))
      {
         if(!ParameterPool::Instance()->IsParamInPool(p)) {
            LOG("UtlOscillation", pFATAL) << "The osc param \'" << util::NameRegistry::Instance()->ParamAsString(p)
                                          << "\' was in the FPL but not the ParameterPool";
            util::Die(-1);
         }
         const Parameter & param = ParameterPool::Instance()->GetParam(p);
         double fix_value = arg.GetDouble(TString::Format("fix-at-%s", simple_name.c_str()).Data());
         double min_value = param.GetLimits().first;
         double max_value = param.GetLimits().second;
         if(fix_value >= min_value && fix_value < max_value)
         {
            fpl.Update(p, fix_value);
            fpl.SetInitValue(p, fix_value);
            fpl.FixParameter(p);
            LOG("UtlOscillation", pNOTICE) << "Setting fixed parameter "
                                           << util::NameRegistry::Instance()->ParamAsString(p) << " := " << fix_value;
         }
         else if(free_param1 == simple_name ||
                 free_param2 == simple_name)
         {
            double free_value = 0.0;
            if(free_param1 == simple_name)
            {
               free_value = arg.GetDouble("fit-param1-value");
            }
            else
            {
               if(arg.Exists("fit-param2-value"))
               {
                  free_value = arg.GetDouble("fit-param2-value");
               }
               else
               {
      
                  LOG("UtlOscillation", pERROR) << "--fit-param2 specified " <<
                     "without accompanying --fit-param2-value.";
                  exit(1);
               }
            }
            fpl.Update(p, free_value);
            fpl.SetInitValue(p, free_value);
            fpl.FreeParameter(p);
            LOG("UtlOscillation", pNOTICE) << "Setting free parameter " <<
              util::NameRegistry::Instance()->ParamAsString(p) << " := " << free_value;
         }
         else
         {
            fpl.SetDoMarg(p, true);
            LOG("UtlOscillation", pNOTICE) << "Setting marginalised parameter " <<
              util::NameRegistry::Instance()->ParamAsString(p);
         }
      }
   }
}

//____________________________________________________________________________
void SetParameterPriors(CmdLnArgList& arg, FitParamList& fpl)
{
   // Read in the priors XML file
   std::vector<std::string> prior_file_names;
   std::vector<std::string> files_with_priors;
   prior_file_names.push_back(arg.GetString("osc-prior-file"));
   files_with_priors.push_back(util::CompleteConfigPath(prior_file_names.back()));

   bool syst_params = arg.Exists("syst-prior-file") and arg.GetString("syst-prior-file") != "-";
   if (syst_params)
   {
      prior_file_names.push_back(arg.GetString("syst-prior-file"));
      files_with_priors.push_back(util::CompleteConfigPath(prior_file_names.back()));
      fpl.SetNonGaussianSystParExists();

   }
   // Read syst priors form XML file 
  
   // Check that the prior file exists, either in the config directory or the current directory
   //Oscillation prios should be set always. sytematic prior is now(2020) used for T2K analysis. 
   //Just in case it thorws only an INFO message if cannot find syst prior file.
   FileStat_t dummy;

   for (int i = 0; i<prior_file_names.size(); i++){
      if (gSystem->GetPathInfo(files_with_priors[i].c_str(), dummy)){
         files_with_priors[i] = prior_file_names[i].c_str();
         if (gSystem->GetPathInfo(files_with_priors[i].c_str(), dummy)) {
            LOG("UtlOscillation", pERROR) << "Prior config file not found: " << prior_file_names[i];
            exit(1);
         }
      }

      // Parse the input XML priors file for oscillation parameters 
      TXMLEngine* xmleng = new TXMLEngine;
      XMLDocPointer_t xmldoc = xmleng->ParseFile(files_with_priors[i].c_str());
      if (xmldoc==0){
         LOG("UtlOscillation", pERROR) << "Can not parse input XML file [" << files_with_priors[i] << "]";
         exit(1);
      }

      XMLNodePointer_t root_node = xmleng->DocGetRootElement(xmldoc);
      const char * list_name_cstr = xmleng->GetAttr(root_node,"name");
      LOG("UtlOscillation", pNOTICE) << "Reading priors from element list named" << list_name_cstr;

      XMLNodePointer_t curr_node = xmleng->GetChild(root_node);
      int npriors_set = 0;
      while(curr_node){
         string curr_node_name = xmleng->GetNodeName(curr_node);
         if(!strcmp(curr_node_name.c_str(),"prior")){
            bool success = util::oscillation::SetSinglePrior(xmleng,curr_node,fpl);
            if (!success){
               LOG("UtlOscillation", pERROR) << "Failed to read prior list. Exiting.";
               exit(1);
            }
            npriors_set++;
         }
         curr_node = xmleng->GetNext(curr_node);
      }
   }

   // TODO: Put in some check to see if we've set priors on every oscillation parameter in our phys hypothesis
}

//____________________________________________________________________________
bool SetSinglePrior(TXMLEngine* xmleng, XMLNodePointer_t top_node, FitParamList& fpl)
{
   // First read the information that's common to each prior type
   string param_names    = "";  // Comma-separated list of simple parameter names
   string prior_type     = "";  // 'flat', 'gaus', or 'user'
   string opt            = "";
   XMLNodePointer_t curr_node = xmleng->GetChild(top_node);
   while(curr_node) {
      string curr_node_name = xmleng->GetNodeName(curr_node);
      if(xmleng->GetNodeContent(curr_node)) {
         string content = util::TrimSpaces(xmleng->GetNodeContent(curr_node));
         if(curr_node_name == "params") { param_names = content; }
         if(curr_node_name == "type"  ) { prior_type = content; }
         if(curr_node_name == "scheme") { opt        = content; }
      }
      curr_node = xmleng->GetNext(curr_node);
   }

   // Now read the information that's specific to each prior type
   string prior_attribute1 = "";    // If type = flat, this is the lower end of the range. If type = gaus, this is the mean.  If type = user, this is the NH prior file
   string prior_attribute2 = "";    // If type = flat, this is the upper end of the range. If type = gaus, this is the sigma. If type = user, this is the IH prior file
   string prior_attribute3 = "";    // If type = user, this is the prior hist name. Otherwise this stays as an empty string
   curr_node = xmleng->GetChild(top_node);   // Put the current node back to the top of the list
   while(curr_node) {
      string curr_node_name = xmleng->GetNodeName(curr_node);
      if(xmleng->GetNodeContent(curr_node)) {
         string content = util::TrimSpaces(xmleng->GetNodeContent(curr_node));
         if(prior_type == "flat") {
            if(curr_node_name == "low" ) { prior_attribute1 = content; }
            if(curr_node_name == "high") { prior_attribute2 = content; }
         }
         else if(prior_type == "gaus") {
            if(curr_node_name == "mean" ) { prior_attribute1 = content; }
            if(curr_node_name == "sigma") { prior_attribute2 = content; }
         }
         else if(prior_type == "user") {
            if(curr_node_name == "file_nh"  ) { prior_attribute1 = content; }
            if(curr_node_name == "file_ih"  ) { prior_attribute2 = content; }
            if(curr_node_name == "hist_name") { prior_attribute3 = content; }
         }
         else {
            LOG("UtlOscillation", pERROR) << "Could not understand the prior type [" << prior_type << "]";
            exit(1);
         }
      }
      curr_node = xmleng->GetNext(curr_node);
   }

   // Check that we read the correct number of attributes for each prior type
   if(prior_type == "flat" && (prior_attribute1 == "" || prior_attribute2 == "" || prior_attribute3 != "")) {
      LOG("UtlOscillation", pERROR) << "Something went wrong when setting the flat prior named [" << param_names << "]";
      exit(1);
   }
   else if(prior_type == "gaus" && (prior_attribute1 == "" || prior_attribute2 == "" || prior_attribute3 != "")) {
      LOG("UtlOscillation", pERROR) << "Something went wrong when setting the Gaussian prior named [" << param_names << "]";
      exit(1);
   }
   else if(prior_type == "user" && (prior_attribute1 == "" || prior_attribute2 == "" || prior_attribute3 == "")) {
      LOG("UtlOscillation", pERROR) << "Something went wrong when setting the user prior named [" << param_names << "]";
      exit(1);
   }

   // Get the parameter(s) that we'll apply this prior to
   const util::NameRegistry * nr = util::NameRegistry::Instance();
   std::vector<std::string> tokens = util::Tokens(param_names, ",");
   std::vector<conv::PhysParamId_t> params;
   for(std::string param_name : tokens) {
      param_name = util::RemoveAllWhitespace(param_name);
      conv::PhysParamId_t p = nr->PhysParamFromString(param_name);
      if(p == conv::kPhypUndefined) {
         LOG("UtlOscillation", pERROR) << "Could not parse parameter [" << param_name << "]";
         exit(1);
      }
      params.push_back(p);
   }

   // Get the mixing/dmsq/phase scheme(s) for this prior (1 scheme per parameter)
   std::vector<std::string> scheme_tokens = util::Tokens(opt, ",");
   std::vector<std::string> schemes;
   for(std::string scheme_name : scheme_tokens) {
      scheme_name = util::RemoveAllWhitespace(scheme_name);
      schemes.push_back(scheme_name);
   }
   if(schemes.size() != params.size()) {
      LOG("UtlOscillation", pERROR) << "The prior named [" << param_names
                                    << "] has a mismatch in the number of schemes and parameters defined";
      exit(1);
   }

   // Check that the number of parameters is as expected
   if((prior_type == "flat" || prior_type == "gaus") && params.size() != 1) {
      LOG("UtlOscillation", pERROR) << "The number of parameters specified for flat or Gaussian prior named [" << param_names << "] was != 1";
      exit(1);
   }
   else if(!(params.size() == 1 || params.size() == 2)){
      LOG("UtlOscillation", pERROR) << "The number of parameters specified for user prior named [" << param_names << "] was not 1 or 2";
      exit(1);
   }

   // Set the prior
   if(prior_type == "flat") {
      double low  = atof(prior_attribute1.c_str());
      double high = atof(prior_attribute2.c_str());
      fpl.SetPriorMode   (params[0], "flat"    );
      fpl.SetMinValueFlat(params[0], low       );
      fpl.SetMaxValueFlat(params[0], high      );
      fpl.SetPriorScheme (params[0], schemes[0]);
      LOG("UtlOscillation", pNOTICE) << "Setting flat prior for param " << param_names
                                     << ": [" << low << ", " << high << "], using scheme: "
                                     << schemes[0];
   }
   else if(prior_type == "gaus") {
      double mean  = atof(prior_attribute1.c_str());
      double sigma = atof(prior_attribute2.c_str());
      fpl.SetPriorMode  (params[0], "gaus"    );
      fpl.SetMeanGaus   (params[0], mean      );
      fpl.SetSigmaGaus  (params[0], sigma     );
      fpl.SetPriorScheme(params[0], schemes[0]);
      LOG("UtlOscillation", pNOTICE) << "Setting Gaussian prior for param " << param_names
                                     << ": mean = " << mean << ", sigma = " << sigma
                                     << ", using scheme: " << schemes[0];
   }
   else {
      // TODO: This will not work if we throw the mass hierarchy!
      std::string prior_hist_file_name = "";
      if(Opt::Instance()->gMassHierarchyAssumed == util::HierarchyFromString("normal")) {
         prior_hist_file_name = prior_attribute1;
      }
      else {
         prior_hist_file_name = prior_attribute2;
      }
      TString base_dir = Env::Instance()->TopLevelDataDir().c_str();
      TFile* prior_root_file = new TFile((base_dir + "/" + prior_hist_file_name.c_str()).Data(), "read");
      if(!prior_root_file->IsOpen()) {
         LOG("UtlOscillation", pERROR) << "Could not find prior hist file [" << prior_hist_file_name << "]";
         exit(1);
      }

      if(params.size() == 1) {
         TH1D* prior_hist = (TH1D*) prior_root_file->Get(prior_attribute3.c_str());
         if(!prior_hist) {
            LOG("UtlOscillation", pERROR) << "Could not find 1D prior hist [" << prior_attribute3 << "] in file ["
                                          << prior_hist_file_name << "]";
            exit(1);
         }
         fpl.SetPriorMode  (params[0], "user");
         fpl.SetPriorHisto (params[0], prior_hist);
         fpl.SetPriorScheme(params[0], schemes[0]);
         LOG("UtlOscillation", pNOTICE) << "Setting 1D hist prior for param " << param_names
                                        << ": file = " << prior_hist_file_name
                                        << ", using scheme: " << schemes[0];
      }
      else {
         // Set the axis titles to the parameter names they correspond to, so we know which is which when we come to throw from the hist
         TH2D* prior_hist = (TH2D*) prior_root_file->Get(prior_attribute3.c_str());
         if(!prior_hist) {
            LOG("UtlOscillation", pERROR) << "Could not find 1D prior hist [" << prior_attribute3 << "] in file ["
                                          << prior_hist_file_name << "]";
            exit(1);
         }
         prior_hist->GetXaxis()->SetTitle(nr->ParamAsStringVerySimple(params[0]).c_str());
         prior_hist->GetYaxis()->SetTitle(nr->ParamAsStringVerySimple(params[1]).c_str());
         fpl.SetPriorMode   (params[0], "user"    );
         fpl.SetPriorHisto2D(params[0], prior_hist);
         fpl.SetPriorScheme (params[0], schemes[0]);
         fpl.SetPriorMode   (params[1], "user"    );
         fpl.SetPriorHisto2D(params[1], prior_hist);
         fpl.SetPriorScheme (params[1], schemes[1]);
         LOG("UtlOscillation", pNOTICE) << "Setting 2D hist prior for params " << param_names
                                        << ": file = " << prior_hist_file_name
                                        << "; Associating x-axis with " << nr->ParamAsStringVerySimple(params[0])
                                        << " (scheme = " << schemes[0] << ")"
                                        << "; Associating y-axis with " << nr->ParamAsStringVerySimple(params[1])
                                        << " (scheme = " << schemes[1] << ")";
      }
      prior_root_file->Close();
      delete prior_root_file;
   }
   // set flag if it is a systematic parameter with a prior ti properly handle it further in CorrelatedSytematics
   if(util::IsSystematic(params[0]))
   {
      fpl.SetSystParWithNonGaussianPrior(params[0]);
      LOG("UtlOscillation", pNOTICE) << "Setting parameter " << nr->ParamAsStringVerySimple(params[0]) << " "
                                          << " as systematic paraemter with " << prior_type << " prior";
   }
   return true;
}

//____________________________________________________________________________
void ThrowOscillationParameters(FitParamList& fpl)
{
   const conv::PhysHypothesis_t hyp_id = fpl.PhysicsHypothesis();
   if(hyp_id == conv::kHypUndefined) {
      LOG("UtlOscillation", pFATAL) << "Could not get a physics hypothesis from the input FitParamList";
      util::Die(-1);
   }
   const PhysHypothesis hyp(hyp_id);
   const std::vector<conv::PhysParamId_t> osc_parameters = hyp.GetParams();

   // First check if we're using any 1D or 2D hists as priors
   std::vector<conv::PhysParamId_t> pars_with_1D_priors;
   std::vector<conv::PhysParamId_t> pars_with_2D_prior_hists;
   for(valor::conv::PhysParamId_t p : osc_parameters)
   {
      if(fpl.Exists(p) && fpl.IsMarginalized(p))
      {
         if(fpl.GetPriorModeName(p) == "user") {
            // Is it a 1D or a 2D prior?
            if     (fpl.GetPriorHisto(p)  ) { pars_with_1D_priors.push_back(p); }
            else if(fpl.GetPriorHisto2D(p)) { pars_with_2D_prior_hists.push_back(p); }
         }
         else {
            pars_with_1D_priors.push_back(p);
         }
      }
   }

   // Make sure we only have 0 or 2 parameters with a 2D prior hist
   if(!(pars_with_2D_prior_hists.size() == 0 || pars_with_2D_prior_hists.size() == 2)) {
      LOG("UtlOscillation", pERROR) << "The number of osc params with 2D priors is " << pars_with_2D_prior_hists.size()
                                    << ". Expected either 0 or 2.";
      exit(1);
   }

   // First deal with parameters that 1D priors
   for(conv::PhysParamId_t p : pars_with_1D_priors) {
      double value = util::oscillation::GetRandomizedParam(p, fpl);
      // Prior range and conversion to the used mixing/dmsq/phase scheme
      // is enforced in util::oscillation::GetRandomizedParam, so no need to check here

      fpl.Update(p, value);
      fpl.SetInitValue(p, value);
   }

   // Finally do the 0 or 2 parameters that have a 2D prior. Throw them together to maintain correlations
   if(pars_with_2D_prior_hists.size() == 2) {
      std::pair<conv::PhysParamId_t,conv::PhysParamId_t> params_2D =
        std::make_pair(pars_with_2D_prior_hists[0], pars_with_2D_prior_hists[1]);

      std::map<conv::PhysParamId_t,double> thrown_values = util::oscillation::GetRandomizedParams2DCorrelated(params_2D, fpl);
      // Prior range and conversion to the used mixing/dmsq/phase scheme
      // is enforced in util::oscillation::GetRandomizedParams2DCorrelated, so no need to check here

      fpl.Update      (params_2D.first , thrown_values.at(params_2D.first));
      fpl.SetInitValue(params_2D.first , thrown_values.at(params_2D.first));
      fpl.Update      (params_2D.second, thrown_values.at(params_2D.second));
      fpl.SetInitValue(params_2D.second, thrown_values.at(params_2D.second));
   }
}

//____________________________________________________________________________
void SetUnoscillated(FitParamList& fpl)
{
   string osc_parameters[4] = { "mixing12", "mixing13", "mixing23", "cpv" };
   string dm_parameters[3] = {"dm23sq", "dm12sq", "dm13sq" };

   for(int i = 0; i < 4; ++i)
   {
      valor::conv::PhysParamId_t p = util::NameRegistry::Instance()->PhysParamFromString(
            osc_parameters[i].c_str());
      if(fpl.Exists(p))
      {
         fpl.Update(p, 0.0);
         fpl.SetInitValue(p, 0.0);
      }
   }

   for(int i = 0; i < 3; ++i)
   {
      valor::conv::PhysParamId_t p = util::NameRegistry::Instance()->PhysParamFromString(
            dm_parameters[i].c_str());
      if(fpl.Exists(p))
      {
         double value = fpl.InitValue(p);
         fpl.Update(p, value);
         fpl.SetInitValue(p, value);
      }
   }
}
//____________________________________________________________________________
double GetRandomizedParam(conv::PhysParamId_t p, FitParamList& fpl)
{
  // Here we throw the parameter according to the scheme set for its prior.
  // After throwing,  if  it is  an oscillation parameter, the value is converted to whichever scheme we're using internally.
   string prior_scheme_name; 
   string target_scheme_name = ""; // Leave this as an empty string to allow util::ConvertOscParamToUsedScheme to set the target scheme according to the values set in Opt::Instance
   string mode = fpl.GetPriorModeName(p);

  if(!ParameterPool::Instance()->IsParamInPool(p)) {
     LOG("UtlOscillation", pFATAL) << "The osc param \'" << util::NameRegistry::Instance()->ParamAsString(p)
                                   << "\' was in the FPL but not the ParameterPool";
     util::Die(-1);
  }
  const Parameter & param = ParameterPool::Instance()->GetParam(p);
  double min = param.GetLimits().first;
  double max = param.GetLimits().second;

  double value = -999;

  if(mode == "flat"){
     // Get the limits imposed by the prior
     double min_prior = fpl.GetMinValueFlat(p);
     double max_prior = fpl.GetMaxValueFlat(p);

     value = min_prior - 1.0;
     do{
         if(!util::IsSystematic(p)){
            value = Rnd::Instance()->RndToyMCosc().Uniform(min_prior, max_prior);
         }
         else{
            value = Rnd::Instance()->RndToyMCsyst().Uniform(min_prior, max_prior);
         }
     }
     while(value < min || value > max || value < min_prior || value > max_prior);
  }
  else if(mode == "gaus"){
     double mean = fpl.GetMeanGaus(p);
     double sigma = fpl.GetSigmaGaus(p);
     // Get the absolute physical limits, these are hard-coded assuming some conventions,
     // so are converted to the conventions used for the throw in e.g. OscParamMinValue

     value = min - 1.0;
     do{
        value = Rnd::Instance()->RndToyMCosc().Gaus(mean, sigma);
     }
     while(value < min || value > max);
  }
  else if(fpl.GetPriorHisto(p)) {
     value = min - 1.0;
     do{
        value = fpl.GetPriorHisto(p)->GetRandom();
     }
     while(value < min || value > max);
  }
  else {
     LOG("UtlOscillation", pERROR) << "Invalid prior type, can not throw oscillation parameter, "
                                   << util::NameRegistry::Instance()->ParamAsStringVerySimple(p);
     exit(1);
  }

  if(!util::IsSystematic(p)){
   prior_scheme_name  = fpl.GetPriorSchemeName(p);
   return util::ConvertOscParamToUsedScheme(p, value, prior_scheme_name, target_scheme_name);
  }
  else {
   return value;
  }
}

//________________________________________________________________________________________
std::map<conv::PhysParamId_t,double> GetRandomizedParams2DCorrelated(std::pair<PhysParamId_t,PhysParamId_t> params, FitParamList& fpl)
{
  if(!fpl.GetPriorHisto2D(params.first)) {
     LOG("UtlOscillation", pERROR) << "No 2D prior hist associated with parameter "
                                   << util::NameRegistry::Instance()->ParamAsStringVerySimple(params.first);
     exit(1);
  }

  // Make sure we know which param corresponds to the x and y axes of the prior hist
  std::string xtitle = ((TH2D*)fpl.GetPriorHisto2D(params.first))->GetXaxis()->GetTitle();
  std::string ytitle = ((TH2D*)fpl.GetPriorHisto2D(params.first))->GetYaxis()->GetTitle();
  int xindex = -1;
  int yindex = -1;
  if     (util::NameRegistry::Instance()->ParamAsStringVerySimple(params.first)  == xtitle) { xindex = 0; }
  else if(util::NameRegistry::Instance()->ParamAsStringVerySimple(params.first)  == ytitle) { yindex = 0; }
  if     (util::NameRegistry::Instance()->ParamAsStringVerySimple(params.second) == xtitle) { xindex = 1; }
  else if(util::NameRegistry::Instance()->ParamAsStringVerySimple(params.second) == ytitle) { yindex = 1; }
  if(xindex == -1 || yindex == -1) {
     LOG("UtlOscillation", pERROR) << "Failed to associate parameter "
                                   << util::NameRegistry::Instance()->ParamAsStringVerySimple(params.first)
                                   << " and/or "
                                   << util::NameRegistry::Instance()->ParamAsStringVerySimple(params.second)
                                   << " with the x (" << xtitle << ") or y  (" << ytitle
                                   << ") titles of the 2D prior hist.";
     exit(1);
  }
  if(xindex == yindex) {
     LOG("UtlOscillation", pERROR) << "Parameters "
                                   << util::NameRegistry::Instance()->ParamAsStringVerySimple(params.first)
                                   << " and "
                                   << util::NameRegistry::Instance()->ParamAsStringVerySimple(params.second)
                                   << " are assigned to the same axis titles of the 2D prior hist!";
     exit(1);
  }

  PhysParamId_t p0;
  PhysParamId_t p1;
  if(xindex == 0) { p0 = params.first;  }
  else            { p0 = params.second; }
  if(yindex == 0) { p1 = params.first;  }
  else            { p1 = params.second; }

  string prior_scheme_name_p0  = fpl.GetPriorSchemeName(p0);
  string prior_scheme_name_p1  = fpl.GetPriorSchemeName(p1);
  string target_scheme_name_p0 = ""; // Leave this as an empty string to allow util::ConvertOscParamToUsedScheme to set the target scheme according to the values set in Opt::Instance
  string target_scheme_name_p1 = ""; // Leave this as an empty string to allow util::ConvertOscParamToUsedScheme to set the target scheme according to the values set in Opt::Instance

  // Rethrow until we get a value inside the physical limits
  if(!ParameterPool::Instance()->IsParamInPool(p0)) {
     LOG("UtlOscillation", pFATAL) << "The osc param \'" << util::NameRegistry::Instance()->ParamAsString(p0)
                                   << "\' was in the FPL but not the ParameterPool";
     util::Die(-1);
  }
   if(!ParameterPool::Instance()->IsParamInPool(p1)) {
      LOG("UtlOscillation", pFATAL) << "The osc param \'" << util::NameRegistry::Instance()->ParamAsString(p1)
                                    << "\' was in the FPL but not the ParameterPool";
      util::Die(-1);
   }
  const Parameter & param0 = ParameterPool::Instance()->GetParam(p0);
  const Parameter & param1 = ParameterPool::Instance()->GetParam(p1);
  double min0 = param0.GetLimits().first;
  double max0 = param0.GetLimits().second;
  double min1 = param1.GetLimits().first;
  double max1 = param1.GetLimits().second;

  double value0 = min0 - 1.0;
  double value1 = min1 - 1.0;
  do
  {
     fpl.GetPriorHisto2D(p0)->GetRandom2(value0, value1);
  }
  while(value0 < min0 || value0 > max0 || value1 < min1 || value1 > max1);

  value0 = util::ConvertOscParamToUsedScheme(p0, value0, prior_scheme_name_p0, target_scheme_name_p0);
  value1 = util::ConvertOscParamToUsedScheme(p1, value1, prior_scheme_name_p1, target_scheme_name_p1);

  std::map<PhysParamId_t,double> return_values;
  return_values[p0] = value0;
  return_values[p1] = value1;

  return return_values;
}


//____________________________________________________________________________
PenaltyTerm SetPenaltyTermParameters(CmdLnArgList& arg,string penaltyparam)
{
   // Read in the priors XML file
   std::string osc_prior_file_name = arg.GetString("osc-prior-file");
   std::string osc_file_priors = util::CompleteConfigPath(osc_prior_file_name);
   // Check that the prior file exists, either in the config directory or the current directory
   FileStat_t dummy;
   if (gSystem->GetPathInfo(osc_file_priors.c_str(), dummy)){
       osc_file_priors = osc_prior_file_name.c_str();
       if (gSystem->GetPathInfo(osc_file_priors.c_str(), dummy)) {
           LOG("UtlOscillation", pERROR) << "Prior config file with penalty info not found: " << osc_prior_file_name;
           exit(1);
       }
   }

   // Parse the input XML priors file
   TXMLEngine* xmleng = new TXMLEngine;
   XMLDocPointer_t xmldoc = xmleng->ParseFile(osc_file_priors.c_str());
   if (xmldoc==0) {
       LOG("UtlOscillation", pERROR) << "Can not parse input XML file [" << osc_file_priors << "]";
       exit(1);
   }
   XMLNodePointer_t root_node = xmleng->DocGetRootElement(xmldoc);
   const char * list_name_cstr = xmleng->GetAttr(root_node,"name");
   LOG("UtlOscillation", pNOTICE) << "Reading penalty from element list named " << list_name_cstr;

   XMLNodePointer_t curr_node = xmleng->GetChild(root_node);
   int npenalty_set = 0;

   PenaltyTerm penaltyset;

   while(curr_node) {
      string curr_node_name = xmleng->GetNodeName(curr_node);
      if(!strcmp(curr_node_name.c_str(),"penalty")) {
         penaltyset = util::oscillation::SetSinglePenalty(xmleng,curr_node,penaltyparam);
         npenalty_set++;
      }
      curr_node = xmleng->GetNext(curr_node);
   }
   return penaltyset;
}

//____________________________________________________________________________
PenaltyTerm SetSinglePenalty(TXMLEngine* xmleng, XMLNodePointer_t top_node, string penaltyparam)
{
   // First read the information that's common to each prior type
   string param_names    = "";  // Comma-separated list of simple parameter names
   string penalty_type   = "";  // 'flat', 'gaus', or 'user'
   string opt            = "";
   XMLNodePointer_t curr_node = xmleng->GetChild(top_node);
   while(curr_node) {
       string curr_node_name = xmleng->GetNodeName(curr_node);
       if(xmleng->GetNodeContent(curr_node)) {
           string content = util::TrimSpaces(xmleng->GetNodeContent(curr_node));
           if(curr_node_name == "params") { param_names = content;
          //     if(strcmp(penaltyparam.c_str(),param_names.c_str())!=0){
          //         LOG("Marginalisation", pERROR) << "Penalty parameter is not specified in xml file, sad it is ";
          //         exit(1);
          //     }
          }
           if(curr_node_name == "type"  ) { penalty_type = content; }
           if(curr_node_name == "scheme") { opt        = content; }
       }
       //TODO: make it for histo penalty too
       curr_node = xmleng->GetNext(curr_node);
   }

  // Now read the information that's specific to each prior type
  string penalty_attribute1 = "";  // If type  = gaus, this is mean.
  string penalty_attribute2 = "";  // If type  = gaus, this is sigma.
  string penalty_attribute3 = "";    // If type = user, this is the prior hist name. Otherwise this stays as an empty string
  curr_node = xmleng->GetChild(top_node);   // Put the current node back to the top of the list
  while(curr_node) {
      string curr_node_name = xmleng->GetNodeName(curr_node);
      if(xmleng->GetNodeContent(curr_node)) {
          string content = util::TrimSpaces(xmleng->GetNodeContent(curr_node));
         if(penalty_type == "gaus") {
            if(curr_node_name == "mean" ) { penalty_attribute1 = content;  }
            if(curr_node_name == "sigma") { penalty_attribute2 = content;  }
         }
            else if(penalty_type == "user") {
            if(curr_node_name == "file_nh"  ) { penalty_attribute1 = content; }
            if(curr_node_name == "file_ih"  ) { penalty_attribute2 = content; }
            if(curr_node_name == "hist_name") { penalty_attribute3 = content; } 
         }
         else {
              LOG("UtlOscillation", pERROR) << "Could not understand the penalty type [" << penalty_type << "]";
              exit(1);
          }
      }
      curr_node = xmleng->GetNext(curr_node);
   }

   // Check that we read the correct number of attributes for each prior type

   if(penalty_type == "gaus" && (penalty_attribute1 == "" || penalty_attribute2 == "")) {
       LOG("UtlOscillation", pERROR) << "Something went wrong when setting the Gaussian penalty named [" << param_names << "]";
       exit(1);
   }
   else if(penalty_type == "user" && (penalty_attribute1 == "" || penalty_attribute2 == "" || penalty_attribute3 == "")) {
      LOG("UtlOscillation", pERROR) << "Something went wrong when setting the user penalty named [" << param_names << "]";
      exit(1);
   }
   // Get the parameter(s) that we'll apply this penalty to
   const util::NameRegistry * nr = util::NameRegistry::Instance();
   std::vector<std::string> tokens = util::Tokens(param_names, ",");
   std::vector<conv::PhysParamId_t> params;
   for(std::string param_name : tokens) {
       param_name = util::RemoveAllWhitespace(param_name);
       conv::PhysParamId_t p = nr->PhysParamFromString(param_name);
       if(p == conv::kPhypUndefined) {
           LOG("UtlOscillation", pERROR) << "Could not parse parameter [" << param_name << "]";
           exit(1);
       }
       params.push_back(p);
   }

   // Get the mixing/dmsq/phase scheme(s) for this prior (1 scheme per parameter)
   std::vector<std::string> scheme_tokens = util::Tokens(opt, ",");
   std::vector<std::string> schemes;
   for(std::string scheme_name : scheme_tokens) {
       scheme_name = util::RemoveAllWhitespace(scheme_name);
       schemes.push_back(scheme_name);
   }
   if(schemes.size() != params.size()) {
      LOG("UtlOscillation", pERROR) << "The prior named [" << param_names
                                    << "] has a mismatch in the number of schemes and parameters defined";
      exit(1);
   }

   if(schemes.size() != params.size()) {
       LOG("UtlOscillation", pERROR) << "The penalty named [" << param_names
                                     << "] has a mismatch in the number of schemes and parameters defined";
       exit(1);
   }
   if((penalty_type == "gaus") && params.size() != 1) {
      LOG("UtlOscillation", pERROR) << "The number of parameters specified for Gaussian penalty term named [" << param_names << "] was != 1";
      exit(1);
   }
   else if(!(params.size() == 1 || params.size() == 2)){
      LOG("UtlOscillation", pERROR) << "The number of parameters specified for user penalty named [" << param_names << "] was not 1 or 2";
      exit(1);
   }
   //!! so far penalty term is supported for only one variable. So only the first entry of the scheme vector
   //UWAGA: test version to handle two penalty types 
   PenaltyTerm pval;
   if (penalty_type == "gaus"){
      double mean  = atof(penalty_attribute1.c_str());
      double sigma = atof(penalty_attribute2.c_str());
      pval.type = penalty_type;
      pval.mean = mean;
      pval.sigma = sigma;
      pval.scheme = schemes[0];
      pval.param_id_0 = params[0];
   }
   else {

      std::string penalty_hist_file_name = "";
      if(Opt::Instance()->gMassHierarchyAssumed == util::HierarchyFromString("normal")) {
         penalty_hist_file_name = penalty_attribute1;
      }
      else {
         penalty_hist_file_name = penalty_attribute2;
      }
      TString base_dir = Env::Instance()->TopLevelDataDir().c_str();
      TFile* penalty_root_file = new TFile((base_dir + "/" + penalty_hist_file_name.c_str()).Data(), "read");
      if(!penalty_root_file->IsOpen()) {
         LOG("UtlOscillation", pERROR) << "Could not find penalty hist file [" << penalty_hist_file_name << "]";
         exit(1);
      }

      if(params.size() == 1) {
         pval.param_id_1 = conv::kPhypUndefined;
         TGraph* penalty_graph = (TGraph*) penalty_root_file->Get(penalty_attribute3.c_str());
         if(!penalty_graph) {
            LOG("UtlOscillation", pERROR) << "Could not find 1D penalty graph [" << penalty_attribute3 << "] in file ["
                                          << penalty_hist_file_name << "]";
            exit(1);
         }
         pval.param_id_0 = params[0];
         pval.type = "user";
         pval.penalty_graph= penalty_graph;
         pval.scheme = schemes[0];
         LOG("UtlOscillation", pNOTICE) << "Setting 1D graph penalty for param " << param_names
                                        << ": file = " << penalty_hist_file_name
                                        << ", using scheme: " << schemes[0];
      }
      else {
         // Set the axis titles to the parameter names they correspond to, so we know which is which when we come to throw from the hist
         TGraph2D* penalty_graph = (TGraph2D*) penalty_root_file->Get(penalty_attribute3.c_str());
         if(!penalty_graph) {
            LOG("UtlOscillation", pERROR) << "Could not find 2D penalty graph [" << penalty_attribute3 << "] in file ["
                                          << penalty_hist_file_name << "]";
            exit(1);
         }

         penalty_graph->GetXaxis()->SetTitle(nr->ParamAsStringVerySimple(params[0]).c_str());
         penalty_graph->GetYaxis()->SetTitle(nr->ParamAsStringVerySimple(params[1]).c_str());
         penalty_graph->SetDirectory(0);
         pval.param_id_0 = params[0];
         pval.param_id_1 = params[1];
         pval.type = "user";
         pval.penalty_graph2D = penalty_graph;
         pval.scheme = schemes[0];
         pval.scheme_par1 = schemes[1];
         LOG("UtlOscillation", pNOTICE) << "Setting 2D graph penalty for params " << param_names
                                        << ": file = " << penalty_hist_file_name
                                        << "; Associating x-axis with " << nr->ParamAsStringVerySimple(params[0])
                                        << " (scheme = " << schemes[0] << ")"
                                        << "; Associating y-axis with " << nr->ParamAsStringVerySimple(params[1])
                                        << " (scheme = " << schemes[1] << ")";
      }
      penalty_root_file->Close();
      delete penalty_root_file;
   }

/*
   double mean  = atof(penalty_attribute1.c_str());
   double sigma = atof(penalty_attribute2.c_str());
   PenaltyTerm pval = {mean,sigma,schemes[0],param_names};
   LOG("UtlOscillation", pNOTICE) << "Setting Gaussian penalty for param " << param_names
                                       << ": mean = " << mean << ", sigma = " << sigma;
                                       */
   return pval;
}


//________________________________________________________________________________________
void IncreaseFreeParameterVal(FitParamList& fpl, std::string param, double init_val, int bin, double step)
{
  valor::conv::PhysParamId_t p = util::NameRegistry::Instance()->PhysParamFromString(param.c_str());
  if(p != conv::kPhypUndefined){
   double free_value = init_val + bin*step;
   fpl.Update(p, free_value);
    
   LOG("UtlOscillation", pDEBUG) << "Increasing free paramter " << param << " value " << free_value
                                 << " for the bin " << bin;
  }

   return;
}

//________________________________________________________________________________________
void ForceFreeParameterVal(FitParamList& fpl, std::string param, double force_val)
{
  valor::conv::PhysParamId_t p = util::NameRegistry::Instance()->PhysParamFromString(param.c_str());
  if(p != conv::kPhypUndefined){
    fpl.Update(p, force_val);

    LOG("UtlOscillation", pDEBUG) << "Forcing free parameter " << param << " value to " << force_val;
  }

  return;
}

//________________________________________________________________________________________
}
}
}

