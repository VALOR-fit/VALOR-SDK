#ifndef READABLECACHE_H
#define READABLECACHE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

         \param data The location into which data will be read.
         \param size The size (in bytes) of the data to be read.
      */
      void Read(char* data, const std::streamsize size);

      /**
         Jump to a specific location in the cache.

         \param pos
            The position in the file to go to (byte offset from the start of the
            cache.
      */
      void Seek(const std::streampos pos);

      /**
         Returns the status of the cache.

         \return True if the cache status is good, false otherwise.
      */
      bool Good();

      /**
         Returns the current cache position.

         \return The current cache position.
      */
      std::streampos Position();

   protected:
      ReadableCache(std::string name, const int numToys,
            const int maxToysPerFile);

   private:
      static ReadableCache* _instance;
//      std::vector<std::fstream*> _streams;
      std::fstream* _stream;
      int _numToysPerFile;
      int _numFiles;
   };
}
}

#endif
