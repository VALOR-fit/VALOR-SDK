#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace valor;
#pragma link C++ namespace valor::conv;
#pragma link C++ namespace valor::util;
#pragma link C++ namespace valor::util::file;
#pragma link C++ namespace valor::util::init;
#pragma link C++ namespace valor::util::plotting;
#pragma link C++ namespace valor::util::binning;

#pragma link C++ class valor::Env+;
#pragma link C++ class valor::Rnd+;

#pragma link C++ class valor::util::KinematicBin;
#pragma link C++ class valor::util::KinematicPoint;

#pragma link C++ class valor::SingleSampleKineBinning;

#pragma link C++ class valor::util::NameRegistry+;
#pragma link C++ class valor::util::Interpolator+;
#pragma link C++ class valor::util::Configuration+;
#pragma link C++ class valor::util::ConfigurationHandler+;
#pragma link C++ class valor::ConfidenceInterval+;
#pragma link C++ class valor::ConfidenceIntervalPlotter+;

#endif
