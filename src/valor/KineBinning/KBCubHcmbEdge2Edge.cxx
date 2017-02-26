//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <TSystem.h>
#include <TMath.h>

#include "valor/Utilities/UtlXML.h"
#include "valor/Messenger/Messenger.h"
#include "valor/VArray/VALORArray.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/Env.h"
#include "valor/KineBinning/KBCubHcmbEdge2Edge.h"

using namespace valor;

ClassImp(KBCubHcmbEdge2Edge)

//________________________________________________________________________________________
KBCubHcmbEdge2Edge::KBCubHcmbEdge2Edge() :
TNamed()
{
  this->Init();
}
//________________________________________________________________________________________
KBCubHcmbEdge2Edge::KBCubHcmbEdge2Edge(string name, string title) :
TNamed(name.c_str(), title.c_str())
{
  this->Init();
}
//________________________________________________________________________________________
KBCubHcmbEdge2Edge::~KBCubHcmbEdge2Edge() 
{
  this->CleanUp();
}
//________________________________________________________________________________________
bool KBCubHcmbEdge2Edge::Read(string file)
{
  this->CleanUp();
  
  file = util::CompleteConfigPath(file);
  TString ts_file = file.c_str();
  gSystem->ExpandPathName(ts_file);
  
  FileStat_t dummy;
  if (gSystem->GetPathInfo(ts_file.Data(), dummy)) {
    LOG("Binning", pERROR) << "Binning file not found: " << file;
    util::Die(1); return false;
  }

  // Parse the input binning configuration XML file
  TXMLEngine* xml = new TXMLEngine;   
  XMLDocPointer_t xmldoc = xml->ParseFile(ts_file.Data());
  if (xmldoc==0) {
    LOG("Binning",pERROR) 
        << "Can not parse input XML file [" << ts_file << "]";
    delete xml;
    util::Die(1); return false;
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);

  //
  string space = util::XML2String(xml,root_node,"space");
  fKineSpace = util::KineSpaceFromString(space);
  if(fKineSpace == conv::kspUndefined) {
    LOG("Binning", pERROR) 
      << "Undefined kinematical space: " << space;
    return false;
  }
  LOG("Binning", pNOTICE) 
    << "Kinematic space: " << util::AsString(fKineSpace);

  //
  fNDimensions = util::NDim(fKineSpace); 
  fKineVars    = util::KineVar(fKineSpace); 
  for(unsigned int i = 0; i < fNDimensions; i++) {
    conv::KineVar_t kv = fKineVars[i];
    fKineVar2Dim[kv] = i;
  }
  fFixedWidth . reserve(fNDimensions);
  fMinimum    . reserve(fNDimensions);
  fMaximum    . reserve(fNDimensions);
  fBinEdges   . reserve(fNDimensions);

  //
  string type = util::XML2String(xml,root_node,"type");
  if(type != "edge-to-edge") {
    LOG("Binning", pERROR) 
      << "Input binning scheme [" << type << "] not `edge-to-edge'";
    return false;
  }

  for(unsigned int idim = 0; idim < fNDimensions; idim++) {

    conv::KineVar_t kine_var = fKineVars[idim];    
    string kine_var_str = util::AsString(kine_var);
    const char * kine_var_ccs = kine_var_str.c_str();

    string bin_size = util::XML2String(
       xml,root_node,Form("%s_binning/bin_size",kine_var_ccs));
    unsigned int nbins = (unsigned int) 
        util::XML2Int(xml,root_node,Form("%s_binning/nbins",kine_var_ccs));

    LOG("Binning", pINFO) 
        << "Dimension: " << idim << " (variable: " 
        << kine_var_str << ") -> bin size: `" << bin_size << "'";

    //
    //
    if(bin_size == "fixed") {
      vector<double> range = util::XML2DblArray(
         xml,root_node,Form("%s_binning/range",kine_var_ccs));
      if(range.size() != 2) {
         LOG("Binning", pERROR) 
           << "No range `min,max' was specified for dimension: " << idim;
         return false;
      }
      VALORArray * arr = new VALORArray(nbins+1);
      double dx = (range[1] - range[0]) / nbins;
      double x = range[0];
      for(unsigned int i = 0; i < nbins+1; i++) {
         arr->SetEntry(x,i);
         x += dx;
      }
      fBinEdges  [idim] = arr;
      fFixedWidth[idim] = true;
      fMinimum   [idim] = range[0];
      fMaximum   [idim] = range[1];
    } 

    //
    else
    if(bin_size == "variable") {
      vector<double> bin_edges = util::XML2DblArray(
         xml,root_node,Form("%s_binning/bin_edges",kine_var_ccs));
      if(bin_edges.size() != nbins+1) {
         LOG("Binning", pERROR) 
           << bin_edges.size() << " bin edges were specified for the " 
           << nbins << " bins in dimension " << idim;
         return false;
      }
      VALORArray * arr = new VALORArray(nbins+1);
      for(unsigned int i = 0; i < nbins+1; i++) {
         arr->SetEntry(bin_edges[i],i);
      }
      fBinEdges  [idim] = arr;
      fFixedWidth[idim] = false;
      fMinimum   [idim] = bin_edges[0];
      fMaximum   [idim] = bin_edges[nbins];
    } 

    else {
      LOG("Binning", pERROR) 
         << "Bin size is neither `fixed' nor `variable'";
      return false;
    }

  }//idim

  return true;
}

KBCubHcmbEdge2Edge* KBCubHcmbEdge2Edge::Contract(conv::KineSpace_t ks) const
{
   KBCubHcmbEdge2Edge* binning = new KBCubHcmbEdge2Edge();

   // Set the new kinematic space
   binning->fKineSpace = ks;

   // Get the subset of the kinematic variables for the desired space
   binning->fKineVars = util::KineVarSubset(fKineSpace, ks);

   // The number of dimensions is equal to the number of kinematic variables in
   // the subset
   binning->fNDimensions = binning->fKineVars.size();
   // Set the mapping from kinematic variable to dimension
   for(unsigned int i = 0; i < binning->fNDimensions; ++i)
   {
      binning->fKineVar2Dim[fKineVars[i]] = i;
   }

   // Reserve space for the bin minima, maxima and edges
   binning->fMinimum.reserve(binning->fNDimensions);
   binning->fMaximum.reserve(binning->fNDimensions);
   binning->fBinEdges.reserve(binning->fNDimensions);

   // Construct new bin edges from old
   for(unsigned int d = 0; d < fNDimensions; ++d)
   {
      if(std::find(binning->fKineVars.begin(), binning->fKineVars.end(),
               fKineVars[d]) != binning->fKineVars.end())
      {  // Current dimension is for a kinematic variable in the target space
         // Add the edges for the current dimension, type, minima and maxima
         binning->fBinEdges.push_back(fBinEdges[d]);
         binning->fFixedWidth.push_back(fFixedWidth[d]);
         binning->fMinimum.push_back(fMinimum[d]);
         binning->fMaximum.push_back(fMaximum[d]);
      }
   }

   return binning;
}

//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::NBins(unsigned int dimension) const
{
  return (fBinEdges[dimension]->GetSize() - 1);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::NBins(conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->NBins(dimension);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::NBins(void) const
{
  unsigned int nbins = this->NBins(0);
  for(unsigned int idim = 1; idim < fNDimensions; idim++) {
     nbins *= this->NBins(idim);
  }
  return nbins;
}
//________________________________________________________________________________________
bool KBCubHcmbEdge2Edge::FixedWidth(unsigned int dimension) const
{
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return false;
  }
  return fFixedWidth[dimension];
}
//________________________________________________________________________________________
bool KBCubHcmbEdge2Edge::FixedWidth(conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->FixedWidth(dimension);
}
//________________________________________________________________________________________
bool KBCubHcmbEdge2Edge::VariableWidth(unsigned int dimension) const
{
  return (! this->FixedWidth(dimension));
}
//________________________________________________________________________________________
bool KBCubHcmbEdge2Edge::VariableWidth(conv::KineVar_t kv) const
{
  return (! this->FixedWidth(kv));
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::Minimum(unsigned int dimension) const
{
  return fMinimum[dimension];
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::Minimum(conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->Minimum(dimension);
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::Maximum(unsigned int dimension) const
{
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return false;
  }
  return fMaximum[dimension];
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::Maximum(conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->Maximum(dimension);
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinMinimum(
  unsigned int linearized_bin, unsigned int dimension) const
{
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return false;
  }
  vector<unsigned int> binarr = this->BinArr(linearized_bin);
  unsigned int bin = binarr[dimension];
  const double * bin_edges = this->BinEdgeArr(dimension);
  return bin_edges[bin];
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinMinimum(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return false;
  }
  vector<unsigned int> binarr = this->BinArr(linearized_bin);
  unsigned int bin = binarr[dimension];
  const double * bin_edges = this->BinEdgeArr(dimension);
  return bin_edges[bin];
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinMaximum(
  unsigned int linearized_bin, unsigned int dimension) const
{
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return false;
  }
  vector<unsigned int> binarr = this->BinArr(linearized_bin);
  unsigned int bin = binarr[dimension];
  const double * bin_edges = this->BinEdgeArr(dimension);
  return bin_edges[bin+1];
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinMaximum(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return false;
  }
  vector<unsigned int> binarr = this->BinArr(linearized_bin);
  unsigned int bin = binarr[dimension];
  const double * bin_edges = this->BinEdgeArr(dimension);
  return bin_edges[bin+1];
}
//________________________________________________________________________________________
vector<double> KBCubHcmbEdge2Edge::BinRange(
  unsigned int linearized_bin, unsigned int dimension) const
{
  vector<double> range(2);
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return range;
  }
  vector<unsigned int> binarr = this->BinArr(linearized_bin);
  unsigned int bin = binarr[dimension];
  const double * bin_edges = this->BinEdgeArr(dimension);
  range[0] = bin_edges[bin];
  range[1] = bin_edges[bin+1];
  return range;
}
//________________________________________________________________________________________
vector<double> KBCubHcmbEdge2Edge::BinRange(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
  vector<double> range(2);
  unsigned int dimension = this->Dimension(kv);
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return range;
  }
  vector<unsigned int> binarr = this->BinArr(linearized_bin);
  unsigned int bin = binarr[dimension];
  const double * bin_edges = this->BinEdgeArr(dimension);
  range[0] = bin_edges[bin];
  range[1] = bin_edges[bin+1];
  return range;
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinWidth(
  unsigned int linearized_bin, unsigned int dimension) const
{
  vector<double> range = this->BinRange(linearized_bin, dimension);
  double width = range[1] - range[0];
  return width;
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinWidth(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
  vector<double> range = this->BinRange(linearized_bin, kv);
  double width = range[1] - range[0];
  return width;
}
//________________________________________________________________________________________
vector<double> KBCubHcmbEdge2Edge::BinWidth(unsigned int linearized_bin) const
{
  vector<double> bwidth(fNDimensions);
  for(unsigned int idim = 0; idim < fNDimensions; idim++) {
    bwidth[idim] = this->BinWidth(linearized_bin, idim);
  }
  return bwidth;
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinCentre(
  unsigned int linearized_bin, unsigned int dimension) const
{
  vector<double> range = this->BinRange(linearized_bin, dimension);
  double centre = 0.5 * (range[1] + range[0]);
  return centre;
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinCentre(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
  vector<double> range = this->BinRange(linearized_bin, kv);
  double centre = 0.5 * (range[1] + range[0]);
  return centre;
}
//________________________________________________________________________________________
vector<double> KBCubHcmbEdge2Edge::BinCentre(unsigned int linearized_bin) const
{
  vector<double> centre(fNDimensions);
  for(unsigned int idim = 0; idim < fNDimensions; idim++) {
    centre[idim] = this->BinCentre(linearized_bin, idim);
  }
  return centre;
}
//________________________________________________________________________________________
const double * KBCubHcmbEdge2Edge::BinEdgeArr(unsigned int dimension) const
{
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return NULL;
  }
  return fBinEdges[dimension]->GetArray();
}
//________________________________________________________________________________________
const double * KBCubHcmbEdge2Edge::BinEdgeArr(conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinEdgeArr(dimension);
}
//________________________________________________________________________________________
const VALORArray * KBCubHcmbEdge2Edge::BinEdgeVArr(unsigned int dimension) const
{
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return NULL;
  }
  return fBinEdges[dimension];
}
//________________________________________________________________________________________
const VALORArray * KBCubHcmbEdge2Edge::BinEdgeVArr(conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinEdgeVArr(dimension);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::FindBin(double x, unsigned int dimension) const
{
  unsigned int n = 1 + this->NBins(dimension);
  const double * arr = this->BinEdgeArr(dimension);
  return TMath::BinarySearch(n,arr,x);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::FindBin(double x, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->FindBin(x,dimension);
}

unsigned int KBCubHcmbEdge2Edge::FindBin(vector<double> & xarr) const
{
  return this->FindLinearizedBin(xarr);
}

//________________________________________________________________________________________
vector<unsigned int> KBCubHcmbEdge2Edge::FindBinArr(vector<double> & xarr) const
{
  vector<unsigned int> binarr;
  if (xarr.size() != fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << fName<<" Vector has wrong dimension, got "<<xarr.size()<<", need "<<fNDimensions;
    util::Die(-1) ; return binarr;
  }
  binarr.reserve(fNDimensions);
  for(unsigned int idim = 0; idim < fNDimensions; idim++) {
    double x = xarr[idim];
    binarr[idim] = this->FindBin(x,idim);
    LOG("Binning", pDEBUG) 
     << "dim = " << idim << ", value = " << x << " -> bin = " << binarr[idim];
  }
  return binarr;
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::FindLinearizedBin(vector<double> & xarr) const
{
  vector<unsigned int> binarr = this->FindBinArr(xarr);
  return this->LinearizedBin(binarr);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::LinearizedBin(vector<unsigned int> & binarr) const
{
  unsigned int linearized_bin = binarr[0];
  for(unsigned int idim = 1; idim < fNDimensions; idim++) {
      unsigned int nbins_subspace = 1;
      for(unsigned int idim2 = 0; idim2 < idim; idim2++) {
         nbins_subspace *= this->NBins(idim2);
      }
      linearized_bin += (nbins_subspace * binarr[idim]);
  }
  return linearized_bin;
}
//________________________________________________________________________________________
vector<unsigned int> KBCubHcmbEdge2Edge::BinArr(unsigned int linearized_bin) const
{
  vector<unsigned int> binarr(fNDimensions);
  unsigned int nbins_subspace = this->NBins();
  for(unsigned int idim = fNDimensions-1; idim > 0; idim--) {
     nbins_subspace /= this->NBins(idim);
     unsigned int bin = linearized_bin / nbins_subspace;
     binarr[idim] = bin;
     linearized_bin -= (bin * nbins_subspace);
  }
  binarr[0] = linearized_bin;
  return binarr;
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::Bin(
  unsigned int linearized_bin, unsigned int dimension) const
{
  return this->BinArr(linearized_bin)[dimension];
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::Dimension(conv::KineVar_t kv) const
{
  std::map<conv::KineVar_t,unsigned int>::const_iterator it = fKineVar2Dim.find(kv);
  if (it == fKineVar2Dim.end()) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad KineVar: "<<util::AsString(kv)<<" ("<<kv<<")";
    util::Die(-1) ; return -1;
  }
  int dimension = it->second;
  return dimension;
}
//________________________________________________________________________________________
conv::KineVar_t KBCubHcmbEdge2Edge::KineVar(unsigned int dimension)
{
  if(dimension < fNDimensions) return fKineVars[dimension];

  return conv::kvUndefined;
}
//________________________________________________________________________________________
void KBCubHcmbEdge2Edge::Init(void)
{
  fKineSpace = conv::kspUndefined;
  fNDimensions = 0;
  fKineVars.clear();
  fKineVar2Dim.clear();
  fFixedWidth.clear();
  fMinimum.clear();
  fMaximum.clear();
  fBinEdges.clear();
}
//________________________________________________________________________________________
void KBCubHcmbEdge2Edge::CleanUp(void)
{
  vector<VALORArray*>::iterator it = fBinEdges.begin();
  for( ; it != fBinEdges.end(); ++it) {
    VALORArray * arr = *it;
    if(arr) delete arr;
    arr = 0;
  }  
  this->Init();
}
//________________________________________________________________________________________


