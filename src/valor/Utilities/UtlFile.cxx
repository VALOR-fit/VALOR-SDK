#include "valor/Utilities/UtlFile.h"

// root
#include <TSystem.h>

// valor
#include "valor/Utilities/Env.h"

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
   path, the alternative path is also searched, and if found, this file
   location is used.

   \param filename The name of the file to be resolved.
   \param altPath The alternative path to search.
   \return The expanded path to the requested file, if it exists, an empty
      string otherwise.
*/
std::string GetExpandedFilename(const std::string& filename,
      const std::string& altPath)
{
   std::string resolved{""};
   const char* temp = gSystem->ExpandPathName(filename.c_str());

   // Check to see if the file exists using the given path.
   FileStat_t dummy;
   if(gSystem->GetPathInfo(temp, dummy)) 
   {  // Strange return value convention - Non-zero means not found.
      // Look in the valor top level configuration folder
      std::ostringstream sstream;
      sstream << altPath << temp;
      delete[] temp;
      temp = gSystem->ExpandPathName(sstream.str().c_str());
      if(not(gSystem->GetPathInfo(temp, dummy)))
      {  // Strange return value convention - zero means found
         resolved = temp;
      }
   }
   else
   {  // Strange return value convention - zero means found
      resolved = temp;
   }

   delete[] temp;

   return resolved;
}

std::string GetConfigurationFilename(const std::string& filename)
{
   return GetExpandedFilename(filename, Env::Instance()->TopLevelConfDir());
}

std::string GetDataFilename(const std::string& filename)
{
   return GetExpandedFilename(filename, Env::Instance()->TopLevelDataDir());
}

std::string GetFilename(const std::string& filename)
{
   return GetExpandedFilename(filename, Env::Instance()->TopLevelDir());
}

}
}
}
