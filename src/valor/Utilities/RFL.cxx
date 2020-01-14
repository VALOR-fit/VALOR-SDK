//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <iostream>
#include <iomanip>

#include "TMath.h"
#include "TString.h"
#include "TSystem.h"

#include "valor/Conventions/NuclearTarget.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/Env.h"
#include "valor/Utilities/UtlNumStat.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlXML.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/RFL.h"
#include "valor/Utilities/SingleSampleKineBinning.h"

#define USE_FLAT_SPLINE_FOR_MISSING

using std::endl;

using namespace valor;
using namespace valor::conv;
using namespace valor::util;

// Initialise the flat spline
static Double_t flat_knots_y[5] = {  1,  1, 1, 1,  1};
const util::Interpolator * valor::util::RFL::kFlatSpline =
  new util::Interpolator(TSpline3("flat_spline",-9E9,9E9,flat_knots_y,5));

// We'll never use this address to dereference - it can be passed as a flag indicating that no spline should exist.
const util::Interpolator* valor::util::RFL::kNoSplineRequired = (util::Interpolator*)&conv::kBadResult;

//________________________________________________________________________________________
namespace valor {
namespace util {
  std::ostream & operator<<(std::ostream & stream, const RFL & rfl)
  {
    rfl.Print(stream);
    return stream;
  }
}
}
//________________________________________________________________________________________
valor::util::RFL::RFL(void)
 :
fBinning       (NULL),
fSplines       (NULL),
fSplineBlockSize(0),
fBeam          (conv::kbUndefined),
fSample        (conv::ksUndefined),
fDetector      (conv::kdUndefined),
fNParams       (0),
fParams        (NULL),
fNBinsTrue     (1),
fNBinsReco     (1),
fSplineNamePattern("spl_[BEAM]_[SAMPLE]_[MODE]_[PARAM]_[ZONE]_[TRUE1]_[TRUE2]_[RECO1]_[RECO2]"),
fSplineWghtMethod(conv::kSplEvalUndefined),
fAllowNegativeSplines(false),
fSplineType(Interpolator::Type::kAKIMA),
fForceNegWghtToExists(false),
fForceNegWghtTo(0.0)
{
  
}
//________________________________________________________________________________________
valor::util::RFL::RFL(const SingleSampleKineBinning * binning, const std::vector<PhysParamId_t> & params)
 : RFL(binning,params.size(),params.data()) {}
//________________________________________________________________________________________
valor::util::RFL::RFL(const SingleSampleKineBinning * binning, int n_params, const PhysParamId_t * param_ids)
 :
fBinning       (binning),
fSplines       (NULL),
fSplineBlockSize(0),
fBeam          (fBinning->GetBeamConf()),
fSample        (fBinning->GetSample()),
fDetector      (fBinning->GetDetector()),
fModes         (fBinning->GetReactionModes()),
fNParams       (n_params),
fParams        (NULL),
fNBinsTrue     (1),
fNBinsReco     (1),
fSplineNamePattern("spl_[BEAM]_[SAMPLE]_[MODE]_[PARAM]_[TRUE]_[RECO]"),
fSplineWghtMethod(conv::kSplEvalUndefined),
fAllowNegativeSplines(false),
fSplineType(Interpolator::Type::kAKIMA),
fForceNegWghtToExists(false),
fForceNegWghtTo(0.0)
{
  fParams = new PhysParamId_t[fNParams];
  memcpy(fParams, param_ids, fNParams*sizeof(PhysParamId_t));
  
  // Fill parameter to index
  for (int ip = 0 ; ip < conv::knppmax ; ip++ ) {
    conv::PhysParamId_t p = (conv::PhysParamId_t)ip;
    fParam2Idx[p] = -1;
    for (int idx = 0 ; idx < fNParams; idx++) {
      if (fParams[idx]==p) {
        fParam2Idx[p] = idx;
      }
    }
  }
  
  const VALORBinning  * reco_ob = fBinning->XsecRecoBinning();

  fNBinsReco  = reco_ob->NBins();
  // Calculate max bin numbers and save the vector mapping mode back to index
  fReactionModeIndex.resize(conv::kNReacModes,-1);
  for (size_t im = 0 ; im < fModes.size() ; im++) {
    conv::ReacMode_t m = fModes[im];
    if (m == conv::kmUndefined) continue;
    const VALORBinning * true_sb = fBinning->XsecTrueBinning(m);
    fNBinsTrue = std::max(fNBinsTrue,true_sb->NBins());
    
    fReactionModeIndex[m] = im;
  }
}
//________________________________________________________________________________________
//~ valor::util::RFL::RFL(const RFL &) 
//~ {
//~ }
//________________________________________________________________________________________
valor::util::RFL::~RFL()
{
  for (unsigned int ispl = 0 ; ispl < fSplineBlockSize; ispl++) {
    if (fSplines[ispl] && fSplines[ispl]!=kFlatSpline && fSplines[ispl]!=kNoSplineRequired) {
       delete fSplines[ispl];
    }
    delete[] fSplines;
  }
  
  if (fParams       )  {delete[] fParams;       }
}
//________________________________________________________________________________________
void valor::util::RFL::Print(ostream & stream) const
{
  stream << "[response functions]" << endl;
}
//________________________________________________________________________________________
const util::Interpolator* valor::util::RFL::GetSpline (
  conv::ReacMode_t m, conv::PhysParamId_t p,
  const int & itrue, const int & ireco) const
{
  const int ip = this->Param2Idx(p);
  long im = fReactionModeIndex[m];
  const int splidx = SplineIndex(im,ip,itrue,ireco);
  
  return fSplines[splidx];
}
//________________________________________________________________________________________
TString valor::util::RFL::BuildSplineName (
  conv::ReacMode_t m, conv::PhysParamId_t p,
  const int& itrue, const int& ireco) const
{
  TString name = fSplineNamePattern;
  
  name = name.ReplaceAll("[BEAM]"   , util::AsString(fBeam  ).c_str());
  name = name.ReplaceAll("[SAMPLE]" , util::AsString(fSample).c_str());
  
  name = name.ReplaceAll("[MODE]"   , util::AsString(m).c_str());
  name = name.ReplaceAll("[PARAM]"  , util::NameRegistry::Instance()->AsString(p).c_str());
  
  name = name.ReplaceAll("[TRUE]" , TString::Format("%d", itrue ));
  name = name.ReplaceAll("[RECO]" , TString::Format("%d", ireco ));
  
  return name;
}
//________________________________________________________________________________________
bool valor::util::RFL::Read(std::string config_file, Option_t * /*option*/)
{
  // Get binning details, allocate the spline block
  unsigned long n_modes = fModes.size();
  fSplineBlockSize = n_modes * fNParams * fNBinsTrue * fNBinsReco;
    
  LOG("RFL",pDEBUG) << "Spline binning for sample "<<util::AsString(fBeam)<<" "<<util::AsString(fSample)<<"\n"
   <<std::setw(7)<<fModes.size()<<" modes.\n"
   <<std::setw(7)<<fNParams<<" parameters with splines.\n"
   <<std::setw(7)<<fNBinsTrue<<" true bins.\n"
   <<std::setw(7)<<fNBinsReco<<" reco bins \n"
   <<std::setw(7)<<fSplineBlockSize<<" maximum.";
    
  fSplines = new const util::Interpolator*[fSplineBlockSize]();

  TString ts_file = config_file.c_str();
  gSystem->ExpandPathName(ts_file);

  // Check for config file.
  // See if it works as a normal path
  FileStat_t dummy;
  if (gSystem->GetPathInfo(ts_file.Data(), dummy)) 
  {
    // If not, try the config folder
    ts_file = TString::Format("%s/%s",Env::Instance()->TopLevelConfDir().c_str(),ts_file.Data());
    if (gSystem->GetPathInfo(ts_file.Data(), dummy)) {
      LOG("RFL",pERROR) << "Config file not found: "<<ts_file;
      util::Die(1); return false;
    }
  }
  TXMLEngine* xml = new TXMLEngine;   
  XMLDocPointer_t xmldoc = xml->ParseFile(ts_file.Data());
  if (xmldoc==0) {
    LOG("RFL",pERROR) << "Can not parse input XML file [" << ts_file << "]";
    delete xml;
    util::Die(1); return false;
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);
  
  string folder = util::XML2String(xml,root_node,"splines/folder");
  if (!folder.size()) {
    LOG("RFL",pFATAL) << "Failed to read spline folder from XML.";
    util::Die(-1); return false;
  }
  
  string spl_file_pattern           = util::XML2String(xml,root_node,"splines/pattern");
  string spl_stored_as              = "";
  string spl_array_pattern          = "";
  bool use_spline_limits            = true;
  bool reorder_out_of_order_splines = false;
  if (util::NodeExists(xml,root_node,"splines/stored_as") && util::NodeExists(xml,root_node,"splines/array_pattern")) {
    spl_stored_as     = util::XML2String(xml,root_node,"splines/stored_as");
    spl_array_pattern = util::XML2String(xml,root_node,"splines/array_pattern");
    LOG("RFL",pDEBUG) << "Splines stored as: [" << spl_stored_as << "]";
    LOG("RFL",pDEBUG) << "Splines array pattern: [" << spl_array_pattern << "]";
  }
  else {
    LOG("RFL",pDEBUG) << "Reading individually stored splines.";
  }


  if (util::NodeExists(xml,root_node,"splines/method")) {
    std::string spline_eval_type_str  = util::XML2String(xml,root_node,"splines/method");
    if(spline_eval_type_str != "") { fSplineWghtMethod = util::SplEvalTypeFromString(spline_eval_type_str); }
    else                           { fSplineWghtMethod = conv::kSplEvalCurrVal; }
  }
  else {
    fSplineWghtMethod = conv::kSplEvalCurrVal;
    LOG("RFL",pWARN) << "Spline evaluation method not specified, defaulting to ["
                     << util::AsString(fSplineWghtMethod) << "]";
  }
  LOG("RFL",pDEBUG) << "Spline evaluation method: [" << util::AsString(fSplineWghtMethod) << "]";

  if (util::NodeExists(xml,root_node,"splines/allow_negative")) {
    fAllowNegativeSplines = util::XML2Bool(xml,root_node,"splines/allow_negative");
    LOG("RFL",pDEBUG) << "Allowing splines to go negative? " << fAllowNegativeSplines;
    if (util::NodeExists(xml,root_node,"splines/set_neg_wght_to")) {
      fForceNegWghtToExists = true;
      fForceNegWghtTo = util::XML2Dbl(xml,root_node,"splines/set_neg_wght_to");
      LOG("RFL",pDEBUG) << "  ..but forcing negative weights to " << fForceNegWghtTo;
    }
  }

  if (util::NodeExists(xml,root_node,"splines/within_limits")) {
    use_spline_limits = util::XML2Bool(xml,root_node,"splines/within_limits");
    LOG("RFL",pDEBUG) << "Forcing splines to stay within their limits? " << use_spline_limits;
  }

  if (util::NodeExists(xml,root_node,"splines/sort_knot_order")) {
    reorder_out_of_order_splines = util::XML2Bool(xml,root_node,"splines/sort_knot_order");
    LOG("RFL",pDEBUG) << "Reorder out-of-order splines? " << reorder_out_of_order_splines;
  }

  if (util::NodeExists(xml,root_node,"splines/type")) {
    std::string type_str = util::XML2String(xml,root_node,"splines/type");
    type_str = util::ToLowerCase(util::RemoveAllWhitespace(type_str));
         if(type_str == "linear")         { fSplineType = Interpolator::Type::kLINEAR;          }
    else if(type_str == "polynomial")     { fSplineType = Interpolator::Type::kPOLYNOMIAL;      }
    else if(type_str == "cubic")          { fSplineType = Interpolator::Type::kCSPLINE;         }
    else if(type_str == "cubic_periodic") { fSplineType = Interpolator::Type::kCSPLINE_PERIODIC;}
    else if(type_str == "akima")          { fSplineType = Interpolator::Type::kAKIMA;           }
    else if(type_str == "akima_periodic") { fSplineType = Interpolator::Type::kAKIMA_PERIODIC;  }
    else if(type_str == "tspline3")       { fSplineType = Interpolator::Type::kTSpline3;        }
    else if(type_str == "tspline5")       { fSplineType = Interpolator::Type::kTSpline5;        }
    else {
     LOG("RFL",pFATAL) << "Did not recognise spline type [" << type_str << "]";
     util::Die(-1); return false;
    }
  }
  LOG("RFL",pDEBUG) << "Spline are of type: [" << util::AsString(fSplineType) << "]";

  if (!spl_file_pattern.size()) {
    LOG("RFL",pFATAL) << "Failed to open spline filename pattern from XML.";
    util::Die(-1); return false;
  }
  
  vector<TFile*> spline_files;
  for (int ip = 0 ; ip < fNParams ; ip++) {
    conv::PhysParamId_t p = fParams[ip];
    if(p == conv::kPhypUndefined) continue;
    TString filename = spl_file_pattern.c_str();
    filename = filename.ReplaceAll("[BEAM]"   , util::AsString(fBeam    ).c_str());
    filename = filename.ReplaceAll("[SAMPLE]" , util::AsString(fSample  ).c_str());
    filename = filename.ReplaceAll("[DET]"    , util::AsString(fDetector).c_str());
    filename = filename.ReplaceAll("[PARAM]"  , util::NameRegistry::Instance()->AsString(p).c_str());
    
    TString base_dir = Env::Instance()->TopLevelDataDir().c_str();
    
    TFile * spline_file = new TFile((base_dir+"/"+folder.c_str()+"/"+filename).Data());
    LOG("RFL",pINFO) << "Will read splines from file "<<spline_file->GetName();
    spline_files.push_back(spline_file);
  }

  int n_splines_read = 0;
  int n_splines_missing = 0;
  int n_splines_expected = fSplineBlockSize;
  int n_splines_flat = 0;
  int n_splines_neg = 0;

  conv::NuclearTarget_t nt = conv::kntInclusive; // Make this an argument

  for(size_t imode = 0; imode < fModes.size(); imode++) {
    conv::ReacMode_t m = fModes[imode];
    if(m == conv::kmUndefined) continue;
    
    const VALORBinning * true_sb = fBinning->XsecTrueBinning(m);
    const int n_bins_true = true_sb->NBins();
    const int n_bins_reco = fNBinsReco;
    
    for (int ip = 0 ; ip < fNParams ; ip++) {
      conv::PhysParamId_t p = fParams[ip];
      if(p == conv::kPhypUndefined) continue;

      const Parameter & param = ParameterPool::Instance()->GetParam(p);
      bool param_applied_to_mode = util::IsAppliedTo(param.GetApplyMask(),fBeam,fSample,fDetector,m,nt);

      TObjArray* spline_array = nullptr;
      if(spl_stored_as == "TObjArray") {
        // Splines are not stored individually in the TFiles, but in TObjArrays
        TString array_name = spl_array_pattern.c_str();
        array_name = array_name.ReplaceAll("[PARAM]", util::NameRegistry::Instance()->AsString(p).c_str());
        array_name = array_name.ReplaceAll("[MODE]" , util::AsString(m).c_str());
        for (unsigned int ifile = 0; ifile < spline_files.size(); ifile++) {
          TFile *spline_file = spline_files[ifile];
          LOG("RFL", pDEBUG) << "Searching for spline array [" << array_name << "] in file ["
                                 << spline_file->GetName() << "]...";
          spline_array = static_cast<TObjArray*>(spline_file->Get(array_name.Data()));
          if (spline_array) {
            LOG("RFL",pDEBUG)<<"   ...found";
            break;
          }
        }
      }
      
      for (int ireco = 0 ; ireco < n_bins_reco ; ireco++) {
        for (int itrue = 0 ; itrue < n_bins_true ; itrue++) {
          TString spl_name = this->BuildSplineName(m,p,itrue,ireco);
          
          const UInt_t spline_index = SplineIndex(imode,ip,itrue,ireco);
          if (spline_index >= fSplineBlockSize) {
            LOG("RFL",pFATAL) << "Bad spline index "<<spline_index; 
            util::Die(-1); return false;
          }
          
          if (param_applied_to_mode) {
            TObject * spline_obj = NULL;
            // Read each file till we get one that contains our spline
            if(spl_stored_as == "TObjArray") {
              // Splines are not stored individually in the TFiles, but in TObjArrays
              int in_idx = itrue * n_bins_reco + ireco;
              spline_obj = static_cast<TSpline3*>(spline_array->At(in_idx));
              if (!spline_obj) {
                LOG("RFL",pWARN)<<"Expected a spline at index: "<<in_idx<<" of array "<<spline_array->GetName()
                                    << ", but could not find it";
              }
              else {
                LOG("RFL", pDEBUG) << "Found spline named [" << spline_obj->GetName() << "] at itrue = "
                                   << itrue << ", ireco = " << ireco << ", mode = " << util::AsString(m)
                                   << ", param = " << NameRegistry::Instance()->AsString(p)
                                   <<  ", in array named [" << spline_array->GetName() << "]";
              }
            }
            else{
              // Splines are stored individually in the TFiles
              for (unsigned int ifile = 0; ifile < spline_files.size(); ifile++) {
                TFile *spline_file = spline_files[ifile];
                LOG("RFL", pDEBUG) << "Searching for spline [" << spl_name << "] in file ["
                                       << spline_file->GetName() << "]...";
                spline_obj = spline_file->Get(spl_name.Data());
                if (spline_obj) {
                  LOG("RFL",pDEBUG)<<"   ...found";
                  break;
                }
              }
            }
            TSpline* spline = dynamic_cast<TSpline*>(spline_obj);
            if (spline) {
              if (util::SplineIsFlat(spline)) {
                fSplines[spline_index] = kFlatSpline;
                n_splines_flat++;
                n_splines_read++;
                delete spline;
                continue;
              }

              // The input spline doesn't necessarily have the knots in the right order, so sort them
              this->SortSpline(dynamic_cast<TSpline3*>(spline), reorder_out_of_order_splines);

              util::Interpolator *  valor_spline = nullptr;
              valor_spline = new util::Interpolator(spline, fSplineType);
              valor_spline->SetUseLimits(use_spline_limits);

              delete spline;
              bool use_valor_spline = true;
              if(!fAllowNegativeSplines) {
                if (this->CheckForNegativeValues(valor_spline)) {
                  LOG("RFL",pWARN)<<"Spline has negative values: "<<spl_name<<" - using flat spline instead.";
                  delete valor_spline;
                  fSplines[spline_index] = kFlatSpline;
                  n_splines_neg++;
                  n_splines_read++;
                  use_valor_spline = false;
                }
              }
              if(use_valor_spline) {
                fSplines[spline_index] = valor_spline;
                n_splines_read++;
              }
            }
            else {
              LOG("RFL",pWARN)<<"Spline not found: "<<spl_name;
#ifdef USE_FLAT_SPLINE_FOR_MISSING
              fSplines[spline_index] = kFlatSpline;
#else
              fSplines[spline_index] = NULL;
#endif
              n_splines_missing++;
            }
          }
          else {
            fSplines[spline_index] = kNoSplineRequired;
            n_splines_expected--;
          }
        } // itrue
      } //ireco
    } //ip
  } //imode
  
  for (unsigned int ifile = 0; ifile < spline_files.size(); ifile++) {
    TFile * spline_file = spline_files[ifile];
    spline_file->Close();
    delete spline_file;
  }
  
  LOG("RFL",pINFO)<<"Read "<<n_splines_read<<" splines, of which "<<n_splines_flat<<" were flat.";
  if (n_splines_neg) {
    LOG("RFL",pWARN) << "Skipped "<<n_splines_neg<<" splines due to negative values. Fix this!";
  }

  if (n_splines_missing) {
#ifdef USE_FLAT_SPLINE_FOR_MISSING
    LOG("RFL",pERROR) << "Missing "<<n_splines_missing<<" splines of "<<n_splines_expected<<" expected. Continuing.";
#else
    LOG("RFL",pFATAL) << "Missing "<<n_splines_missing<<" splines of "<<n_splines_expected<<" expected. Exiting.";
    util::Die(-1);return false;
#endif
  }
  
  xml->FreeDoc(xmldoc);
  delete xml;
  
  return true;
}
//________________________________________________________________________________________
std::pair<int,int> valor::util::RFL::GetBin(const util::KinematicPoint & kp, conv::ReacMode_t m) const
{
  const VALORBinning * true_binning = fBinning->XsecTrueBinning(m);
  const VALORBinning * reco_binning = fBinning->XsecRecoBinning();
  
  int nrecodim = reco_binning->NDimensions();
  std::vector<double> recovec;
  if (nrecodim > 0) { recovec.push_back(kp.reco1); }
  if (nrecodim > 1) { recovec.push_back(kp.reco2); }
  if (nrecodim > 2) { recovec.push_back(kp.reco3); }
  
  int ntruedim = true_binning->NDimensions();
  std::vector<double> truevec;
  if (ntruedim > 0) { truevec.push_back(kp.true1); }
  if (ntruedim > 1) { truevec.push_back(kp.true2); }
  if (ntruedim > 2) { truevec.push_back(kp.true3); }
  
  const int itrue = true_binning->FindBin(truevec);
  const int ireco = reco_binning->FindBin(recovec);
  return std::pair<int,int>(itrue,ireco);
}
//________________________________________________________________________________________
int valor::util::RFL::Param2Idx(conv::PhysParamId_t p) const
{
  if (p < 0 || p > conv::knppmax) {
    LOG("RFL",pFATAL) << "Parameter out of range "<<p<<"\t"<<util::NameRegistry::Instance()->AsString(p).c_str();
  }
  return fParam2Idx[p];
}
//________________________________________________________________________________________
void valor::util::RFL::SetSplineNamePattern(std::string pattern) {
  fSplineNamePattern = pattern.c_str();
}
//________________________________________________________________________________________
bool valor::util::RFL::CheckForNegativeValues(const util::Interpolator * spl)
{
  double min_val = spl->Xmin();
  double max_val = spl->Xmax();
  static const int evaluations = 30;
  for (int i = 0 ; i < evaluations; i++) {
    double x = min_val + (i+0.5)*(max_val-min_val)/evaluations;
    if (spl->Eval(x) < 0) {return true;}
  }
  return false;
}
//________________________________________________________________________________________
void valor::util::RFL::SortSpline(TSpline3 * spline, bool sort_spline)
{
  bool knots_in_order = true;
  int n_knots = spline->GetNp();
  double this_x = -conv::kALargeNum;

  for (int i = 0 ; i < n_knots; i++) {
    double x = 0;
    double y = 0;
    spline->GetKnot(i,x,y);
    if (x < this_x) {
      knots_in_order = false;
      break;
    }
    this_x = x;
  }

  if(!knots_in_order){
    if(sort_spline) {
      std::map<double,double> knots;
      std::vector<double> x_vals;
      for (int i = 0 ; i < n_knots; i++) {
        double x = 0;
        double y = 0;
        spline->GetKnot(i,x,y);
        knots[x] = y;
        x_vals.push_back(x);
      }

      std::sort(x_vals.begin(), x_vals.end());
      for (int i = 0 ; i < n_knots; i++) {
        spline->SetPoint(i, x_vals[i], knots.at(x_vals[i]));
      }
    }
    else {
      LOG("RFL",pWARN) << "Spline [" << spline->GetName()
                           << "] has knots that are out-of-order and the option to sort them was not specified";
    }
  }
}
//________________________________________________________________________________________
