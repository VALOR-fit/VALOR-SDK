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
const unsigned int & VALORBinning::Dimension(conv::KineVar_t kv) const
{
  if (fKineVarToDim[kv] < 0 ) {
    LOG("VALORBinning",pFATAL) << "Bad KineVar: "<<util::AsString(kv) << " ("<<kv<<")"<< " in "<<util::AsString(fKineSpace);
    util::Die(-1) ; return 0;
  }
  return fKineVarToDim[kv];
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
