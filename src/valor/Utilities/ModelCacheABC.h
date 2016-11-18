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
         Reads the file header into the cache.

         \return True if the read succeeded, false otherwise.
      */ 
      virtual bool ReadFileHeader() = 0;

      /**
         Reads a toy header into the cache.

         \return True if the read succeeded, false otherwise.
      */ 
      virtual bool ReadToyHeader() = 0;

      /**
         Reads a model header into the cache based on the most recently read toy
         information.

         \return True if the read succeeded, false otherwise.
      */ 
      virtual bool ReadModelHeader() = 0;

      /**
         Reads data into the cache based on the most recently read model
         information. That is, the data read will be that of the most recently
         processed toy and model headers.

         \return True if the read succeeded, false otherwise.
      */ 
      virtual bool ReadModelData() = 0; 

      /**
         Writes the file header.

         \return True if written succeeded, false otherwise.
      */ 
      virtual bool WriteFileHeader() = 0;

      /**
         Writes a toy header.

         \return True if the write succeeded, false otherwise.
      */ 
      virtual bool WriteToyHeader() = 0;

      /**
         Writes a model header.

         \return True if the write succeeded, false otherwise.
      */ 
      virtual bool WriteModelHeader() = 0;

      /**
         Writes a model's data.

         \return True if the write succeeded, false otherwise.
      */ 
      virtual bool WriteModelData() = 0;

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
