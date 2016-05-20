//____________________________________________________________________________
/*
 \copyright (c) 2010-2016, The VALOR group.
 
 >> ChangeLog:
 
 */
//____________________________________________________________________________

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <string>

#include "Utilities/RunOpt.h"
#include "Utilities/Rnd.h"

#include "Conventions/Opt.h"
#include "Conventions/Constants.h"
#include "Conventions/PhysParamId.h"
#include "Utilities/Env.h"

#include "TMath.h"
#include "TString.h"

using std::cout;
using std::cerr;
using std::endl;
using std::setw;

using namespace valor;
using namespace valor::conv;

//____________________________________________________________________________
namespace valor {
  ostream & operator << (ostream & stream, const RunOpt & opt)
  {
    opt.Print(stream);
    return stream;
  }
}
//____________________________________________________________________________
valor::RunOpt::RunOpt()
{
  Init();
}
//____________________________________________________________________________
valor::RunOpt::RunOpt(int argc, char ** argv, int parseopt)
{
  Init();
  ReadFromCommandLine(argc, argv, parseopt);
  Print(cout);
}
//____________________________________________________________________________
valor::RunOpt::~RunOpt()
{
  delete fParser;
}
//____________________________________________________________________________
/* uncomment this if we want to make the class a singleton again (also remove the public constructor/destructor)
 RunOpt * valor::RunOpt::fInstance = 0;
 //____________________________________________________________________________
 valor::RunOpt::RunOpt()
 {
 fInstance = 0;
 
 this->Init();
 }
 //____________________________________________________________________________
 valor::RunOpt::~RunOpt()
 {
 fInstance = 0;
 }
 //____________________________________________________________________________
 RunOpt * valor::RunOpt::Instance()
 {
 if(fInstance == 0) {
 static valor::RunOpt::Cleaner cleaner;
 cleaner.DummyMethodAndSilentCompiler();
 fInstance = new RunOpt;
 }
 return fInstance;
 }
 */
//____________________________________________________________________________
void valor::RunOpt::Init(void)
{
  fParser     = 0;
  fParseType  = kParseUndefined;
  fHierStr[0] = "nh";
  fHierStr[1] = "ih";
  //common options
  fConfigFile      = util::FormFilePath("", "config");
  fVerbosityFile   = util::FormFilePath("verbosity.xml", "config");
  fFileTag         = "tag";
  fRun             = 0;
  //toy options
  fFirstExperiment     = 0;
  fLastExperiment      = 0;
  fNExperiments        = 1;
  fStatFluctuations    = true;
  fSystTweakMode       = 1;
  fRethrowIfUnphysical = true;
  fVerboseSystThrows   = false; // Print out the output systematic tweaks
  fVerboseFit   = false; // Print out the output systematic tweaks
  //global options
  fGlobalDmsqScheme     = "auto";
  fGlobalMassHierarchy  = "normal";
  fGlobalMixParamScheme = "sinsqtheta";
  fGlobalMixParamOctant = "one";
  fGlobalCPVPhaseScheme = "delta";
  //3f osc options
  InitOscParams(-99);
  fHypothesis = "3f_osc";
  
  // Seeds
  fSeedToyMCstat = 0;
  fSeedToyMCsyst = 0;
  fSeedToyMCosc  = 0;
  fSeedFitter    = 0;
  fSeedOther     = 0;
  
  
  // Priors
  
  fPriorId = conv::kBadResult;
  
  fPriorsFile = ""; //use this for default OFF
  //fPriorsFile = util::FormFilePath("priors/joint_T2Kreact_run14/Priors_T2Kreact_run14_Mach3_22Maj15.root", "inputs"); //use this for default ON
  
  fThrowMH       = false;
  fPriorProbNH  = 0.5;
  
  // NEW
  fMargMixing12 = false;
  fMargMixing13 = false;
  fMargMixing23 = false;
  fMargDm12sq   = false;
  fMargDm23sq   = false;
  fMargDeltaCP  = false;
  
  fPriorMixing12 = "";
  fPriorMixing13 = "";
  fPriorMixing23 = "";
  fPriorDm12sq   = "";
  fPriorDm23sq   = "";
  fPriorDeltaCP  = "";
  
  fMixModeMixing12 = "";
  fMixModeMixing23 = "";
  fMixModeMixing13 = "";
  
  for(int i=0;i<2;i++){
    fPriorParMixing12[i] = conv::kBadResult;
    fPriorParMixing13[i] = conv::kBadResult;
    fPriorParMixing23[i] = conv::kBadResult;
    fPriorParDm12sq[i]   = conv::kBadResult;
    fPriorParDm23sq[i]   = conv::kBadResult;
    fPriorParDeltaCP[i]  = conv::kBadResult;
  }
  fSynchronizeIHtoNH = true;
  
  // fit marg options
  fXmlfile          = "";
  fRootfile         = "";
  fUsing_xml_file   = false;
  fDoToys           = true;
  fReadSKExp        = false;
  fFit_param_list   = "nuosc3f+nuisance";

  fInfilename       = "";
  fInfilename_nh    = "";
  fInfilename_ih    = "";
  fDoFC             = false;

  fNfiles           = 9;
  fNtoysMarg        = 100;
  fFreeparname0     = "deltacp";
  fFreeparmin0      = -3.14159265358979312e+00;
  fFreeparmax0      = +3.14159265358979312e+00;
  fNptScan0         = 9;
  fFreeparname1     = "";
  fFreeparmin1      = 0.;
  fFreeparmax1      = 0.;
  fNptScan1         = 1;
  fDoMargMH         = false;
  fHierarchy_marg   = "normal";
  fDoMinimize       = true;
  fTypeError        = "";
  fDrawFullChisq    = true;
  fTeststat         = "rate+shape";
  fStoreLikeWeights = false;
  fPenaltyTerm0     = false;
  fPenaltyTerm1     = false;
}
//____________________________________________________________________________
void valor::RunOpt::InitOscParams(double value)
{
  for(int i = 0; i < 2; i++) {
    fMix12 [i] = value;
    fMix13 [i] = value;
    fMix23 [i] = value;
    fCPV   [i] = value;
    fDm12sq[i] = value;
    fDm23sq[i] = value;
    fMix12Bar [i] = value;
    fMix13Bar [i] = value;
    fMix23Bar [i] = value;
    fCPVBar   [i] = value;
    fDm12sqBar[i] = value;
    fDm23sqBar[i] = value;
  }//i
  fBetaBar     = value;
  
  //randomizing the prior makes no sense, so only set it if it's not -999
  //if(! (TMath::Abs(value + 999) < kASmallNum))
  //fPriorProbNH = value;
}
//____________________________________________________________________________
void valor::RunOpt::ReadFromCommandLine(int argc, char ** argv, int parseopt)
{
  //save which options we're using
  fParseType = parseopt;
  if(fParseType & kParseUndefined) {
    cerr << "kParseUndefined given to valor::RunOpt::ReadFromCommandLine()" << endl;
    exit(-1);
  }
  
  //get the parser
  fParser = new CmdLnArgParser(argc,argv);
  
  if(fParser->OptionExists("help") || fParser->OptionExists('h')) {
    Usage();
    exit(1);
  }
  
  //read the options of each group
  if(parseopt & kParseCommon)
    ReadCommonOptions();
  if(parseopt & kParseToys)
    ReadToyOptions();
  if(parseopt & kParseGlobal)
    ReadGlobalOptions();
  if(parseopt & kParse3FOscParams)
    Read3FOscParamsOptions();
  
  // NEW FIT MARG
  if(parseopt & kParseFitMarg)
    ReadFitMargOptions();
  //
}
//____________________________________________________________________________
void valor::RunOpt::ReadCommonOptions()
{
  // Set file tag
  if(fParser->OptionExists("file-tag")){
    fFileTag = fParser->ArgAsString("file-tag");
  }
  else{
    cerr << endl;
    cerr << "Common options:" << endl;
    cerr << "You must pass the tag of the file" << endl;
    cerr << "Use option --file-tag" << endl;
    cerr << endl;
    exit(1);
  }
  
  // Set run
  if(fParser->OptionExists("run")){
    fRun = fParser->ArgAsInt("run");
  }
  else{
    cerr << endl;
    cerr << "Common options:" << endl;
    cerr << "You must pass the run file" << endl;
    cerr << "Use option --run" << endl;
    cerr << endl;
    exit(1);
  }
  
  // Set configuration file
  if(fParser->OptionExists("config-file")){
    fConfigFile = util::FormFilePath(fParser->ArgAsString("config-file").c_str(), "config");
  }
  else{
    cerr << endl;
    cerr << "Common options:" << endl;
    cerr << "You must pass the configuration file" << endl;
    cerr << "Use option --config-file" << endl;
    cerr << endl;
    exit(1);
  }
  
  // Set verbosity file
  if(fParser->OptionExists("verbosity-file")){
    fVerbosityFile = util::FormFilePath(fParser->ArgAsString("verbosity-file").c_str(), "config");
  }
  
    
  // seeds
  if (fParser->OptionExists("seed-toys-stat")) {
    fSeedToyMCstat = fParser->ArgAsUInt("seed-toys-stat");
  }
  else {
    fSeedToyMCstat = 0;
  }
  
  if (fParser->OptionExists("seed-toys-syst")) {
    fSeedToyMCsyst = fParser->ArgAsUInt("seed-toys-syst");
  }
  else {
    fSeedToyMCsyst = 0;
  }
  
  if (fParser->OptionExists("seed-toys-osc")) {
    fSeedToyMCosc = fParser->ArgAsUInt("seed-toys-osc");
  }
  else {
    fSeedToyMCosc = 0;
  }
  
  if (fParser->OptionExists("seed-fitter")) {
    fSeedFitter = fParser->ArgAsUInt("seed-fitter");
  }
  else {
    fSeedFitter = 0;
  }
  
  if (fParser->OptionExists("seed-misc")) {
    fSeedOther = fParser->ArgAsUInt("seed-misc");
  }
  else {
    fSeedOther = 0;
  }
  
  // Should this be done here? Or in macro code.
  Rnd::Instance()->SetSeeds(fSeedToyMCstat,fSeedToyMCsyst,fSeedToyMCosc,fSeedFitter,fSeedOther);
  
}
//____________________________________________________________________________
void valor::RunOpt::ReadToyOptions()
{
  
  // Set experiment id and # of experiments
  if( !fParser->OptionExists("first-experiment") ||
     !fParser->OptionExists("n-experiments")
     ) {
    cerr << endl;
    cerr << "Toy options:" << endl;
    cerr << "You must set the first experiment id using: --first-experiment" << endl;
    cerr << "and the total number of experiments using: --n-experiments" << endl;
    cerr << endl;
    exit(1);
  }
  else{
    fNExperiments = fParser->ArgAsInt("n-experiments");
    fFirstExperiment = fParser->ArgAsInt("first-experiment");
    fLastExperiment = fFirstExperiment + fNExperiments - 1;
  }
  
  // Set statistical fluctuations mode
  if(fParser->OptionExists("stat-fluct")) {
    int stafluct_integer =  fParser->ArgAsInt("stat-fluct");
    fStatFluctuations = (bool)stafluct_integer;
    if(stafluct_integer<0 || stafluct_integer>1){
      cerr << endl;
      cerr << "Toy options:" << endl;
      cerr << "You set a wrong statistical fluctuation mode" << endl;
      cerr << "Options --stat-fluct can have only the following input parameter:" << endl;
      cerr << " 0 --> no statistical fluctuations" << endl;
      cerr << " 1 --> make statistical fluctuations" << endl;
      cerr << endl;
      exit(1);
    }
  }
  else{
    cerr << endl;
    cerr << "Toy options:" << endl;
    cerr << "You didn't set the statistical fluctuations mode" << endl;
    cerr << "Use options --stat-fluct with the following input parameter:" << endl;
    cerr << " 0 --> no statistical fluctuations" << endl;
    cerr << " 1 --> make statistical fluctuations" << endl;
    cerr << endl;
    exit(1);
  }
  
  // Set systematic tweak mode
  if(fParser->OptionExists("syst-tweak-mode")){
    fSystTweakMode = fParser->ArgAsInt("syst-tweak-mode");
    // The check of the systematic tweak is done in UtlForMacros.cxx (T2K trunk)
    // Also it's different depending on the analysis framework
  }
  else{
    cerr << endl;
    cerr << "Toy options:" << endl;
    cerr << "You didn't set the tweak systematic mode" << endl;
    cerr << "Use options --syst-tweak-mode with the following input parameter:" << endl;
    cerr << " 0 --> no systematic tweak" << endl;
    cerr << " 1 --> do systematic tweak" << endl;
    cerr << endl;
    exit(1);
  }
  
  // Set retrhowing of unphysical toys mode
  if( fParser->OptionExists("rethrow-unphysical-toys") ){
    fRethrowIfUnphysical = (bool) fParser->ArgAsInt("rethrow-unphysical-toys");
  }
  else{
    cerr << endl;
    cerr << "Toy options:" << endl;
    cerr << "You must enable or disable the rethrowing of unphysical toys" << endl;
    cerr << "Use --rethrow-unphysical-toys with inputs 0 or 1" << endl;
    cerr << endl;
    exit(1);
  }
  
  // Set verbosity of systematic throws
  if(fParser->OptionExists("verbose-syst-throws")){
    fVerboseSystThrows = (bool) fParser->ArgAsInt("verbose-syst-throws");
  }
  else{
    cerr << endl;
    cerr << "Toy options:" << endl;
    cerr << "You didn't set the option --verbose-syst-throws (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }

  // Set verbosity of fits
  if(fParser->OptionExists("verbose-fit")){
    fVerboseFit = (bool) fParser->ArgAsInt("verbose-fit");
  }
  else{
    cerr << endl;
    cerr << "Toy options:" << endl;
    cerr << "You didn't set the option --verbose-fit (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }
}
//____________________________________________________________________________
void valor::RunOpt::ReadGlobalOptions()
{
  if(fParser->OptionExists("dmsq-scheme")){
    fGlobalDmsqScheme = fParser->ArgAsString("dmsq-scheme");
  }
  else{
    cerr << endl;
    cerr << "Global options:" << endl;
    cerr << "You must set the dmsq scheme using --dmsq-scheme" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("mix-param-scheme")){
    fGlobalMixParamScheme = fParser->ArgAsString("mix-param-scheme");
  }
  else{
    cerr << endl;
    cerr << "Global options:" << endl;
    cerr << "You must set the mixing parameters scheme using --mix-param-scheme" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("cpv-phase-scheme")){
    fGlobalCPVPhaseScheme = fParser->ArgAsString("cpv-phase-scheme");
  }
  else{
    cerr << endl;
    cerr << "Global options:" << endl;
    cerr << "You must set the cpv scheme using --cpv-phase-scheme" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("mass-hierarchy")){
    fGlobalMassHierarchy = fParser->ArgAsString("mass-hierarchy");
  }
  else{
    cerr << endl;
    cerr << "Global options:" << endl;
    cerr << "You must set the mass hierarchy using --mass-hierarchy" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("mix-param-octant")){
    fGlobalMixParamOctant = fParser->ArgAsString("mix-param-octant");
  }
  else{
    cerr << endl;
    cerr << "Global options:" << endl;
    cerr << "You must set the mixing parameter octant using --mix-param-octant" << endl;
    cerr << endl;
    exit(1);
  }
  
  cout << "ReadGlobalOptions" << endl;
  
  Opt::Instance()->SetValues(fGlobalDmsqScheme.c_str(),     fGlobalMassHierarchy.c_str(),
                             fGlobalMixParamScheme.c_str(), fGlobalMixParamOctant.c_str(),
                             fGlobalCPVPhaseScheme.c_str());
}
//____________________________________________________________________________
void valor::RunOpt::Read3FOscParamsOptions()
{
  
  if(fParser->OptionExists("hypothesis")){
    fHypothesis = fParser->ArgAsString("hypothesis");
  }
  else{
    cerr << endl;
    cerr << "3FOscParams options:" << endl;
    cerr << "You didn't set the physics hypothesis" << endl;
    cerr << "Use option --hypothesis" << endl;
    cerr << endl;
    exit(1);
  }
  
  //
  // Keep the following order:
  // - Set the priors: 0 --> fixed osc params (default), 1 --> T2K+react, 2 --> T2K only
  // - set each single osc params (instead of default one). If prior is set, the parameter it's fixed anymore
  //
  
  if(fParser->OptionExists("synch-osc-ih-to-nh")){
    fSynchronizeIHtoNH = (bool) fParser->ArgAsInt("synch-osc-ih-to-nh");
  }
  else{
    cerr << endl;
    cerr << "3FOscParams options:" << endl;
    cerr << "You didn't enabled or disabled the synchronization of IH to NH osc params" << endl;
    cerr << "Use --synch-osc-ih-to-nh (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }
  
  // Set Mass Hierarchy (default value and prior)
  
  if(fParser->OptionExists("normal-mass-hierarchy-prior")){
    fPriorProbNH = fParser->ArgAsDouble("normal-mass-hierarchy-prior");
  }
  else{
    cerr << endl;
    cerr << "3FOscParams options:" << endl;
    cerr << "You didn't set the prior for mass hierarchy (Normal Hierarchy, IH = 1-NH)" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("throw-mass-hierarchy")){
    fThrowMH = (bool) fParser->ArgAsInt("throw-mass-hierarchy");
  }
  else{
    cerr << endl;
    cerr << "3FOscParams options:" << endl;
    cerr << "You must enable or disable the throwing of mass hierarchy for fakedata sets" << endl;
    cerr << "Use --throw-mass-hierarchy (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }
  
  // Set the priors of oscillation parameters
  
  if( fParser->OptionExists("prior-mode-toy") ){
    fPriorId = fParser->ArgAsInt("prior-mode-toy");
    cout << endl;
    cout << "Set priors for fake data sets:" << endl;
  }
  
  if(fPriorId == 0){
    cout << "Fix parameters to default values" << endl;
    cout << endl;
    InitOscParams(-99);
  }
  else if(fPriorId == 1){
    cout << "Set prior to T2K+reactors" << endl;
    cout << endl;
    
    InitOscParams(-999); // Randomize all osc params
    
    fMargMixing12        = true;
    fPriorMixing12       = "gaus";
    fPriorParMixing12[0] = conv::kPrior_Sinsq2th12_Used_Mean; //0.846;
    fPriorParMixing12[1] = conv::kPrior_Sinsq2th12_Used_RMS;  //0.021;
    fMixModeMixing12     = "sinsq2th";
    
    fMargMixing13        = true;
    fPriorMixing13       = "gaus";
    fPriorParMixing13[0] = conv::kPrior_Sinsq2th13_Used_Mean; //0.093;
    fPriorParMixing13[1] = conv::kPrior_Sinsq2th13_Used_RMS; //0.008;
    fMixModeMixing13     = "sinsq2th";
    
    fMargMixing23        = true;
    fPriorMixing23       = "flat";
    fPriorParMixing23[0] = conv::kPrior_Sinsq2th23_Flat_Low; //0.3;
    fPriorParMixing23[1] = conv::kPrior_Sinsq2th23_Flat_Up;  //0.7;
    fMixModeMixing23     = "sinsqth";
    
    fMargDm23sq          = true;
    fPriorDm23sq         = "flat";
    fPriorParDm23sq[0]   = conv::kPrior_Dm23sq_Flat_Low; //2E-3;
    fPriorParDm23sq[1]   = conv::kPrior_Dm23sq_Flat_Up;  //3E-3;
    
    fMargDm12sq          = true;
    fPriorDm12sq         = "gaus";
    fPriorParDm12sq[0]   = conv::kPrior_Dm12sq_Used_Mean; //7.53E-5;
    fPriorParDm12sq[1]   = conv::kPrior_Dm12sq_Used_RMS;  //0.18E-5;
    
    fMargDeltaCP         = true;
    fPriorDeltaCP        = "flat";
    fPriorParDeltaCP[0]  = conv::kPrior_deltaCP_Flat_Low; //-TMath::Pi();
    fPriorParDeltaCP[1]  = conv::kPrior_deltaCP_Flat_Up;  //+TMath::Pi();
  }
  else if(fPriorId == 2){
    
    cout << endl;
    cout << "Set prior to T2K only" << endl;
    cout << endl;
    
    InitOscParams(-999);
    
    fMargMixing12        = true;
    fPriorMixing12       = "gaus";
    fPriorParMixing12[0] = conv::kPrior_Sinsq2th12_Used_Mean; //0.846;
    fPriorParMixing12[1] = conv::kPrior_Sinsq2th12_Used_RMS;  //0.021;
    fMixModeMixing12     = "sinsq2th";
    
    fMargMixing13        = true;
    fPriorMixing13       = "gaus";
    //fPriorParMixing13[0] = kPrior_Sinsq2th13_PDG2013_Mean; //0.095;
    //fPriorParMixing13[1] = kPrior_Sinsq2th13_PDG2013_RMS; //0.010;
    fPriorParMixing13[0] = kPrior_Sinsq2th13_Flat_Low; 
    fPriorParMixing13[1] = kPrior_Sinsq2th13_Flat_Up; 
    fMixModeMixing13     = "sinsq2th";
    
    fMargMixing23        = true;
    fPriorMixing23       = "flat";
    fPriorParMixing23[0] = conv::kPrior_Sinsq2th23_Flat_Low; //0.3;
    fPriorParMixing23[1] = conv::kPrior_Sinsq2th23_Flat_Up;  //0.7;
    fMixModeMixing23     = "sinsqth";
    
    fMargDm23sq          = true;
    fPriorDm23sq         = "flat";
    fPriorParDm23sq[0]   = conv::kPrior_Dm23sq_Flat_Low; //2E-3;
    fPriorParDm23sq[1]   = conv::kPrior_Dm23sq_Flat_Up; //3E-3;
    
    fMargDm12sq          = true;
    fPriorDm12sq         = "gaus";
    fPriorParDm12sq[0]   = conv::kPrior_Dm12sq_Used_Mean; //7.53E-5;
    fPriorParDm12sq[1]   = conv::kPrior_Dm12sq_Used_RMS;  //0.18E-5;
    
    fMargDeltaCP         = true;
    fPriorDeltaCP        = "flat";
    fPriorParDeltaCP[0]  = conv::kPrior_deltaCP_Flat_Low; //-TMath::Pi();
    fPriorParDeltaCP[1]  = conv::kPrior_deltaCP_Flat_Up;  //+TMath::Pi();
  }
  else{
    cerr << endl;
    cerr << "3FOscParams options:" << endl;
    cerr << "You must set the prior to make fakedata sets using option: --prior-mode-toy" << endl;
    cerr << "0 --> No priors (default parameteres), "
    << "1 --> T2K+reactors (PDG2014), "
    << "2 --> T2K only" << endl;
    cerr << endl;
    exit(1);
  }
  
  // // Set prior independently for each osc parameters
  // if(fParser->OptionExists("prior-mixing12")){
  //   fMargMixing12 = true;
  //   vector<string> input = fParser->ArgAsStringTokens("prior-mixing12",",");
  //   fPriorMixing12       = input[0];
  //   fPriorParMixing12[0] = (double)atof(input[1].c_str());
  //   fPriorParMixing12[1] = (double)atof(input[2].c_str());
  //   fMixModeMixing12     = input[3];
  // }
  // if(fParser->OptionExists("prior-mixing13")){
  //   fMargMixing13 = true;
  //   vector<string> input = fParser->ArgAsStringTokens("prior-mixing13",",");
  //   fPriorMixing13       = input[0];
  //   fPriorParMixing13[0] = (double)atof(input[1].c_str());
  //   fPriorParMixing13[1] = (double)atof(input[2].c_str());
  //   fMixModeMixing13     = input[3];
  // }
  // if(fParser->OptionExists("prior-mixing23")){
  //   fMargMixing23 = true;
  //   vector<string> input = fParser->ArgAsStringTokens("prior-mixing23",",");
  //   fPriorMixing23       = input[0];
  //   fPriorParMixing23[0] = (double)atof(input[1].c_str());
  //   fPriorParMixing23[1] = (double)atof(input[2].c_str());
  //   fMixModeMixing23     = input[3];
  // }
  // if(fParser->OptionExists("prior-dmsq23")){
  //   fMargDm23sq = true;
  //   vector<string> input = fParser->ArgAsStringTokens("prior-dmsq23",",");
  //   fPriorDm23sq       = input[0];
  //   fPriorParDm23sq[0] = (double)atof(input[1].c_str());
  //   fPriorParDm23sq[1] = (double)atof(input[2].c_str());
  // }
  // if(fParser->OptionExists("prior-dmsq12")){
  //   fMargDm12sq = true;
  //   vector<string> input = fParser->ArgAsStringTokens("prior-dmsq12",",");
  //   fPriorDm12sq       = input[0];
  //   fPriorParDm12sq[0] = (double)atof(input[1].c_str());
  //   fPriorParDm12sq[1] = (double)atof(input[2].c_str());
  // }
  // if(fParser->OptionExists("prior-deltacp")){
  //   fMargDeltaCP = true;
  //   vector<string> input = fParser->ArgAsStringTokens("prior-deltacp",",");
  //   fPriorDeltaCP       = input[0];
  //   fPriorParDeltaCP[0] = (double)atof(input[1].c_str());
  //   fPriorParDeltaCP[1] = (double)atof(input[2].c_str());
  // }
  
  
  // Set nominal oscillation parameters (not mandatory, default values already set)
  
  for(int i = 0; i < 2; i++) {
    if(fParser->OptionExists(Form("mixing12-%s", fHierStr[i].c_str())))
      fMix12[i] = fParser->ArgAsDouble(Form("mixing12-%s", fHierStr[i].c_str()));
    
    if(fParser->OptionExists(Form("mixing13-%s", fHierStr[i].c_str())))
      fMix13[i] = fParser->ArgAsDouble(Form("mixing13-%s", fHierStr[i].c_str()));
    
    if(fParser->OptionExists(Form("mixing23-%s", fHierStr[i].c_str())))
      fMix23[i] = fParser->ArgAsDouble(Form("mixing23-%s", fHierStr[i].c_str()));
    
    if(fParser->OptionExists(Form("cpv-%s", fHierStr[i].c_str())))
      fCPV[i] = fParser->ArgAsDouble(Form("cpv-%s", fHierStr[i].c_str()));
    
    if(fParser->OptionExists(Form("dm12sq-%s", fHierStr[i].c_str())))
      fDm12sq[i] = fParser->ArgAsDouble(Form("dm12sq-%s", fHierStr[i].c_str()));
    
    if(fParser->OptionExists(Form("dm23sq-%s", fHierStr[i].c_str())))
      fDm23sq[i] = fParser->ArgAsDouble(Form("dm23sq-%s", fHierStr[i].c_str()));
    
    if(fParser->OptionExists(Form("mixing12Bar-%s", fHierStr[i].c_str())))
      fMix12Bar[i] = fParser->ArgAsDouble(Form("mixing12Bar-%s", fHierStr[i].c_str()));
    
    if(fParser->OptionExists(Form("mixing13Bar-%s", fHierStr[i].c_str())))
      fMix13Bar[i] = fParser->ArgAsDouble(Form("mixing13Bar-%s", fHierStr[i].c_str()));
    
    if(fParser->OptionExists(Form("mixing23Bar-%s", fHierStr[i].c_str())))
      fMix23Bar[i] = fParser->ArgAsDouble(Form("mixing23Bar-%s", fHierStr[i].c_str()));
    
    if(fParser->OptionExists(Form("cpvBar-%s", fHierStr[i].c_str())))
      fCPVBar[i] = fParser->ArgAsDouble(Form("cpvBar-%s", fHierStr[i].c_str()));
    
    if(fParser->OptionExists(Form("dm12sqBar-%s", fHierStr[i].c_str())))
      fDm12sqBar[i] = fParser->ArgAsDouble(Form("dm12sqBar-%s", fHierStr[i].c_str()));
    
    if(fParser->OptionExists(Form("dm23sqBar-%s", fHierStr[i].c_str())))
      fDm23sqBar[i] = fParser->ArgAsDouble(Form("dm23sqBar-%s", fHierStr[i].c_str()));
  }//i
  if(fParser->OptionExists("beta-bar")){
    fBetaBar = fParser->ArgAsDouble("beta-bar");
  }
  
  // Synchronize IH to NH osc params
  if(fSynchronizeIHtoNH){
    fMix12[1]  = fMix12[0];
    fMix13[1]  = fMix13[0];
    fMix23[1]  = fMix23[0];
    fCPV[1]    = fCPV[0];
    fDm12sq[1] = fDm12sq[0];
    fDm23sq[1] = fDm23sq[0];
    fMix12Bar[1]  = fMix12Bar[0];
    fMix13Bar[1]  = fMix13Bar[0];
    fMix23Bar[1]  = fMix23Bar[0];
    fCPVBar[1]    = fCPVBar[0];
    fDm12sqBar[1] = fDm12sqBar[0];
    fDm23sqBar[1] = fDm23sqBar[0];
    
  }
  
  // Set Osc param values
  util::OscParams::Instance()->SetValues(fMix12[0], fMix13[0], fMix23[0], fCPV[0], fDm12sq[0], fDm23sq[0],
                                         fBetaBar,
                                         fMix12[1], fMix13[1], fMix23[1], fCPV[1], fDm12sq[1], fDm23sq[1],
                                         fPriorProbNH, fThrowMH,
                                         fMix12Bar[0], fMix13Bar[0], fMix23Bar[0], fCPVBar[0], fDm12sqBar[0], fDm23sqBar[0],
                                         fMix12Bar[1], fMix13Bar[1], fMix23Bar[1], fCPVBar[1], fDm12sqBar[1], fDm23sqBar[1]
                                         );
  
  // Set priors of osc params to defaults, or set them to be randomised
  if(fMargMixing23) util::OscParams::Instance()->SetPrior(conv::kPhypMixing23, fPriorMixing23,  fPriorParMixing23[0], fPriorParMixing23[1], fMixModeMixing23);
  if(fMargDm23sq)   util::OscParams::Instance()->SetPrior(conv::kPhypDm23sq,   fPriorDm23sq,    fPriorParDm23sq[0],   fPriorParDm23sq[1],   ""              );
  if(fMargMixing13) util::OscParams::Instance()->SetPrior(conv::kPhypMixing13, fPriorMixing13,  fPriorParMixing13[0], fPriorParMixing13[1], fMixModeMixing13); // Convert to sinsqth
  if(fMargDeltaCP)  util::OscParams::Instance()->SetPrior(conv::kPhypCPV,      fPriorDeltaCP,  fPriorParDeltaCP[0],  fPriorParDeltaCP[1],  ""              );
  if(fMargMixing12) util::OscParams::Instance()->SetPrior(conv::kPhypMixing12, fPriorMixing12, fPriorParMixing12[0], fPriorParMixing12[1], fMixModeMixing12); // Convert to sinsqth
  if(fMargDm12sq)   util::OscParams::Instance()->SetPrior(conv::kPhypDm12sq,   fPriorDm12sq,   fPriorParDm12sq[0],   fPriorParDm12sq[1],   ""              );
  
  //if(fParser->OptionExists("disable-priors")) {
  //fPriorsFile = "";
  //}
  //else if(fParser->OptionExists("priors-file")) {
  //if(fParser->ArgAsString("priors-file").compare("off"))
  //fPriorsFile = "";
  //else
  //fPriorsFile = util::FormFilePath(fParser->ArgAsString("priors-file").c_str(), "inputs");
  //}
}


//____________________________________________________________________________
void valor::RunOpt::ReadFitMargOptions()
{
  if(fParser->OptionExists("do-toys")){
    fDoToys = (bool)fParser->ArgAsInt("do-toys");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't enabled or disabled the fake data set generation" << endl;
    cerr << "Use --do-toys (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("read-skexp")){
    fReadSKExp = (bool)fParser->ArgAsInt("read-skexp");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't enabled or disabled the reading SKExp from file" << endl;
    cerr << "Use --read-skexp (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("use-xml-file")){
    fUsing_xml_file = (bool)fParser->ArgAsInt("use-xml-file");
  }
  else{
    cerr << endl;
    cerr << "You didn't decide whether to use root or xml files for input fakedata sets" << endl;
    cerr << "Use --use-xml-file (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fUsing_xml_file){ // Pass input xml file for fakedata sets
    if(fParser->OptionExists("fakedata-xml-file")){
      fXmlfile = fParser->ArgAsString("fakedata-xml-file");
      fRootfile = "";
    }
    else{
      cerr << endl;
      cerr << "FitMarg options:" << endl;
      cerr << "You didn't pass the input xml file for input fakedata sets" << endl;
      cerr << "Use --fakedata-xml-file" << endl;
      cerr << endl;
      exit(1);
    }
  }
  else{ // Pass input root file for fakedata sets
    if(fParser->OptionExists("fakedata-root-file")){
      fRootfile = fParser->ArgAsString("fakedata-root-file");
      fXmlfile = "";
    }
    else{
      cerr << endl;
      cerr << "FitMarg options:" << endl;
      cerr << "You didn't pass the input root file for input fakedata sets" << endl;
      cerr << "Use --fakedata-root-file" << endl;
      cerr << endl;
      exit(1);
    }
  }
  
  if(fParser->OptionExists("fit-param-list")){
    fFit_param_list = fParser->ArgAsString("fit-param-list");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't pass the input fit parameter list" << endl;
    cerr << "Use --fit-param-list" << endl;
    cerr << endl;
    exit(1);
  }
  





  if(fParser->OptionExists("do-fc")){
    fDoFC = (bool)fParser->ArgAsInt("do-fc");
  }
  else{
    cerr << endl;
    cerr << "You didn't decide whether to run Feldman-Cousins or a single fit" << endl;
    cerr << "Use --do-fc (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }

  
  if(fDoFC==false){
    
    if(fParser->OptionExists("file-margtoys")){
      fInfilename = fParser->ArgAsString("file-margtoys");
    }
    else{
      cerr << endl;
      cerr << "FitMarg options:" << endl;
      cerr << "You didn't pass the toy marginalization input file" << endl;
      cerr << "Use --file-margtoys" << endl;
      cerr << endl;
      exit(1);
    }
  }
  
  else{

    if(fParser->OptionExists("file-margtoys-nh")){
      fInfilename_nh= fParser->ArgAsString("file-margtoys-nh");
    }
    else{
      cerr << endl;
      cerr << "FitMarg options:" << endl;
      cerr << "You are running Feldman-Cousins adn you didn't pass the toy marginalization input file for Normal Hierarchy" << endl;
      cerr << "Use --file-margtoys-nh" << endl;
      cerr << endl;
      exit(1);
    }

    if(fParser->OptionExists("file-margtoys-ih")){
      fInfilename_ih = fParser->ArgAsString("file-margtoys-ih");
    }
    else{
      cerr << endl;
      cerr << "FitMarg options:" << endl;
      cerr << "You are running Feldman-Cousins adn you didn't pass the toy marginalization input file for Inverted Hierarchy" << endl;
      cerr << "Use --file-margtoys-ih" << endl;
      cerr << endl;
      exit(1);
    }

  }
  











  if(fParser->OptionExists("nfiles-margtoys")){
    fNfiles = fParser->ArgAsInt("nfiles-margtoys");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't pass the # of toy marginalization input files" << endl;
    cerr << "Use --nfiles-margtoys" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("ntoys-margtoys")){
    fNtoysMarg = fParser->ArgAsInt("ntoys-margtoys");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't pass the total # toys of marginalization" << endl;
    cerr << "Use --ntoys-margtoys" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("freepar0-name")){
    fFreeparname0 = fParser->ArgAsString("freepar0-name");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't pass the fit marginalization free parameter 0" << endl;
    cerr << "Use --freepar0-name" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("freepar1-name")){
    fFreeparname1 = fParser->ArgAsString("freepar1-name");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't pass the fit marginalization free parameter 1" << endl;
    cerr << "Use --freepar1-name" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("freepar0-min")){
    fFreeparmin0 = fParser->ArgAsDouble("freepar0-min");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't pass the min value of fit marginalization free parameter 0" << endl;
    cerr << "Use --freepar0-min" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("freepar1-min")){
    fFreeparmin1 = fParser->ArgAsDouble("freepar1-min");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't pass the min value of fit marginalization free parameter 1" << endl;
    cerr << "Use --freepar1-min" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("freepar0-max")){
    fFreeparmax0 = fParser->ArgAsDouble("freepar0-max");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't pass the max value of fit marginalization free parameter 0" << endl;
    cerr << "Use --freepar0-max" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("freepar1-max")){
    fFreeparmax1 = fParser->ArgAsDouble("freepar1-max");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't pass the max value of fit marginalization free parameter 1" << endl;
    cerr << "Use --freepar1-max" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("freepar0-npt")){
    fNptScan0 = fParser->ArgAsInt("freepar0-npt");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't pass the # of grid point of fit marginalization free parameter 0" << endl;
    cerr << "Use --freepar0-npt" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("freepar1-npt")){
    fNptScan1 = fParser->ArgAsInt("freepar1-npt");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't pass the # of grid point of fit marginalization free parameter 1" << endl;
    cerr << "Use --freepar1-npt" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("marginalize-mh-margtoys")){
    fDoMargMH = (bool) fParser->ArgAsInt("marginalize-mh-margtoys");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't decide whether to marginalize mass hierarchy for marginalization toys files" << endl;
    cerr << "Use --marginalize-mh-margtoys (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("mass-hierarchy-margtoys")){
    fHierarchy_marg = fParser->ArgAsString("mass-hierarchy-margtoys");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't choose mass hierarchy for marginalization toys files" << endl;
    cerr << "Use --mass-hierarchy-margtoys" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("do-minimize")){
    fDoMinimize = (bool) fParser->ArgAsInt("do-minimize");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't decide whether to minimize using descent gradient" << endl;
    cerr << "Use --do-minimize (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }

  if(fParser->OptionExists("type-error")){
    fTypeError = fParser->ArgAsString("type-error");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't set the error type for fit with marginalization" << endl;
    cerr << "Use --type-error (\"HESSE\" or \"MINOS\")" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("full-chisq")){
    fDrawFullChisq = (bool) fParser->ArgAsInt("full-chisq");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't decide whether to draw the full chisq in fit with marginalization" << endl;
    cerr << "Use --full-chisq (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("test-statistic")){
    fTeststat = fParser->ArgAsString("test-statistic");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "The test statistic for marginalization is not defined" << endl;
    cerr << "Use --test-statistic" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("store-like-weights")){
    fStoreLikeWeights = (bool) fParser->ArgAsInt("store-like-weights");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't decide whether to store the likelihood weights for each toy of fit with marginalization" << endl;
    cerr << "To use it only when a single fit is done" << endl;
    cerr << "Use --store-like-weights" << endl;
    cerr << endl;
    exit(1);
  }
  
  // Cannot store likelihood weights if more than 1 fit w/ marginalization is performed
  if(fStoreLikeWeights){
    if(fNExperiments>1 || fNExperiments<0){
      cerr << endl;
      cerr << "Cannot store likelihood weights if more than 1 fit w/ marginalization is performed" << endl;
      cerr << "Either set --n-experiments to 1 or disable --store-like-weights" << endl;
      cerr << endl;
      exit(1);
    }
  }
  
  if(fParser->OptionExists("penalty-freepar0")){
    fPenaltyTerm0 = (bool) fParser->ArgAsInt("penalty-freepar0");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't decide whether to penalty term for parameter 0 is used" << endl;
    cerr << "Use --penalty-freepar0 (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }
  
  if(fParser->OptionExists("penalty-freepar1")){
    fPenaltyTerm1 = (bool) fParser->ArgAsInt("penalty-freepar1");
  }
  else{
    cerr << endl;
    cerr << "FitMarg options:" << endl;
    cerr << "You didn't decide whether to penalty term for parameter 1 is used" << endl;
    cerr << "Use --penalty-freepar1 (0 or 1)" << endl;
    cerr << endl;
    exit(1);
  }
  
}
//____________________________________________________________________________
void valor::RunOpt::Usage(const int w)
{
  cout << std::left;
  cout << endl;
  std::stringstream note_stream;
  
  cout << " " << setw(w) << "--help -h" << "Print this help and exit" << endl
  << endl;
  
  if(fParseType & kParseCommon) {
    cout << "Common running options:" << endl
    << " " << setw(w) << "--config-file VALUE"      << "analysis configuration file to use (in $T2KOSCANA/config/)" << endl
    << " " << setw(w) << "--file-tag VALUE"         << "tag for output file(s)" << endl
    << " " << setw(w) << "--run VALUE"              << "job number for output file(s)" << endl
    << endl;
  }//kParseCommon
  if(fParseType & kParseToys) {
    cout << "Toy running options:" << endl
    << " " << setw(w) << "--first-experiment VALUE"               << "first experiment to read/write" << endl
    << " " << setw(w) << "--n-experiments VALUE"                  << "number of experiments to read/write. Use this with first above" << endl
    << " " << setw(w) << "--stat-fluct VALUE"              << "statistical fluctuations or not" << endl
    << " " << setw(w) << "--syst-tweak-mode VALUE"                << "which systematics to throw" << endl
    << " " << setw(w) << "--rethrow-unphysical-toys VALUE" << "rethrow unphysical toys or not" << endl
    << " " << setw(w) << "--verbose-syst-throws VALUE"     << "print the SKExp everytime you tweak systematics or not" << endl
    << " " << setw(w) << "--verbose-fit VALUE"     << "Print out chisq distribution from fit" << endl
    << endl;
  }//kParseToys
  if(fParseType & kParseGlobal) {
    cout << "Global running options:" << endl
    << " " << setw(w) << "--dmsq-scheme VALUE"        << "auto abs23 abs31 fogli" << endl
    << " " << setw(w) << "--mix-param-scheme VALUE"   << "sinsqtheta sinsq2theta" << endl
    << " " << setw(w) << "--cpv-phase-scheme VALUE"   << "delta sindelta" << endl
    << " " << setw(w) << "--mass-hierarchy VALUE"     << "normal inverted" << endl
    << " " << setw(w) << "--mix-param-octant VALUE"   << "one two" << endl
    << endl;
  }//kParseGlobal
  if(fParseType & kParse3FOscParams) {
    cout << "3 flavour running options:" << endl
    << " " << setw(w) << "--hypothesis VALUE"         << "3f_osc 3f_oscbar 3f_osc_normbar 3f_oscbar_normbar 2f_numu_disap 3f_oscbar_disapp" << endl
    << " " << setw(w) << "--synch-osc-ih-to-nh VALUE" << "Synchronize IH to NH init oscillation parameters" << endl
    << " " << setw(w) << "--normal-mass-hierarchy-prior VALUE" << "The prior probability of normal hierarchy" << endl
    << " " << setw(w) << "--throw-mass-hierarchy"      << "Throw the mass hierarchy for fakedata sets" << endl
    << " " << setw(w) << "--prior-mode-toy VALUE"      << "Set the prior mode for fakedata sets" << endl;
    
    note_stream << "Note: options are read in the order above, so you can randomize all then set e.g. beta-bar to something specific" << endl;
    
    for(int i = 0; i < 2; i++) {
      cout << " " << setw(w) << Form("--mixing12-%s VALUE", fHierStr[i].c_str()) << endl
      << " " << setw(w) << Form("--mixing13-%s VALUE", fHierStr[i].c_str()) << endl
      << " " << setw(w) << Form("--mixing23-%s VALUE", fHierStr[i].c_str()) << endl
      << " " << setw(w) << Form("--cpv-%s VALUE",      fHierStr[i].c_str()) << endl
      << " " << setw(w) << Form("--dm12sq-%s VALUE",   fHierStr[i].c_str()) << endl
      << " " << setw(w) << Form("--dm23sq-%s VALUE",   fHierStr[i].c_str()) << endl;
    }//i
    for(int i = 0; i < 2; i++) {
      cout << " " << setw(w) << Form("--mixing12Bar-%s VALUE", fHierStr[i].c_str()) << endl
      << " " << setw(w) << Form("--mixing13Bar-%s VALUE", fHierStr[i].c_str()) << endl
      << " " << setw(w) << Form("--mixing23Bar-%s VALUE", fHierStr[i].c_str()) << endl
      << " " << setw(w) << Form("--cpvBar-%s VALUE",      fHierStr[i].c_str()) << endl
      << " " << setw(w) << Form("--dm12sqBar-%s VALUE",   fHierStr[i].c_str()) << endl
      << " " << setw(w) << Form("--dm23sqBar-%s VALUE",   fHierStr[i].c_str()) << endl;
    }//i
    cout << " " << setw(w) << "--beta-bar VALUE" << endl;
  }//kParse3FOscParams
  
  if(fParseType & kParseFitMarg) {
    cout << "Fit with marginalization running options:" << endl
    << " " << setw(w) << "--do-toys VALUE"         << "Set whether to make fakedata sets on the fly or not" << endl
    << " " << setw(w) << "--use-xml-file VALUE"    << "Choose between using root or xml input file for fakedata sets" << endl
    << " " << setw(w) << "--fakedata-xml-file VALUE" << "Set the name of the input xml file for fakedata sets" << endl
    << " " << setw(w) << "--fakedata-root-file VALUE" << "Set the name of the input root file for fakedata sets" << endl
    << " " << setw(w) << "--fit-param-list VALUE"     << "Set the fit parameter list" << endl
    << " " << setw(w) << "--file-margtoys VALUE"      << "Set the input file tag for marginalization toys" << endl
    << " " << setw(w) << "--nfiles-margtoys VALUE"    << "Set the # of files for marginalization toys" << endl
    << " " << setw(w) << "--ntoys-margtoys VALUE"         << "Set the # of toys for marginalization toys" << endl
    << " " << setw(w) << "--freepar0-name VALUE"      << "Set the fit marginalization free parameter 0" << endl
    << " " << setw(w) << "--freepar1-name VALUE"      << "Set the fit marginalization free parameter 1" << endl
    << " " << setw(w) << "--freepar0-min VALUE"       << "Set the fit marginalization min value of free parameter 0" << endl
    << " " << setw(w) << "--freepar1-min VALUE"       << "Set the fit marginalization min value of free parameter 1" << endl
    << " " << setw(w) << "--freepar0-max VALUE"       << "Set the fit marginalization max value of free parameter 0" << endl
    << " " << setw(w) << "--freepar1-max VALUE"       << "Set the fit marginalization max value of free parameter 1" << endl
    << " " << setw(w) << "--freepar0-npt VALUE"       << "Set the fit marginalization # of grid points of free parameter 0" << endl
    << " " << setw(w) << "--freepar1-npt VALUE"       << "Set the fit marginalization # of grid points of free parameter 1" << endl
    << " " << setw(w) << "--marginalize-mh-margtoys VALUE" << "Set marginalization of mass hierarchy for input toy marginalization files" << endl
    << " " << setw(w) << "--mass-hierarchy-margtoys VALUE" << "Set prior of mass hierarchy for input toy marginalization files" << endl
    << " " << setw(w) << "--type-error VALUE"         << "Set error type for fit marginalization (HESSE or MINOS)" << endl
    << " " << setw(w) << "--full-chisq VALUE"         << "Decide whether to draw the full marginalized chisq or not" << endl
    << " " << setw(w) << "--test-statistic VALUE"     << "Set the test statistic for fit with marginalization (rate+shape or rate-only)" << endl
    << " " << setw(w) << "--store-like-weights VALUE" << "Decide whether to store likelihood weights of fit with marginalization" << endl;
  }//kParseFitMarg
  
  cout << note_stream.str();
  cout << "Note: all 'enable' options can be turned off by substituting 'disable'" << endl
  << "Note: 'enable' takes precedence" << endl
  << endl;
}
//____________________________________________________________________________
void valor::RunOpt::Print(ostream & stream, const int w) const
{
  cout << std::left;
  cout << endl;
  
  TString seedToyMCstat  ("random");
  TString seedToyMCsyst  ("random");
  TString seedToyMCosc   ("random");
  TString seedFitter     ("random");
  TString seedOther      ("random");
  if (fSeedToyMCstat  ){seedToyMCstat = TString::Format("%u",fSeedToyMCstat);}
  if (fSeedToyMCsyst  ){seedToyMCsyst = TString::Format("%u",fSeedToyMCsyst);}
  if (fSeedToyMCosc   ){seedToyMCosc  = TString::Format("%u",fSeedToyMCosc );}
  if (fSeedFitter     ){seedFitter    = TString::Format("%u",fSeedFitter   );}
  if (fSeedOther      ){seedOther     = TString::Format("%u",fSeedOther    );}
  
  
  if(fParseType & kParseCommon) {
    stream << "Common running options:" << endl
           << " - " << setw(w) << "Config file:  "   << fConfigFile    << endl
           << " - " << setw(w) << "Verbosity file:"  << fVerbosityFile << endl
           << " - " << setw(w) << "Output file tag:" << fFileTag       << endl
           << " - " << setw(w) << "Run number:"      << fRun           << endl
           << " - " << setw(w) << "Seed (stat):   "  << seedToyMCstat  << endl
           << " - " << setw(w) << "Seed (syst):   "  << seedToyMCsyst  << endl
           << " - " << setw(w) << "Seed (osc):    "  << seedToyMCosc   << endl
           << " - " << setw(w) << "Seed (fitter): "  << seedFitter     << endl
           << " - " << setw(w) << "Seed (other):  "  << seedOther      << endl
           
           << endl;
  }//kParseCommon
  if(fParseType & kParseToys) {
    stream << "Toy running options:" << endl
           << " - " << setw(w) << "First experiment:"         << fFirstExperiment << endl
           << " - " << setw(w) << "Last experiment:"          << fLastExperiment << endl
           << " - " << setw(w) << "N experiments:"            << fNExperiments << endl
           << " - " << setw(w) << "Statistical fluctuations:" << ((fStatFluctuations) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Randomize systematics:"    << util::SysThrowIdToString(fSystTweakMode) << endl
           << " - " << setw(w) << "Rethrow unphysical:"       << ((fRethrowIfUnphysical) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Verbose skexp::Tweak():"   << ((fVerboseSystThrows) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Verbose fit:"              << ((fVerboseFit) ? "YES " : "NO") << endl
           << endl;
  }//kParseToys
  if(fParseType & kParseGlobal) {
    stream << "Global running options:" << endl
           << " - " << setw(w) << "Dmsq scheme:"              << fGlobalDmsqScheme << endl
           << " - " << setw(w) << "Mixing parameters scheme:" << fGlobalMixParamScheme << endl
           << " - " << setw(w) << "CPV phase scheme:"         << fGlobalCPVPhaseScheme << endl
           << " - " << setw(w) << "Mass hierarchy:"           << fGlobalMassHierarchy << endl
           << " - " << setw(w) << "Mixing parameter octant:"  << fGlobalMixParamOctant << endl
           << endl;
  }//kParseGlobal
  if(fParseType & kParse3FOscParams) {
    stream << "3f oscillation running options:" << endl
           << " - " << setw(w) << "Hypothesis:" << fHypothesis << endl
           << " - " << setw(w) << "Synchronize IH to NH osc params:"  << ((fSynchronizeIHtoNH) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Normal hierarchy prior prob:" << fPriorProbNH << endl
           << " - " << setw(w) << "Throw mass hierarchy:"  << ((fThrowMH) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Prior mode:"  << fPriorId << endl;
    
    for(int i = 0; i < 2; i++) {
      stream << " - " << setw(w) << Form("%s_%s", util::RawParamAsString(kPhypMixing12).c_str(), fHierStr[i].c_str()) << fMix12[i] << endl
             << " - " << setw(w) << Form("%s_%s", util::RawParamAsString(kPhypMixing13).c_str(), fHierStr[i].c_str()) << fMix13[i] << endl
             << " - " << setw(w) << Form("%s_%s", util::RawParamAsString(kPhypMixing23).c_str(), fHierStr[i].c_str()) << fMix23[i] << endl
             << " - " << setw(w) << Form("%s_%s", util::RawParamAsString(kPhypCPV).c_str(),      fHierStr[i].c_str()) << fCPV[i] << endl
             << " - " << setw(w) << Form("%s_%s", util::RawParamAsString(kPhypDm12sq).c_str(),   fHierStr[i].c_str()) << fDm12sq[i] << endl
             << " - " << setw(w) << Form("%s_%s", util::RawParamAsString(kPhypDm23sq).c_str(),   fHierStr[i].c_str()) << fDm23sq[i] << endl
      << " - " << setw(w) << Form("%s_%s", util::RawParamAsString(kPhypMixing13Bar).c_str(), fHierStr[i].c_str()) << fMix13Bar[i] << endl
      << " - " << setw(w) << Form("%s_%s", util::RawParamAsString(kPhypMixing23Bar).c_str(), fHierStr[i].c_str()) << fMix23Bar[i] << endl
      << " - " << setw(w) << Form("%s_%s", util::RawParamAsString(kPhypCPVBar).c_str(),      fHierStr[i].c_str()) << fCPVBar[i] << endl
      << " - " << setw(w) << Form("%s_%s", util::RawParamAsString(kPhypDm12sqBar).c_str(),   fHierStr[i].c_str()) << fDm12sqBar[i] << endl
      << " - " << setw(w) << Form("%s_%s", util::RawParamAsString(kPhypDm23sqBar).c_str(),   fHierStr[i].c_str()) << fDm23sqBar[i] << endl;
      
    }//i
    stream << " - " << setw(w) << "Beta bar:"                    << fBetaBar << endl
    //<< " - " << setw(w) << "Using priors:"                << (fPriorsFile.empty() ? "NO" : Form("YES: %s", fPriorsFile.c_str())) << endl
           << endl;
  }//kParse3FOscParams
  
  if(fParseType & kParseFitMarg) {
    stream << "Fit with marginalization running options:" << endl
           << " - " << setw(w) << "Do toys for fake datasets on the fly:" << ((fDoToys) ? "YES " : "NO") << endl
	   << " - " << setw(w) << "Use xml file for fakedata sets (if not use root file):"  << ((fUsing_xml_file) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Input xml file for fakedata sets:" << fXmlfile << endl
           << " - " << setw(w) << "Input root file for fakedata sets:" << fRootfile << endl
          << " - " << setw(w) << "Read SKExp with data files:" << ((fReadSKExp) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Fit parameter list:"  << fFit_param_list << endl;
    
    
    stream << " - " << setw(w) << "Running Feldman-Cousins:" << ((fDoFC) ? "YES " : "NO") << endl;
    if(fDoFC==false){
      stream << " - " << setw(w) << "Input file for marginalization toys:"  << fInfilename << endl;
    }
    else{
      stream << " - " << setw(w) << "Input file for marginalization toys NH:"  << fInfilename_nh << endl;
      stream << " - " << setw(w) << "Input file for marginalization toys IH:"  << fInfilename_ih << endl;
    }
    
    stream << " - " << setw(w) << "Number of input file for marginalization toys:"  << fNfiles << endl
           << " - " << setw(w) << "Number of toys for marginalization:"  << fNtoysMarg << endl
           << " - " << setw(w) << "Name of free parameter 0 for fit with marginalization:"  << fFreeparname0 << endl
           << " - " << setw(w) << "Name of free parameter 1 for fit with marginalization:"  << fFreeparname1 << endl
           << " - " << setw(w) << "Min value of free parameter 0 for fit with marginalization:"  << fFreeparmin0 << endl
           << " - " << setw(w) << "Min value of free parameter 1 for fit with marginalization:"  << fFreeparmin1 << endl
           << " - " << setw(w) << "Max value of free parameter 0 for fit with marginalization:"  << fFreeparmax0 << endl
           << " - " << setw(w) << "Max value of free parameter 1 for fit with marginalization:"  << fFreeparmax1 << endl
           << " - " << setw(w) << "Number of points of free parameter 0 for fit with marginalization:"  << fNptScan0 << endl
           << " - " << setw(w) << "Number of points of free parameter 1 for fit with marginalization:"  << fNptScan1 << endl
           << " - " << setw(w) << "Marginalization of mass hierarchy for input marginalization files:"  << ((fDoMargMH) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Mass hierarchy for input marginalization files:"  << fHierarchy_marg << endl
           << " - " << setw(w) << "Use Minimization (descent-gradient):"  << ((fDoMinimize) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Error type for fit with marginalization:"  << fTypeError << endl
           << " - " << setw(w) << "Draw fill marginalized chisq:"  << ((fDrawFullChisq) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Test statistic for fit with marginalization:"  << fTeststat << endl
           << " - " << setw(w) << "Store likelihood weights of fit with marginalization?:"  << ((fStoreLikeWeights) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Use penalty term for free parameter 0:"  << ((fPenaltyTerm0) ? "YES " : "NO") << endl
           << " - " << setw(w) << "Use penalty term for free parameter 1:"  << ((fPenaltyTerm1) ? "YES " : "NO") << endl
    << endl;
  }//kParseFitMarg
}
//___________________________________________________________________________


