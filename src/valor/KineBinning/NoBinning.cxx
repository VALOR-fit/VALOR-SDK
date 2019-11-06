//_____________________o___________________________________________________________________
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
#include "valor/KineBinning/NoBinning.h"

using namespace valor;
using std::vector;

ClassImp(NoBinning)

//________________________________________________________________________________________
NoBinning::NoBinning() :
VALORBinning()
{
  this->Init();
}
//________________________________________________________________________________________
NoBinning::NoBinning(string name, string title) :
VALORBinning(name.c_str(), title.c_str())
{
  this->Init();
}
//________________________________________________________________________________________
NoBinning::~NoBinning() 
{
  this->CleanUp();
}
//________________________________________________________________________________________
bool NoBinning::Read(string file)
{
  std::cout << "reading" << std::endl;
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
  std::cout << "fKineSpace: " << (int) fKineSpace << std::endl;
  for(unsigned int i = 0; i < fNDimensions; i++) {
    conv::KineVar_t kv = fKineVars[i];
    std::cout << "fKineVars: " << (int) kv << std::endl;
    fKineVarToDim[kv] = i;
  }

  return true;
}
//________________________________________________________________________________________
VALORBinning* NoBinning::Contract(conv::KineSpace_t ks) const
{
   NoBinning* binning = new NoBinning();

   return dynamic_cast<VALORBinning*>(binning);
}
//________________________________________________________________________________________
unsigned int NoBinning::NBins(unsigned int dimension) const
{
  return 1;
}
//________________________________________________________________________________________
unsigned int NoBinning::NBins(conv::KineVar_t kv) const
{
  return 1;
}
//________________________________________________________________________________________
unsigned int NoBinning::NBins(conv::KineVar_t kv, const std::map<conv::KineVar_t, double> & /*other_kv_vals*/) const
{
  return 1;
}
// //________________________________________________________________________________________
// bool NoBinning::FixedWidth(unsigned int dimension) const
// {
//   if (dimension >= fNDimensions) {
//     LOG("NoBinning",pWARN) << "Bad dimension.";
//     util::Die(-1) ; return false;
//   }
//   return fFixedWidth[dimension];
// }
// //________________________________________________________________________________________
// bool NoBinning::FixedWidth(conv::KineVar_t kv) const
// {
//   const unsigned int & dimension = this->Dimension(kv);
//   return this->FixedWidth(dimension);
// }
// //________________________________________________________________________________________
// bool NoBinning::VariableWidth(unsigned int dimension) const
// {
//   return (! this->FixedWidth(dimension));
// }
// //________________________________________________________________________________________
// bool NoBinning::VariableWidth(conv::KineVar_t kv) const
// {
//   return (! this->FixedWidth(kv));
// }
// //________________________________________________________________________________________

//________________________________________________________________________________________
double NoBinning::BinMinimum(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
    LOG("NoBinning",pWARN) << "Bad dimension.";
    return false;

}
//________________________________________________________________________________________
double NoBinning::BinMinimum(
  unsigned int linearized_bin, unsigned int dimension) const
{
    LOG("NoBinning",pWARN) << "Bad dimension.";
    return false;
}
//________________________________________________________________________________________
double NoBinning::BinMaximum(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
    LOG("NoBinning",pWARN) << "Bad dimension.";
    return false;

}
double NoBinning::BinMaximum(
  unsigned int linearized_bin, unsigned int dimension) const
{
    LOG("NoBinning",pWARN) << "Bad dimension.";
    return false;
}

//________________________________________________________________________________________
const vector<double> & NoBinning::BinRange(
  unsigned int linearized_bin, unsigned int dimension) const
{
  LOG("NoBinning",pWARN) << "Bad dimension. Input dimension = " << dimension
			    << "; binning kine space = " << util::AsString(this->KineSpace());
  return fBinRanges[0][0];
}

//________________________________________________________________________________________
const vector<double> & NoBinning::BinRange(
  unsigned int linearized_bin, conv::KineVar_t kv) const
{
  const unsigned int & dimension = this->Dimension(kv);
  return this->BinRange(linearized_bin, dimension);
}
// //________________________________________________________________________________________
// double NoBinning::BinWidth(
//   unsigned int linearized_bin, unsigned int dimension) const
// {
//   const vector<double>& range = this->BinRange(linearized_bin, dimension);
//   double width = range[1] - range[0];
//   return width;
// }
// //________________________________________________________________________________________
// double NoBinning::BinWidth(
//   unsigned int linearized_bin, conv::KineVar_t kv) const
// {
//   const vector<double>& range = this->BinRange(linearized_bin, kv);
//   double width = range[1] - range[0];
//   return width;
// }
// //________________________________________________________________________________________
// vector<double> NoBinning::BinWidth(unsigned int linearized_bin) const
// {
//   vector<double> bwidth(fNDimensions);
//   for(unsigned int idim = 0; idim < fNDimensions; idim++) {
//     bwidth[idim] = this->BinWidth(linearized_bin, idim);
//   }
//   return bwidth;
// }
// //________________________________________________________________________________________
// double NoBinning::BinCentre(
//   unsigned int linearized_bin, unsigned int dimension) const
// {
//   const vector<double>& range = this->BinRange(linearized_bin, dimension);
//   double centre = 0.5 * (range[1] + range[0]);
//   return centre;
// }
// //________________________________________________________________________________________
// double NoBinning::BinCentre(
//   unsigned int linearized_bin, conv::KineVar_t kv) const
// {
//   const vector<double>& range = this->BinRange(linearized_bin, kv);
//   double centre = 0.5 * (range[1] + range[0]);
//   return centre;
// }
// //________________________________________________________________________________________
// vector<double> NoBinning::BinCentre(unsigned int linearized_bin) const
// {
//   vector<double> centre(fNDimensions);
//   for(unsigned int idim = 0; idim < fNDimensions; idim++) {
//     centre[idim] = this->BinCentre(linearized_bin, idim);
//   }
//   return centre;
// }
// //________________________________________________________________________________________
// const double * NoBinning::BinEdgeArr(unsigned int dimension) const
// {
//   if (dimension >= fNDimensions) {
//     LOG("NoBinning",pWARN) << "Bad dimension.";
//     util::Die(-1) ; return NULL;
//   }
//   return fBinEdges[dimension].data();
// }
// //________________________________________________________________________________________
// const double * NoBinning::BinEdgeArr(conv::KineVar_t kv) const
// {
//   const unsigned int & dimension = this->Dimension(kv);
//   return this->BinEdgeArr(dimension);
// }
//________________________________________________________________________________________
unsigned int NoBinning::FindBin1D(double x, unsigned int dimension) const
{
  return 0;
}
//________________________________________________________________________________________
unsigned int NoBinning::FindBin1D(double x, conv::KineVar_t kv) const
{
  return 0;
}
//________________________________________________________________________________________
unsigned int NoBinning::FindBin(const vector<double> & xarr) const
{
  return 0;
}
//________________________________________________________________________________________
unsigned int NoBinning::FindBin(const std::map<conv::KineVar_t, double> & xarr) const
{
  return 0;
}
// //________________________________________________________________________________________
// vector<unsigned int> NoBinning::FindBin1DArr(const vector<double> & xarr) const
// {
//   vector<unsigned int> binarr;
//   if (xarr.size() != fNDimensions) {
//     LOG("NoBinning",pWARN) << fName<<" Vector has wrong dimension, got "<<xarr.size()<<", need "<<fNDimensions;
//     util::Die(-1) ; return binarr;
//   }
//   binarr.reserve(fNDimensions);
//   for(unsigned int idim = 0; idim < fNDimensions; idim++) {
//     double x = xarr[idim];
//     binarr[idim] = this->FindBin1D(x,idim);
//     LOG("Binning", pDEBUG) 
//      << "dim = " << idim << ", value = " << x << " -> bin = " << binarr[idim];
//   }
//   return binarr;
// }
// //________________________________________________________________________________________
// unsigned int NoBinning::LinearizedBin(const vector<unsigned int> & binarr) const
// {
//   unsigned int linearized_bin = binarr[0];
//   for(unsigned int idim = 1; idim < fNDimensions; idim++) {
//       unsigned int nbins_subspace = 1;
//       for(unsigned int idim2 = 0; idim2 < idim; idim2++) {
//          nbins_subspace *= this->NBins(idim2);
//       }
//       linearized_bin += (nbins_subspace * binarr[idim]);
//   }
//   return linearized_bin;
// }
// //________________________________________________________________________________________
void NoBinning::Init(void)
{
  fKineSpace = conv::kspUndefined;
  fNBins = 1;
  fNDimensions = 0;
  fKineVars.clear();
  fKineVarToDim.clear();
  //fFixedWidth.clear();
  fMinimum.clear();
  fMaximum.clear();
  //fBinEdges.clear();
  //fBinRanges.clear();
  //fBin1DArrs.clear();
}
//________________________________________________________________________________________
void NoBinning::CleanUp(void)
{
  this->Init();
}
// //________________________________________________________________________________________
std::vector<TLine>  NoBinning::GetBinningTLines(void) const
{
  std::vector<TLine> lines;
  return lines;
}
//________________________________________________________________________________________
void NoBinning::Print(std::ostream & stream) const
{
  stream<<"This is has no binning" << std::endl;
}
//________________________________________________________________________________________
