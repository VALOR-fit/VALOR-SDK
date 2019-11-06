//________________________________________________________________________________________
/*

Copyright (c) 2010-2017, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include "VALORBinning.h"

#include "TSystem.h"
#include "TXMLEngine.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlXML.h"
#include "valor/Utilities/UtlStr.h"

#include "valor/KineBinning/KBCubHcmbMondrian.h"
#include "valor/KineBinning/KBCubHcmbEdge2Edge.h"
#include "valor/KineBinning/NoBinning.h"

using namespace valor;
using std::vector;

//________________________________________________________________________________________
VALORBinning::VALORBinning() :
 TNamed("uninitialised","uninitialised"),
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
const int & VALORBinning::Dimension(conv::KineVar_t kv) const
{
  if (fKineVarToDim[kv] < 0 ) {
    LOG("VALORBinning",pFATAL) << "Bad KineVar: "<<util::AsString(kv) << " ("<<kv<<")"<< " in "<<util::AsString(fKineSpace);
    util::Die(-1) ;
    static const int zero = 0;
    return zero;
  }
  return fKineVarToDim[kv];
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
  const unsigned int & dimension = this->Dimension(kv);
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
  const unsigned int & dimension = this->Dimension(kv);
  return this->Maximum(dimension);
}
//________________________________________________________________________________________
double VALORBinning::BinMinimum(unsigned int bin, conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
  return this->BinMinimum(bin,dimension);
}
//________________________________________________________________________________________
double VALORBinning::BinMaximum(unsigned int bin, conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
  return this->BinMaximum(bin,dimension);
}
//________________________________________________________________________________________
const vector<double> & VALORBinning::BinRange(
  unsigned int bin, conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
  return this->BinRange(bin,dimension);
}
//________________________________________________________________________________________
double VALORBinning::BinWidth(unsigned int bin, unsigned int dimension) const
{
  const vector<double> & range = this->BinRange(bin,dimension);
  double width = range[1] - range[0];
  return width;
}
//________________________________________________________________________________________
double VALORBinning::BinWidth(unsigned int bin, conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
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
  const vector<double> & range = this->BinRange(bin,dimension);
  double centre = 0.5 * (range[1] + range[0]);
  return centre;
}
//________________________________________________________________________________________
double VALORBinning::BinCentre(unsigned int bin, conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
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
VALORBinning * VALORBinning::BinningFromFile(const std::string & xml_file, bool read_now)
{
  TString ts_file(util::CompleteConfigPath(xml_file).c_str());
  gSystem->ExpandPathName(ts_file);
  
  FileStat_t dummy;
  if (gSystem->GetPathInfo(ts_file.Data(), dummy)) {
    LOG("VALORBinning", pERROR) << "Binning file not found: " << xml_file;
    util::Die(1); return nullptr;
  }

  // Parse the input binning configuration XML file
  TXMLEngine* xml = new TXMLEngine;   
  XMLDocPointer_t xmldoc = xml->ParseFile(ts_file.Data());
  if (xmldoc==0) {
    LOG("VALORBinning",pERROR) 
        << "Can not parse input XML file [" << ts_file << "]";
    delete xml;
    util::Die(1); return nullptr;
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);

  VALORBinning * binning = nullptr;
  string type = util::RemoveAllWhitespace(util::XML2String(xml,root_node,"type"));

  if(type == "edge-to-edge") {
    binning = new KBCubHcmbEdge2Edge;
  }
  else if (type == "mondrian") {
    binning = new KBCubHcmbMondrian;
  }
  else if ( type == "none"){
    std::cout << "is non"<< std::endl;
    binning = new NoBinning;
  }
  else {
    LOG("VALORBinning",pFATAL) << "Unable to initialise binning type "<<type<<" in xml file "<<xml_file;
    util::Die(-1) ; return nullptr;
  }
  if (read_now) {
    bool success = binning->Read(xml_file);
    if (!success) {
      LOG("VALORBinning",pFATAL) << "Failed to read binning from file "<<xml_file;
      util::Die(-1) ; return nullptr;
    }
  }

  return binning;
}
//________________________________________________________________________________________

std::string VALORBinning::GetType(const std::string & xml_file, bool read_now)
{
  TString ts_file(util::CompleteConfigPath(xml_file).c_str());
  gSystem->ExpandPathName(ts_file);
  
  FileStat_t dummy;
  if (gSystem->GetPathInfo(ts_file.Data(), dummy)) {
    LOG("VALORBinning", pERROR) << "Binning file not found: " << xml_file;
    util::Die(1); return nullptr;
  }

  // Parse the input binning configuration XML file
  TXMLEngine* xml = new TXMLEngine;   
  XMLDocPointer_t xmldoc = xml->ParseFile(ts_file.Data());
  if (xmldoc==0) {
    LOG("VALORBinning",pERROR) 
        << "Can not parse input XML file [" << ts_file << "]";
    delete xml;
    util::Die(1); return nullptr;
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);

  VALORBinning * binning = nullptr;
  string type = util::RemoveAllWhitespace(util::XML2String(xml,root_node,"type"));

  return type;
}
//________________________________________________________________________________________

int VALORBinning::GetNextBin (int current_bin, conv::KineVar_t kv, const std::map<conv::KineVar_t, double> & other_kv_vals) const
{
  // Check that the input current bin number is sensible. Allow current_bin = -1 for the start of the iteration,
  // as we immediately increment it
  if(current_bin < -1 || current_bin >= (int)NBins()) {
    LOG("KBCubHcmbMondrian", pERROR) << "Invalid input current bin number [" << current_bin << "]";
    util::Die(-1);
  }

  //check that the kine vars from other_kv_vals + kv gives ALL of the binning kvs
  unsigned int n_dims_match = 0;
  for(auto iter = other_kv_vals.cbegin(); iter != other_kv_vals.cend(); ++iter) {
    conv::KineVar_t kv_fixed = iter->first;
    if(this->Contains(kv_fixed)) { n_dims_match++; }
    if(kv_fixed == kv) {
      LOG("KBCubHcmbMondrian", pERROR) << "The kine var that we're getting the next bin of [" << util::AsString(kv)
                                       << "] is also present in the fixed kine var map!";
      util::Die(-1);
    }
  }
  if(this->Contains(kv)) { n_dims_match++; }
  if(n_dims_match != NDimensions()) {
    LOG("KBCubHcmbMondrian", pERROR) << "The input kine vars don't match ALL of the binning kine vars";
    util::Die(-1);
  }

  for(int iglob = current_bin+1; iglob < (int)NBins(); iglob++) {  // Start at the global bin number of the previous bin +1 (for efficiency)
    // search for the first bin where the kvs we're NOT getting the bin number for have
    // values that match the input constant values
    unsigned int n_matches = 0;
    for(auto iter = other_kv_vals.cbegin(); iter != other_kv_vals.cend(); ++iter) {
      conv::KineVar_t kv_fixed = iter->first;
      double kv_val_fixed = iter->second;
      double min = BinMinimum(iglob, kv_fixed);
      double max = BinMaximum(iglob, kv_fixed);
      if(kv_val_fixed >= min && kv_val_fixed < max) { n_matches++; }
    }
    if(n_matches != NDimensions() - 1) { continue; }
    // This global bin number must correspond to the next x bin that we want
    return iglob;
  }
  // If we've reached the end of the binning scheme without finding a next bin, return a nonsense value
  // to signal that we should stop iterating
  return -999;
}
//________________________________________________________________________________________
