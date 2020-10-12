//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include <string>

#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/ToyStream.h"

using namespace valor;

using std::string;

//______________________________________________________________________________
valor::util::ToyStream::ToyStream(const string file_name, const string read_or_write, const std::type_info& type,
                                  const std::string name, const string valor_array_format, const long toy_size)
{
  fToySize          = toy_size;
  fFileName         = file_name;

  if(valor_array_format.empty()) {
    LOG("ToyStream",pFATAL) << "Tried to setup a ToyStream object for \'" << fFileName
                            << "\', but did not specify a non-empty VALORArray format";
    util::Die(-1);
  }
  fVALORArrayFormat = valor_array_format;

  fValidationString = this->MakeValidationString(type, name);
  LOG("ToyStream",pFATAL) << "*** test: validation str = " << fValidationString;

  if(read_or_write == "read") {
    fStream = new std::fstream(file_name, std::ios::binary | std::ios::in);

    if(this->FileIsEmpty()) {
      LOG("ToyStream",pFATAL) << "Tried to read toys from file \'" << fFileName << "\', but it was empty";
      util::Die(-1);
    }

    // The current validation string should match the one stored in the toy file.
    // If it doesn't, then reading the toy file would likely fill the wrong VALORArrays
    const string read_validation_string = this->ReadValidationString();

    if(read_validation_string != fValidationString) {
      LOG("ToyStream",pFATAL) << "Tried to read toys from file \'" << fFileName
                              << "\', but the constructed validation string \n\'" << fValidationString
                              << "\'\ndid not match the stored validation string \n\'" << read_validation_string << "\'";
      util::Die(-1);
    }

    LOG("ToyStream",pINFO) << "Reading toys from file \'" << file_name << "\' with valid validation string \'"
                             << fValidationString << "\'";
  }
  else if(read_or_write == "write") {
    // Overwrites any existing content
    fStream = new std::fstream(file_name, std::ios::binary | std::ios::out | std::ios::trunc);

    if(!this->FileIsEmpty()) {
      LOG("ToyStream",pFATAL) << "Tried to write toys to file \'" << fFileName << "\', but it was NOT empty";
      util::Die(-1);
    }

    LOG("ToyStream",pINFO) << "Writing toys to file \'" << file_name << "\' with validation string \'"
                             << fValidationString << "\'";
    this->WriteValidationString();
  }
  else {
    LOG("ToyStream",pFATAL) << "ToyStream must be constructed with read_or_write set to either \'read\' or \'write\'. "
                            << "Instead got \'" << read_or_write << "\'";
    util::Die(-1);
  }

  if(!fStream->good()) {
    LOG("ToyStream",pFATAL) << "Could not open toy file \'" << fFileName << "\'";
    util::Die(-1);
  }

  // Note: At this point, the stream position should be set to the position of the start of the first toy,
  // so is ready for reading/writing
}
//______________________________________________________________________________
valor::util::ToyStream::~ToyStream()
{
  fStream->close();
  delete fStream;
}
//______________________________________________________________________________
bool valor::util::ToyStream::FileIsEmpty() const
{
  fStream->seekg(std::ios_base::beg);  // Go to the beginning of the file

  // Check if we reach the end of the file after trying to get the next character
  return fStream->peek() == std::ifstream::traits_type::eof();
}
//______________________________________________________________________________
const string valor::util::ToyStream::MakeValidationString(const std::type_info& type, const string compound_name) const
{
  return ((string) type.name()) + "__" + compound_name + "__" + fVALORArrayFormat;
}
//______________________________________________________________________________
void valor::util::ToyStream::WriteValidationString()
{
  fStream->seekg(std::ios_base::beg);  // Go to the beginning of the file

  // To properly write a string to the binary file,
  // must first write the length of the string, then the string itself
  size_t len = fValidationString.size();
  fStream->write((char*)&len, sizeof(size_t));
  fStream->write(fValidationString.c_str(), len);
}
//______________________________________________________________________________
const string valor::util::ToyStream::ReadValidationString() const
{
  string read_validation_string = "";
  string invalid = "NO VALIDATION STRING FOUND";
  fStream->seekg(std::ios_base::beg);  // Go to the beginning of the file

  // First read what the length of the string will be
  const size_t len = ReadValidationStringSize();
  if(len <= 0) { read_validation_string = invalid; }

  // Now read the validation string in via a buffer
  try {
    char* temp = new char[len+1];
    fStream->read(temp, len);
    temp[len] = '\0';
    read_validation_string = temp;
    delete [] temp;
  }
  catch(...) {
    read_validation_string = invalid;
  }

  if(!fStream->good()) { read_validation_string =  invalid; }
  return read_validation_string;
}
//______________________________________________________________________________
size_t valor::util::ToyStream::ReadValidationStringSize() const
{
  fStream->seekg(std::ios_base::beg);  // Go to the beginning of the file
  size_t len = 0;
  fStream->read((char*)&len, sizeof(size_t));

  if(!fStream->good()) { return -1; }
  return len;
}
//______________________________________________________________________________
void valor::util::ToyStream::Write(VALORArray* va)
{
  va->WriteStream(*fStream);

  // Check status of stream AFTER we try to write, as std::fstream allows write()
  // to be called even if you can't write to the file and it won't throw an error
  if(fStream->fail()) {   // Does not check end-of-file bit
    LOG("ToyStream",pFATAL) << "Could not write to toy file \'" << fFileName << "\'";
    util::Die(-1);
  }
}
//______________________________________________________________________________
void valor::util::ToyStream::Read(VALORArray* va)
{
  fStream->peek();
  if(!fStream->good()) {   // Checks end-of-file bit
    LOG("ToyStream",pFATAL) << "Could not read from toy file \'" << fFileName << "\'";
    util::Die(-1);
  }

  va->ReadStream(*fStream);

  // Check status of stream AFTER we try to read, as std::fstream allows read()
  // to be called even if you can't read from the file and it won't throw an error
  if(fStream->fail()) {   // Does not check end-of-file bit
    LOG("ToyStream",pFATAL) << "Could not read from toy file \'" << fFileName << "\'";
    util::Die(-1);
  }
}
//______________________________________________________________________________
void valor::util::ToyStream::SetPositionToToy(unsigned int toy_index)
{
  fStream->seekg(std::ios_base::beg);  // Go to the beginning of the file

  // Get the length of the validation string, which we need to skip over
  const size_t len_validation_string = ReadValidationStringSize();

  // Set the new position of the stream
  fStream->seekg(len_validation_string + (toy_index * fToySize), std::ios_base::beg);

  // Check that we're at a valid position in the file
  fStream->peek();
  if(!fStream->good()) {
    LOG("ToyStream",pFATAL) << "Tried to set the ToyStream to the position of toy #" << toy_index
                            << ", but there are not this many toys stored";
    util::Die(-1);
  }
}
//______________________________________________________________________________
Option_t* valor::util::ToyStream::GetVALORArrayFormat(void) const
{
  if(fVALORArrayFormat.empty()) {
    // Needed for checks like opt = (format) ? format : "some_string"
    LOG("ToyStream",pWARN) << "The VALORArray format string is empty, this shouldn't happen!";
    return nullptr;
  }

  return fVALORArrayFormat.c_str();
}
//______________________________________________________________________________
