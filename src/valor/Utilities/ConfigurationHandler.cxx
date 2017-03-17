#include "valor/Utilities/ConfigurationHandler.h"

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

   // Find parameters configuration
   config.Set("Analysis.parameters", util::XML2String(_engine, rootNode,
            "parameters"));

   // Find splines configuration
   config.Set("Analysis.splines", util::XML2String(_engine, rootNode,
            "splines"));
}

}
}

