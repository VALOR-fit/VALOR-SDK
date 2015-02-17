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

#include "Utilities/VALORArrayI.h"

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

ClassImp(VALORArrayI);

//#define DEBUG_VA_ADDRESS
//#define VALORARRAY_USE_GSL

//________________________________________________________________________________________
VALORArrayI::VALORArrayI() :
  TNamed(),
  fNDim(0),
  fIncrement(NULL),
  fArray(NULL)
{
}
//________________________________________________________________________________________

VALORArrayI::VALORArrayI(const vector<int> dimensions) :
  TNamed()
{
  this->Construct(dimensions.size(), &dimensions[0]);
}
//________________________________________________________________________________________
VALORArrayI::VALORArrayI(int nd, const int * dimensions) :
  TNamed()
{
  this->Construct(nd,dimensions);
}
//________________________________________________________________________________________
VALORArrayI::VALORArrayI(std::string file_to_read, int nd, const int * dimensions) :
  TNamed()
{
  this->Construct(nd,dimensions);
  this->ReadFile(file_to_read);
}
//________________________________________________________________________________________
VALORArrayI::VALORArrayI(int dim1, int dim2, int dim3, int dim4, int dim5, int dim6) :
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
VALORArrayI::VALORArrayI(std::string file_to_read, int dim1, int dim2, int dim3, int dim4, int dim5, int dim6) :
  TNamed()
{
  int dimensions[6] = {dim1,dim2,dim3,dim4,dim5,dim6};
  int ndim=0;
  for (int ii = 0 ; ii < 6 ; ii++) {
    if (dimensions[ii] > 0 ) ndim+=1;
    else break;
  }
  this->Construct(ndim, dimensions);
  this->ReadFile(file_to_read);
}
//________________________________________________________________________________________
void VALORArrayI::Construct(int nd, const int * dimensions)
{
  assert(nd > 0 && nd <= 6);
  fNDim = nd;
  fIncrement  = new int[nd];
  for (int i = 0 ; i < 6 ; i++) {
    fDimensions[i] = 1;
    fNames[i] = "";
  }
  memcpy(fDimensions,dimensions,nd*sizeof(int));
  fSize = 1;
  for (int i = fNDim-1 ; i >= 0 ; i--) {
    fIncrement[i] = fSize;
    fSize *= dimensions[i];
  }
  fArray = new int[fSize];
  
  this->SetAll(0.);
}
//________________________________________________________________________________________
VALORArrayI::VALORArrayI(const VALORArrayI & arr) :
TNamed(arr),
fNDim(arr.fNDim),
fSize(arr.fSize)
{
  fIncrement  = new int[fNDim];
  fArray      = new int[fSize];
  
  memcpy(fDimensions , arr.fDimensions , 6*sizeof(int));
  memcpy(fIncrement  , arr.fIncrement  , fNDim*sizeof(int));
  memcpy(fArray      , arr.fArray      , fSize*sizeof(int));
}
//________________________________________________________________________________________
VALORArrayI::~VALORArrayI()
{
  delete[] fIncrement;
  delete[] fArray;
}
//________________________________________________________________________________________
int VALORArrayI::GetSize(int dim) const
{
  if (dim<0) return fSize;
  return fDimensions[dim];
}
//________________________________________________________________________________________
int VALORArrayI::Address(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
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
void VALORArrayI::InverseAddress(int address, int * return_indices) const
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
int & VALORArrayI::GetEntryRef(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) 
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  return fArray[address];
}
//________________________________________________________________________________________
const int & VALORArrayI::GetEntryRef(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  return fArray[address];
}
//________________________________________________________________________________________
int VALORArrayI::GetEntry(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  return fArray[address];
}
//________________________________________________________________________________________
void VALORArrayI::SetEntry(int value, int ind1, int ind2, int ind3, int ind4, int ind5, int ind6)
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  fArray[address] = value;
  return;
}
//________________________________________________________________________________________
void VALORArrayI::AddToEntry(int value, int ind1, int ind2, int ind3, int ind4, int ind5, int ind6)
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  fArray[address] += value;
  return;
}
//________________________________________________________________________________________
int VALORArrayI::GetNDim() const
{
  return fNDim;
}
//________________________________________________________________________________________
const int * VALORArrayI::GetArray(void) const
{
  return fArray;
}
//________________________________________________________________________________________
int * VALORArrayI::GetArray(void)
{
  return fArray;
}
//________________________________________________________________________________________
const int * VALORArrayI::GetDimensions(void) const
{
  return fDimensions;
}
//________________________________________________________________________________________
void VALORArrayI::SetAll(int value)
{
  for (int ii = 0 ; ii < fSize ; ii++) {
    fArray[ii] = value;
  }
}
//________________________________________________________________________________________
void VALORArrayI::SetDimensionName(int dimension, const std::string & name)
{
  assert(dimension<fNDim && dimension >= 0);
  fNames[dimension] = name;
}
//________________________________________________________________________________________
std::string VALORArrayI::GetDimensionName(int dimension) const
{
  assert(dimension<fNDim && dimension >= 0);
  return fNames[dimension];
}
//________________________________________________________________________________________
VALORArrayI VALORArrayI::GetSlice(int dimension, int min_bin, int max_bin) const
{
  if (fNDim < 2) return VALORArrayI();
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
  
  VALORArrayI slice(new_ndim,new_dims); // make new array
  
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
int VALORArrayI::Integral() const
{
  return std::accumulate(fArray,fArray+fSize, 0.);
}
//________________________________________________________________________________________
int VALORArrayI::Integral(int min_dim1 , int max_dim1, int min_dim2, int max_dim2, 
                            int min_dim3 , int max_dim3, int min_dim4, int max_dim4, 
                            int min_dim5 , int max_dim5, int min_dim6, int max_dim6 ) const
{
  int integral = 0;
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
void VALORArrayI::Scale(int factor) {
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
void VALORArrayI::ScaleBins(int factor, int ibin1, int ibin2, int ibin3,
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
void VALORArrayI::ScaleBinRange(int factor,
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
void VALORArrayI::ReadFile(std::string file_to_read) {
  ifstream is(file_to_read.c_str(),ios::binary|ios::in);
  assert(is);
#ifndef NDEBUG
  is.seekg(0,is.end);
  unsigned int n = is.tellg();
  is.seekg(0,is.beg);
  assert(n==fSize*sizeof(double));
#endif
  is.read( (char*)fArray,fSize*sizeof(int));
  is.close();
  return;
}
//________________________________________________________________________________________
void VALORArrayI::WriteFile(std::string file_to_write) {
  ofstream os(file_to_write.c_str(),ios::binary|ios::out);
  assert(os);
  os.write( (char*)fArray,fSize*sizeof(int));
  os.close();
  return;
}
//________________________________________________________________________________________
