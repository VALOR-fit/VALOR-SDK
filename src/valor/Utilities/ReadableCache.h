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

         \param name The name of the file from which the cache will be built.
         \param numToys The total number of toys in the file.
         \param cacheSize The maximum number of toys in the cache at any time.

         \return A ReadableCache.
      */
      static ReadableCache* Instance(std::string name, const int numToys,
            const int cacheSize);

      /**
         Reads data from the cache.

         \return The data read.
      */
      CacheItem* Read();

      // Temporary function
      void Reset();

   protected:
      ReadableCache(std::string name, const int numToys, const int cacheSize);

   private:
      static ReadableCache* _instance;
      std::fstream* _hStream;
      std::fstream* _tStream;
      int _numToys;
      int _cacheSize;
      int _firstToy;
      int _lastToy;
      int _currentToy;
      long _toySize;
      long* _offsets;
      char* _raw;
      CacheItem* _cache;
   };
}
}

#endif
