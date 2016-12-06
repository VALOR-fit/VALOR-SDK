#ifndef WRITEABLECACHEABC_H
#define WRITEABLECACHEABC_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "valor/Utilities/CacheItem.h"

namespace valor
{
namespace util
{
   class WriteableCache
   {
   public:
      virtual ~WriteableCache();

      /**
         Retrieves a WriteableCache. This method expects that a WriteableCache has
         already been initialised.

         \return A WriteableCache.
      */
      static WriteableCache* Instance();

      /**
         Retrieves a WriteableCache. This method either retrieves an existing
         WriteableCache if one has already been initialised, or initialises a new
         WriteableCache and returns that.

         The toys will be written to files with names constructed as
         <name>_<filenum>
         where filenum is in the range [0, 1 + numToys // maxToysPerFile)

         \param name The base name of the file to the cache will be written to.
         \param numToys The total number of toys represented by the files.
         \param maxToysPerFile The maximum number of toys in any file.

         \return A WriteableCache.
      */
      static WriteableCache* Instance(std::string name, const int numToys,
            const int maxToysPerFile);

      /**
         Writes an item to the cache.

         \param item The item to be stored.
      */
      void Write(CacheItem* item);

   protected:
      WriteableCache(std::string name, const int numToys,
           const int maxToysPerFile);

   private:
      static WriteableCache* _instance;
//      std::vector<std::fstream*> _streams;
      std::fstream* _hStream;
      std::fstream* _tStream;
      int _numToysPerFile;
      int _numFiles;
   };
}
}

#endif
