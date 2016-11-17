#ifndef MODELCACHEABC_H
#define MODELCACHEABC_H

#include <iostream>
#include <fstream>

namespace valor
{
namespace util
{
   class ModelCacheABC
   {
   public:
      virtual ~ModelCacheABC(){ };

      /**
         Reads the file header from the specified stream into the cache.

         \param in The input stream to read from.

         \return True if the read succeeded, false otherwise.
      */ 
      virtual bool ReadFileHeader(std::ifstream& in) = 0;

      /**
         Reads a toy header from the specified stream into the cache.

         \param in The input stream to read from.
         \param toy The toy to be read (indexed from zero).

         \return True if the read succeeded, false otherwise.
      */ 
      virtual bool ReadToyHeader(std::ifstream& in, const int toy) = 0;

      /**
         Reads a model header from the specified stream into the cache based on
         the most recently read toy information.

         \param in
            The input stream to read from.
         \param model
            The model to be read (indexed from zero relative to the current toy).

         \return True if the read succeeded, false otherwise.
      */ 
      virtual bool ReadModelHeader(std::ifstream& in, const int model) = 0;

      /**
         Reads data from the specified stream into the cache based on the most
         recently read model information. That is, the data read will be that of
         the most recently processed toy and model headers.

         \param in
            The input stream to read from.

         \return True if the read succeeded, false otherwise.
      */ 
      virtual bool ReadModelData(std::ifstream& in) = 0; 

      /**
         Writes the file header to the specified stream.

         \param out The output stream to write to.

         \return True if written succeeded, false otherwise.
      */ 
      virtual bool WriteFileHeader(std::ofstream& out) = 0;

      /**
         Writes a toy header to the specified stream.

         \param out The output stream to write to.

         \return True if the write succeeded, false otherwise.
      */ 
      virtual bool WriteToyHeader(std::ofstream& out) = 0;

      /**
         Writes a model header to the specified stream.

         \param out The output stream to write to.

         \return True if the write succeeded, false otherwise.
      */ 
      virtual bool WriteModelHeader(std::ofstream& out) = 0;

      /**
         Writes a model's data to the specified stream.

         \param out The output stream to write to.

         \return True if the write succeeded, false otherwise.
      */ 
      virtual bool WriteModelData(std::ofstream& out) = 0;

      /**
         Updates the cache to populate a new toy.

         \param numModels The number of models within this toy.
      */
      virtual void MakeToy(int numModels) = 0;

      /**
         Updates the cache to populate a new model.
      */
      virtual void MakeModel() = 0;

      /**
         Returns a string representation of the cache.

         \return A string representation of the cache.
      */
      virtual std::string ToString() = 0;

   protected:
      ModelCacheABC(){ };
   };
}
}

#endif
