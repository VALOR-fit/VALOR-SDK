#ifndef READABLECACHE_H
#define READABLECACHE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "valor/Utilities/CacheItem.h"

namespace valor
{
namespace util
{
   class ReadableCache
   {
   public:
      virtual ~ReadableCache();

      /**
         Retrieves a ReadableCache. This method expects that a ReadableCache has
         already been initialised.

         \return A ReadableCache.
      */
      static ReadableCache* Instance();

      /**
         Retrieves a ReadableCache. This method either retrieves an existing
         ReadableCache if one has already been initialised, or initialises a new
         ReadableCache and returns that.

         The toys will be accessed from files with names constructed as
         <name>_<filenum>
         where filenum is in the range [0, 1 + numToys // maxToysPerFile)

         \param name The base name of the file from which the cache will be built.
         \param numToys The total number of toys represented by the files.
         \param maxToysPerFile The maximum number of toys in any file.

         \return A ReadableCache.
      */
      static ReadableCache* Instance(std::string name, const int numToys,
            const int maxToysPerFile);

      /**
         Reads data from the cache.

         \return The data read.
      */
      CacheItem* Read();

      // Temporary function
      void Reset();

   protected:
      ReadableCache(std::string name, const int numToys,
            const int maxToysPerFile);

   private:
      static ReadableCache* _instance;
//      std::vector<std::fstream*> _streams;
      std::fstream* _hStream;
      std::fstream* _tStream;
      int _numToysPerFile;
      int _numFiles;
      CacheItem* _cache;
   };
}
}

#endif
