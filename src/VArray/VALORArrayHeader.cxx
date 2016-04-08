//____________________________________________________________________________ 
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

*/ 
//____________________________________________________________________________

#include <cstring>

#include "VArray/VALORArrayHeader.h"

using namespace valor;
using std::endl;
using std::cout;
using std::cerr;

// First 8 bytes of file header (magic number)
// Inspired by PNG: https://en.wikipedia.org/wiki/Portable_Network_Graphics#File_header
const char VALORArrayHeader::kIDString[] = "\211VLR\r\n\032\n";

// Once we support C++11, be sure of type sizes:
//~ #include <type_traits>
//~ static_assert(sizeof(UInt_t)==4, "VALORArrayHeader format only works with UInt_t at 4 chars");
//~ static_assert(sizeof(VALORArrayHeader::kIDString)==9, "VALORArrayHeader ID string must be 8 characters (plus null)");

//____________________________________________________________________________
VALORArrayHeader::VALORArrayHeader(void)
 : fBlock(),
   fBlockAsInt ( ( Int_t *)fBlock ),
   fBlockAsUInt( (UInt_t *)fBlock )
{
  return;
}
//____________________________________________________________________________
VALORArrayHeader::VALORArrayHeader(kVArrayType type, UInt_t ndims, Int_t * dimensions, UInt_t compressed_size, UInt_t compression_level)
 : fBlock(),
   fBlockAsInt ( ( Int_t *)fBlock ),
   fBlockAsUInt( (UInt_t *)fBlock )
{
  UInt_t type_as_uint    = (UInt_t) type;
  UInt_t version_as_uint = (UInt_t) kArrayVersion;
  
  char * block_pointer = fBlock;
  memcpy(block_pointer , kIDString          ,8  ); block_pointer += 8; // Add the ID string
  memcpy(block_pointer , &version_as_uint   ,4  ); block_pointer += 4;
  memcpy(block_pointer , &type_as_uint      ,4  ); block_pointer += 4;
  memcpy(block_pointer , &compression_level ,4  ); block_pointer += 4;
  memcpy(block_pointer , &compressed_size   ,4  ); block_pointer += 4;
  memcpy(block_pointer , &ndims             ,4  ); block_pointer += 4;
  memcpy(block_pointer , dimensions         ,4*8); block_pointer += 4*8; // Space for 8 dims in header, not 6.
  return;
}
//____________________________________________________________________________
UInt_t VALORArrayHeader::GetVersion() const
{
  return fBlockAsUInt[2];
}
//____________________________________________________________________________
VALORArrayHeader::kVArrayType VALORArrayHeader::GetDataType() const
{
  kVArrayType t = (kVArrayType)fBlockAsUInt[3];
  return t;
}
//____________________________________________________________________________
UInt_t VALORArrayHeader::GetCompressionLevel() const
{
  return fBlockAsUInt[4];
}
//____________________________________________________________________________
UInt_t VALORArrayHeader::GetCompressedSize() const
{
  return fBlockAsUInt[5];
}
//____________________________________________________________________________
UInt_t VALORArrayHeader::GetNDimensions() const
{
  return fBlockAsUInt[6];
}
//____________________________________________________________________________
const Int_t * VALORArrayHeader::GetDimensions() const
{
  return &fBlockAsInt[7];
}
//____________________________________________________________________________
void VALORArrayHeader::Write(std::ostream & stream_to_write_to) const
{
  stream_to_write_to.write(fBlock,kSize);
}
//____________________________________________________________________________
int VALORArrayHeader::Read(std::istream & stream_to_read_from)
{
  stream_to_read_from.read(fBlock,kSize);
  if (memcmp(fBlock,kIDString,8)) { // Check the ID string is in place.
    std::cerr<<"Warning: failed to read VALORArray header."<<std::endl;
    return 1;
  }
  if (this->GetVersion()!=kArrayVersion) {
    std::cerr<<"Warning: reading VALORArray file version "<<this->GetVersion()<<" with code for version "<<kArrayVersion<<std::endl;
    return 2;
  }
  
  return 0;
}
//____________________________________________________________________________
