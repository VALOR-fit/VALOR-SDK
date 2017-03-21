#include "valor/Utilities/ConfigurationHandler.h"

#include <sstream>

// valor
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/Configuration.h"
#include "valor/Utilities/UtlFile.h"
#include "valor/Utilities/UtlXML.h"

namespace valor
{
namespace util
{

ConfigurationHandler::ConfigurationHandler():
   _engine{new TXMLEngine()},
   _nextParamId{1}
{
}

ConfigurationHandler::~ConfigurationHandler()
{
   delete _engine;
   _nextParamId = 1;
}

void ConfigurationHandler::ReadAnalysis(const std::string& filename)
{
   string fullFilename = file::GetConfigurationFilename(filename);

   // Check that the config file exists.
   if(fullFilename.empty())
   {  // File not found
      LOG("ConfigurationHandler", pERROR) << "Config file not found: " << filename;
      util::Die(1);
   }

   // Parse the XML configuration file
   XMLDocPointer_t doc = _engine->ParseFile(fullFilename.c_str());
   if(doc == 0)
   {
      LOG("ConfigurationHandler", pERROR) << "Cannot parse XML file [" << fullFilename <<
         "]";
      util::Die(1);
   }

   XMLNodePointer_t rootNode = _engine->DocGetRootElement(doc);
   Configuration& config = (*Configuration::Instance());

   // Find samples configuration
   config.Set("analysis.samples", util::XML2String(_engine, rootNode,
            "samples"));
   ReadSampleList(config.GetString("analysis.samples"));

   // Find parameters configuration
   config.Set("analysis.parameters", util::XML2String(_engine, rootNode,
            "parameters"));
   ReadParameterList(config.GetString("analysis.parameters"));

   // Find splines configuration
   config.Set("analysis.splines", util::XML2String(_engine, rootNode,
            "splines"));
   ReadSplines(config.GetString("analysis.splines"));
}

void ConfigurationHandler::ReadSampleList(const std::string& filename)
{
   string fullFilename = file::GetConfigurationFilename(filename);

   // Check that the config file exists.
   if(fullFilename.empty())
   {  // File not found
      LOG("ConfigurationHandler", pERROR) << "Config file not found: " << filename;
      util::Die(1);
   }

   // Parse the XML configuration file
   XMLDocPointer_t doc = _engine->ParseFile(fullFilename.c_str());
   if(doc == 0)
   {
      LOG("ConfigurationHandler", pERROR) << "Cannot parse XML file [" << fullFilename <<
         "]";
      util::Die(1);
   }

   Configuration& config = (*Configuration::Instance());
   XMLNodePointer_t rootNode = _engine->DocGetRootElement(doc);
   config.Set("sample_list.name", _engine->GetAttr(rootNode, "name"));
   // Process each sample within the sample list
   ProcessSamples(rootNode);
}

void ConfigurationHandler::ProcessSamples(XMLNodePointer_t rootNode)
{
   Configuration& config = (*Configuration::Instance());
   // Retrieve all of the sample nodes
   auto samples = util::FindNodes(_engine, rootNode, "sample");
   for(unsigned int i = 0; i < samples.size(); ++i)
   {
      auto sample = samples[i];
      config.Set(BuildString("sample_list.sample_", i + 1, ".name"),
               _engine->GetAttr(sample, "name"));
      config.Set(BuildString("sample_list.sample_", i + 1, ".detector"),
               util::XML2String(_engine, sample, "detector"));
      config.Set(BuildString("sample_list.sample_", i + 1, ".beam"),
               util::XML2String(_engine, sample, "beam"));
      config.Set(BuildString("sample_list.sample_", i + 1, ".selection"),
               util::XML2String(_engine, sample, "selection"));
      config.Set(BuildString("sample_list.sample_", i + 1, ".data"),
               util::XML2String(_engine, sample, "data"));  
      config.Set(BuildString("sample_list.sample_", i + 1, ".mc"),
               util::XML2String(_engine, sample, "mc"));
      config.Set(BuildString("sample_list.sample_", i + 1, ".data_class"),
               util::XML2String(_engine, sample, "data_class"));
      config.Set(BuildString("sample_list.sample_", i + 1, ".model_class"),
               util::XML2String(_engine, sample, "model_class"));
      ReadMC(config.GetString(BuildString("sample_list.sample_", i + 1, ".mc")));
   }
}

void ConfigurationHandler::ReadMC(const std::string& filename)
{
   string fullFilename = file::GetConfigurationFilename(filename);

   // Check that the config file exists.
   if(fullFilename.empty())
   {  // File not found
      LOG("ConfigurationHandler", pERROR) << "Config file not found: " <<
         filename;
      util::Die(1);
   }

   // Parse the XML configuration file
   XMLDocPointer_t doc = _engine->ParseFile(fullFilename.c_str());
   if(doc == 0)
   {
      LOG("ConfigurationHandler", pERROR) << "Cannot parse XML file [" <<
         fullFilename << "]";
      util::Die(1);
   }

   Configuration& config = (*Configuration::Instance());
   XMLNodePointer_t rootNode = _engine->DocGetRootElement(doc);
   // Process the MC
   config.Set("mc.name", _engine->GetAttr(rootNode, "name"));
   config.Set("mc.version", _engine->GetAttr(rootNode, "version"));
   config.Set("mc.detector", _engine->GetAttr(rootNode, "detector"));
   config.Set("mc.exposure.fhc", util::XML2Dbl(_engine, rootNode,
            "data/exposure/fhc"));
   config.Set("mc.exposure.rhc", util::XML2Dbl(_engine, rootNode,
            "data/exposure/rhc"));
   config.Set("mc.fiducial_mass", util::XML2Dbl(_engine, rootNode,
            "fiducial_mass"));
   config.Set("mc.use_dst", util::XML2Bool(_engine, rootNode,
            "mc/using_dst_format"));
   config.Set("mc.dst.fhc.filename", util::XML2String(_engine, rootNode,
            "mc/dst/fhc/filename"));
   config.Set("mc.dst.fhc.objname", util::XML2String(_engine, rootNode,
            "mc/dst/fhc/objname"));
   config.Set("mc.dst.fhc.objtype", util::XML2String(_engine, rootNode,
            "mc/dst/fhc/objtype"));
   config.Set("mc.dst.rhc.filename", util::XML2String(_engine, rootNode,
            "mc/dst/rhc/filename"));
   config.Set("mc.dst.rhc.objname", util::XML2String(_engine, rootNode,
            "mc/dst/rhc/objname"));
   config.Set("mc.dst.rhc.objtype", util::XML2String(_engine, rootNode,
            "mc/dst/rhc/objtype"));
   config.Set("mc.template.filename", util::XML2String(_engine, rootNode,
            "mc/template/filename"));
   config.Set("mc.template.tag", util::XML2String(_engine, rootNode,
            "mc/template/tag"));
   config.Set("mc.interaction_mass", util::XML2Dbl(_engine, rootNode,
            "mc/interaction_mass"));
   config.Set("mc.default_binning", util::XML2String(_engine, rootNode,
            "default_binning"));
}

void ConfigurationHandler::ReadParameterList(const std::string& filename)
{
   string fullFilename = file::GetConfigurationFilename(filename);

   // Check that the config file exists.
   if(fullFilename.empty())
   {  // File not found
      LOG("ConfigurationHandler", pERROR) << "Config file not found: " <<
         filename;
      util::Die(1);
   }

   // Parse the XML configuration file
   XMLDocPointer_t doc = _engine->ParseFile(fullFilename.c_str());
   if(doc == 0)
   {
      LOG("ConfigurationHandler", pERROR) << "Cannot parse XML file [" <<
         fullFilename << "]";
      util::Die(1);
   }

   Configuration& config = (*Configuration::Instance());
   XMLNodePointer_t rootNode = _engine->DocGetRootElement(doc);
   config.Set("param_list.name", _engine->GetAttr(rootNode, "name"));

   // Process the default parameter values
   ProcessDefaults(rootNode);

   // Process the parameter groups within the parameter list
   auto groups = util::FindNodes(_engine, rootNode, "parameter_group");
   for(unsigned int i = 0; i < groups.size(); ++i)
   {
      auto group = groups[i];
      std::string name = _engine->GetAttr(group, "name");
      if(name == "oscillation")
      {
         ProcessOscillationParameters(group);
         // Read the corresponding oscillation parameters
         ReadParameters(config.GetString("param_list.osc.parameters"));
      }
      else if(name == "flux" || name == "interaction" || name == "detector")
      {
         ProcessSystematicParameters(group, name);
         // Read the corresponding oscillation parameters
         ReadParameters(config.GetString(BuildString("param_list.", name,
                     ".parameters")));
      }
      else
      {  // Undefined group
         LOG("ConfigurationHandler", pERROR) << "Unknown parameter group " <<
            name;
         util::Die(1);
      }
   }
}

void ConfigurationHandler::ReadSplines(const std::string& filename)
{
   string fullFilename = file::GetConfigurationFilename(filename);

   // Check that the config file exists.
   if(fullFilename.empty())
   {  // File not found
      LOG("ConfigurationHandler", pERROR) << "Config file not found: " <<
         filename;
      util::Die(1);
   }

   // Parse the XML configuration file
   XMLDocPointer_t doc = _engine->ParseFile(fullFilename.c_str());
   if(doc == 0)
   {
      LOG("ConfigurationHandler", pERROR) << "Cannot parse XML file [" <<
         fullFilename << "]";
      util::Die(1);
   }

   Configuration& config = (*Configuration::Instance());
   XMLNodePointer_t rootNode = _engine->DocGetRootElement(doc);
   config.Set("splines.folder", util::XML2String(_engine, rootNode, "folder"));
   config.Set("splines.pattern", util::XML2String(_engine, rootNode, "pattern"));
}

void ConfigurationHandler::ProcessOscillationParameters(XMLNodePointer_t rootNode)
{
   Configuration& config = (*Configuration::Instance());
   // Find the XML file containing the oscillation parameter details
   config.Set("param_list.osc.parameters",
         util::XML2String(_engine, rootNode, "parameters"));
   // Look for a penalty term - penalty terms are optional
   auto penalty = util::FindNode(_engine, rootNode, "penalty_term");
   if(penalty != nullptr)
   {  // Penalty term found
      config.Set("param_list.osc.penalty.type", _engine->GetAttr(penalty, "type"));
      config.Set("param_list.osc.penalty.cov.filename",
            util::XML2String(_engine, penalty, "covariance_matrix/filename"));
      config.Set("param_list.osc.penalty.cov.objname",
            util::XML2String(_engine, penalty, "covariance_matrix/objname"));
      config.Set("param_list.osc.penalty.cov.objtype",
            util::XML2String(_engine, penalty, "covariance_matrix/objtype"));
   }
}

void ConfigurationHandler::ProcessSystematicParameters(XMLNodePointer_t rootNode,
      const std::string& type)
{
   Configuration& config = (*Configuration::Instance());
   // Find the XML file containing the oscillation parameter details
   config.Set(BuildString("param_list.", type, ".parameters"),
         util::XML2String(_engine, rootNode, "parameters"));
   // Look for a penalty term - penalty terms are optional
   auto penalty = util::FindNode(_engine, rootNode, "penalty_term");
   if(penalty != nullptr)
   {  // Penalty term found
      config.Set(BuildString("param_list.", type, ".penalty.type"),
               _engine->GetAttr(penalty, "type"));
      config.Set(BuildString("param_list.", type, ".penalty.cov.filename"),
            util::XML2String(_engine, penalty, "covariance_matrix/filename"));
      config.Set(BuildString("param_list.", type, ".penalty.cov.objname"),
            util::XML2String(_engine, penalty, "covariance_matrix/objname"));
      config.Set(BuildString("param_list.", type, ".penalty.cov.objtype"),
            util::XML2String(_engine, penalty, "covariance_matrix/objtype"));
   }
   // Look for a postfit term - postfit terms are optional
   auto postfit = util::FindNode(_engine, rootNode, "postfit");
   if(postfit != nullptr)
   {  // Postfit term found
      config.Set(BuildString("param_list.", type, ".postfit.filename"),
            util::XML2String(_engine, postfit, "filename"));
      config.Set(BuildString("param_list.", type, ".postfit.objname"),
            util::XML2String(_engine, postfit, "objname"));
      config.Set(BuildString("param_list.", type, ".postfit.objtype"),
            util::XML2String(_engine, postfit, "objtype"));
   }
}

void ConfigurationHandler::ReadParameters(const std::string& filename)
{
   string fullFilename = file::GetConfigurationFilename(filename);

   // Check that the config file exists.
   if(fullFilename.empty())
   {  // File not found
      LOG("ConfigurationHandler", pERROR) << "Config file not found: " <<
         filename;
      util::Die(1);
   }

   // Parse the XML configuration file
   XMLDocPointer_t doc = _engine->ParseFile(fullFilename.c_str());
   if(doc == 0)
   {
      LOG("ConfigurationHandler", pERROR) << "Cannot parse XML file [" <<
         fullFilename << "]";
      util::Die(1);
   }

   XMLNodePointer_t rootNode = _engine->DocGetRootElement(doc);

   // Process parameters
   ProcessParameters(rootNode);
}

void ConfigurationHandler::ProcessDefaults(XMLNodePointer_t rootNode)
{
   Configuration& config = (*Configuration::Instance());
   auto defaults = util::FindNode(_engine, rootNode, "defaults");
   config.Set("param_list.defaults.bestfit",
         util::XML2Dbl(_engine, defaults, "bestfit"));
   config.Set("param_list.defaults.init",
         util::XML2Dbl(_engine, defaults, "init"));
   config.Set("param_list.defaults.minimum",
         util::XML2Dbl(_engine, defaults, "minimum"));
   config.Set("param_list.defaults.maximum",
         util::XML2Dbl(_engine, defaults, "maximum"));
   config.Set("param_list.defaults.step_size",
         util::XML2Dbl(_engine, defaults, "step_size"));
   config.Set("param_list.defaults.scaling",
         util::XML2Dbl(_engine, defaults, "scaling"));
   config.Set("param_list.defaults.fixed",
         util::XML2Bool(_engine, defaults, "fixed"));
   config.Set("param_list.defaults.nuisance",
         util::XML2Bool(_engine, defaults, "nuisance"));
   config.Set("param_list.defaults.marginalise",
         util::XML2Bool(_engine, defaults, "marginalise"));
}

void ConfigurationHandler::ProcessParameters(XMLNodePointer_t rootNode)
{
   Configuration& config = (*Configuration::Instance());

   // Process the parameter nodes within the parameters node
   auto params = util::FindNodes(_engine, rootNode, "parameter");
   for(unsigned int i = 0; i < params.size(); ++i)
   {
      auto param = params[i];
      config.Set(BuildString("param_list.param_", _nextParamId, ".name"),
            _engine->GetAttr(param, "name"));
      // All of the remaining parameters are optional, so we need to check if
      // they exist before attempting to retrieve their values, using the
      // default values instead
      if(util::NodeExists(_engine, param, "bestfit"))
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".bestfit"),
               util::XML2Dbl(_engine, param, "bestfit"));
      }
      else
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".bestfit"),
               config.GetDouble("param_list.defaults.bestfit"));
      }
      if(util::NodeExists(_engine, param, "init"))
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".init"),
               util::XML2Dbl(_engine, param, "init"));
      }
      else
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".init"),
               config.GetDouble("param_list.defaults.init"));
      }
      if(util::NodeExists(_engine, param, "minimum"))
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".minimum"),
               util::XML2Dbl(_engine, param, "minimum"));
      }
      else
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".minimum"),
               config.GetDouble("param_list.defaults.minimum"));
      }
      if(util::NodeExists(_engine, param, "maximum"))
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".maximum"),
               util::XML2Dbl(_engine, param, "maximum"));
      }
      else
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".maximum"),
               config.GetDouble("param_list.defaults.maximum"));
      }
      if(util::NodeExists(_engine, param, "step_size"))
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".step_size"),
               util::XML2Dbl(_engine, param, "step_size"));
      }
      else
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".step_size"),
               config.GetDouble("param_list.defaults.step_size"));
      }
      if(util::NodeExists(_engine, param, "scaling"))
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".scaling"),
               util::XML2Dbl(_engine, param, "scaling"));
      }
      else
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".scaling"),
               config.GetDouble("param_list.defaults.scaling"));
      }
      if(util::NodeExists(_engine, param, "fixed"))
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".fixed"),
               util::XML2Bool(_engine, param, "fixed"));
      }
      else
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".fixed"),
               config.GetBool("param_list.defaults.fixed"));
      }
      if(util::NodeExists(_engine, param, "nuisance"))
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".nuisance"),
               util::XML2Bool(_engine, param, "nuisance"));
      }
      else
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".nuisance"),
               config.GetBool("param_list.defaults.nuisance"));
      }
      if(util::NodeExists(_engine, param, "marginalise"))
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".marginalise"),
               util::XML2Bool(_engine, param, "marginalise"));
      }
      else
      {
         config.Set(BuildString("param_list.param_", _nextParamId, ".marginalise"),
               config.GetBool("param_list.defaults.marginalise"));
      }
      if(util::NodeExists(_engine, param, "errors"))
      {  // Process the errors node
         auto errors = util::FindNode(_engine, param, "errors");
         ProcessErrors(errors);
      }
      ++_nextParamId;
   }
}

void ConfigurationHandler::ProcessErrors(XMLNodePointer_t rootNode)
{
   Configuration& config = (*Configuration::Instance());

   // Process the parameter nodes within the parameters node
   if(util::NodeExists(_engine, rootNode, "migrad"))
   {  // Migrad error
      config.Set(BuildString("param_list.param_", _nextParamId,
               ".errors.migrad.error"), util::XML2Dbl(_engine, rootNode,
               "migrad/error"));
   }
   else if(util::NodeExists(_engine, rootNode, "hesse"))
   {  // Hesse error
      config.Set(BuildString("param_list.param_", _nextParamId,
               ".errors.hesse.error"), util::XML2Dbl(_engine, rootNode,
               "hesse/error"));
   }
   else if(util::NodeExists(_engine, rootNode, "minos"))
   {  // Minos error
      config.Set(BuildString("param_list.param_", _nextParamId,
               ".errors.minos.low_error"), util::XML2Dbl(_engine, rootNode,
               "minos/low_error"));
      config.Set(BuildString("param_list.param_", _nextParamId,
               ".errors.minos.high_error"), util::XML2Dbl(_engine, rootNode,
               "minos/high_error"));
   }
}

std::string ConfigurationHandler::BuildString(const std::string& prefix,
      const int id, const std::string& suffix)
{
   std::ostringstream ss;
   ss << prefix << id << suffix;
   return ss.str();
}

std::string ConfigurationHandler::BuildString(const std::string& prefix,
      const std::string& infix, const std::string& suffix)
{
   std::ostringstream ss;
   ss << prefix << infix << suffix;
   return ss.str();
}

}
}

