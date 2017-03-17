/*!

\class     valor::util::ConfigurationHandler

\brief     This class handles the processing of configuration files.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick
          
\created   Mar 16, 2017

\copyright (c) 2010-2017, The VALOR group

*/

#ifndef valor_util_configuration_handler
#define valor_util_configuration_handler

#include <string>

// Root
#include <TXMLEngine.h>

namespace valor
{
namespace util
{

class ConfigurationHandler
{
public:
   /**
      Constructor.

      This is a singleton, so hide the constructor.
   */
   ConfigurationHandler();

   /**
      Destructor.
   */
   ~ConfigurationHandler();

   /**
      Reads the top-level analysis configuration file.

      \param filename The name of the file to read.
   */
   void ReadAnalysis(const std::string& filename);

private:
   TXMLEngine* _engine;
};

}
}

#endif

