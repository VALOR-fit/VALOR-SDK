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

#include "Utilities/VALORArray.h"

using namespace valor;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

ClassImp(VALORArray);

//________________________________________________________________________________________
VALORArray::VALORArray() :
  TNamed(),
  fNDim(0),
  fDimensions(NULL),
  fIncrement(NULL),
  fArray(NULL)
{
}
//________________________________________________________________________________________

VALORArray::VALORArray(const vector<int> dimensions) :
  TNamed()
{
  this->Construct(dimensions.size(), &dimensions[0]);
}
//________________________________________________________________________________________
VALORArray::VALORArray(int nd, const int * dimensions) :
  TNamed()
{
  this->Construct(nd,dimensions);
}
//________________________________________________________________________________________
VALORArray::VALORArray(int dim1, int dim2, int dim3, int dim4, int dim5, int dim6) :
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
void VALORArray::Construct(int nd, const int * dimensions)
{
  assert(nd > 0 && nd <= 6);
  fNDim = nd;
  fDimensions = new int[nd];
  fIncrement  = new int[nd];
  memcpy(fDimensions,dimensions,nd*sizeof(int));
  fSize = 1;
  for (int i = fNDim-1 ; i >= 0 ; i--) {
    fIncrement[i] = fSize;
    fSize *= dimensions[i];
  }
  fArray = new double[fSize];
  
  this->SetAll(0.);
}
//________________________________________________________________________________________
VALORArray::VALORArray(const VALORArray & arr) :
TNamed(),
fNDim(arr.fNDim),
fSize(arr.fSize)
{
  fDimensions = new int[fNDim];
  fIncrement  = new int[fNDim];
  fArray      = new double[fSize];
  
  memcpy(fDimensions , arr.fDimensions , fNDim*sizeof(int));
  memcpy(fIncrement  , arr.fIncrement  , fNDim*sizeof(int));
  memcpy(fArray      , arr.fArray      , fSize*sizeof(double));
}
//________________________________________________________________________________________
VALORArray::~VALORArray()
{
  delete fDimensions;
  delete fIncrement;
  delete fArray;
}
//________________________________________________________________________________________
int VALORArray::GetSize(int dim) const
{
  if (dim<0) return fSize;
  assert(dim < fNDim);
  return fDimensions[dim];
}
//________________________________________________________________________________________
int VALORArray::Address(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
{
  int indices[6] = {ind1,ind2,ind3,ind4,ind5,ind6};
  int address = 0;
  for (int i = 0 ; i < fNDim; i++) {
    assert(indices[i]>=0);
    address += indices[i] * fIncrement[i];
  }
  return address;
}
//________________________________________________________________________________________
double & VALORArray::GetEntryRef(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) 
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  return fArray[address];
}
//________________________________________________________________________________________
const double & VALORArray::GetEntryRef(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  return fArray[address];
}
//________________________________________________________________________________________
double VALORArray::GetEntry(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6) const
{
  return this->GetEntryRef(ind1, ind2, ind3, ind4, ind5, ind6);
}
//________________________________________________________________________________________
void VALORArray::SetEntry(double value, int ind1, int ind2, int ind3, int ind4, int ind5, int ind6)
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  fArray[address] = value;
  return;
}
//________________________________________________________________________________________
void VALORArray::AddToEntry(double value, int ind1, int ind2, int ind3, int ind4, int ind5, int ind6)
{
  int address = this->Address(ind1, ind2, ind3, ind4, ind5, ind6);
  fArray[address] += value;
  return;
}
//________________________________________________________________________________________
int VALORArray::GetNDim() const
{
  return fNDim;
}
//________________________________________________________________________________________
const double * VALORArray::GetArray(void) const
{
  return fArray;
}
//________________________________________________________________________________________
double * VALORArray::GetArray(void)
{
  return fArray;
}
//________________________________________________________________________________________
const int * VALORArray::GetDimensions(void) const
{
  return fDimensions;
}
//________________________________________________________________________________________
void VALORArray::SetAll(double value)
{
  for (int ii = 0 ; ii < fSize ; ii++) {
    fArray[ii] = value;
  }
}
//________________________________________________________________________________________
