//________________________________________________________________________________________
/*

Copyright (c) 2010-2017, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include "VALORBinning.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"

using namespace valor;
using std::vector;

//________________________________________________________________________________________
VALORBinning::VALORBinning() :
 TNamed(),
 fKineSpace(conv::kspUndefined),
 fNDimensions(0),
 fKineVarToDim(conv::kNKineVarMax,-1),
 fNBins(0)
{
  
}
//________________________________________________________________________________________
VALORBinning::VALORBinning(string name, string title) :
 TNamed(name.c_str(), title.c_str()),
 fKineSpace(conv::kspUndefined),
 fNDimensions(0),
 fKineVarToDim(conv::kNKineVarMax,-1),
 fNBins(0)
{
  
}
//________________________________________________________________________________________
VALORBinning::~VALORBinning()
{
}
//________________________________________________________________________________________
unsigned int VALORBinning::Dimension(conv::KineVar_t kv) const
{
  int dim = fKineVarToDim[kv];
  if (dim < 0 ) {
    LOG("VALORBinning",pFATAL) << "Bad KineVar: "<<util::AsString(kv) << " ("<<kv<<")"<< " in "<<util::AsString(fKineSpace);
    util::Die(-1) ; return 0;
  }
  return dim;
}
//________________________________________________________________________________________
bool VALORBinning::Contains(conv::KineVar_t kv) const
{
  return fKineVarToDim[kv]>=0;
}
//________________________________________________________________________________________
double VALORBinning::Minimum(unsigned int dimension) const
{
  return fMinimum[dimension];
}
//________________________________________________________________________________________
double VALORBinning::Minimum(conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->Minimum(dimension);
}
//________________________________________________________________________________________
double VALORBinning::Maximum(unsigned int dimension) const
{
  return fMaximum[dimension];
}
//________________________________________________________________________________________
double VALORBinning::Maximum(conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->Maximum(dimension);
}
//________________________________________________________________________________________
double VALORBinning::BinMinimum(unsigned int bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinMinimum(bin,dimension);
}
//________________________________________________________________________________________
double VALORBinning::BinMaximum(unsigned int bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinMaximum(bin,dimension);
}
//________________________________________________________________________________________
vector<double> VALORBinning::BinRange(
  unsigned int bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinRange(bin,dimension);
}
//________________________________________________________________________________________
double VALORBinning::BinWidth(unsigned int bin, unsigned int dimension) const
{
  vector<double> range = this->BinRange(bin,dimension);
  double width = range[1] - range[0];
  return width;
}
//________________________________________________________________________________________
double VALORBinning::BinWidth(unsigned int bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinWidth(bin,dimension);
}
//________________________________________________________________________________________
vector<double> VALORBinning::BinWidth(unsigned int bin) const
{
 vector<double> bwidth(fNDimensions);
  for(unsigned int idim = 0; idim < fNDimensions; idim++) {
    bwidth[idim] = this->BinWidth(bin, idim);
  }
  return bwidth;
}
//________________________________________________________________________________________
double VALORBinning::BinCentre(unsigned int bin, unsigned int dimension) const
{
  vector<double> range = this->BinRange(bin,dimension);
  double centre = 0.5 * (range[1] + range[0]);
  return centre;
}
//________________________________________________________________________________________
double VALORBinning::BinCentre(unsigned int bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinCentre(bin,dimension);
}
//________________________________________________________________________________________
vector<double> VALORBinning::BinCentre(unsigned int bin) const
{
 vector<double> centre(fNDimensions);
  for(unsigned int idim = 0; idim < fNDimensions; idim++) {
    centre[idim] = this->BinCentre(bin, idim);
  }
  return centre;
}
//________________________________________________________________________________________
conv::KineVar_t VALORBinning::KineVar(unsigned int dimension)
{
  if(dimension < fNDimensions) return fKineVars[dimension];

  return conv::kvUndefined;
}
//________________________________________________________________________________________
