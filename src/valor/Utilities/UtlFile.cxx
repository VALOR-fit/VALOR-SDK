#include "valor/Utilities/UtlFile.h"

// root
#include <TSystem.h>

// valor
#include "valor/Utilities/Env.h"

namespace valor
{
namespace util
{

std::string UtlFile::GetConfigurationFilename(const std::string& filename)
{
   return GetFilename(filename, Env::Instance()->TopLevelConfDir());
}

std::string UtlFile::GetDataFilename(const std::string& filename)
{
   return GetFilename(filename, Env::Instance()->TopLevelDataDir());
}

std::string UtlFile::GetFilename(const std::string& filename)
{
   return GetFilename(filename, Env::Instance()->TopLevelDir());
}

std::string UtlFile::GetFilename(const std::string& filename,
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
      {  // Strange return value convenction - zero means found
         resolved = temp;
      }
   }
   else
   {  // Strange return value convenction - zero means found
      resolved = temp;
   }

   delete[] temp;

   return resolved;
}

}
}
