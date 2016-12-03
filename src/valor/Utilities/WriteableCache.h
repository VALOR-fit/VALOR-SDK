#ifndef WRITEABLECACHEABC_H
#define WRITEABLECACHEABC_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
         Writes data to the cache.

         \param data The data to be stored.
         \param size The size (in bytes) of the data.
      */
      void Write(const char* data, const std::streamsize size);

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
      WriteableCache(std::string name, const int numToys,
           const int maxToysPerFile);

   private:
      static WriteableCache* _instance;
//      std::vector<std::fstream*> _streams;
      std::fstream* _stream;
      int _numToysPerFile;
      int _numFiles;
   };
}
}

#endif
