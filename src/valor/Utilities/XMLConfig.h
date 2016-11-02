#ifndef XMLCONFIG_H
#define XMLCONFIG_H

#include "TXMLEngine.h"
#include <string>

namespace valor
{
namespace util
{
   class XMLConfig
   {
   public:
      /**
         Constructs an XMLConfig object using the filename for the associated
         data file.

         \param filename
            The name for the associated data file.
      */
      XMLConfig(std::string filename);
      virtual ~XMLConfig();

      /**
         Returns a TXMLEngine to construct the XML file(s).

         \return The TXMLEngine to use in constructing the XML file(s).
      */
      TXMLEngine* GetXMLEngine() { return fXML; }

      /**
         Returns a XMLDocPointer_t.

         \return The XMLDocPointer_t.
      */
      XMLDocPointer_t GetXMLDocument() { return fDoc; }

      /**
         Returns the filename.

         \return The filename of the configuration file to be stored.
      */
      std::string GetMasterFilename();

      /**
         Returns the filename for a particular sub model.

         \param n The sub model number.

         \return The filename of the configuration file to be stored.
      */
      std::string GetModelFilename(const int n);

      /**
         Returns the MC filename.

         \return The MC filename for which the configuration file is produced.
      */
      std::string GetMCFilename();

      /**
         Returns the base name (this is the filename without the type).

         \return The base name of the configuration file.
      */
      std::string GetName();

   private:
      void ParseFilename(std::string& filename);

   private:
      TXMLEngine* fXML;
      XMLDocPointer_t fDoc;
      std::string fName;
   };
}
}

#endif
