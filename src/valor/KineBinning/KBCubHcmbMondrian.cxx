//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <cassert>

#include <TSystem.h>
#include <TMath.h>

#include "valor/Utilities/UtlXML.h"
#include "valor/Messenger/Messenger.h"
#include "valor/VArray/VALORArray.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/Env.h"
#include "valor/KineBinning/KBCubHcmbEdge2Edge.h"
#include "valor/KineBinning/KBCubHcmbMondrian.h"

using std::pair;
using namespace valor;

ClassImp(KBCubHcmbMondrian)

//________________________________________________________________________________________
KBCubHcmbMondrian::KBCubHcmbMondrian() :
TNamed()
{
  this->Init();
}
//________________________________________________________________________________________
KBCubHcmbMondrian::KBCubHcmbMondrian(string name, string title) :
TNamed(name.c_str(), title.c_str())
{
  this->Init();
}
//________________________________________________________________________________________
KBCubHcmbMondrian::~KBCubHcmbMondrian() 
{
  this->CleanUp();
}
//________________________________________________________________________________________
bool KBCubHcmbMondrian::Read(string file)
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
  LOG("Binning", pNOTICE) 
     << "Number of dimensions = " << fNDimensions;
  fKineVars    = util::KineVar(fKineSpace);
  for(unsigned int i = 0; i < fNDimensions; i++) {
    conv::KineVar_t kv = fKineVars[i];
    fKineVar2Dim[kv] = i;
  }
  fMinimum.reserve(fNDimensions);
  fMaximum.reserve(fNDimensions);

  //
  string type = util::XML2String(xml,root_node,"type");
  if(type != "mondrian") {
    LOG("Binning", pERROR)
      << "Input binning scheme [" << type << "] not `mondrian'";
    return false;
  }

  //
  int nbins = util::XML2Int(xml,root_node,"nbins");
  if(nbins <= 0) {
    LOG("Binning", pERROR) << "Invalid number of bins";
    return false;
  }
  fNBins = (unsigned int) nbins;
  LOG("Binning", pNOTICE) << "Number of bins = " << fNBins;

  fBinEdges.reserve(fNBins * fNDimensions * 2);
  
  unsigned int l = 0;
  for(unsigned int ibin = 0; ibin < fNBins; ibin++) {
     LOG("Binning", pNOTICE) << "Reading edges of bin: " << ibin;
     vector<double> edges = util::XML2DblArray(
                 xml,root_node,Form("bin_%d",ibin));
     if(edges.size() != 2*fNDimensions) {
         LOG("Binning", pERROR)
           << edges.size() << " edges were specified for "
           << "a cuboid bin in " << fNDimensions << " dimensions";
         return false;
     }
     unsigned int k = 0;
     for(unsigned int idim = 0; idim < fNDimensions; idim++) {
         double min = edges[k++];
         double max = edges[k++];
         fBinEdges[l++] = min;
         fBinEdges[l++] = max;
         LOG("Binning", pNOTICE) 
            << "\t Dimension: " << idim 
            << " -> [" << min << ", " << max << "]";
         if(ibin == 0) {
           fMinimum[idim] = min;
           fMaximum[idim] = max;
         } else {
           fMinimum[idim] = TMath::Min(fMinimum[idim], min);
           fMaximum[idim] = TMath::Max(fMaximum[idim], max);
         }
     }
  }

  return true;
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::Minimum(unsigned int dimension) const
{
  return fMinimum[dimension];
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::Minimum(conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->Minimum(dimension);
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::Maximum(unsigned int dimension) const
{
  assert(dimension < fNDimensions);
  return fMaximum[dimension];
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::Maximum(conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->Maximum(dimension);
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::BinMinimum(unsigned int bin, unsigned int dimension) const
{
  assert(bin < fNBins);
  assert(dimension < fNDimensions);
  unsigned int idx = (2 * fNDimensions * bin) + (2 * dimension);
  return fBinEdges[idx];
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::BinMinimum(unsigned int bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinMinimum(bin,dimension);
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::BinMaximum(unsigned int bin, unsigned int dimension) const
{
  assert(bin < fNBins);
  assert(dimension < fNDimensions);
  unsigned int idx = (2 * fNDimensions * bin) + (2 * dimension) + 1;
  return fBinEdges[idx];
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::BinMaximum(unsigned int bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinMaximum(bin,dimension);
}
//________________________________________________________________________________________
vector<double> KBCubHcmbMondrian::BinRange(
  unsigned int bin, unsigned int dimension) const
{
  vector<double> range(2);
  assert(bin < fNBins);
  assert(dimension < fNDimensions);
  unsigned int idx_base = (2 * fNDimensions * bin) + (2 * dimension);
  range[0] = fBinEdges[idx_base];
  range[1] = fBinEdges[idx_base+1];
  return range;
}
//________________________________________________________________________________________
vector<double> KBCubHcmbMondrian::BinRange(
  unsigned int bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinRange(bin,dimension);
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::BinWidth(unsigned int bin, unsigned int dimension) const
{
  vector<double> range = this->BinRange(bin,dimension);
  double width = range[1] - range[0];
  return width;
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::BinWidth(unsigned int bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinWidth(bin,dimension);
}
//________________________________________________________________________________________
vector<double> KBCubHcmbMondrian::BinWidth(unsigned int bin) const
{
 vector<double> bwidth(fNDimensions);
  for(unsigned int idim = 0; idim < fNDimensions; idim++) {
    bwidth[idim] = this->BinWidth(bin, idim);
  }
  return bwidth;
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::BinCentre(unsigned int bin, unsigned int dimension) const
{
  vector<double> range = this->BinRange(bin,dimension);
  double centre = 0.5 * (range[1] + range[0]);
  return centre;
}
//________________________________________________________________________________________
double KBCubHcmbMondrian::BinCentre(unsigned int bin, conv::KineVar_t kv) const
{
  unsigned int dimension = this->Dimension(kv);
  return this->BinCentre(bin,dimension);
}
//________________________________________________________________________________________
vector<double> KBCubHcmbMondrian::BinCentre(unsigned int bin) const
{
 vector<double> centre(fNDimensions);
  for(unsigned int idim = 0; idim < fNDimensions; idim++) {
    centre[idim] = this->BinCentre(bin, idim);
  }
  return centre;
}
//________________________________________________________________________________________
unsigned int KBCubHcmbMondrian::FindBin(vector<double> & xarr) const
{
  assert(xarr.size() == fNDimensions);

  for(unsigned int ibin = 0; ibin < fNBins; ibin++) {
     bool inside = true;     
     for(unsigned int idim = 0; idim < fNDimensions; idim++) {
        double x   = xarr[idim];
        unsigned int idx = (2 * fNDimensions * ibin) + (2 * idim);
        double min = fBinEdges[idx];
        double max = fBinEdges[idx+1];
        inside = inside && (x >= min && x < max);
        if(!inside) break;
     }  
     if(inside) {
       return ibin;
     }
  }

  LOG("Binning", pERROR) << "Mondrian binning doesn't span all space?";
  exit(1);
}
//________________________________________________________________________________________
void KBCubHcmbMondrian::UseEdge2EdgeBinning(
  const KBCubHcmbEdge2Edge * kbe2e)
{
  fKBE2E = kbe2e;

  // Start working out the mapping between the mondrian <-> edge-to-edge schemes
  unsigned int ne2ebins = fKBE2E->NBins();
  for(unsigned int e2e_lin_bin = 0; e2e_lin_bin < ne2ebins; e2e_lin_bin++) {
    vector<double> xarr = fKBE2E->BinCentre(e2e_lin_bin);
    unsigned int mondrian_bin = this->FindBin(xarr);

    fBinMondrianFromE2E.insert( 
       pair<unsigned int,unsigned int>(e2e_lin_bin, mondrian_bin) );
    fBinMondrianToE2E.insert(
       pair<unsigned int,unsigned int>(mondrian_bin, e2e_lin_bin) );
  }

}
//________________________________________________________________________________________
unsigned int KBCubHcmbMondrian::MapFromLinearizedE2E(unsigned int e2e_lin_bin) const
{
  std::map<unsigned int, unsigned int>::const_iterator it = fBinMondrianFromE2E.find(e2e_lin_bin);
  assert(it != fBinMondrianFromE2E.end());
  return it->second;
}
//________________________________________________________________________________________
unsigned int KBCubHcmbMondrian::Dimension(conv::KineVar_t kv) const
{
  std::map<conv::KineVar_t,unsigned int>::const_iterator it = fKineVar2Dim.find(kv);
  assert(it != fKineVar2Dim.end());
  int dimension = it->second;
  return dimension;
}
//________________________________________________________________________________________
std::vector<TLine>  KBCubHcmbMondrian::GetBinningTLines(void) const
{
  //
  // Needs to be modified to loop over linearised bins and grab the bin edges
  //
  std::vector<TLine> bin_edge_lines;
  if (fNDimensions == 2) {
    bin_edge_lines.reserve(fNBins * fNDimensions * 4);
    for (unsigned int ibin = 0; ibin < fNBins ; ibin++ ) {
      double x0 = this->BinMinimum(ibin,0);
      double x1 = this->BinMaximum(ibin,0);
      double y0 = this->BinMinimum(ibin,1);
      double y1 = this->BinMaximum(ibin,1);
      
      TLine lxl = TLine(x0,y0,x1,y0);
      TLine lxh = TLine(x0,y1,x1,y1);
      TLine lyl = TLine(x0,y0,x0,y1);
      TLine lyh = TLine(x1,y0,x1,y1);

      bin_edge_lines.push_back(lxl);
      bin_edge_lines.push_back(lxh);
      bin_edge_lines.push_back(lyl);
      bin_edge_lines.push_back(lyh);    
    }
  }
  return bin_edge_lines;
}
//________________________________________________________________________________________
conv::KineVar_t KBCubHcmbMondrian::KineVar(unsigned int dimension)
{
  if(dimension < fNDimensions) return fKineVars[dimension];

  return conv::kvUndefined;
}
//________________________________________________________________________________________
void KBCubHcmbMondrian::Init(void)
{
  fKineSpace = conv::kspUndefined;
  fNDimensions = 0;
  fKineVars.clear();
  fKineVar2Dim.clear();
  fNBins = 0;
  fBinEdges.clear();
  fMinimum.clear();
  fMaximum.clear();
  fBinMondrianFromE2E.clear();
  fBinMondrianToE2E.clear();
}
//________________________________________________________________________________________
void KBCubHcmbMondrian::CleanUp(void)
{
  this->Init();
}
//________________________________________________________________________________________


