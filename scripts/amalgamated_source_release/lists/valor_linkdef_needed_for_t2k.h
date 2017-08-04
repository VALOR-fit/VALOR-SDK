#ifdef __CINT__

#include <map>
#include <vector>
#include <utility>

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace log4cpp;
#pragma link C++ namespace valor;

#pragma link C++ namespace valor::conv;
#pragma link C++ namespace valor::util;
#pragma link C++ namespace valor::util;
#pragma link C++ namespace valor::util::file;
#pragma link C++ namespace valor::util::init;

#pragma link C++ class valor::CmdLnArgList+;
#pragma link C++ class valor::CmdLnArgParser+;

#pragma link C++ class valor::RunOpt+;

#pragma link C++ enum  valor::conv::ConfLevel_t;

#pragma link C++ enum  valor::conv::KineVar_t;
#pragma link C++ enum  valor::conv::KineSpace_t;

#pragma link C++ enum  valor::conv::NuMassHierarchy_t;
#pragma link C++ enum  valor::conv::NuOscDeltaCP_t;
#pragma link C++ enum  valor::conv::NuOscDmsqScheme_t;
#pragma link C++ enum  valor::conv::NuOscMixParam_t;
#pragma link C++ enum  valor::conv::NuOscOctant_t;
#pragma link C++ enum  valor::conv::NuFlavour_t;
#pragma link C++ enum  valor::conv::OscChannelId_t;

#pragma link C++ enum  valor::conv::PhysParamId_t;
#pragma link C++ enum  valor::conv::PhysHypothesis_t;

#pragma link C++ class valor::Opt+;
#pragma link C++ class valor::Opt::Persistable+;

// Part of FitParamList
#pragma link C++ class std::map <valor::conv::PhysParamId_t,int>;
#pragma link C++ class std::pair<valor::conv::PhysParamId_t,int>;
// Also used
#pragma link C++ class std::map <valor::conv::PhysParamId_t,bool>;
#pragma link C++ class std::pair<valor::conv::PhysParamId_t,bool>;

#pragma link C++ class std::vector<valor::conv::PhysParamId_t>;
#pragma link C++ class std::vector<valor::conv::KineVar_t>;
#pragma link C++ class std::vector<valor::conv::KineSpace_t>;



#pragma link C++ class valor::CorrelatedSystematics+;
#pragma link C++ class valor::CompoundCorrelatedSystematics+;

#pragma link C++ class valor::PhysHypothesis+;

#pragma link C++ class valor::FitParamList+;
#pragma link C++ class valor::PhysParamList+;

#pragma link C++ class valor::FitDataABC+;
#pragma link C++ class valor::FitModelABC+;

#pragma link C++ class valor::CompoundFitData+;
#pragma link C++ class valor::CompoundFitModel+;

#pragma link C++ class valor::FitInputs+;

#pragma link C++ class valor::FitterABC+;
#pragma link C++ class valor::FitterMinuit+;
#pragma link C++ class valor::FitterMinuit2+;

#pragma link C++ class valor::FitterABC_new+;


#pragma link C++ class valor::FitterFactory+;
#pragma link C++ class valor::MinuitFitter+;
//#pragma link C++ class valor::Minuit2Fitter+;
//#pragma link C++ class valor::GSLFitter+;

#pragma link C++ class valor::KBCubHcmbEdge2Edge;
#pragma link C++ class valor::KBCubHcmbMondrian;

// Standard ROOT Objects STL containers
#pragma link C++ class std::vector<TLine>;                                                 
#pragma link C++ class std::vector<TLine>::iterator;                                       
#pragma link C++ function operator != ( vector<TLine>::iterator, vector<TLine>::iterator );

#pragma link C++ class valor::Messenger+;

#pragma link C++ class valor::StdFitNtp+;

#pragma link C++ class valor::NuOsc3Flavour+;
#pragma link C++ class valor::NuOscSterile+;

#pragma link C++ class valor::util::OscParams+;

#pragma link C++ class valor::Registry;
#pragma link C++ class valor::RegistryItem<RgBool>+;
#pragma link C++ class valor::RegistryItem<RgInt>+;
#pragma link C++ class valor::RegistryItem<RgDbl>+;
#pragma link C++ class valor::RegistryItem<RgStr>+;
#pragma link C++ class valor::RegistryItem<RgH1F>+;
#pragma link C++ class valor::RegistryItem<RgH2F>+;
#pragma link C++ class valor::RegistryItem<RgTree>+;
#pragma link C++ class valor::RegistryItem<RgObject>+;
#pragma link C++ class valor::RegistryItemI;

#pragma link C++ class valor::Env+;
#pragma link C++ class valor::Rnd+;

#pragma link C++ class valor::util::NameRegistry+;
#pragma link C++ class valor::util::Interpolator+;
#pragma link C++ class valor::util::Configuration+;
#pragma link C++ class valor::util::ConfigurationHandler+;
#pragma link C++ class valor::ConfidenceInterval+;
#pragma link C++ class valor::ConfidenceIntervalPlotter+;

#pragma link C++ class valor::VALORArrayT<double>+;
#pragma link C++ class valor::VALORArrayT<float>+;
#pragma link C++ class valor::VALORArrayT<int>+;
#pragma link C++ class valor::VALORArrayT<Long64_t>+;
#pragma link C++ class valor::VALORArrayT<ULong64_t>+;

#pragma link C++ class valor::VALORArrayBin+;
#pragma link C++ class valor::VALORArrayHeader+;

#pragma link C++ enum  valor::conv::PhysParamId_t;
#pragma link C++ enum  valor::conv::ConfLevel_t;
#pragma link C++ enum  valor::conv::SuperkSample_t;
#pragma link C++ enum  valor::conv::SuperkMode_t;

#pragma link C++ class valor::util::ArrIdx;
#pragma link C++ class valor::util::UtlMakeFakeData;

#pragma link C++ class valor::T2KPhysicalParameterLimits;
#pragma link C++ class valor::T2KOpt;

#pragma link C++ class valor::SuperkObservation;
#pragma link C++ class valor::SuperkExpectation;
#pragma link C++ class valor::SuperkDetectorFSI;
#pragma link C++ class valor::SuperkMC+;
#pragma link C++ class valor::BANFF;
#pragma link C++ class valor::NuXSec+;
#pragma link C++ class valor::NuFlux+;
#pragma link C++ class valor::FitMarginalization+;
#pragma link C++ class valor::RootFinder;


#endif
