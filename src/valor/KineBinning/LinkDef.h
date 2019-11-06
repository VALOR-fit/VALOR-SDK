#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace valor;
#pragma link C++ namespace valor::conv;

#pragma link C++ namespace valor::util::binning;

#pragma link C++ class valor::VALORBinning;
#pragma link C++ class valor::KBCubHcmbEdge2Edge;
#pragma link C++ class valor::KBCubHcmbMondrian;
#pragma link C++ class valor::NoBinning;

// Standard ROOT Objects STL containers
#pragma link C++ class std::vector<TLine>;                                                 
#pragma link C++ class std::vector<TLine>::iterator;                                       
#pragma link C++ function operator != ( vector<TLine>::iterator, vector<TLine>::iterator );

#endif
