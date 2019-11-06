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
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/Env.h"
#include "valor/KineBinning/KBCubHcmbEdge2Edge.h"

using namespace valor;
using std::vector;

ClassImp(KBCubHcmbEdge2Edge)

//________________________________________________________________________________________
KBCubHcmbEdge2Edge::KBCubHcmbEdge2Edge() :
VALORBinning()
{
  this->Init();
}
//________________________________________________________________________________________
KBCubHcmbEdge2Edge::KBCubHcmbEdge2Edge(string name, string title) :
VALORBinning(name.c_str(), title.c_str())
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
  string space = util::RemoveAllWhitespace(util::XML2String(xml,root_node,"space"));
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
    fKineVarToDim[kv] = i;
  }
  fFixedWidth . reserve(fNDimensions);
  fMinimum    . reserve(fNDimensions);
  fMaximum    . reserve(fNDimensions);
  fBinEdges   . reserve(fNDimensions);
  fBinRanges  . reserve(fNDimensions);

  //
  string type = util::RemoveAllWhitespace(util::XML2String(xml,root_node,"type"));
  if(type != "edge-to-edge") {
    LOG("Binning", pERROR) 
      << "Input binning scheme [" << type << "] not `edge-to-edge'";
    return false;
  }
  
  fNBins = 1;
  for(unsigned int idim = 0; idim < fNDimensions; idim++) {

    conv::KineVar_t kine_var = fKineVars[idim];    
    string kine_var_str = util::AsString(kine_var);
    const char * kine_var_ccs = kine_var_str.c_str();

    string bin_size = util::RemoveAllWhitespace(util::XML2String(
       xml,root_node,Form("%s_binning/bin_size",kine_var_ccs)));
    unsigned int nbins_this_dim = (unsigned int) 
        util::XML2Int(xml,root_node,Form("%s_binning/nbins",kine_var_ccs));
        
    fNBins *= nbins_this_dim;

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
      fBinEdges.push_back(std::vector<double>());
      double dx = (range[1] - range[0]) / nbins_this_dim;
      double x = range[0];
      for(unsigned int i = 0; i < nbins_this_dim+1; i++) {
        fBinEdges[idim].push_back(x);
        x += dx;
      }
      fFixedWidth[idim] = true;
      fMinimum   [idim] = range[0];
      fMaximum   [idim] = range[1];
    } 

    //
    else if(bin_size == "variable") {
      vector<double> bin_edges = util::XML2DblArray(
         xml,root_node,Form("%s_binning/bin_edges",kine_var_ccs));
      if(bin_edges.size() != nbins_this_dim+1) {
         LOG("Binning", pERROR) 
           << bin_edges.size() << " bin edges were specified for the " 
           << nbins_this_dim << " bins in dimension " << idim;
         return false;
      }
      fBinEdges  .push_back(bin_edges);
      fFixedWidth[idim] = false;
      fMinimum   [idim] = bin_edges[0];
      fMaximum   [idim] = bin_edges[nbins_this_dim];
    } 

    else {
      LOG("Binning", pERROR) 
         << "Bin size is neither `fixed' nor `variable'";
      return false;
    }

    // Build a vector of ranges in each dimension for each bin
    vector< vector<double> > vec_ranges_idim(nbins_this_dim, vector<double>(2));
    for(unsigned int ibin = 0; ibin < nbins_this_dim; ibin++) {
      vector<double> range(2);
      range[0] = fBinEdges[idim][ibin];
      range[1] = fBinEdges[idim][ibin+1];
      vec_ranges_idim[ibin] = range;
    }
    fBinRanges.push_back(vec_ranges_idim);

  }//idim

  // Build a vector of bin numbers for each 1D linearized bin
  fBin1DArrs = vector< vector<unsigned int> > (fNDimensions, vector<unsigned int>(this->NBins()));
  for(unsigned int ibin = 0; ibin < this->NBins(); ibin++) {
    unsigned int linearized_bin = ibin;
    unsigned int nbins_subspace = this->NBins();
    for(unsigned int idim = fNDimensions-1; idim > 0; idim--) {
      nbins_subspace /= this->NBins(idim);
      unsigned int bin = linearized_bin / nbins_subspace;
      fBin1DArrs[idim][ibin] = bin;
      linearized_bin -= (bin * nbins_subspace);
    }
    fBin1DArrs[0][ibin] = linearized_bin;
  }
  
  std::ostringstream binning_print;
  binning_print << fName<< " total bins "<<this->NBins() << " in range: ";
  for (unsigned int i = 0 ; i < this->NDimensions() ; i++) {
    binning_print<< "("<<this->Minimum(i)<<" < "<<util::AsString(this->KineVar(i))<<" < "<<this->Maximum(i)<<") ";
  }
  LOG("Binning", pNOTICE) << binning_print.str();
   
  xml->FreeDoc(xmldoc);
  delete xml;
  
  if (fName=="" || fName=="uninitialised") {
    std::ostringstream name;
    name<<"e2e";
    for (const conv::KineVar_t kv : fKineVars) {
      name<<"_"<<util::AsString(kv);
    }
    this->SetName(name.str().c_str());
  }

  return true;
}
//________________________________________________________________________________________
VALORBinning* KBCubHcmbEdge2Edge::Contract(conv::KineSpace_t ks) const
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
   unsigned int i = 0;
   for(auto this_kv : fKineVars)
   {
      // Only add kinevars to the contracted binning scheme if they are included in the contracted kinespace
      if(util::KineSpaceContains(ks, this_kv))
      {
         binning->fKineVarToDim[this_kv] = i;
         i++;
      }
   }
   if(i != binning->fNDimensions)
   {
     LOG("KBCubHcmbEdge2Edge",pFATAL) << "Wrong number of kinevars stored in contracted binning scheme.";
     util::Die(-1) ; return nullptr;
   }

   // Reserve space for the bin minima, maxima and edges
   binning->fMinimum  .reserve(binning->fNDimensions);
   binning->fMaximum  .reserve(binning->fNDimensions);
   binning->fBinEdges .reserve(binning->fNDimensions);
   binning->fBinRanges.reserve(binning->fNDimensions);

   size_t nbins = 1;

   // Construct new bin edges from old
   for(unsigned int d = 0; d < fNDimensions; ++d)
   {
      if(std::find(binning->fKineVars.begin(), binning->fKineVars.end(),
               fKineVars[d]) != binning->fKineVars.end())
      {  // Current dimension is for a kinematic variable in the target space
         // Add the edges for the current dimension, type, minima and maxima
         binning->fBinEdges.push_back  (fBinEdges[d]  );
         binning->fFixedWidth.push_back(fFixedWidth[d]);
         binning->fMinimum.push_back   (fMinimum[d]   );
         binning->fMaximum.push_back   (fMaximum[d]   );
         binning->fBinRanges.push_back (fBinRanges[d] );

         nbins *= fBinEdges[d].size()-1;
      }
   }
  
   binning->fNBins = nbins;

   binning->fBin1DArrs = vector< vector<unsigned int> > (binning->fNDimensions, vector<unsigned int>(binning->NBins()));
   for(unsigned int ibin = 0; ibin < binning->NBins(); ibin++) {
     unsigned int linearized_bin = ibin;
     unsigned int nbins_subspace = binning->NBins();
     for(unsigned int idim = binning->fNDimensions-1; idim > 0; idim--) {
       nbins_subspace /= binning->NBins(idim);
       unsigned int bin = linearized_bin / nbins_subspace;
       binning->fBin1DArrs[idim][ibin] = bin;
       linearized_bin -= (bin * nbins_subspace);
     }
     binning->fBin1DArrs[0][ibin] = linearized_bin;
   }
   
   TString new_name = TString::Format("%s_contracted_to_%s",this->GetName(),util::AsString(ks).c_str());
   binning->SetName(new_name.Data());

   return dynamic_cast<VALORBinning*>(binning);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::NBins(unsigned int dimension) const
{
  return (fBinEdges[dimension].size() - 1);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::NBins(conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
  return this->NBins(dimension);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::NBins(conv::KineVar_t kv, const std::map<conv::KineVar_t, double> & /*other_kv_vals*/) const
{
  return NBins(kv);
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
  const unsigned int & dimension = this->Dimension(kv);
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
double KBCubHcmbEdge2Edge::BinMinimum(
  unsigned int linearized_bin, unsigned int dimension) const
{
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return false;
  }
  if (linearized_bin >= this->NBins()) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad linearized bin: " << linearized_bin;
    util::Die(-1) ; return false;
  }
  const unsigned int & bin = fBin1DArrs[dimension][linearized_bin];
  const double * bin_edges = this->BinEdgeArr(dimension);
  return bin_edges[bin];
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinMinimum(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return false;
  }
  if (linearized_bin >= this->NBins()) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad linearized bin: " << linearized_bin;
    util::Die(-1) ; return false;
  }
  const unsigned int & bin = fBin1DArrs[dimension][linearized_bin];
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
  if (linearized_bin >= this->NBins()) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad linearized bin: " << linearized_bin;
    util::Die(-1) ; return false;
  }
  const unsigned int & bin = fBin1DArrs[dimension][linearized_bin];
  const double * bin_edges = this->BinEdgeArr(dimension);
  return bin_edges[bin+1];
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinMaximum(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension.";
    util::Die(-1) ; return false;
  }
  if (linearized_bin >= this->NBins()) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad linearized bin: " << linearized_bin;
    util::Die(-1) ; return false;
  }
  const unsigned int & bin = fBin1DArrs[dimension][linearized_bin];
  const double * bin_edges = this->BinEdgeArr(dimension);
  return bin_edges[bin+1];
}
//________________________________________________________________________________________
const vector<double> & KBCubHcmbEdge2Edge::BinRange(
  unsigned int linearized_bin, unsigned int dimension) const
{
  if (dimension >= fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad dimension. Input dimension = " << dimension
                                     << "; binning kine space = " << util::AsString(this->KineSpace());
    util::Die(-1) ; return fBinRanges[0][0];
  }
  if (linearized_bin >= this->NBins()) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << "Bad linearized bin: " << linearized_bin;
    util::Die(-1) ; return fBinRanges[0][0];
  }
  const unsigned int & bin = fBin1DArrs[dimension][linearized_bin];
  return fBinRanges[dimension][bin];
}
//________________________________________________________________________________________
const vector<double> & KBCubHcmbEdge2Edge::BinRange(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
  return this->BinRange(linearized_bin, dimension);
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinWidth(
  unsigned int linearized_bin, unsigned int dimension) const
{
  const vector<double>& range = this->BinRange(linearized_bin, dimension);
  double width = range[1] - range[0];
  return width;
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinWidth(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
  const vector<double>& range = this->BinRange(linearized_bin, kv);
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
  const vector<double>& range = this->BinRange(linearized_bin, dimension);
  double centre = 0.5 * (range[1] + range[0]);
  return centre;
}
//________________________________________________________________________________________
double KBCubHcmbEdge2Edge::BinCentre(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
  const vector<double>& range = this->BinRange(linearized_bin, kv);
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
  return fBinEdges[dimension].data();
}
//________________________________________________________________________________________
const double * KBCubHcmbEdge2Edge::BinEdgeArr(conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
  return this->BinEdgeArr(dimension);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::FindBin1D(double x, unsigned int dimension) const
{
  unsigned int n = 1 + this->NBins(dimension);
  const double * arr = this->BinEdgeArr(dimension);
  return TMath::BinarySearch(n,arr,x);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::FindBin1D(double x, conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
  return this->FindBin1D(x,dimension);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::FindBin(const vector<double> & xarr) const
{
  vector<unsigned int> binarr = this->FindBin1DArr(xarr);
  return this->LinearizedBin(binarr);
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::FindBin(const std::map<conv::KineVar_t, double> & xarr) const
{
  std::vector<double> xarr_odered;
  std::vector<conv::KineVar_t> kvs = KineVars();
  for(unsigned int idim = 0; idim < fNDimensions; idim++) {
    conv::KineVar_t kv = kvs[idim];
    xarr_odered.push_back(xarr.at(kv));
  }
  vector<unsigned int> binarr = this->FindBin1DArr(xarr_odered);
  return this->LinearizedBin(binarr);
}
//________________________________________________________________________________________
vector<unsigned int> KBCubHcmbEdge2Edge::FindBin1DArr(const vector<double> & xarr) const
{
  vector<unsigned int> binarr;
  if (xarr.size() != fNDimensions) {
    LOG("KBCubHcmbEdge2Edge",pFATAL) << fName<<" Vector has wrong dimension, got "<<xarr.size()<<", need "<<fNDimensions;
    util::Die(-1) ; return binarr;
  }
  binarr.reserve(fNDimensions);
  for(unsigned int idim = 0; idim < fNDimensions; idim++) {
    double x = xarr[idim];
    binarr[idim] = this->FindBin1D(x,idim);
    LOG("Binning", pDEBUG) 
     << "dim = " << idim << ", value = " << x << " -> bin = " << binarr[idim];
  }
  return binarr;
}
//________________________________________________________________________________________
unsigned int KBCubHcmbEdge2Edge::LinearizedBin(const vector<unsigned int> & binarr) const
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
void KBCubHcmbEdge2Edge::Init(void)
{
  fKineSpace = conv::kspUndefined;
  fNDimensions = 0;
  fKineVars.clear();
  fKineVarToDim.clear();
  fFixedWidth.clear();
  fMinimum.clear();
  fMaximum.clear();
  fBinEdges.clear();
  fBinRanges.clear();
  fBin1DArrs.clear();
}
//________________________________________________________________________________________
void KBCubHcmbEdge2Edge::CleanUp(void)
{
  this->Init();
}
//________________________________________________________________________________________
std::vector<TLine>  KBCubHcmbEdge2Edge::GetBinningTLines(void) const
{
  std::vector<TLine> lines;
  if (this->NDimensions()==2) {
    double x_min = this->Minimum(0);
    double y_min = this->Minimum(1);
    double x_max = this->Maximum(0);
    double y_max = this->Maximum(1);
    const double * x_arr = this->BinEdgeArr(0);
    const double * y_arr = this->BinEdgeArr(1);
    for (unsigned int i = 0 ; i <= this->NBins(0) ; i++) { // horizontal lines
      lines.push_back(TLine(x_min,y_arr[i],x_max,y_arr[i]));
    }
    for (unsigned int i = 0 ; i <= this->NBins(1) ; i++) { // vertical lines
      lines.push_back(TLine(x_arr[i],y_min,x_arr[i],y_max));
    }
  }
  else {
    LOG("KBCubHcmbMondrian",pWARN) << "GetBinningTLines can only be used with 2D binnings.";
  }
  return lines;
}
//________________________________________________________________________________________
void KBCubHcmbEdge2Edge::Print(std::ostream & stream) const
{
  stream<<"Edge to edge binning: "<<this->GetName()<<" "<<this->GetTitle()<<" ("<<fNBins<<" bins)"<<std::endl;
  for (size_t ibin = 0; ibin < fNBins; ibin++) {
    stream<<"  Bin "<<std::setw(4)<<ibin<<"\t";
    for (const conv::KineVar_t & kv : fKineVars) {
      stream<<std::setw(4)<<this->BinMinimum(ibin,kv)<<" < "
            <<util::AsString(kv)
            <<" < "<<std::setw(4)<<this->BinMaximum(ibin,kv)<<" ; ";
    }
    stream<<std::endl;
  }
}
//________________________________________________________________________________________
