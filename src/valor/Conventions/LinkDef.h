#ifdef __CINT__
#include <map>
#include <vector>
#include <utility>

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace valor;
#pragma link C++ namespace valor::conv;
#pragma link C++ namespace valor::conv::duneosc;
#pragma link C++ namespace valor::util;
#pragma link C++ namespace valor::util::nm;

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

#pragma link C++ enum valor::conv::FitParamGroup_t;

#pragma link C++ enum valor::conv::ReacMode_t;
#pragma link C++ enum valor::conv::ReacModeCoarse_t;
#pragma link C++ enum valor::ModeList;

#pragma link C++ enum valor::conv::BeamConf_t;
#pragma link C++ enum valor::conv::Sample_t;
#pragma link C++ enum valor::conv::Detector_t;

#pragma link C++ enum valor::conv::NuclearTarget_t;
#pragma link C++ class std::vector<valor::conv::NuclearTarget_t>+;

#pragma link C++ struct valor::UniqueSampleID;
#pragma link C++ class std::vector<valor::UniqueSampleID>+;

#pragma link C++ class valor::Opt+;
#pragma link C++ class valor::Opt::Persistable+;

#pragma link C++ class valor::ExperimentSetup+;

// Part of FitParamList
#pragma link C++ class std::map <valor::conv::PhysParamId_t,int>;
#pragma link C++ class std::pair<valor::conv::PhysParamId_t,int>;
// Also used
#pragma link C++ class std::map <valor::conv::PhysParamId_t,bool>;
#pragma link C++ class std::pair<valor::conv::PhysParamId_t,bool>;

#pragma link C++ class std::vector<valor::conv::PhysParamId_t>;
#pragma link C++ class std::vector<valor::conv::KineVar_t>;
#pragma link C++ class std::vector<valor::conv::KineSpace_t>;


#endif
