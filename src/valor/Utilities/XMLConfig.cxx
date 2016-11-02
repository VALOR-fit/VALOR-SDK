#include "XMLConfig.h"
#include "TString.h"
#include "valor/Messenger/Messenger.h"

namespace valor
{
namespace util
{
   XMLConfig::XMLConfig(std::string filename):
      fXML(new TXMLEngine)
   {
      fDoc = fXML->NewDoc();
      ParseFilename(filename);
   }

   XMLConfig::~XMLConfig()
   {
      if(fXML)
      {
         fXML->FreeDoc(fDoc);
         delete fXML;
      }
   }

   std::string XMLConfig::GetMasterFilename()
   {
      return std::string(TString::Format("%s.xml", fName.c_str()).Data());
   }

   std::string XMLConfig::GetModelFilename(const int n)
   {
      return std::string(TString::Format("%s_%d.xml", fName.c_str(), n).Data());
   }

   std::string XMLConfig::GetMCFilename()
   {
      return std::string(TString::Format("%s.root", fName.c_str()).Data());
   }

   std::string XMLConfig::GetName()
   {
      return fName;
   }

   void XMLConfig::ParseFilename(std::string& name)
   {
      std::size_t preidx = name.rfind("/");
      if(preidx == std::string::npos)
      {
         // This needs to retrieve a value from the command line
         fName = std::string(TString::Format(
                  "/data/t2k/phrdqd/VALOR/softw/dune/branches/v3.0.0_marginalisation/data/"
                  "toymc/%s", name.c_str()).Data());
      }
      else
      {
         fName = name.substr(preidx + 1);
         fName = std::string(TString::Format(
                  "/data/t2k/phrdqd/VALOR/softw/dune/branches/v3.0.0_marginalisation/data/"
                  "toymc/%s", fName.c_str()).Data());
      }
      std::size_t postidx = fName.rfind(".");
      if(postidx != std::string::npos)
      {
         fName = fName.substr(0, postidx);
      }
   }
}
}

