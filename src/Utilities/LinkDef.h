#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace valor;
#pragma link C++ namespace valor::conv;
#pragma link C++ namespace valor::util;

#pragma link C++ class valor::VALORArrayT<double>+;
#pragma link C++ class valor::VALORArrayT<float>+;
#pragma link C++ class valor::VALORArrayT<int>+;

//~ #pragma link C++ typedef VALORArray;
//~ #pragma link C++ typedef VALORArrayF;
//~ #pragma link C++ typedef VALORArrayI;
//~ #pragma link C++ typedef VALORArrayD;

#pragma link C++ class valor::VALORArrayHeader+;

#pragma link C++ class valor::Env+;
#pragma link C++ class valor::Rnd+;
#pragma link C++ class valor::NuOsc3Flavour+;
#pragma link C++ class valor::NuOscSterile+;

#pragma link C++ class valor::util::OscParams+;
#pragma link C++ class valor::RunOpt+;

#pragma link C++ class valor::util::NameRegistry+;


#endif
