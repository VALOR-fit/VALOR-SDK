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
   ReadSamples(config.GetString("Analysis.samples"));

   // Find parameters configuration
   config.Set("Analysis.parameters", util::XML2String(_engine, rootNode,
            "parameters"));

   // Find splines configuration
   config.Set("Analysis.splines", util::XML2String(_engine, rootNode,
            "splines"));
}

void ConfigurationHandler::ReadSamples(const std::string& filename)
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
   // Retrieve all of the asmple nodes
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

std::string ConfigurationHandler::BuildString(const std::string& prefix,
      const int id, const std::string& suffix)
{
   std::ostringstream ss;
   ss << prefix << id << suffix;
   return ss.str();
}

}
}

