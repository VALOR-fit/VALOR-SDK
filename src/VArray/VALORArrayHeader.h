//____________________________________________________________________________
/*!

\class     valor::VALORArrayHeader

\brief     Simple header format for writing VALORArrays as blocks.
           No dependence on stored data type - so it should be common 
           to all VALORArray types.

\author    Steve Dennis <s.r.dennis@liverpool.ac.uk>
           University of Liverpool

\created   November 2015

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_array_header_h_
#define _valor_array_header_h_

#include <cstddef>
#include <iostream>

#include "Rtypes.h"

#include "Conventions/Build.h"

namespace valor { 

class VALORArrayHeader
{
public:
  static const std::size_t kSize = 128;
  static const unsigned int kArrayVersion = 1;
  
  typedef enum EVArrayType {
    kVATUndefined = -1,
    kVATDouble    =  0,
    kVATFloat     =  1,
    kVATInt       =  2,
  } kVArrayType;
    
  VALORArrayHeader(void);
  VALORArrayHeader(kVArrayType type, UInt_t ndims, Int_t * dimensions, UInt_t compressed_size, UInt_t compression_level);
  
  void Write(std::ostream & stream_to_write_to) const;
  int Read (std::istream & stream_to_read_from); // Returns 0 on sucess
  
  UInt_t  GetVersion() const;
  UInt_t  GetCompressionLevel() const;
  UInt_t  GetCompressedSize  () const;
  UInt_t  GetNDimensions() const;
  const Int_t * GetDimensions() const;
  
  kVArrayType GetDataType() const;
  
private:
  char fBlock[kSize];
  
  
  // Format is: 128 byte block (each line represents 4 bytes -> 32 lines)
  //   0: Format_ID_String part 1 (constant)
  //   1: Format_ID_String part 2 (constant)
  //   2: Version of format (kArrayVersion)
  //   3: Data type in Array (kVArrayType)
  //   4: Compression level of data (0-9, see TFile::SetCompressionLevel)
  //   5: Compressed size (in bytes)
  //   6: Number of dimensions
  //   7: Dimension 0 size
  //   8: Dimension 1 size
  //   9: Dimension 2 size
  //  10: Dimension 3 size
  //  11: Dimension 4 size
  //  12: Dimension 5 size
  //  13: Dimension 6 size
  //  14: Dimension 7 size (UNUSED)
  //  15: Dimension 8 size (UNUSED)
  //  16+ Unused
  
  Int_t  * fBlockAsInt;  // Reinterpreted pointer to fBlock
  UInt_t * fBlockAsUInt; // Reinterpreted pointer to fBlock
  
  static const char kIDString[]; // Magic number for file format. Defined in CXX file.
  
};

namespace conv {
  static const int kDefaultCompressionLevel = 3;
}

} //valor

#endif
