#ifdef __CINT__
#include <vector>
#include <map>

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace valor;
#pragma link C++ namespace valor::util;
#pragma link C++ namespace valor::util::oscillation;

#pragma link C++ class valor::CompoundFitElements;
#pragma link C++ class valor::SingleSampleFitModelABC;
#pragma link C++ class valor::SingleSampleFitDataABC;

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

#pragma link C++ class std::vector<std::vector<float> >;

#pragma link C++ defined_in "valor/FitCore/UtlFitParamList.h";

#endif
