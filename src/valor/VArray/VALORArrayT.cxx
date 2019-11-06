//____________________________________________________________________________ 
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

*/ 
//____________________________________________________________________________

#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include <iostream>
#include <fstream>
#include <numeric>

#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"
#include "valor/VArray/VALORArrayT.h"

#ifdef __VALOR_FMWK_GSL_ENABLED__
#include "gsl/gsl_vector.h"
#include "gsl/gsl_blas.h"
#endif

using namespace valor;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;

//#define CHECK_VA_ADDRESS
//#define VALORARRAY_USE_GSL
//#define USE_UNCOMPRESSED_VARRAY_FORMAT

// Compression function prototypes. Declared in RZip.h for ROOT versions above 5.34.21, but we can't rely on that.
extern "C" void R__zip(int cxlevel, int *srcsize, char *src, int *tgtsize, char *tgt, int *irep);
extern "C" void R__unzip(int *srcsize, unsigned char *src, int *tgtsize, unsigned char *tgt, int *irep);

//________________________________________________________________________________________
template<typename numtype> VALORArrayT<numtype>::VALORArrayT() :
  TNamed(),
  fNDim(0),
  fArray(NULL),
  fCompressionLevel(conv::kDefaultCompressionLevel)
{
}
//________________________________________________________________________________________

template<typename numtype> VALORArrayT<numtype>::VALORArrayT(const vector<int> dimensions) :
  TNamed()
{
  this->Construct(dimensions.size(), &dimensions[0]);
}
//________________________________________________________________________________________
template<typename numtype> VALORArrayT<numtype>::VALORArrayT(int nd, const int * dimensions) :
  TNamed()
{
  this->Construct(nd,dimensions);
}
//________________________________________________________________________________________
template<typename numtype> VALORArrayT<numtype>::VALORArrayT(std::string file_to_read) :
  TNamed()
{
  this->ReadFile(file_to_read);
}
//________________________________________________________________________________________
template<typename numtype> VALORArrayT<numtype>::VALORArrayT(int dim1, int dim2, int dim3, int dim4, int dim5, int dim6) :
  TNamed()
{
  int dimensions[6] = {dim1,dim2,dim3,dim4,dim5,dim6};
  int ndim=0;
  for (int ii = 0 ; ii < 6 ; ii++) {
    if (dimensions[ii] > 0 ) ndim+=1;
    else break;
  }
  this->Construct(ndim, dimensions);
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::Construct(int nd, const int * dimensions)
{
  fCompressionLevel = conv::kDefaultCompressionLevel;
  assert(nd > 0 && nd <= 6);
  fNDim = nd;
  for (int i = 0 ; i < 6 ; i++) {
    fDimensions[i] = (i<nd ? dimensions[i] : 1);
    fNames[i] = "";
    fIncrement[i] = 0;
  }
  fSize = 1;
  for (int i = fNDim-1 ; i >= 0 ; i--) {
    fIncrement[i] = fSize;
    fSize *= dimensions[i];
  }
  fArray = new numtype[fSize];
  
  this->SetAll(0.);
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::ConstructT(int nd, const Int_t * dimensions)
{
  fCompressionLevel = conv::kDefaultCompressionLevel;
  assert(nd > 0 && nd <= 6);
  fNDim = nd;
  for (int i = 0 ; i < 6 ; i++) {
    fDimensions[i] = (i<nd ? dimensions[i] : 1);
    fNames[i] = "";
    fIncrement[i] = 0;
  }
  fSize = 1;
  for (int i = fNDim-1 ; i >= 0 ; i--) {
    fIncrement[i] = fSize;
    fSize *= dimensions[i];
  }
  fArray = new numtype[fSize];
  
  this->SetAll(0.);
}
//________________________________________________________________________________________
template<typename numtype> VALORArrayT<numtype>::VALORArrayT(const VALORArrayT<numtype> & arr) :
TNamed(arr),
fNDim(arr.fNDim),
fSize(arr.fSize),
fArray(NULL)
{
  this->CopyFrom(arr);
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::CopyFrom(const VALORArrayT<numtype> & arr)
{
  this->Cleanup();
  arr.Copy(*this);
  this->Construct(arr.GetNDim(),arr.GetDimensions());
  
  assert(this->fSize>=0);
  
  memcpy(fDimensions , arr.fDimensions , 6*sizeof(Int_t));
  memcpy(fIncrement  , arr.fIncrement  , 6*sizeof(int));
  memcpy(fArray      , arr.fArray      , fSize*sizeof(numtype));
  
  for (int i = 0 ; i < 6; i++) {
    fNames[i] = arr.fNames[i];
  }
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::CopyDataOnlyFrom(const VALORArrayT<numtype> & arr)
{
  //~ assert(arr.GetDimensionSizes()==this->GetDimensionSizes());// Does this check work?
  memcpy(fArray      , arr.fArray      , fSize*sizeof(numtype));
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::Cleanup()
{
  if (fArray) {delete[] fArray; fArray = 0;}
  
  for (int i = 0 ; i < 6 ; i++) {
    fDimensions[i] = 1;
    fNames[i] = "";
  }
}
//________________________________________________________________________________________
template<typename numtype> VALORArrayT<numtype>::~VALORArrayT()
{
  this->Cleanup();
}
//________________________________________________________________________________________
template<typename numtype> int VALORArrayT<numtype>::GetSize(int dim) const
{
  if (dim<0) return fSize;
  return fDimensions[dim];
}
//________________________________________________________________________________________
template<typename numtype> int VALORArrayT<numtype>::
  Address(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
{
#ifdef CHECK_VA_ADDRESS
  int indices[6] = {ind1,ind2,ind3,ind4,ind5,ind6};
  for (int i = 0 ; i < fNDim; i++) {
    if (indices[i]<0 || indices[i]>=fDimensions[i]) {
      LOG("VALORArray",pFATAL) <<fName<<" bad index "<<indices[i]<< " for dimension "<<i<<" ("<<fNames[i]<<"), max = "<<fDimensions[i];
      util::Die(-1) ; return 0;
    }
  }
  for (int i = fNDim ; i < 6 ; i++) {
    assert(indices[i]==-1);
    assert(fIncrement[i]==0);
  }
#endif
  
  int address = ind1 * fIncrement[0] +
                ind2 * fIncrement[1] +
                ind3 * fIncrement[2] +
                ind4 * fIncrement[3] +
                ind5 * fIncrement[4] +
                ind6 * fIncrement[5];
  return address;
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  InverseAddress(int address, int * return_indices) const
{
  for (int i = 0 ; i < fNDim; i++) {
    return_indices[i] = 0;
    while (address >= fIncrement[i]) {
      address-=fIncrement[i];
      return_indices[i]+=1;
    }
  }
  return;
}
//________________________________________________________________________________________
template<typename numtype> numtype & VALORArrayT<numtype>::
  GetEntryRef(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) 
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  return fArray[address];
}
//________________________________________________________________________________________
template<typename numtype> const numtype & VALORArrayT<numtype>::
  GetEntryRef(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  return fArray[address];
}
//________________________________________________________________________________________
template<typename numtype> numtype VALORArrayT<numtype>::
  GetEntry(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  return fArray[address];
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  SetEntry(numtype value, int ind1, int ind2, int ind3, int ind4, int ind5, int ind6)
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  fArray[address] = value;
  return;
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  AddToEntry(numtype value, int ind1, int ind2, int ind3, int ind4, int ind5, int ind6)
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  fArray[address] += value;
  return;
}
//________________________________________________________________________________________
// Similar to before but with bin class
//________________________________________________________________________________________
template<typename numtype> int VALORArrayT<numtype>::
  Address(const VALORArrayBin & bin) const
{
#ifdef CHECK_VA_ADDRESS
  int indices[6] = {bin.bin1,bin.bin2,bin.bin3,bin.bin4,bin.bin5,bin.bin6};
  //~ cerr<<this->GetName()<<"\t"<<fNDim<<endl;
  for (int i = 0 ; i < fNDim; i++) {
    //~ cerr<<indices[i]<<"\t"<<fDimensions[i]<<endl;
  }
  for (int i = 0 ; i < fNDim; i++) {
    assert(indices[i]>=0);
  }
  for (int i = fNDim ; i < 6 ; i++) {
    assert(indices[i]==-1);
    assert(fIncrement[i]==0);
  }
#endif
  
  int address = bin.bin1 * fIncrement[0] +
                bin.bin2 * fIncrement[1] +
                bin.bin3 * fIncrement[2] +
                bin.bin4 * fIncrement[3] +
                bin.bin5 * fIncrement[4] +
                bin.bin6 * fIncrement[5];
  return address;
}
//________________________________________________________________________________________
template<typename numtype> VALORArrayBin VALORArrayT<numtype>::
  InverseAddress(int address) const
{
  int return_indices[6] = {-1,-1,-1,-1,-1,-1};
  for (int i = 0 ; i < fNDim; i++) {
    return_indices[i] = 0;
    while (address >= fIncrement[i]) {
      address-=fIncrement[i];
      return_indices[i]+=1;
    }
  }
  return VALORArrayBin(
    return_indices[0],
    return_indices[1],
    return_indices[2],
    return_indices[3],
    return_indices[4],
    return_indices[5]);
}
//________________________________________________________________________________________
template<typename numtype> numtype & VALORArrayT<numtype>::
  GetEntryRef(const VALORArrayBin & bin) 
{
  int address = this->Address(bin);
  return fArray[address];
}
//________________________________________________________________________________________
template<typename numtype> const numtype & VALORArrayT<numtype>::
  GetEntryRef(const VALORArrayBin & bin) const
{
  int address = this->Address(bin);
  return fArray[address];
}
//________________________________________________________________________________________
template<typename numtype> numtype VALORArrayT<numtype>::
  GetEntry(const VALORArrayBin & bin) const
{
  int address = this->Address(bin);
  return fArray[address];
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  SetEntry(numtype value, const VALORArrayBin & bin)
{
  int address = this->Address(bin);
  fArray[address] = value;
  return;
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  AddToEntry(numtype value, const VALORArrayBin & bin)
{
  int address = this->Address(bin);
  fArray[address] += value;
  return;
}
//________________________________________________________________________________________
template<typename numtype> int VALORArrayT<numtype>::
  GetNDim() const
{
  return fNDim;
}
//________________________________________________________________________________________
template<typename numtype> const numtype *  VALORArrayT<numtype>::
  GetArray(void) const
{
  return fArray;
}
//________________________________________________________________________________________
template<typename numtype> numtype * VALORArrayT<numtype>::
  GetArray(void)
{
  return fArray;
}
//________________________________________________________________________________________
template<typename numtype> const int * VALORArrayT<numtype>::
  GetDimensions(void) const
{
  return fDimensions;
}
//________________________________________________________________________________________
template<typename numtype> VALORArrayBin VALORArrayT<numtype>::
  GetDimensionSizes(void) const
{
  return VALORArrayBin(fDimensions);
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  SetAll(numtype value)
{
  for (int ii = 0 ; ii < fSize ; ii++) {
    fArray[ii] = value;
  }
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  SetDimensionName(int dimension, const std::string & name)
{
  fNames[dimension] = name;
}
//________________________________________________________________________________________
template<typename numtype> std::string VALORArrayT<numtype>::
  GetDimensionName(int dimension) const
{
  return fNames[dimension];
}
//________________________________________________________________________________________
template<typename numtype> VALORArrayT<numtype> VALORArrayT<numtype>::
  GetSlice(int dimension, int min_bin, int max_bin) const
{
  if (fNDim < 2) return VALORArrayT();
  // Find out dimensions of new array
  int new_ndim = fNDim-1;
  int new_dims[new_ndim];
  int new_dim_id[6] = {6,6,6,6,6,6}; // default index - will always have -1 in it, for unused dimensions
  int i_new_dim = 0;
  for (int idim = 0 ; idim < fNDim; idim++) {
    if (idim==dimension) {
      continue;
    }
    new_dim_id[i_new_dim] = idim;
    new_dims[i_new_dim] = fDimensions[idim];
    i_new_dim++;
  }
  
  VALORArrayT<numtype> slice(new_ndim,new_dims); // make new array
  
  // copy dimension names
  i_new_dim = 0;
  for (int idim = 0 ; idim < fNDim; idim++) { 
    if (idim==dimension) {
      continue;
    }
    slice.SetDimensionName(i_new_dim, fNames[idim]);
    i_new_dim++;
  }
  
  // Handle default bin limits (-1 uses full range)
  min_bin = (min_bin<0 ? 0 : min_bin) ; max_bin = (max_bin<0 ? fDimensions[dimension]-1 : max_bin) ;
                       
  // Copy dimension contents.
  slice.SetAll(0);
  for (int i = 0 ; i < fSize ; i++) {
    int old_address[7] = {-1,-1,-1,-1,-1,-1,-1}; // 7th address is for unused IDs
    this->InverseAddress(i, old_address);
    for (int bin = min_bin; bin <= max_bin ; bin++) {
      if (old_address[dimension]!=bin) continue;
      slice.AddToEntry(fArray[i],
                       old_address[new_dim_id[0]], old_address[new_dim_id[1]], 
                       old_address[new_dim_id[2]], old_address[new_dim_id[3]], 
                       old_address[new_dim_id[4]] );
    }
  }
  
  return slice;
}
//________________________________________________________________________________________
template<typename numtype> numtype VALORArrayT<numtype>::
  Integral() const
{
  if (fNDim) return std::accumulate(fArray,fArray+fSize, numtype(0));
  return 0;
}
//________________________________________________________________________________________
template<typename numtype> numtype VALORArrayT<numtype>::
  Integral(int min_dim1 , int max_dim1, int min_dim2, int max_dim2, 
           int min_dim3 , int max_dim3, int min_dim4, int max_dim4, 
           int min_dim5 , int max_dim5, int min_dim6, int max_dim6 ) const
{
  numtype integral = 0;
  min_dim1 = (min_dim1<0 ? 0 : min_dim1) ; max_dim1 = (max_dim1<0 ? fDimensions[0]-1 : max_dim1) ;
  min_dim2 = (min_dim2<0 ? 0 : min_dim2) ; max_dim2 = (max_dim2<0 ? fDimensions[1]-1 : max_dim2) ;
  min_dim3 = (min_dim3<0 ? 0 : min_dim3) ; max_dim3 = (max_dim3<0 ? fDimensions[2]-1 : max_dim3) ;
  min_dim4 = (min_dim4<0 ? 0 : min_dim4) ; max_dim4 = (max_dim4<0 ? fDimensions[3]-1 : max_dim4) ;
  min_dim5 = (min_dim5<0 ? 0 : min_dim5) ; max_dim5 = (max_dim5<0 ? fDimensions[4]-1 : max_dim5) ;
  min_dim6 = (min_dim6<0 ? 0 : min_dim6) ; max_dim6 = (max_dim6<0 ? fDimensions[5]-1 : max_dim6) ;
  
  for (int i1 = min_dim1 ; i1 <= max_dim1 ; i1++) {
    for (int i2 = min_dim2 ; i2 <= max_dim2 ; i2++) {
      for (int i3 = min_dim3 ; i3 <= max_dim3 ; i3++) {
        for (int i4 = min_dim4 ; i4 <= max_dim4 ; i4++) {
          for (int i5 = min_dim5 ; i5 <= max_dim5 ; i5++) {
            for (int i6 = min_dim6 ; i6 <= max_dim6 ; i6++) {
              int address = fIncrement[0]*i1+
                            fIncrement[1]*i2+
                            fIncrement[2]*i3+
                            fIncrement[3]*i4+
                            fIncrement[4]*i5+
                            fIncrement[5]*i6 ;
              integral+=fArray[address];
            }
          }
        }
      }
    }
  }
  
  return integral;
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  Scale(numtype factor) {
#if defined(__VALOR_FMWK_GSL_ENABLED__) && defined(VALORARRAY_USE_GSL)
  gsl_vector_view v = gsl_vector_view_array(fArray,fSize);
  gsl_blas_dscal(factor, (&v.vector));
#else
  for (int i = 0 ; i < fSize; i++) {
    fArray[i] *= factor;
  }
#endif
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  ScaleBins(numtype factor, int ibin1, int ibin2, int ibin3,
                            int ibin4, int ibin5, int ibin6)
{
  for (int i1 = 0 ; i1 < fDimensions[0] ; i1++) {
    if (ibin1>=0 && ibin1 !=i1 ) continue;
    for (int i2 = 0 ; i2 < fDimensions[1] ; i2++) {
      if (ibin2>=0 && ibin2 !=i2 ) continue;
      for (int i3 = 0 ; i3 < fDimensions[2] ; i3++) {
        if (ibin3>=0 && ibin3 !=i3 ) continue;
        for (int i4 = 0 ; i4 < fDimensions[3] ; i4++) {
          if (ibin4>=0 && ibin4 !=i4 ) continue;
          for (int i5 = 0 ; i5 < fDimensions[4] ; i5++) {
            if (ibin5>=0 && ibin5 !=i5 ) continue;
            for (int i6 = 0 ; i6 < fDimensions[5] ; i6++) {
              if (ibin6>=0 && ibin6 !=i6 ) continue;
              int address = fIncrement[0]*i1+
                            fIncrement[1]*i2+
                            fIncrement[2]*i3+
                            fIncrement[3]*i4+
                            fIncrement[4]*i5+
                            fIncrement[5]*i6 ;
              fArray[address]*=factor;
            }
          }
        }
      }
    }
  }
  return;
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  ScaleBinRange(numtype factor,
                int min_dim1 , int max_dim1, int min_dim2, int max_dim2, 
                int min_dim3 , int max_dim3, int min_dim4, int max_dim4, 
                int min_dim5 , int max_dim5, int min_dim6, int max_dim6 )
{
  min_dim1 = (min_dim1<0 ? 0 : min_dim1) ; max_dim1 = (max_dim1<0 ? fDimensions[0]-1 : max_dim1) ;
  min_dim2 = (min_dim2<0 ? 0 : min_dim2) ; max_dim2 = (max_dim2<0 ? fDimensions[1]-1 : max_dim2) ;
  min_dim3 = (min_dim3<0 ? 0 : min_dim3) ; max_dim3 = (max_dim3<0 ? fDimensions[2]-1 : max_dim3) ;
  min_dim4 = (min_dim4<0 ? 0 : min_dim4) ; max_dim4 = (max_dim4<0 ? fDimensions[3]-1 : max_dim4) ;
  min_dim5 = (min_dim5<0 ? 0 : min_dim5) ; max_dim5 = (max_dim5<0 ? fDimensions[4]-1 : max_dim5) ;
  min_dim6 = (min_dim6<0 ? 0 : min_dim6) ; max_dim6 = (max_dim6<0 ? fDimensions[5]-1 : max_dim6) ;
  
  for (int i1 = min_dim1 ; i1 <= max_dim1 ; i1++) {
    for (int i2 = min_dim2 ; i2 <= max_dim2 ; i2++) {
      for (int i3 = min_dim3 ; i3 <= max_dim3 ; i3++) {
        for (int i4 = min_dim4 ; i4 <= max_dim4 ; i4++) {
          for (int i5 = min_dim5 ; i5 <= max_dim5 ; i5++) {
            for (int i6 = min_dim6 ; i6 <= max_dim6 ; i6++) {
              int address = fIncrement[0]*i1+
                            fIncrement[1]*i2+
                            fIncrement[2]*i3+
                            fIncrement[3]*i4+
                            fIncrement[4]*i5+
                            fIncrement[5]*i6 ;
              fArray[address]*=factor;
            }
          }
        }
      }
    }
  }
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  ReadFile(std::string file_to_read)
{
  this->Cleanup();
  ifstream is(file_to_read.c_str(),ios::binary|ios::in);
  if (not is) {
    std::cerr<<"VALORArrayT/ReadFile >> Can't open file "<<file_to_read<<" - giving up."<<std::endl;
    util::Die(1); return;
  }
  
#ifndef USE_UNCOMPRESSED_VARRAY_FORMAT
  VALORArrayHeader header;
  int result = header.Read(is);
  
  if (result) {
    std::cerr<<"VALORArrayT/ReadFile >> Bad header from file "<<file_to_read<<" - giving up."<<std::endl;
    is.close();
    util::Die(1); return;
  }
  if (header.GetDataType() != this->GetHeaderType()) {
    std::cerr<<"VALORArrayT/ReadFile >> Bad variable type from file "<<file_to_read<<" - giving up."<<std::endl;
    is.close();
    util::Die(1); return;
  }
  this->ConstructT(header.GetNDimensions(),header.GetDimensions());
  
  int compressed_data_size   = header.GetCompressedSize();
  //~ int compressed_buffer_size = sizeof(numtype) * (fSize+2);
  int out_data_size          = sizeof(numtype) * (fSize);
  unsigned char * compressed_buffer = new unsigned char[compressed_data_size];
  is.read( (char*) compressed_buffer, compressed_data_size );
  
  //~ cerr<<"Pre:  Arguments are "<<compressed_data_size<<"\t"<<(void*)compressed_buffer<<"\t"<<out_data_size<<"\t"<<(void*)fArray<<"\t"<<result<<std::endl;
  R__unzip(&compressed_data_size,compressed_buffer,&out_data_size,(unsigned char *)fArray, &result);
  //~ cerr<<"Post: Arguments are "<<compressed_data_size<<"\t"<<(void*)compressed_buffer<<"\t"<<out_data_size<<"\t"<<(void*)fArray<<"\t"<<result<<std::endl;
  
  assert((size_t)result==sizeof(numtype)*fSize);
#else // with USE_UNCOMPRESSED_VARRAY_FORMAT
  int ndim = 0;
  int dims[6] = {};
  
  is.read( (char*)&ndim  ,sizeof(int));
  is.read( (char*)dims   ,sizeof(int)*6);
  this->Construct(ndim,dims);
  
#ifndef NDEBUG
  unsigned int startn = is.tellg();
  is.seekg(0,is.end);
  unsigned int n = is.tellg();
  is.seekg(0,is.beg);
  assert(n==(7*sizeof(int) + fSize*sizeof(numtype)));
  is.seekg(startn);
#endif //NDEBUG
  is.read( (char*)fArray,fSize*sizeof(numtype));
#endif //USE_UNCOMPRESSED_VARRAY_FORMAT
  is.close();
  return;
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  WriteFile(std::string file_to_write)
{
  ofstream os(file_to_write.c_str(),ios::binary|ios::out);
  assert(os);
#ifndef USE_UNCOMPRESSED_VARRAY_FORMAT
  
  int nbytes_compressed=-999;
  
  int uncompressed_buffer_size = (fSize           )*sizeof(numtype);
  int compressed_buffer_size   = (fSize*1.0004    )*sizeof(numtype) + 11;
  // ^^ For possible expansion, need 5 bytes per 16kb, plus 6 - I padded it a bit more than that. http://www.zlib.net/zlib_tech.html 
  char * compressed_buffer = new char[compressed_buffer_size];
  
  //~ cerr<<"Doing compression"<<endl;
  //~ cerr<<"Pre:  Arguments are "<<fCompressionLevel<<"\t"<<uncompressed_buffer_size<<"\t"<<fArray<<"\t"<<compressed_buffer_size<<"\t"<<(void*)compressed_buffer<<std::endl;
  R__zip(fCompressionLevel, &uncompressed_buffer_size, (char*)fArray, &compressed_buffer_size, compressed_buffer, &nbytes_compressed);
  //~ cerr<<"Post: Arguments are "<<fCompressionLevel<<"\t"<<uncompressed_buffer_size<<"\t"<<fArray<<"\t"<<compressed_buffer_size<<"\t"<<(void*)compressed_buffer<<std::endl;
  
  VALORArrayHeader header(this->GetHeaderType(),fNDim,fDimensions,nbytes_compressed,fCompressionLevel);
  
  header.Write(os);
  os.write( (char*)compressed_buffer,nbytes_compressed);
  
  delete[] compressed_buffer;
#else // with USE_UNCOMPRESSED_VARRAY_FORMAT
  os.write( (char*)&fNDim      ,sizeof(int));
  os.write( (char*)fDimensions ,sizeof(Int_t)*6);
  os.write( (char*)fArray,fSize*sizeof(numtype) );
#endif //USE_UNCOMPRESSED_VARRAY_FORMAT
  os.close();
  return;
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
   ReadStream(std::ifstream& stream)
{
   int n_dim = 0;
   int dimensions[6] = {0};
   stream.read((char*)&n_dim, sizeof(int));
   stream.read((char*)dimensions, 6 * sizeof(int));
   if(fArray) { delete fArray; }    // Delete any existing stored memory before making a new array
   this->Construct(n_dim, dimensions);

   LOG("VALORArrayT", pDEBUG) << "Reading " << this->GetName() << " " <<
      fDimensions[0] << " " << fDimensions[1] << " (" << fSize << ")";
   stream.read((char*)fArray, fSize * sizeof(numtype));
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
   WriteStream(std::ofstream& stream)
{
   LOG("VALORArrayT", pDEBUG) << "Writing " << this->GetName() << " " <<
      fDimensions[0] << " " << fDimensions[1] << "( " << fSize << ")";
   stream.write((char*)&fNDim, sizeof(int));
   stream.write((char*)fDimensions, 6 * sizeof(int));
   stream.write((char*)fArray, fSize * sizeof(numtype));
}
//________________________________________________________________________________________
template<typename numtype> long VALORArrayT<numtype>::
  StreamSize()
{
  return 7 * sizeof(int) + fSize * sizeof(numtype);
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  Resize(const std::vector<int> dimensions)
{
  this->Cleanup();
  this->Construct(dimensions.size(), &dimensions[0]);
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  Resize(int nd, const int * dimensions)
{
  this->Cleanup();
  this->Construct(nd,dimensions);
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  Resize(int dim1, int dim2, int dim3, int dim4, int dim5, int dim6)
{
  this->Cleanup();
  int dimensions[6] = {dim1,dim2,dim3,dim4,dim5,dim6};
  int ndim=0;
  for (int ii = 0 ; ii < 6 ; ii++) {
    if (dimensions[ii] > 0 ) ndim+=1;
    else break;
  }
  this->Construct(ndim, dimensions);
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  SetCompressionLevel(int level)
{
  fCompressionLevel = level;
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  PrintEntries(std::ostream & stream)
{
  for (int i1 = 0 ; i1 < fDimensions[0] ; i1++) {
    stream<<"{ ";
    for (int i2 = 0 ; i2 < fDimensions[1] ; i2++) {
      if (fDimensions[2]>1){ stream<<" { ";}
      for (int i3 = 0 ; i3 < fDimensions[2] ; i3++) {
        if (fDimensions[3]>1){ stream<<" { ";}
        for (int i4 = 0 ; i4 < fDimensions[3] ; i4++) {
          if (fDimensions[4]>1){ stream<<" { ";}
          for (int i5 = 0 ; i5 < fDimensions[4] ; i5++) {
            if (fDimensions[5]>1){ stream<<" { ";}
            for (int i6 = 0 ; i6 < fDimensions[5] ; i6++) {
              int address = fIncrement[0]*i1+
                            fIncrement[1]*i2+
                            fIncrement[2]*i3+
                            fIncrement[3]*i4+
                            fIncrement[4]*i5+
                            fIncrement[5]*i6 ;
              stream<<fArray[address]<<(fDimensions[5]-i6 > 1 ? ", ":"");
            }
            if (fDimensions[5]     >1){ stream<<" }";}
            if (fDimensions[4]-i5 > 1){ stream<<", ";}
          }
          if (fDimensions[4]     >1){ stream<<" }";}
          if (fDimensions[3]-i4 > 1){ stream<<", ";}
        }
        if (fDimensions[3]     >1){ stream<<" }";}
        if (fDimensions[2]-i3 > 1){ stream<<", ";}
      }
      if (fDimensions[2]     >1){ stream<<" }";}
      if (fDimensions[1]-i2 > 1){ stream<<", ";}
    }
    stream<<" }";
    if (fDimensions[0]-i1 > 1){ stream<<",\n";}
  }
  stream<<std::endl;
  return;
}
//________________________________________________________________________________________
template<typename numtype> void VALORArrayT<numtype>::
  PrintDimensions(std::ostream & stream)
{
  stream<<fName<<" {\n";
  for (int i = 0 ; i < fNDim ; i++) {
    stream<<"   "<<fDimensions[i]<<"\t"<<fNames[i]<<"\n";
  }
  stream<<"} with total size "<<fSize<<std::endl;
}
//________________________________________________________________________________________
namespace valor { // g++ bug requires specialisations to be in namespace
template <typename numtype> VALORArrayHeader::kVArrayType VALORArrayT<numtype>::
 GetHeaderType(void)
{
  return VALORArrayHeader::kVATUndefined;
}
//________________________________________________________________________________________
template <> VALORArrayHeader::kVArrayType VALORArrayT<double>::
 GetHeaderType(void)
{
  return VALORArrayHeader::kVATDouble;
}
//________________________________________________________________________________________
template <> VALORArrayHeader::kVArrayType VALORArrayT<float>::
 GetHeaderType(void)
{
  return VALORArrayHeader::kVATFloat;
}
//________________________________________________________________________________________
template <> VALORArrayHeader::kVArrayType VALORArrayT<int>::
 GetHeaderType(void)
{
  return VALORArrayHeader::kVATInt;
}
//________________________________________________________________________________________
template <> VALORArrayHeader::kVArrayType VALORArrayT<Long64_t>::
 GetHeaderType(void)
{
  return VALORArrayHeader::kVATInt64;
}
//________________________________________________________________________________________
template <> VALORArrayHeader::kVArrayType VALORArrayT<ULong64_t>::
 GetHeaderType(void)
{
  return VALORArrayHeader::kVATUInt64;
}
//________________________________________________________________________________________
}

//________________________________________________________________________________________
// Explicit template instantiation - must be last.
template class valor::VALORArrayT<double>;
template class valor::VALORArrayT<float>;
template class valor::VALORArrayT<int>;
template class valor::VALORArrayT<Long64_t>;
template class valor::VALORArrayT<ULong64_t>;
//________________________________________________________________________________________
