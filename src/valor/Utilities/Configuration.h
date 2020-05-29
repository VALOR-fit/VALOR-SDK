/*!

\class     valor::util::Configuration

\brief     This class provides access to configuration information across the
           VALOR framework.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick
          
\created   Mar 14, 2017

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/

#ifndef valor_util_configuration
#define valor_util_configuration

// valor
#include "valor/Registry/Registry.h"

namespace valor
{
namespace util
{

class Configuration : public Registry
{
public:
   /**
      Retrieves the instance of the Configuration singleton.

      \return The Configuration singleton instance.
   */
   static Configuration* Instance();

   /**
      Destructor.
   */
   ~Configuration();

private:
   /**
      Constructor.

      This is a singleton, so hide the constructor.
   */
   Configuration();

private:
   /// The singleton
   static Configuration* _instance;
};

}
}

#endif

