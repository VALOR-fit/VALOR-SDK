/*!

\class     valor::util::ToyStream

\brief     Wrapper for std::fstream that is used to read/write toys from/to binary files.
           It inserts a validation string at the beginning of toy files that is a combination of the
           class name used to read/build toys (i.e. CompoundFitModel/Data), the names of each
           submodel/data, and the option used to read/write with CompoundFitModel/Data.
           The validation string is build when constructing ToyStream and is automatically written
           to the toy files and checked against that stored in the file when reading toys.

\author    Francis Bench <f.bench \at liverpool.ac.uk
           University of Liverpool
          
\created   Nov. 19, 2019

\copyright (c) 2010-2019, The VALOR group

*/

#ifndef valor_util_toy_stream
#define valor_util_toy_stream

#include <string>
#include <fstream>
#include <ios>
#include <iostream>

// valor
#include "valor/Utilities/UtlSystem.h"
#include "valor/VArray/VALORArray.h"

using std::string;

namespace valor
{
namespace util
{
  class ToyStream {
    public:

      /***
       * Construct a ToyStream to read/write toys from/to a binary file
       *
       * @param file_name Name of the file to read/write to. If writing, the file will be overwritten.
       * @param read_or_write Set to "read" or "write".
       * @param type The std::type_info of the class used to read/write the toy. i.e. pass typeid(class)
       * @param name A name used to identify the beam/detector/sample etc. enums held by the class used to
       *             read/write the toy. e.g. CFM::CompoundName() or GSSFM::Name()
       * @param opt The read/write option passed to the class used to read/write the toy
       * @param toy_size The size of each toy in bytes. Pass e.g. CFM::StreamSize()
       */
      ToyStream(const string file_name, const string read_or_write, const std::type_info& type,
                const std::string name, const string opt, const long toy_size);

      /***
       * Closes the toy file
       */
      ~ToyStream();

      /***
       * Write a VALORArray to the toy file
       *
       * @param va A Pointer to the VALORArray which will be written
       */
      void Write(VALORArray* va);

      /***
       * Read a VALORArray from the toy file
       *
       * @param va A Pointer to the VALORArray which will store the read data
       */
      void Read(VALORArray* va);

      /***
       * Set the position of the stream to the start of a particular toy number,
       * e.g. to read/write from not the first toy
       *
       * @param toy_index The toy number
       */
      void SetPositionToToy(unsigned int toy_index);

      /***
       * Get the format used to read/write the toys from/to VALOR Arrays.
       *
       * NOTE: It's perfectly fine for this to be an empty string, in which case the
       * data or model class using ToyStream will use whatever defaults they have set.
       *
       */
      // TODO: change this behaviour, so that it can't be empty (as changing the default could silently cause problems reading toys)
      Option_t* GetVALORArrayFormat(void) const;

    private:
      void         Init                    (string file_name, string read_or_write, string validation_string, long toy_size);
      bool         FileIsEmpty             () const;
      const string MakeValidationString    (const std::type_info& type, const string compound_name) const;
      void         WriteValidationString   ();
      const string ReadValidationString    () const;
      size_t       ReadValidationStringSize() const;

      std::fstream* fStream;
      long          fToySize;
      string        fFileName;
      string        fValidationString;
      string        fVALORArrayFormat;
  };
}
}

#endif

