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

ConfigurationHandler::ConfigurationHandler()
{
   _engine = new TXMLEngine();
}

ConfigurationHandler::~ConfigurationHandler()
{
   delete _engine;
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
   config.Set("Analysis.samples", util::XML2String(_engine, rootNode,
            "samples"));
   ReadSampleList(config.GetString("Analysis.samples"));

   // Find parameters configuration
   config.Set("Analysis.parameters", util::XML2String(_engine, rootNode,
            "parameters"));
   ReadParameterList(config.GetString("Analysis.parameters"));

   // Find splines configuration
   config.Set("Analysis.splines", util::XML2String(_engine, rootNode,
            "splines"));
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
   }
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

   // Process the parameter groups within the parameter list
   auto groups = util::FindNodes(_engine, rootNode, "parameter_group");
   for(unsigned int i = 0; i < groups.size(); ++i)
   {
      auto group = groups[i];
      std::string name = _engine->GetAttr(group, "name");
      if(name == "oscillation")
      {
         ProcessOscillationParameters(group);
      }
      else if(name == "flux" || name == "interaction" || name == "detector")
      {
         ProcessSystematicParameters(group, name);
      }
      else
      {  // Undefined group
         LOG("ConfigurationHandler", pERROR) << "Unknown parameter group " <<
            name;
         util::Die(1);
      }
   }
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

