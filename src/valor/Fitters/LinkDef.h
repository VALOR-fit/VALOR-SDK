#ifdef __CINT__
#include <vector>

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace valor;
#pragma link C++ namespace valor::util;

#pragma link C++ class valor::FitterFactory+;
//#pragma link C++ class valor::MinuitFitter+;
//#pragma link C++ class valor::Minuit2Fitter+;
//#pragma link C++ class valor::GSLFitter+;

#pragma link C++ class valor::FitterMinuit+;
#pragma link C++ class valor::FitterMinuit2+;
#pragma link C++ class valor::FitterGSL+;

#pragma link C++ class valor::LikelihoodMinuit+;
#pragma link C++ class valor::LikelihoodMinuit2+;
#pragma link C++ class valor::LikelihoodGSL+;

#endif
