//____________________________________________________________________________ 
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.


ChangeLog:

*/ 
//____________________________________________________________________________

#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include <iostream>
#include <fstream>
#include <numeric>

#include "Utilities/UtlSystem.h"

#include "Utilities/VALORArray.h"
#include "Utilities/VALORArrayF.h"

#ifdef __VALOR_FMWK_GSL_ENABLED__
#include "gsl/gsl_vector.h"
#include "gsl/gsl_blas.h"
#endif

// Compression function prototypes. Declared in RZip.h for ROOT versions above 5.34.21, but we can't rely on that.
extern "C" void R__zip(int cxlevel, int *srcsize, char *src, int *tgtsize, char *tgt, int *irep);
extern "C" void R__unzip(int *srcsize, unsigned char *src, int *tgtsize, unsigned char *tgt, int *irep);

using namespace valor;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;

ClassImp(VALORArrayF);

//#define DEBUG_VA_ADDRESS
//#define VALORARRAY_USE_GSL

//________________________________________________________________________________________
VALORArrayF::VALORArrayF() :
  TNamed(),
  fNDim(0),
  fIncrement(NULL),
  fArray(NULL),
  fCompressionLevel(conv::kDefaultCompressionLevel)
{
}
//________________________________________________________________________________________

VALORArrayF::VALORArrayF(const vector<int> dimensions) :
  TNamed()
{
  this->Construct(dimensions.size(), &dimensions[0]);
}
//________________________________________________________________________________________
VALORArrayF::VALORArrayF(int nd, const int * dimensions) :
  TNamed()
{
  this->Construct(nd,dimensions);
}
//________________________________________________________________________________________
VALORArrayF::VALORArrayF(std::string file_to_read) :
  TNamed()
{
  this->ReadFile(file_to_read);
}
//________________________________________________________________________________________
VALORArrayF::VALORArrayF(int dim1, int dim2, int dim3, int dim4, int dim5, int dim6) :
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
void VALORArrayF::Construct(int nd, const int * dimensions)
{
  fCompressionLevel = conv::kDefaultCompressionLevel;
  assert(nd > 0 && nd <= 6);
  fNDim = nd;
  fIncrement  = new int[nd];
  for (int i = 0 ; i < 6 ; i++) {
    fDimensions[i] = (i<nd ? dimensions[i] : 1);
    fNames[i] = "";
  }
  fSize = 1;
  for (int i = fNDim-1 ; i >= 0 ; i--) {
    fIncrement[i] = fSize;
    fSize *= dimensions[i];
  }
  fArray = new float[fSize];
  
  this->SetAll(0.);
}
//________________________________________________________________________________________
void VALORArrayF::ConstructT(int nd, const Int_t * dimensions)
{
  fCompressionLevel = conv::kDefaultCompressionLevel;
  assert(nd > 0 && nd <= 6);
  fNDim = nd;
  fIncrement  = new int[nd];
  for (int i = 0 ; i < 6 ; i++) {
    fDimensions[i] = (i<nd ? dimensions[i] : 1);
    fNames[i] = "";
  }
  fSize = 1;
  for (int i = fNDim-1 ; i >= 0 ; i--) {
    fIncrement[i] = fSize;
    fSize *= dimensions[i];
  }
  fArray = new float[fSize];
  
  this->SetAll(0.);
}
//________________________________________________________________________________________
VALORArrayF::VALORArrayF(const VALORArrayF & arr) :
TNamed(arr),
fNDim(arr.fNDim),
fSize(arr.fSize)
{
  fIncrement  = new int[fNDim];
  fArray      = new float[fSize];
  
  memcpy(fDimensions , arr.fDimensions , 6*sizeof(int));
  memcpy(fIncrement  , arr.fIncrement  , fNDim*sizeof(int));
  memcpy(fArray      , arr.fArray      , fSize*sizeof(float));
  
  for (int i = 0 ; i < 6; i++) {
    fNames[i] = arr.fNames[i];
  }
}
//________________________________________________________________________________________
void VALORArrayF::CopyFrom(const VALORArrayF & arr)
{
  this->Cleanup();
  arr.Copy(*this);
  this->Construct(arr.GetNDim(),arr.GetDimensions());
  
  memcpy(fDimensions , arr.fDimensions , 6*sizeof(Int_t));
  memcpy(fIncrement  , arr.fIncrement  , fNDim*sizeof(int));
  memcpy(fArray      , arr.fArray      , fSize*sizeof(float));
  
  for (int i = 0 ; i < 6; i++) {
    fNames[i] = arr.fNames[i];
  }
}
//________________________________________________________________________________________
VALORArrayF::VALORArrayF(const VALORArray & arr) :
TNamed(arr)
{
  this->Construct(arr.GetNDim(),arr.GetDimensions());
  
  const double * x = arr.GetArray();
  for (int i = 0 ; i < fSize ; i++) {
    fArray[i] = (float) x[i];
  }
  for (int i = 0 ; i < fNDim; i++) {
    fNames[i] = arr.GetDimensionName(i);
  }
}
//________________________________________________________________________________________
void VALORArrayF::Cleanup()
{
  delete[] fIncrement;
  delete[] fArray;
  
  for (int i = 0 ; i < 6 ; i++) {
    fDimensions[i] = 1;
    fNames[i] = "";
  }
}
//________________________________________________________________________________________
VALORArrayF::~VALORArrayF()
{
  this->Cleanup();
}
//________________________________________________________________________________________
int VALORArrayF::GetSize(int dim) const
{
  if (dim<0) return fSize;
  return fDimensions[dim];
}
//________________________________________________________________________________________
int VALORArrayF::Address(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
{
  int indices[6] = {ind1,ind2,ind3,ind4,ind5,ind6};

#ifdef DEBUG_VA_ADDRESS
  cerr<<this->GetName()<<"\t"<<fNDim<<endl;
  for (int i = 0 ; i < fNDim; i++) {
    cerr<<indices[i]<<"\t"<<fDimensions[i]<<endl;
  }
  for (int i = 0 ; i < fNDim; i++) {
    assert(indices[i]>=0);
  }
  for (int i = fNDim ; i < 6 ; i++) {
    assert(indices[i]==-1);
  }
#endif
  
  int address = 0;
  for (int i = 0 ; i < fNDim; i++) {
    assert(indices[i]>=0); //Check correct number of dimensions specified
    assert(indices[i] < fDimensions[i]); //Check array bounds in range.
    address += indices[i] * fIncrement[i];
  }
  return address;
}
//________________________________________________________________________________________
void VALORArrayF::InverseAddress(int address, int * return_indices) const
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
float & VALORArrayF::GetEntryRef(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) 
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  return fArray[address];
}
//________________________________________________________________________________________
const float & VALORArrayF::GetEntryRef(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  return fArray[address];
}
//________________________________________________________________________________________
float VALORArrayF::GetEntry(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  return fArray[address];
}
//________________________________________________________________________________________
void VALORArrayF::SetEntry(float value, int ind1, int ind2, int ind3, int ind4, int ind5, int ind6)
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  fArray[address] = value;
  return;
}
//________________________________________________________________________________________
void VALORArrayF::AddToEntry(float value, int ind1, int ind2, int ind3, int ind4, int ind5, int ind6)
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  fArray[address] += value;
  return;
}
//________________________________________________________________________________________
int VALORArrayF::GetNDim() const
{
  return fNDim;
}
//________________________________________________________________________________________
const float * VALORArrayF::GetArray(void) const
{
  return fArray;
}
//________________________________________________________________________________________
float * VALORArrayF::GetArray(void)
{
  return fArray;
}
//________________________________________________________________________________________
const int * VALORArrayF::GetDimensions(void) const
{
  return fDimensions;
}
//________________________________________________________________________________________
void VALORArrayF::SetAll(float value)
{
  for (int ii = 0 ; ii < fSize ; ii++) {
    fArray[ii] = value;
  }
}
//________________________________________________________________________________________
void VALORArrayF::SetDimensionName(int dimension, const std::string & name)
{
  fNames[dimension] = name;
}
//________________________________________________________________________________________
std::string VALORArrayF::GetDimensionName(int dimension) const
{
  return fNames[dimension];
}
//________________________________________________________________________________________
VALORArrayF VALORArrayF::GetSlice(int dimension, int min_bin, int max_bin) const
{
  if (fNDim < 2) return VALORArrayF();
  // Find out dimensions of new array
  int new_ndim = fNDim-1;
  int new_dims[new_ndim];
  int new_dim_id[6] = {-1};
  int i_new_dim = 0;
  for (int idim = 0 ; idim < fNDim; idim++) {
    if (idim==dimension) {
      continue;
    }
    new_dim_id[i_new_dim] = idim;
    new_dims[i_new_dim] = fDimensions[idim];
    i_new_dim++;
  }
  
  VALORArrayF slice(new_ndim,new_dims); // make new array
  
  // copy dimension names
  i_new_dim = 0;
  for (int idim = 0 ; idim < fNDim; idim++) { 
    if (idim==dimension) {
      continue;
    }
    slice.SetDimensionName(i_new_dim, fNames[idim]);
    i_new_dim++;
  }
  
  int old_address[6] = {};
  
  // Handle default bin limits (-1 uses full range)
  min_bin = (min_bin<0 ? 0 : min_bin) ; max_bin = (max_bin<0 ? fDimensions[dimension]-1 : max_bin) ;
    
  // Copy dimension contents.
  slice.SetAll(0);
  for (int i = 0 ; i < fSize ; i++) {
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
float VALORArrayF::Integral() const
{
  if (fNDim) return std::accumulate(fArray,fArray+fSize, 0.);
  return 0;
}
//________________________________________________________________________________________
float VALORArrayF::Integral(int min_dim1 , int max_dim1, int min_dim2, int max_dim2, 
                            int min_dim3 , int max_dim3, int min_dim4, int max_dim4, 
                            int min_dim5 , int max_dim5, int min_dim6, int max_dim6 ) const
{
  float integral = 0;
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
void VALORArrayF::Scale(float factor) {
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
void VALORArrayF::ScaleBins(float factor, int ibin1, int ibin2, int ibin3,
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
void VALORArrayF::ScaleBinRange(float factor,
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
void VALORArrayF::ReadFile(std::string file_to_read) {
  this->Cleanup();
  ifstream is(file_to_read.c_str(),ios::binary|ios::in);
  assert(is);
  
#ifndef USE_UNCOMPRESSED_VARRAY_FORMAT
  VALORArrayHeader header;
  int result = header.Read(is);
  
  if (result) {
    std::cerr<<"VALORArray/ReadFile >> Bad header from file "<<file_to_read<<" - giving up."<<std::endl;
    is.close();
    util::Die(1); return;
  }
  this->ConstructT(header.GetNDimensions(),header.GetDimensions());
  
  int compressed_data_size   = header.GetCompressedSize();
  //~ int compressed_buffer_size = sizeof(float) * (fSize+2);
  int out_data_size          = sizeof(float) * (fSize);
  unsigned char * compressed_buffer = new unsigned char[compressed_data_size];
  is.read( (char*) compressed_buffer, compressed_data_size );
  
  //~ cerr<<"Pre:  Arguments are "<<compressed_data_size<<"\t"<<(void*)compressed_buffer<<"\t"<<out_data_size<<"\t"<<(void*)fArray<<"\t"<<result<<std::endl;
  R__unzip(&compressed_data_size,compressed_buffer,&out_data_size,(unsigned char *)fArray, &result);
  //~ cerr<<"Post: Arguments are "<<compressed_data_size<<"\t"<<(void*)compressed_buffer<<"\t"<<out_data_size<<"\t"<<(void*)fArray<<"\t"<<result<<std::endl;
  
  assert(result==sizeof(float)*fSize);
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
  assert(n==(7*sizeof(int) + fSize*sizeof(float)));
  is.seekg(startn);
#endif //NDEBUG
  is.read( (char*)fArray,fSize*sizeof(float));
#endif //USE_UNCOMPRESSED_VARRAY_FORMAT
  is.close();
  return;
}
//________________________________________________________________________________________
void VALORArrayF::WriteFile(std::string file_to_write) {
  ofstream os(file_to_write.c_str(),ios::binary|ios::out);
  assert(os);
#ifndef USE_UNCOMPRESSED_VARRAY_FORMAT
  
  int nbytes_compressed=-999;
  
  int uncompressed_buffer_size = (fSize           )*sizeof(float);
  int compressed_buffer_size   = (fSize*1.0004    )*sizeof(float) + 11;
  // ^^ For possible expansion, need 5 bytes per 16kb, plus 6 - I padded it a bit more than that. http://www.zlib.net/zlib_tech.html 
  char * compressed_buffer = new char[compressed_buffer_size];
  
  //~ cerr<<"Doing compression"<<endl;
  //~ cerr<<"Pre:  Arguments are "<<fCompressionLevel<<"\t"<<uncompressed_buffer_size<<"\t"<<fArray<<"\t"<<compressed_buffer_size<<"\t"<<(void*)compressed_buffer<<std::endl;
  R__zip(fCompressionLevel, &uncompressed_buffer_size, (char*)fArray, &compressed_buffer_size, compressed_buffer, &nbytes_compressed);
  //~ cerr<<"Post: Arguments are "<<fCompressionLevel<<"\t"<<uncompressed_buffer_size<<"\t"<<fArray<<"\t"<<compressed_buffer_size<<"\t"<<(void*)compressed_buffer<<std::endl;
  
  VALORArrayHeader header(VALORArrayF::kDataType,fNDim,fDimensions,nbytes_compressed,fCompressionLevel);
  
  header.Write(os);
  os.write( (char*)compressed_buffer,nbytes_compressed);
  
  delete compressed_buffer;
#else // with USE_UNCOMPRESSED_VARRAY_FORMAT
  os.write( (char*)&fNDim      ,sizeof(int));
  os.write( (char*)fDimensions ,sizeof(Int_t)*6);
  os.write( (char*)fArray,fSize*sizeof(float) );
#endif //USE_UNCOMPRESSED_VARRAY_FORMAT
  os.close();
  return;
}
//________________________________________________________________________________________
void VALORArrayF::Resize(const std::vector<int> dimensions)
{
  this->Cleanup();
  this->Construct(dimensions.size(), &dimensions[0]);
}
//________________________________________________________________________________________
void VALORArrayF::Resize(int nd, const int * dimensions)
{
  this->Cleanup();
  this->Construct(nd,dimensions);
}
//________________________________________________________________________________________
void VALORArrayF::Resize(int dim1, int dim2, int dim3, int dim4, int dim5, int dim6)
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
void VALORArrayF::SetCompressionLevel(int level)
{
  fCompressionLevel = level;
}
//________________________________________________________________________________________
