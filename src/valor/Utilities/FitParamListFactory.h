/*!

\class     valor::util::FitParamListFactory

\brief     This factory class constructs FitParamLists based on configuration
           files.

\author    Andy Chappell <A.Chappell \t warwick.ac.uk
           University of Warwick
          
\created   Feb 27, 2017

\copyright (c) 2010-2017, The VALOR group

*/

#ifndef valor_util_fit_param_list_factory
#define valor_util_fit_param_list_factory

#include <string>

// valor
#include "valor/FitCore/FitParamList.h"

namespace valor
{
namespace util
{

class FitParamListFactory
{
public:
   /**
      Construct a FitParamListFactory.
   */
   FitParamListFactory(){ }

   /**
      Constructs a FitParamList based on the content of the configuration file.

      This method retrieves all of the parameters specified in the configuration
      file.

      \param filename The name of the configuration file specifying the fit
         parameters.
      \return The complete list of fit parameters specified by the configuration
         file.

   */
   FitParamList* MakeParamList(const std::string& filename) const;

   /**
      Constructs a FitParamList based on the content of the configuration file.

      This method retrieves only oscillation parameters specified in the
      configuration file.

      \param filename The name of the configuration file specifying the fit
         parameters.
      \return The oscillation parameters specified by the configuration file.

   */
   FitParamList* MakeOscillationParamList(const std::string& filename) const;

   /**
      Constructs a FitParamList based on the content of the configuration file.

      This method retrieves only systematic parameters specified in the
      configuration file.

      \param filename The name of the configuration file specifying the fit
         parameters.
      \return The systematic parameters by the configuration file.

   */
   FitParamList* MakeSystematicParamList(const std::string& filename) const;
};

}
}

#endif

