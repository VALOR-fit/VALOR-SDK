/*!

\class     valor::util::file

\brief     Gets a file from the relevant location

\author    Andy Chappell <A.Chappell \t warwick.ac.uk
           University of Warwick
          
\created   Feb 27, 2017

\copyright (c) 2010-2017, The VALOR group

*/

#ifndef valor_util_utl_file
#define valor_util_utl_file

#include <string>
#include <sstream>

// valor
#include "valor/Utilities/UtlSystem.h"

namespace valor
{
namespace util
{
namespace file
{
   /**
      Takes an input filename and returns a filename with special characters
      expanded and relative paths resolved.

      This method first expands any special characters (e.g ${HOME}) and then
      attempts to resolve the file location. If the file is not found using this
      path, the valor configruation directory path is also searched, and if
      found, this file location is used.

      \param filename The name of the file to be resolved.
      \return The expanded path to the requested file, if it exists, an empty
         string otherwise.
   */
   std::string GetConfigurationFilename(const std::string& filename);

   /**
      Takes an input filename and returns a filename with special characters
      expanded and relative paths resolved.

      This method first expands any special characters (e.g ${HOME}) and then
      attempts to resolve the file location. If the file is not found using this
      path, the valor data directory path is also searched, and if found, this
      file location is used.

      \param filename The name of the file to be resolved.
      \return The expanded path to the requested file, if it exists, an empty
         string otherwise.
   */
   std::string GetDataFilename(const std::string& filename);

   /**
      Takes an input filename and returns a filename with special characters
      expanded and relative paths resolved.

      This method first expands any special characters (e.g ${HOME}) and then
      attempts to resolve the file location. If the file is not found using this
      path, the valor directory path is also searched, and if found, this file
      location is used.

      \param filename The name of the file to be resolved.
      \return The expanded path to the requested file, if it exists, an empty
         string otherwise.
   */
   std::string GetFilename(const std::string& filename);
}
}
}

#endif

