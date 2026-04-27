//____________________________________________________________________________
/*!

\class     valor::GenericSingleSampleFitModel

brief      A generic implementation of the fit model for a single sample,
           in a specific beam configuration and for a specific detector.
           //
           // Some description about what info is stored for the sample
           //
           Derives from FitModelABC

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Steve Dennis <s.r.dennis@liverpool.ac.uk>
           University of Liverpool

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_generic_single_sample_model_h_
#define _valor_generic_single_sample_model_h_

#include <string>
#include <utility>

#include <TMatrixD.h>
#include <TObjArray.h>
#include <TTree.h>

//fmwk
#include "valor/Conventions/Detector.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/Sample.h"
#include "valor/Conventions/KineVar.h"
#include "valor/Conventions/FitParamApply.h"
#include "valor/Conventions/ModeList.h"
#include "valor/FitCore/CorrelatedSystematics.h"
#include "valor/FitCore/PhysHypothesis.h"
#include "valor/FitCore/PhysParamList.h"
#include "valor/FitCore/SingleSampleFitModelABC.h"
#include "valor/Oscillation/NuOsc3Flavour.h"
#include "valor/VArray/VALORArray.h"
#include "valor/VArray/VALORArrayF.h"
#include "valor/VArray/VALORArrayBin.h"
#include "valor/Utilities/RFL.h"
#include "valor/Utilities/ToyStream.h"
#include "valor/Utilities/UtlParam.h"
#include "valor/Utilities/SingleSampleKineBinning.h"

using std::string;

namespace valor {

// --------------------------------------

class GenericSingleSampleFitModel: public SingleSampleFitModelABC
{
public:

  GenericSingleSampleFitModel();
  virtual ~GenericSingleSampleFitModel();

  //
  // Implement the FitModelABC interface
  // See FitModelABC for method decription
  //

  string        Name                   (void) const;

  // Options available to Read() include nosplines, nomc, forcedst, pot_override, binning=myfile.xml
  bool          Read                     (string file, Option_t * opt = 0);
  virtual bool  Read                     (util::ToyStream* stream);
  bool          Generate                 (const PhysHypothesis & ph, Option_t * opt = 0);
  void          RandomizeSystematics     (FitParamList& fpl, Option_t * opt = 0);
  void          RandomizeSystematics     (bool (*should_randomize)(conv::PhysParamId_t), FitParamList& fpl, Option_t * opt = 0);
  void          RandomizeSystematics     (std::map<valor::conv::PhysParamId_t, bool> should_randomize,FitParamList& fpl,  Option_t * opt = 0);
  void          TweakSystematic          (string syst_name, double twkdial=0., Option_t * opt = 0);
  void          TweakSystematic          (conv::PhysParamId_t syst, double twkdial=0., Option_t * opt = 0);
  void          ResetSystematics         (FitParamList& fpl,Option_t * opt = 0);
  void          ForceSystematicsToUntuned(FitParamList& fpl,Option_t * opt = 0);
  void          PrintSystematics         (ostream & stream = std::cout, Option_t * opt = 0) const;
  PhysParamList Tweaks                   (Option_t * opt = 0) const;
  bool          Generate                 (const FitParamList & fp, Option_t * opt = 0);
  bool          Write                    (TObjArray * collection, const char * tag = "model", Option_t * write_opt = 0);
  bool          Write                    (const char * filename,  const char * tag = "model", Option_t * write_opt = 0, Option_t * file_opt = 0);
  virtual bool  Write                    (util::ToyStream* stream);
  virtual long  StreamSize               (Option_t * opt = 0) const;  // Get the size (in bytes) that would be used to read/write the VALORArrays from/to a binary toy file with the specified option
  double        GetFitStatisticPenalty   (Option_t * opt = 0) const;

  void          SetSystConf            (CorrelatedSystematics * syst, Option_t * opt = "");
  CorrelatedSystematics *       GetSystematics(Option_t * opt = "");
  const CorrelatedSystematics * GetSystematics(Option_t * opt = "") const;

  //
  // Auxiliary methods, not part of the  FitModelABC interface (needed?)
  //
  
  // Get the modelist we're using
  const valor::ModeList * GetModeList(Option_t * opt = "") const;
  const std::vector<conv::ReacMode_t> GetReactionModes(Option_t * opt = "") const;
  
  // Get MC DST
  TTree * GetDST() const;

  // Get POT
  const double & GetPOT() const;

  // Get Fiducial mass
  const double & GetFiducialMass(void) const;
  
  const std::string & GetLastConfigFileRead() const;

  // Get the response function library and binning
  const util::RFL * GetRFL(void) const;
  const SingleSampleKineBinning * GetBinning(void) const;
  
  PhysParamList OneSigmaErrors (Option_t * opt = "");
//  TMatrixD CovarianceMatrix(Option_t * opt = "");

  // Get a string including the model name and the total PDF integral
  virtual std::string GetPDFIntegralAsString(Option_t * opt = 0);

  // Calculate the OB manually from the content of the simple bins. Can be used to verify the spectra are the same for SB/OB
  void UpdateObservableBinningFromSimple (Option_t * opt = 0);
  
  // Add information on which systematic universes to read in if they're coming from the DST.
  // Must be called *before* Read() since these are event-by-event
  // If the list is unsorted, can pass a vector of IDs (for instance when writing response functions)
  // Can also just loop from first to last
  void SetUniversesToRead(const std::vector<unsigned int> &ids);
  void SetUniverseRangeToRead(size_t first, size_t last);
  void SetUniverseParameterToRead(const std::string & param_name);
  void ResetUniversesToRead();
  
  void SetSystematicVariationIDToApply(const std::string & param_name,size_t id);
  void ResetSystematicVariationIDs();
  
  // Initialise and return a new ToyStream object, with setting set appropriately for reading/writing using this class
  virtual util::ToyStream* InitToyStream(const string file_name, const string read_or_write = "read", string valor_array_format = "");

private:

  void Init  (void);
  void Book  (void);
  void Clean (void);
  void Zero  (void);
  void FillFromDST  (TTree * t);
  bool FillFromTemplateFile (TFile * f, std::string tag, double desired_POT); // If desired_POT is a positive number, assume template being read is 1E21 and reweight accordingly.
  bool FillSplines(string file, Option_t * opt);

  void ApplyTuning                       (Option_t * opt = 0);
  void FluctuateMCStat                   (Option_t * opt = 0);
  void ApplySystematics                  (Option_t * opt = 0);
  void ApplyPhysics                      (Option_t * opt = 0);
  void ApplyOscProb                      (Option_t * opt = 0);
  void UpdatePDFs                        (Option_t * opt = 0);
  void ApplyPostOscLateralSystematics    (VALORArray * pdf, bool simple_binning = false);
  void ApplyLateralSystematic            (VALORArray * pdf, conv::PhysParamId_t p, bool simple_binning = false);
  void ReadExtTuningHistsAll             (TXMLEngine* xmleng, XMLNodePointer_t top_node);
  void ReadTune                          (TXMLEngine* xmleng, XMLNodePointer_t top_node, std::vector< std::pair< std::pair<conv::ApplyMask_t, std::vector<conv::KineVar_t> >, TH1* > > * hist_vec, Option_t * opt = "");
  double GetExternalWeight               (std::map<conv::KineVar_t,double> kinematics, conv::ReacMode_t m, conv::NuclearTarget_t nt); // new function to read external weight containing histo

  // TODO: Should really be using a simple struct here instead of this unwieldy object
  // TODO: Put in the operation type into TuningInfo (e.g. multiply or divide), but turn TuningInfo into a struct first!
  typedef std::pair< std::pair<conv::ApplyMask_t, std::vector<conv::KineVar_t> >, TH1* > TuningInfo;

  // Apply bin-by-bin tuning to the input MC templates based on the ratio of the input tuning hists.
  // Loops over the input vectors of TuningInfo to determine which tunes to apply.
  void ApplyBinnedRatioTuningToTemplates (std::vector<TuningInfo> & numerator_vec, std::vector<TuningInfo> & denominator_vec,
                                          std::vector<std::string> templates_to_update);
  // Apply a single bin-by-bin tune
  void ApplyBinnedRatioTuningToTemplates (TH1* numerator_hist, TH1* denominator_hist, std::map<conv::ReacMode_t, bool> apply_to_mode,
                                          std::vector<conv::KineVar_t> kvs, std::vector<std::string> templates_to_update,
                                          bool use_simple_binning);

  // Multiply the content of each bin in the hist (within range) by the initial values of the input parameters, but only use params that are compatible with an input apply mask
  void ReweightHistUsingParamSet(TH1* hist_to_reweight, std::vector<conv::KineVar_t> kvs, CorrelatedSystematics* syst,
                                 std::string param_set_str, conv::ApplyMask_t mask, bool use_current_vals = true);

  void FillNonEmptyBinVectors(void);

  // There are many large vectors used when applying lateral systematics. They only depend on the PDF reco binning
  // and the applied kine var, so pre-compute them instead of calculating them for every event and mode.
  void PreComputeLateralSystVectors(bool use_simple_binning);
  
  // Keep track of the parameters we need to loop over
  // Normalisation, spline and post-oscillation lateral parameters divided
  std::vector<conv::PhysParamId_t> fNormParams;
  std::vector<conv::PhysParamId_t> fSplineParams;
  std::vector<conv::PhysParamId_t> fLateralPostOscParams;

  // Keep a list of the parameters for every given mode
  std::vector<std::vector<conv::PhysParamId_t>> fNormParamsAppliedTo  ;
  std::vector<std::vector<conv::PhysParamId_t>> fSplineParamsAppliedTo;
  
  // Fill these arrays
  void BuildParamLists();
  
  // Enable/disable Earth oscillation (baseline depends on zenith angle)
  // If disabled, uses fixed baseline from util::Baseline()
  void SetUseEarthOscillation(bool use_earth_osc) { fUseEarthOscillation = use_earth_osc; }
  bool GetUseEarthOscillation() const { return fUseEarthOscillation; }
  
  NuOsc3Flavour  fNuOsc3Flavour;
  PhysHypothesis fPhysHypothesis;

  // Name of this VALOR fit model (see Conventions/Names.h)
  string fName;

  // Exposure for corresponding sample (in POT)
  double fPOT;

  // Fiducial mass (ktonnes)
  double fFiducialMass;

  // Name of the config file read
  std::string fConfigFileRead;

  // For directly accessing DST Tree if needed
  TTree * fMCDST;

  // Input VALOR MC DSTs
  bool fUseMCDST;

  // Whether or not we should apply the osc prob to ALL detectors (i.e. skipping the util::OscProbIsAppliedTo check)
  bool fSkipOscProbApplyCheck;

  // Whether or not we should throw a warning message if any event read from an MC DST has kinematics
  // outside of the binning range
  bool fSuppressOutOfBinningRangeWarning;
  
  // Whether to use Earth oscillation (baseline depends on zenith angle) or fixed baseline
  // Only applies to atmospheric neutrino samples (beam = kbJUNO_Atmo)
  bool fUseEarthOscillation;
  
  // MC templates (MCT) 
  // Each template is a 2D array, with (reco,true) bins linearised in whatever kinematic space we need.
  // There is a template for all beam/sample/reac_mode combinations.
  // Four types of MC templates are constructed:
  // - nom: Nominal MC 
  // - tun: Nominal MC + Ext. tuning (eg tuning based on *external* hadro-production or neutrino cross-section data)
  // - sys: Nominal MC + Ext. tuning + Systematic param variations
  // - fin: Nominal MC + Ext. tuning + Systematic param variations + Physics param variations
  // All MC template contents are scaled to the desired data statistics.
  // We also construct an additional template with the MC statistics.
  // Note: 
  // The language/convention above (nom, tun, sys, fin) derives from our oscillation analysis.
  // All parameters we are handling here are "systematics" from the oscilation analysis point of view.
  // However, the following distrinction is made:
  // - Systematics "systematics". These are the ones we marginalize (eg detector systematics, uncorrelated cross-sections). 
  //   The effect of their variation is included in the sys templates.
  // - Physics "systematics" are the ones we attempt to constrain and are included in the output covariance matrix (eg. 
  //   correlated flux and cross-sections). The effect of their variation is included in the fin templates.
  //
  // Below are MCT with binning heavily optimized to reduce the CPU intensity of the fit.
  std::vector<bool        >fMCTNullOb;
  std::vector<VALORArray *>fMCTNomOb ; 
  std::vector<VALORArray *>fMCTTunOb ; 
  std::vector<VALORArray *>fMCTSysOb ; 
  std::vector<VALORArray *>fMCTFinOb ; 
  std::vector<VALORArray *>fMCStatOb ; // MC statistics 

  // We'll keep our response function pointers in a VALORArray of longs.
  // If we move to C++ 11, should use intptr_t instead.
  // This has an extra dimension above those  in the arrays above - parameter
  VALORArrayL * fRFPtr[conv::kNReacModes];
  
  // Below are MCT versions the use a simple (~fine and ~regular) binning throughout the whole range.
  // These templates are not used in the fit, but mainly for the visualization of the fit results.
  // As such, they are not all filled-in in each iteration of the fit but, typically, at the end of the fit or upon request.
  std::vector<bool        > fMCTNullSb;
  std::vector<VALORArray *> fMCTNomSb ; 
  std::vector<VALORArray *> fMCTTunSb ; 
  std::vector<VALORArray *> fMCTSysSb ; 
  std::vector<VALORArray *> fMCTFinSb ; 
  std::vector<VALORArray *> fMCStatSb ; // MC statistics
  
  // Holds L/E per event, if we're doing sterile oscillations
  std::vector<std::vector<std::vector<std::pair<double,double>> > > fLoverEPerEvent;


  // MC PDFs
  // Each PDF is (currently always) a 2-D Nev = Nev(Enu_reco, y_reco) array that can be directly compared with data.
  // The PDFs are computed from the `fin' MC templates, integrated over true kinematic variables, and include all effects.
  // There is a PDF for all beams/samples/reac_modes/bin_zones combinations.
  // The `Tot' versions are integrated over the true reac_modes and can be used for comparisons with data.
  //
  // Below are PDFs with binning heavily optimized to reduce the CPU intensity of the fit.
  bool                      fPDFTotNullOb; //
  VALORArray *              fPDFTotOb    ; // total
  std::vector<VALORArray *> fPDFCompOb   ; // components
  // Below are PDFs versions the use a simple (~fine and ~regular) binning throughout the whole range.
  bool         fPDFTotNullSb;           // 
  VALORArray * fPDFTotSb;               // total
  std::vector<VALORArray *> fPDFCompSb; // components
  
  // Binning
  SingleSampleKineBinning * fBinning;
  bool fSingleTrueBinning;
  
  // The list of reaction modes we're using
  ModeList* fML;
  std::vector<conv::ReacMode_t> fReactionModes;
  // and a mapping from these to their index in here
  std::vector<long> fReactionModeIndex;
  
  // Spline library
  util::RFL * fRFL;

  // Pointer to our systematics
  CorrelatedSystematics * fSystematics;
  bool fOwnerOfSystematics;
  
  // For implementing of event-by-event systematics
  // eg for making spectra that can be used for covariance matrices
  bool fApplyUniverseVariations;
  std::map<std::string,size_t> fSystUniverseToApply; // uses universe in DST, not universe in here.
  // For caching which universes to store so they can be rapidly applied
  std::vector<std::string> fUniverseParametersToSave;
  size_t fFirstUniverseToSave;
  size_t fLastUniverseToSave;
  std::vector<unsigned int> fUniversesToRead;
  bool fUniversesAsList;
  // and the actual variations
  // param name, mode(nmodes)->universe variations(reco,true,universe)
  std::map<std::string,std::vector<VALORArrayF> > fUniverseVariations;
  std::vector<VALORArrayF> fUniverseVariationProducts;

  // Vector of tuning hists and the apply masks and kine vars that they should be used for
  std::vector<TuningInfo> fEventByEventTuningHists;
  std::vector<TuningInfo> fBinByBinTuningHists;

  // Vector of non-empty bins in each MC template. Built as vec[imode][ibin] = Ntrue * ireco + itrue
  std::vector<std::vector<int>> fNonEmptyBinsOb;
  std::vector<std::vector<int>> fNonEmptyBinsSb;
  std::vector<int> fNumOfNonEmptyBinsOb;   // The number of non-empty bins in each mode
  std::vector<int> fNumOfNonEmptyBinsSb;

  // Vectors used when applying lateral systematics. Pre-compute these during Read().
  std::map<conv::KineVar_t, std::vector<double>             > fUnscaledBinEdgesOb;
  std::map<conv::KineVar_t, std::vector<double>             > fScaledBinEdgesOb;
  std::map<conv::KineVar_t, std::vector<bool>               > fIsUnscaledBinOverflowOb;
  std::map<conv::KineVar_t, std::vector<unsigned int>       > fScaledBinToGlobalBinOb;
  std::map<conv::KineVar_t, std::vector<unsigned int>       > fUnscaledBinToGlobalBinOb;
  std::map<conv::KineVar_t, std::vector<std::vector<bool> > > fSkipBinOb;
  std::map<conv::KineVar_t, unsigned int                    > fNOverflowBinsOb;

  // Same as above, but for the simple binning scheme
  std::map<conv::KineVar_t, std::vector<double>             > fUnscaledBinEdgesSb;
  std::map<conv::KineVar_t, std::vector<double>             > fScaledBinEdgesSb;
  std::map<conv::KineVar_t, std::vector<bool>               > fIsUnscaledBinOverflowSb;
  std::map<conv::KineVar_t, std::vector<unsigned int>       > fScaledBinToGlobalBinSb;
  std::map<conv::KineVar_t, std::vector<unsigned int>       > fUnscaledBinToGlobalBinSb;
  std::map<conv::KineVar_t, std::vector<std::vector<bool> > > fSkipBinSb;
  std::map<conv::KineVar_t, unsigned int                    > fNOverflowBinsSb;

  std::string fDefVALORArrayFormat; // The default VALORArray format used to read/write toys
};

} // valor

#endif

