#include "valor/FitCore/PhysParams.h"

#include <vector>

// Root
#include <TSystem.h>
#include <TXMLEngine.h>

// valor
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlFile.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlXML.h"

namespace valor
{

PhysParams* PhysParams::_instance = nullptr;

using std::string;
using std::vector;
using util::UtlFile;

PhysParams::PhysParams(): _nextId{0}
{

}

PhysParams::~PhysParams()
{
   delete _instance; _instance = nullptr;
}

PhysParams* PhysParams::Instance()
{
   if(not(_instance))
   {
      _instance = new PhysParams();
   }

   return _instance;
}

void PhysParams::Read(const string& filename)
{
   string fullFilename = UtlFile::GetConfigurationFilename(filename);

   // Check that the config file exists.
   if(fullFilename.empty())
   {  // File not found
      LOG("PhysParams", pERROR) << "Config file not found: " << filename;
      util::Die(1);
   }

   // Parse the XML configuration file
   TXMLEngine* xml = new TXMLEngine();
   XMLDocPointer_t xmlDoc = xml->ParseFile(fullFilename.c_str());
   if(xmlDoc == 0)
   {
      LOG("PhysParams", pERROR) << "Cannot parse XML file [" << fullFilename <<
         "]";
      delete xml;
      util::Die(1);
   }

   XMLNodePointer_t rootNode = xml->DocGetRootElement(xmlDoc);

   // Get oscillation parameter node
   ProcessParameters(util::XML2String(xml, rootNode, "osc_params/param_list"));

   // Get systematic parameters
   vector<XMLNodePointer_t> sysParams = util::FindNodes(xml, rootNode,
         "rule");
   for(auto node : sysParams)
   {
      ProcessParameters(util::XML2String(xml, node, "param_list"));
   }

   delete xml;
}

conv::PhysParamId_t PhysParams::GetParamId(const std::string& param) const
{
   // Check if the parameter exists
   auto item = _parameters.find(param);
   if(item != _parameters.end())
   {  // Parameter exists, return its ID
      return item->second;
   }
   else
   {  // Parameter does not exist, return undefined
      return Undefined;
   }
}

conv::PhysParamId_t PhysParams::operator[](const std::string& param) const
{
   return GetParamId(param);
}

void PhysParams::AddParameter(const string& param)
{
   // Attempt to Insert the parameter
   auto value = _parameters.insert(std::make_pair(param,
            static_cast<conv::PhysParamId_t>(_nextId)));
   ++_nextId;
   // Check to see if the value was inserted (i.e. didn't already exist)
   if(not(value.second))
   {  // The element already exists and so was not inserted
      // Attempts to insert parameters already present suggests a problem in the
      // XML specification
      LOG("PhysParams", pERROR) << "Duplicate parameter: " << param;
      util::Die(-1);
   }
}

void PhysParams::ProcessParameters(const string& parameterList)
{
   vector<string> tokens = util::Tokens(parameterList, ",");
   for(auto token : tokens)
   {
      // Want to check for empty strings here, they are possible
      string param = util::TrimSpaces(token);
      if(not(token.empty()))
      {
         AddParameter(param);
      }
   }
}

}
