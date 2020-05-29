//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//____________________________________________________________________________


#include "valor/Utilities/UtlForMacros.h"

#include "valor/Conventions/Opt.h"
#include "valor/Conventions/PhysParamId.h"
#include "valor/Conventions/PhysHypothesisId.h"
#include "valor/Conventions/Constants.h"
#include "valor/FitCore/PhysHypothesis.h"
#include "valor/Utilities/Env.h"
#include "valor/Utilities/UtlXML.h"

#include "TSystem.h"
#include "TCanvas.h"
#include "TMath.h"

#include <iostream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

using namespace valor;
using namespace valor::conv;

//____________________________________________________________________________
std::string valor::util::FormFilePath(const char * file, const char * dir)
{
  FileStat_t dummy;
  string filename(TString::Format("%s/%s/%s", Env::Instance()->TopLevelDir().c_str(), dir, file).Data());
  if(gSystem->GetPathInfo(filename.c_str(), dummy)) {
    cerr << "Utils/UtilForMacros/FormFilePath >> File not found: " << filename << endl;
    exit(-1);
  }
  return filename;
}
//____________________________________________________________________________
string valor::util::SysThrowIdToString(unsigned int tweak_systematics)
{
  string tweak_sys_str;
  string exptname = "t2xk"; // CHANGE THIS
  if(!exptname.compare("t2xk")) {
    switch(tweak_systematics)
    {
      case(0):
        tweak_sys_str = "None"; break;
      case(1):
        tweak_sys_str = "All"; break;
      case(2):
        tweak_sys_str = "Special low energy numu flux increase"; break;
      case(3):
        tweak_sys_str = "Tweak all but SK and FSI"; break;
      case(4):
        tweak_sys_str = "NC1pi norm = 0.5"; break;
      case(5):
        tweak_sys_str = "High energy >1.5 GeV flux increase"; break;
      case(6):
        tweak_sys_str = "BANFF"; break;
      case(7):
        tweak_sys_str = "XSec"; break;
      case(8):
        tweak_sys_str = "SK+FSI"; break;
      default:
        cerr << "SysThrowIdToString() tweak_systematics = " << tweak_systematics
             << " not implemented" << endl;
        exit(-1);
    }
  }
  else {
    cerr << "util::SysThrowIdToString() not implemented for VALOR experiment " << exptname << endl;
    exit(-1);
  }
  return tweak_sys_str;
}
//____________________________________________________________________________
TH2D * valor::util::GetTH2DFromTFile(TFile * f, const char * name)
{
  TH2D * h = 0;
  f->GetObject(name, h);
  if(!h || h->IsZombie()) {
    cerr << "Utils/UtilForMacros/GetTH2DFromTFile >> Error: the histogram " << name << " hasn't been found" << endl;
    exit(1);
  }
  return h;
}
//____________________________________________________________________________
//____________________________________________________________________________

