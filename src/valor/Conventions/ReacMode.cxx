//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include <algorithm>
#include <iostream>

#include "valor/Conventions/ReacMode.h"
#include "valor/Utilities/UtlStr.h"

using namespace valor;
using namespace valor::conv;

using std::string;

//________________________________________________________________________________________
string util::AsString(ReacMode_t m)
{
  switch(m) {
  case(kmNuMuCCQE        ): return "numuCCQE";
  case(kmNuMuCCRESpPiP   ): return "numuCCRESpPiP";
  case(kmNuMuCCRESnPi0   ): return "numuCCRESnPi0";
  case(kmNuMuCCRESnPiP   ): return "numuCCRESnPiP";
  case(kmNuMuCCDIS       ): return "numuCCDIS";
  case(kmNuMuCCCOH       ): return "numuCCCOH";
  case(kmNuMuCCMEC       ): return "numuCCMEC";
  case(kmNuMuNCQE        ): return "numuNCQE";
  case(kmNuMuNCRESpPi0   ): return "numuNCRESpPi0";
  case(kmNuMuNCRESpPiP   ): return "numuNCRESpPiP";
  case(kmNuMuNCRESnPi0   ): return "numuNCRESnPi0";
  case(kmNuMuNCRESnPiM   ): return "numuNCRESnPiM";
  case(kmNuMuNCDIS       ): return "numuNCDIS";
  case(kmNuMuNCCOH       ): return "numuNCCOH";
  case(kmNuMuNCMEC       ): return "numuNCMEC";
  case(kmNuMuBarCCQE     ): return "numubarCCQE";
  case(kmNuMuBarCCRESnPiM): return "numubarCCRESnPiM";
  case(kmNuMuBarCCRESpPi0): return "numubarCCRESpPi0";
  case(kmNuMuBarCCRESpPiM): return "numubarCCRESpPiM";
  case(kmNuMuBarCCDIS    ): return "numubarCCDIS";
  case(kmNuMuBarCCCOH    ): return "numubarCCCOH";
  case(kmNuMuBarCCMEC    ): return "numubarCCMEC";
  case(kmNuMuBarNCQE     ): return "numubarNCQE";
  case(kmNuMuBarNCRESpPi0): return "numubarNCRESpPi0";
  case(kmNuMuBarNCRESpPiP): return "numubarNCRESpPiP";
  case(kmNuMuBarNCRESnPi0): return "numubarNCRESnPi0";
  case(kmNuMuBarNCRESnPiM): return "numubarNCRESnPiM";
  case(kmNuMuBarNCDIS    ): return "numubarNCDIS";
  case(kmNuMuBarNCCOH    ): return "numubarNCCOH";
  case(kmNuMuBarNCMEC    ): return "numubarNCMEC";
  case(kmNuECCQE         ): return "nueCCQE";
  case(kmNuECCRESpPiP    ): return "nueCCRESpPiP";
  case(kmNuECCRESnPi0    ): return "nueCCRESnPi0";
  case(kmNuECCRESnPiP    ): return "nueCCRESnPiP";
  case(kmNuECCDIS        ): return "nueCCDIS";
  case(kmNuECCCOH        ): return "nueCCCOH";
  case(kmNuECCMEC        ): return "nueCCMEC";
  case(kmNuENCQE         ): return "nueNCQE";
  case(kmNuENCRESpPi0    ): return "nueNCRESpPi0";
  case(kmNuENCRESpPiP    ): return "nueNCRESpPiP";
  case(kmNuENCRESnPi0    ): return "nueNCRESnPi0";
  case(kmNuENCRESnPiM    ): return "nueNCRESnPiM";
  case(kmNuENCDIS        ): return "nueNCDIS";
  case(kmNuENCCOH        ): return "nueNCCOH";
  case(kmNuENCMEC        ): return "nueNCMEC";
  case(kmNuEBarCCQE      ): return "nuebarCCQE";
  case(kmNuEBarCCRESnPiM ): return "nuebarCCRESnPiM";
  case(kmNuEBarCCRESpPi0 ): return "nuebarCCRESpPi0";
  case(kmNuEBarCCRESpPiM ): return "nuebarCCRESpPiM";
  case(kmNuEBarCCDIS     ): return "nuebarCCDIS";
  case(kmNuEBarCCCOH     ): return "nuebarCCCOH";
  case(kmNuEBarCCMEC     ): return "nuebarCCMEC";
  case(kmNuEBarNCQE      ): return "nuebarNCQE";
  case(kmNuEBarNCRESpPi0 ): return "nuebarNCRESpPi0";
  case(kmNuEBarNCRESpPiP ): return "nuebarNCRESpPiP";
  case(kmNuEBarNCRESnPi0 ): return "nuebarNCRESnPi0";
  case(kmNuEBarNCRESnPiM ): return "nuebarNCRESnPiM";
  case(kmNuEBarNCDIS     ): return "nuebarNCDIS";
  case(kmNuEBarNCCOH     ): return "nuebarNCCOH";
  case(kmNuEBarNCMEC     ): return "nuebarNCMEC";
  case(kmNuMuToNuECCQE             ): return "numutonueCCQE";
  case(kmNuMuToNuECCRESpPiP        ): return "numutonueCCRESpPiP";
  case(kmNuMuToNuECCRESnPi0        ): return "numutonueCCRESnPi0";
  case(kmNuMuToNuECCRESnPiP        ): return "numutonueCCRESnPiP";
  case(kmNuMuToNuECCDIS            ): return "numutonueCCDIS";
  case(kmNuMuToNuECCCOH            ): return "numutonueCCCOH";
  case(kmNuMuToNuECCMEC            ): return "numutonueCCMEC";
  case(kmNuMuToNuENCQE             ): return "numutonueNCQE";
  case(kmNuMuToNuENCRESpPi0        ): return "numutonueNCRESpPi0";
  case(kmNuMuToNuENCRESpPiP        ): return "numutonueNCRESpPiP";
  case(kmNuMuToNuENCRESnPi0        ): return "numutonueNCRESnPi0";
  case(kmNuMuToNuENCRESnPiM        ): return "numutonueNCRESnPiM";
  case(kmNuMuToNuENCDIS            ): return "numutonueNCDIS";
  case(kmNuMuToNuENCCOH            ): return "numutonueNCCOH";
  case(kmNuMuToNuENCMEC            ): return "numutonueNCMEC";
  case(kmNuMuToNuTauCCQE           ): return "numutonutauCCQE";
  case(kmNuMuToNuTauCCRESpPiP      ): return "numutonutauCCRESpPiP";
  case(kmNuMuToNuTauCCRESnPi0      ): return "numutonutauCCRESnPi0";
  case(kmNuMuToNuTauCCRESnPiP      ): return "numutonutauCCRESnPiP";
  case(kmNuMuToNuTauCCDIS          ): return "numutonutauCCDIS";
  case(kmNuMuToNuTauCCCOH          ): return "numutonutauCCCOH";
  case(kmNuMuToNuTauCCMEC          ): return "numutonutauCCMEC";
  case(kmNuMuToNuTauNCQE           ): return "numutonutauNCQE";
  case(kmNuMuToNuTauNCRESpPi0      ): return "numutonutauNCRESpPi0";
  case(kmNuMuToNuTauNCRESpPiP      ): return "numutonutauNCRESpPiP";
  case(kmNuMuToNuTauNCRESnPi0      ): return "numutonutauNCRESnPi0";
  case(kmNuMuToNuTauNCRESnPiM      ): return "numutonutauNCRESnPiM";
  case(kmNuMuToNuTauNCDIS          ): return "numutonutauNCDIS";
  case(kmNuMuToNuTauNCCOH          ): return "numutonutauNCCOH";
  case(kmNuMuToNuTauNCMEC          ): return "numutonutauNCMEC";
  case(kmNuMuBarToNuEBarCCQE       ): return "numubartonuebarCCQE";
  case(kmNuMuBarToNuEBarCCRESnPiM  ): return "numubartonuebarCCRESnPiM";
  case(kmNuMuBarToNuEBarCCRESpPi0  ): return "numubartonuebarCCRESpPi0";
  case(kmNuMuBarToNuEBarCCRESpPiM  ): return "numubartonuebarCCRESpPiM";
  case(kmNuMuBarToNuEBarCCDIS      ): return "numubartonuebarCCDIS";
  case(kmNuMuBarToNuEBarCCCOH      ): return "numubartonuebarCCCOH";
  case(kmNuMuBarToNuEBarCCMEC      ): return "numubartonuebarCCMEC";
  case(kmNuMuBarToNuEBarNCQE       ): return "numubartonuebarNCQE";
  case(kmNuMuBarToNuEBarNCRESpPi0  ): return "numubartonuebarNCRESpPi0";
  case(kmNuMuBarToNuEBarNCRESpPiP  ): return "numubartonuebarNCRESpPiP";
  case(kmNuMuBarToNuEBarNCRESnPi0  ): return "numubartonuebarNCRESnPi0";
  case(kmNuMuBarToNuEBarNCRESnPiM  ): return "numubartonuebarNCRESnPiM";
  case(kmNuMuBarToNuEBarNCDIS      ): return "numubartonuebarNCDIS";
  case(kmNuMuBarToNuEBarNCCOH      ): return "numubartonuebarNCCOH";
  case(kmNuMuBarToNuEBarNCMEC      ): return "numubartonuebarNCMEC";
  case(kmNuMuBarToNuTauBarCCQE     ): return "numubartonutaubarCCQE";
  case(kmNuMuBarToNuTauBarCCRESnPiM): return "numubartonutaubarCCRESnPiM";
  case(kmNuMuBarToNuTauBarCCRESpPi0): return "numubartonutaubarCCRESpPi0";
  case(kmNuMuBarToNuTauBarCCRESpPiM): return "numubartonutaubarCCRESpPiM";
  case(kmNuMuBarToNuTauBarCCDIS    ): return "numubartonutaubarCCDIS";
  case(kmNuMuBarToNuTauBarCCCOH    ): return "numubartonutaubarCCCOH";
  case(kmNuMuBarToNuTauBarCCMEC    ): return "numubartonutaubarCCMEC";
  case(kmNuMuBarToNuTauBarNCQE     ): return "numubartonutaubarNCQE";
  case(kmNuMuBarToNuTauBarNCRESpPi0): return "numubartonutaubarNCRESpPi0";
  case(kmNuMuBarToNuTauBarNCRESpPiP): return "numubartonutaubarNCRESpPiP";
  case(kmNuMuBarToNuTauBarNCRESnPi0): return "numubartonutaubarNCRESnPi0";
  case(kmNuMuBarToNuTauBarNCRESnPiM): return "numubartonutaubarNCRESnPiM";
  case(kmNuMuBarToNuTauBarNCDIS    ): return "numubartonutaubarNCDIS";
  case(kmNuMuBarToNuTauBarNCCOH    ): return "numubartonutaubarNCCOH";
  case(kmNuMuBarToNuTauBarNCMEC    ): return "numubartonutaubarNCMEC";
  case(kmNuEToNuMuCCQE             ): return "nuetonumuCCQE";
  case(kmNuEToNuMuCCRESpPiP        ): return "nuetonumuCCRESpPiP";
  case(kmNuEToNuMuCCRESnPi0        ): return "nuetonumuCCRESnPi0";
  case(kmNuEToNuMuCCRESnPiP        ): return "nuetonumuCCRESnPiP";
  case(kmNuEToNuMuCCDIS            ): return "nuetonumuCCDIS";
  case(kmNuEToNuMuCCCOH            ): return "nuetonumuCCCOH";
  case(kmNuEToNuMuCCMEC            ): return "nuetonumuCCMEC";
  case(kmNuEToNuMuNCQE             ): return "nuetonumuNCQE";
  case(kmNuEToNuMuNCRESpPi0        ): return "nuetonumuNCRESpPi0";
  case(kmNuEToNuMuNCRESpPiP        ): return "nuetonumuNCRESpPiP";
  case(kmNuEToNuMuNCRESnPi0        ): return "nuetonumuNCRESnPi0";
  case(kmNuEToNuMuNCRESnPiM        ): return "nuetonumuNCRESnPiM";
  case(kmNuEToNuMuNCDIS            ): return "nuetonumuNCDIS";
  case(kmNuEToNuMuNCCOH            ): return "nuetonumuNCCOH";
  case(kmNuEToNuMuNCMEC            ): return "nuetonumuNCMEC";
  case(kmNuEToNuTauCCQE            ): return "nuetonutauCCQE";
  case(kmNuEToNuTauCCRESpPiP       ): return "nuetonutauCCRESpPiP";
  case(kmNuEToNuTauCCRESnPi0       ): return "nuetonutauCCRESnPi0";
  case(kmNuEToNuTauCCRESnPiP       ): return "nuetonutauCCRESnPiP";
  case(kmNuEToNuTauCCDIS           ): return "nuetonutauCCDIS";
  case(kmNuEToNuTauCCCOH           ): return "nuetonutauCCCOH";
  case(kmNuEToNuTauCCMEC           ): return "nuetonutauCCMEC";
  case(kmNuEToNuTauNCQE            ): return "nuetonutauNCQE";
  case(kmNuEToNuTauNCRESpPi0       ): return "nuetonutauNCRESpPi0";
  case(kmNuEToNuTauNCRESpPiP       ): return "nuetonutauNCRESpPiP";
  case(kmNuEToNuTauNCRESnPi0       ): return "nuetonutauNCRESnPi0";
  case(kmNuEToNuTauNCRESnPiM       ): return "nuetonutauNCRESnPiM";
  case(kmNuEToNuTauNCDIS           ): return "nuetonutauNCDIS";
  case(kmNuEToNuTauNCCOH           ): return "nuetonutauNCCOH";
  case(kmNuEToNuTauNCMEC           ): return "nuetonutauNCMEC";
  case(kmNuEBarToNuMuBarCCQE       ): return "nuebartonumubarCCQE";
  case(kmNuEBarToNuMuBarCCRESnPiM  ): return "nuebartonumubarCCRESnPiM";
  case(kmNuEBarToNuMuBarCCRESpPi0  ): return "nuebartonumubarCCRESpPi0";
  case(kmNuEBarToNuMuBarCCRESpPiM  ): return "nuebartonumubarCCRESpPiM";
  case(kmNuEBarToNuMuBarCCDIS      ): return "nuebartonumubarCCDIS";
  case(kmNuEBarToNuMuBarCCCOH      ): return "nuebartonumubarCCCOH";
  case(kmNuEBarToNuMuBarCCMEC      ): return "nuebartonumubarCCMEC";
  case(kmNuEBarToNuMuBarNCQE       ): return "nuebartonumubarNCQE";
  case(kmNuEBarToNuMuBarNCRESpPi0  ): return "nuebartonumubarNCRESpPi0";
  case(kmNuEBarToNuMuBarNCRESpPiP  ): return "nuebartonumubarNCRESpPiP";
  case(kmNuEBarToNuMuBarNCRESnPi0  ): return "nuebartonumubarNCRESnPi0";
  case(kmNuEBarToNuMuBarNCRESnPiM  ): return "nuebartonumubarNCRESnPiM";
  case(kmNuEBarToNuMuBarNCDIS      ): return "nuebartonumubarNCDIS";
  case(kmNuEBarToNuMuBarNCCOH      ): return "nuebartonumubarNCCOH";
  case(kmNuEBarToNuMuBarNCMEC      ): return "nuebartonumubarNCMEC";
  case(kmNuEBarToNuTauBarCCQE      ): return "nuebartonutaubarCCQE";
  case(kmNuEBarToNuTauBarCCRESnPiM ): return "nuebartonutaubarCCRESnPiM";
  case(kmNuEBarToNuTauBarCCRESpPi0 ): return "nuebartonutaubarCCRESpPi0";
  case(kmNuEBarToNuTauBarCCRESpPiM ): return "nuebartonutaubarCCRESpPiM";
  case(kmNuEBarToNuTauBarCCDIS     ): return "nuebartonutaubarCCDIS";
  case(kmNuEBarToNuTauBarCCCOH     ): return "nuebartonutaubarCCCOH";
  case(kmNuEBarToNuTauBarCCMEC     ): return "nuebartonutaubarCCMEC";
  case(kmNuEBarToNuTauBarNCQE      ): return "nuebartonutaubarNCQE";
  case(kmNuEBarToNuTauBarNCRESpPi0 ): return "nuebartonutaubarNCRESpPi0";
  case(kmNuEBarToNuTauBarNCRESpPiP ): return "nuebartonutaubarNCRESpPiP";
  case(kmNuEBarToNuTauBarNCRESnPi0 ): return "nuebartonutaubarNCRESnPi0";
  case(kmNuEBarToNuTauBarNCRESnPiM ): return "nuebartonutaubarNCRESnPiM";
  case(kmNuEBarToNuTauBarNCDIS     ): return "nuebartonutaubarNCDIS";
  case(kmNuEBarToNuTauBarNCCOH     ): return "nuebartonutaubarNCCOH";
  case(kmNuEBarToNuTauBarNCMEC     ): return "nuebartonutaubarNCMEC";
  case(kmCohElastic                ): return "COHElastic";
  case(kmElScat                    ): return "ELScat";
  case(kmIMDAnh                    ): return "IMDAnh";
  case(kmIBD                       ): return "IBD";
  case(kmGlashow                   ): return "Glashow";
  case(kmAMNuGamma                 ): return "AMNuGamma";
  case(kmDiffractive               ): return "Diffractive";
  case(kmEM                        ): return "EM";
  case(kmWeakMix                   ): return "WeakMix";
  case(kmCosmic                    ): return "Cosmic";
  case(kmDirt                      ): return "Dirt";
  default                  : return "--"; 
  }
  return "";
}
//________________________________________________________________________________________
string util::AsStringTeX(ReacMode_t m, bool root)
{
  string name = "";

  switch(m) {
  case(kmNuMuCCQE        ): name =  "numuCCQE"; break;
  case(kmNuMuCCRESpPiP   ): name =  "numuCCRESpPiP"; break;
  case(kmNuMuCCRESnPi0   ): name =  "numuCCRESnPi0"; break;
  case(kmNuMuCCRESnPiP   ): name =  "numuCCRESnPiP"; break;
  case(kmNuMuCCDIS       ): name =  "numuCCDIS"; break;
  case(kmNuMuCCCOH       ): name =  "numuCCCOH"; break;
  case(kmNuMuCCMEC       ): name =  "numuCCMEC"; break;
  case(kmNuMuNCQE        ): name =  "numuNCQE"; break;
  case(kmNuMuNCRESpPi0   ): name =  "numuNCRESpPi0"; break;
  case(kmNuMuNCRESpPiP   ): name =  "numuNCRESpPiP"; break;
  case(kmNuMuNCRESnPi0   ): name =  "numuNCRESnPi0"; break;
  case(kmNuMuNCRESnPiM   ): name =  "numuNCRESnPiM"; break;
  case(kmNuMuNCDIS       ): name =  "numuNCDIS"; break;
  case(kmNuMuNCCOH       ): name =  "numuNCCOH"; break;
  case(kmNuMuNCMEC       ): name =  "numuNCMEC"; break;
  case(kmNuMuBarCCQE     ): name =  "numubarCCQE"; break;
  case(kmNuMuBarCCRESnPiM): name =  "numubarCCRESnPiM"; break;
  case(kmNuMuBarCCRESpPi0): name =  "numubarCCRESpPi0"; break;
  case(kmNuMuBarCCRESpPiM): name =  "numubarCCRESpPiM"; break;
  case(kmNuMuBarCCDIS    ): name =  "numubarCCDIS"; break;
  case(kmNuMuBarCCCOH    ): name =  "numubarCCCOH"; break;
  case(kmNuMuBarCCMEC    ): name =  "numubarCCMEC"; break;
  case(kmNuMuBarNCQE     ): name =  "numubarNCQE"; break;
  case(kmNuMuBarNCRESpPi0): name =  "numubarNCRESpPi0"; break;
  case(kmNuMuBarNCRESpPiP): name =  "numubarNCRESpPiP"; break;
  case(kmNuMuBarNCRESnPi0): name =  "numubarNCRESnPi0"; break;
  case(kmNuMuBarNCRESnPiM): name =  "numubarNCRESnPiM"; break;
  case(kmNuMuBarNCDIS    ): name =  "numubarNCDIS"; break;
  case(kmNuMuBarNCCOH    ): name =  "numubarNCCOH"; break;
  case(kmNuMuBarNCMEC    ): name =  "numubarNCMEC"; break;
  case(kmNuECCQE         ): name =  "nueCCQE"; break;
  case(kmNuECCRESpPiP    ): name =  "nueCCRESpPiP"; break;
  case(kmNuECCRESnPi0    ): name =  "nueCCRESnPi0"; break;
  case(kmNuECCRESnPiP    ): name =  "nueCCRESnPiP"; break;
  case(kmNuECCDIS        ): name =  "nueCCDIS"; break;
  case(kmNuECCCOH        ): name =  "nueCCCOH"; break;
  case(kmNuECCMEC        ): name =  "nueCCMEC"; break;
  case(kmNuENCQE         ): name =  "nueNCQE"; break;
  case(kmNuENCRESpPi0    ): name =  "nueNCRESpPi0"; break;
  case(kmNuENCRESpPiP    ): name =  "nueNCRESpPiP"; break;
  case(kmNuENCRESnPi0    ): name =  "nueNCRESnPi0"; break;
  case(kmNuENCRESnPiM    ): name =  "nueNCRESnPiM"; break;
  case(kmNuENCDIS        ): name =  "nueNCDIS"; break;
  case(kmNuENCCOH        ): name =  "nueNCCOH"; break;
  case(kmNuENCMEC        ): name =  "nueNCMEC"; break;
  case(kmNuEBarCCQE      ): name =  "nuebarCCQE"; break;
  case(kmNuEBarCCRESnPiM ): name =  "nuebarCCRESnPiM"; break;
  case(kmNuEBarCCRESpPi0 ): name =  "nuebarCCRESpPi0"; break;
  case(kmNuEBarCCRESpPiM ): name =  "nuebarCCRESpPiM"; break;
  case(kmNuEBarCCDIS     ): name =  "nuebarCCDIS"; break;
  case(kmNuEBarCCCOH     ): name =  "nuebarCCCOH"; break;
  case(kmNuEBarCCMEC     ): name =  "nuebarCCMEC"; break;
  case(kmNuEBarNCQE      ): name =  "nuebarNCQE"; break;
  case(kmNuEBarNCRESpPi0 ): name =  "nuebarNCRESpPi0"; break;
  case(kmNuEBarNCRESpPiP ): name =  "nuebarNCRESpPiP"; break;
  case(kmNuEBarNCRESnPi0 ): name =  "nuebarNCRESnPi0"; break;
  case(kmNuEBarNCRESnPiM ): name =  "nuebarNCRESnPiM"; break;
  case(kmNuEBarNCDIS     ): name =  "nuebarNCDIS"; break;
  case(kmNuEBarNCCOH     ): name =  "nuebarNCCOH"; break;
  case(kmNuEBarNCMEC     ): name =  "nuebarNCMEC"; break;
  case(kmNuMuToNuECCQE             ): name =  "numutonueCCQE"; break;
  case(kmNuMuToNuECCRESpPiP        ): name =  "numutonueCCRESpPiP"; break;
  case(kmNuMuToNuECCRESnPi0        ): name =  "numutonueCCRESnPi0"; break;
  case(kmNuMuToNuECCRESnPiP        ): name =  "numutonueCCRESnPiP"; break;
  case(kmNuMuToNuECCDIS            ): name =  "numutonueCCDIS"; break;
  case(kmNuMuToNuECCCOH            ): name =  "numutonueCCCOH"; break;
  case(kmNuMuToNuECCMEC            ): name =  "numutonueCCMEC"; break;
  case(kmNuMuToNuENCQE             ): name =  "numutonueNCQE"; break;
  case(kmNuMuToNuENCRESpPi0        ): name =  "numutonueNCRESpPi0"; break;
  case(kmNuMuToNuENCRESpPiP        ): name =  "numutonueNCRESpPiP"; break;
  case(kmNuMuToNuENCRESnPi0        ): name =  "numutonueNCRESnPi0"; break;
  case(kmNuMuToNuENCRESnPiM        ): name =  "numutonueNCRESnPiM"; break;
  case(kmNuMuToNuENCDIS            ): name =  "numutonueNCDIS"; break;
  case(kmNuMuToNuENCCOH            ): name =  "numutonueNCCOH"; break;
  case(kmNuMuToNuENCMEC            ): name =  "numutonueNCMEC"; break;
  case(kmNuMuToNuTauCCQE           ): name =  "numutonutauCCQE"; break;
  case(kmNuMuToNuTauCCRESpPiP      ): name =  "numutonutauCCRESpPiP"; break;
  case(kmNuMuToNuTauCCRESnPi0      ): name =  "numutonutauCCRESnPi0"; break;
  case(kmNuMuToNuTauCCRESnPiP      ): name =  "numutonutauCCRESnPiP"; break;
  case(kmNuMuToNuTauCCDIS          ): name =  "numutonutauCCDIS"; break;
  case(kmNuMuToNuTauCCCOH          ): name =  "numutonutauCCCOH"; break;
  case(kmNuMuToNuTauCCMEC          ): name =  "numutonutauCCMEC"; break;
  case(kmNuMuToNuTauNCQE           ): name =  "numutonutauNCQE"; break;
  case(kmNuMuToNuTauNCRESpPi0      ): name =  "numutonutauNCRESpPi0"; break;
  case(kmNuMuToNuTauNCRESpPiP      ): name =  "numutonutauNCRESpPiP"; break;
  case(kmNuMuToNuTauNCRESnPi0      ): name =  "numutonutauNCRESnPi0"; break;
  case(kmNuMuToNuTauNCRESnPiM      ): name =  "numutonutauNCRESnPiM"; break;
  case(kmNuMuToNuTauNCDIS          ): name =  "numutonutauNCDIS"; break;
  case(kmNuMuToNuTauNCCOH          ): name =  "numutonutauNCCOH"; break;
  case(kmNuMuToNuTauNCMEC          ): name =  "numutonutauNCMEC"; break;
  case(kmNuMuBarToNuEBarCCQE       ): name =  "numubartonuebarCCQE"; break;
  case(kmNuMuBarToNuEBarCCRESnPiM  ): name =  "numubartonuebarCCRESnPiM"; break;
  case(kmNuMuBarToNuEBarCCRESpPi0  ): name =  "numubartonuebarCCRESpPi0"; break;
  case(kmNuMuBarToNuEBarCCRESpPiM  ): name =  "numubartonuebarCCRESpPiM"; break;
  case(kmNuMuBarToNuEBarCCDIS      ): name =  "numubartonuebarCCDIS"; break;
  case(kmNuMuBarToNuEBarCCCOH      ): name =  "numubartonuebarCCCOH"; break;
  case(kmNuMuBarToNuEBarCCMEC      ): name =  "numubartonuebarCCMEC"; break;
  case(kmNuMuBarToNuEBarNCQE       ): name =  "numubartonuebarNCQE"; break;
  case(kmNuMuBarToNuEBarNCRESpPi0  ): name =  "numubartonuebarNCRESpPi0"; break;
  case(kmNuMuBarToNuEBarNCRESpPiP  ): name =  "numubartonuebarNCRESpPiP"; break;
  case(kmNuMuBarToNuEBarNCRESnPi0  ): name =  "numubartonuebarNCRESnPi0"; break;
  case(kmNuMuBarToNuEBarNCRESnPiM  ): name =  "numubartonuebarNCRESnPiM"; break;
  case(kmNuMuBarToNuEBarNCDIS      ): name =  "numubartonuebarNCDIS"; break;
  case(kmNuMuBarToNuEBarNCCOH      ): name =  "numubartonuebarNCCOH"; break;
  case(kmNuMuBarToNuEBarNCMEC      ): name =  "numubartonuebarNCMEC"; break;
  case(kmNuMuBarToNuTauBarCCQE     ): name =  "numubartonutaubarCCQE"; break;
  case(kmNuMuBarToNuTauBarCCRESnPiM): name =  "numubartonutaubarCCRESnPiM"; break;
  case(kmNuMuBarToNuTauBarCCRESpPi0): name =  "numubartonutaubarCCRESpPi0"; break;
  case(kmNuMuBarToNuTauBarCCRESpPiM): name =  "numubartonutaubarCCRESpPiM"; break;
  case(kmNuMuBarToNuTauBarCCDIS    ): name =  "numubartonutaubarCCDIS"; break;
  case(kmNuMuBarToNuTauBarCCCOH    ): name =  "numubartonutaubarCCCOH"; break;
  case(kmNuMuBarToNuTauBarCCMEC    ): name =  "numubartonutaubarCCMEC"; break;
  case(kmNuMuBarToNuTauBarNCQE     ): name =  "numubartonutaubarNCQE"; break;
  case(kmNuMuBarToNuTauBarNCRESpPi0): name =  "numubartonutaubarNCRESpPi0"; break;
  case(kmNuMuBarToNuTauBarNCRESpPiP): name =  "numubartonutaubarNCRESpPiP"; break;
  case(kmNuMuBarToNuTauBarNCRESnPi0): name =  "numubartonutaubarNCRESnPi0"; break;
  case(kmNuMuBarToNuTauBarNCRESnPiM): name =  "numubartonutaubarNCRESnPiM"; break;
  case(kmNuMuBarToNuTauBarNCDIS    ): name =  "numubartonutaubarNCDIS"; break;
  case(kmNuMuBarToNuTauBarNCCOH    ): name =  "numubartonutaubarNCCOH"; break;
  case(kmNuMuBarToNuTauBarNCMEC    ): name =  "numubartonutaubarNCMEC"; break;
  case(kmNuEToNuMuCCQE             ): name =  "nuetonumuCCQE"; break;
  case(kmNuEToNuMuCCRESpPiP        ): name =  "nuetonumuCCRESpPiP"; break;
  case(kmNuEToNuMuCCRESnPi0        ): name =  "nuetonumuCCRESnPi0"; break;
  case(kmNuEToNuMuCCRESnPiP        ): name =  "nuetonumuCCRESnPiP"; break;
  case(kmNuEToNuMuCCDIS            ): name =  "nuetonumuCCDIS"; break;
  case(kmNuEToNuMuCCCOH            ): name =  "nuetonumuCCCOH"; break;
  case(kmNuEToNuMuCCMEC            ): name =  "nuetonumuCCMEC"; break;
  case(kmNuEToNuMuNCQE             ): name =  "nuetonumuNCQE"; break;
  case(kmNuEToNuMuNCRESpPi0        ): name =  "nuetonumuNCRESpPi0"; break;
  case(kmNuEToNuMuNCRESpPiP        ): name =  "nuetonumuNCRESpPiP"; break;
  case(kmNuEToNuMuNCRESnPi0        ): name =  "nuetonumuNCRESnPi0"; break;
  case(kmNuEToNuMuNCRESnPiM        ): name =  "nuetonumuNCRESnPiM"; break;
  case(kmNuEToNuMuNCDIS            ): name =  "nuetonumuNCDIS"; break;
  case(kmNuEToNuMuNCCOH            ): name =  "nuetonumuNCCOH"; break;
  case(kmNuEToNuMuNCMEC            ): name =  "nuetonumuNCMEC"; break;
  case(kmNuEToNuTauCCQE            ): name =  "nuetonutauCCQE"; break;
  case(kmNuEToNuTauCCRESpPiP       ): name =  "nuetonutauCCRESpPiP"; break;
  case(kmNuEToNuTauCCRESnPi0       ): name =  "nuetonutauCCRESnPi0"; break;
  case(kmNuEToNuTauCCRESnPiP       ): name =  "nuetonutauCCRESnPiP"; break;
  case(kmNuEToNuTauCCDIS           ): name =  "nuetonutauCCDIS"; break;
  case(kmNuEToNuTauCCCOH           ): name =  "nuetonutauCCCOH"; break;
  case(kmNuEToNuTauCCMEC           ): name =  "nuetonutauCCMEC"; break;
  case(kmNuEToNuTauNCQE            ): name =  "nuetonutauNCQE"; break;
  case(kmNuEToNuTauNCRESpPi0       ): name =  "nuetonutauNCRESpPi0"; break;
  case(kmNuEToNuTauNCRESpPiP       ): name =  "nuetonutauNCRESpPiP"; break;
  case(kmNuEToNuTauNCRESnPi0       ): name =  "nuetonutauNCRESnPi0"; break;
  case(kmNuEToNuTauNCRESnPiM       ): name =  "nuetonutauNCRESnPiM"; break;
  case(kmNuEToNuTauNCDIS           ): name =  "nuetonutauNCDIS"; break;
  case(kmNuEToNuTauNCCOH           ): name =  "nuetonutauNCCOH"; break;
  case(kmNuEToNuTauNCMEC           ): name =  "nuetonutauNCMEC"; break;
  case(kmNuEBarToNuMuBarCCQE       ): name =  "nuebartonumubarCCQE"; break;
  case(kmNuEBarToNuMuBarCCRESnPiM  ): name =  "nuebartonumubarCCRESnPiM"; break;
  case(kmNuEBarToNuMuBarCCRESpPi0  ): name =  "nuebartonumubarCCRESpPi0"; break;
  case(kmNuEBarToNuMuBarCCRESpPiM  ): name =  "nuebartonumubarCCRESpPiM"; break;
  case(kmNuEBarToNuMuBarCCDIS      ): name =  "nuebartonumubarCCDIS"; break;
  case(kmNuEBarToNuMuBarCCCOH      ): name =  "nuebartonumubarCCCOH"; break;
  case(kmNuEBarToNuMuBarCCMEC      ): name =  "nuebartonumubarCCMEC"; break;
  case(kmNuEBarToNuMuBarNCQE       ): name =  "nuebartonumubarNCQE"; break;
  case(kmNuEBarToNuMuBarNCRESpPi0  ): name =  "nuebartonumubarNCRESpPi0"; break;
  case(kmNuEBarToNuMuBarNCRESpPiP  ): name =  "nuebartonumubarNCRESpPiP"; break;
  case(kmNuEBarToNuMuBarNCRESnPi0  ): name =  "nuebartonumubarNCRESnPi0"; break;
  case(kmNuEBarToNuMuBarNCRESnPiM  ): name =  "nuebartonumubarNCRESnPiM"; break;
  case(kmNuEBarToNuMuBarNCDIS      ): name =  "nuebartonumubarNCDIS"; break;
  case(kmNuEBarToNuMuBarNCCOH      ): name =  "nuebartonumubarNCCOH"; break;
  case(kmNuEBarToNuMuBarNCMEC      ): name =  "nuebartonumubarNCMEC"; break;
  case(kmNuEBarToNuTauBarCCQE      ): name =  "nuebartonutaubarCCQE"; break;
  case(kmNuEBarToNuTauBarCCRESnPiM ): name =  "nuebartonutaubarCCRESnPiM"; break;
  case(kmNuEBarToNuTauBarCCRESpPi0 ): name =  "nuebartonutaubarCCRESpPi0"; break;
  case(kmNuEBarToNuTauBarCCRESpPiM ): name =  "nuebartonutaubarCCRESpPiM"; break;
  case(kmNuEBarToNuTauBarCCDIS     ): name =  "nuebartonutaubarCCDIS"; break;
  case(kmNuEBarToNuTauBarCCCOH     ): name =  "nuebartonutaubarCCCOH"; break;
  case(kmNuEBarToNuTauBarCCMEC     ): name =  "nuebartonutaubarCCMEC"; break;
  case(kmNuEBarToNuTauBarNCQE      ): name =  "nuebartonutaubarNCQE"; break;
  case(kmNuEBarToNuTauBarNCRESpPi0 ): name =  "nuebartonutaubarNCRESpPi0"; break;
  case(kmNuEBarToNuTauBarNCRESpPiP ): name =  "nuebartonutaubarNCRESpPiP"; break;
  case(kmNuEBarToNuTauBarNCRESnPi0 ): name =  "nuebartonutaubarNCRESnPi0"; break;
  case(kmNuEBarToNuTauBarNCRESnPiM ): name =  "nuebartonutaubarNCRESnPiM"; break;
  case(kmNuEBarToNuTauBarNCDIS     ): name =  "nuebartonutaubarNCDIS"; break;
  case(kmNuEBarToNuTauBarNCCOH     ): name =  "nuebartonutaubarNCCOH"; break;
  case(kmNuEBarToNuTauBarNCMEC     ): name =  "nuebartonutaubarNCMEC"; break;
  case(kmCohElastic                ): name =  "COHElastic"; break;
  case(kmElScat                    ): name =  "ELScat"; break;
  case(kmIMDAnh                    ): name =  "IMDAnh"; break;
  case(kmIBD                       ): name =  "IBD"; break;
  case(kmGlashow                   ): name =  "Glashow"; break;
  case(kmAMNuGamma                 ): name =  "AMNuGamma"; break;
  case(kmDiffractive               ): name =  "Diffractive"; break;
  case(kmEM                        ): name =  "EM"; break;
  case(kmWeakMix                   ): name =  "WeakMix"; break;
  case(kmCosmic                    ): name =  "Cosmic"; break;
  case(kmDirt                      ): name =  "Dirt"; break;
  default                  : name = "--"; 
  }

  if(root) return name;

  // normal TeX, not ROOT TeX - change '#' with '\'
  replace( name.begin(), name.end(), '#', '\\');

  return name;
}
//________________________________________________________________________________________
std::string util::AsString(conv::ReacType_t t)
{
  switch (t) {
  case(kmtCCQE       ): return "CCQE";
  case(kmtCCRESvpPiP ): return "CCRESvpPiP";
  case(kmtCCRESvnPi0 ): return "CCRESvnPi0";
  case(kmtCCRESvnPiP ): return "CCRESvnPiP";
  case(kmtCCRESvbnPiM): return "CCRESvbnPiM";
  case(kmtCCRESvbpPi0): return "CCRESvbpPi0";
  case(kmtCCRESvbpPiM): return "CCRESvbpPiM";
  case(kmtCCDIS      ): return "CCDIS";
  case(kmtCCCOH      ): return "CCCOH";
  case(kmtCCMEC      ): return "CCMEC";
  case(kmtNCQE       ): return "NCQE";
  case(kmtNCRESvpPi0 ): return "NCRESvpPi0";
  case(kmtNCRESvpPiP ): return "NCRESvpPiP";
  case(kmtNCRESvnPi0 ): return "NCRESvnPi0";
  case(kmtNCRESvnPiM ): return "NCRESvnPiM";
  case(kmtNCRESvbpPi0): return "NCRESvbpPi0";
  case(kmtNCRESvbpPiP): return "NCRESvbpPiP";
  case(kmtNCRESvbnPi0): return "NCRESvbnPi0";
  case(kmtNCRESvbnPiM): return "NCRESvbnPiM";
  case(kmtNCDIS      ): return "NCDIS";
  case(kmtNCCOH      ): return "NCCOH";
  case(kmtNCMEC      ): return "NCMEC";
  default             : return "--";
  }
  return "--";
}
//________________________________________________________________________________________
conv::NuFlavour_t  util::ReacModeToInitFlavor(conv::ReacMode_t m)
{
  //~ int idx = ArrIdx::Instance()->ReacModeIdx(m);
  return kReacModeToInitFlavour[m];
}
//________________________________________________________________________________________
conv::NuFlavour_t  util::ReacModeToFinalFlavor(conv::ReacMode_t m)
{
  //~ int idx = ArrIdx::Instance()->ReacModeIdx(m);
  return kReacModeToFinalFlavour[m];
}
//________________________________________________________________________________________
conv::ReacType_t  util::ReacModeToType(conv::ReacMode_t m)
{
  //~ int idx = ArrIdx::Instance()->ReacModeIdx(m);
  return kReacModeToType[m];
}
//________________________________________________________________________________________
bool util::IsNuMu(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuMu && kReacModeToFinalFlavour[m]==kNuMu)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsNuMuBar(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuMuBar && kReacModeToFinalFlavour[m]==kNuMuBar)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsNuE(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuE && kReacModeToFinalFlavour[m]==kNuE)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsNuEBar(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuEBar && kReacModeToFinalFlavour[m]==kNuEBar)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsNuMuToNuE(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuMu && kReacModeToFinalFlavour[m]==kNuE)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsNuMuToNuTau(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuMu && kReacModeToFinalFlavour[m]==kNuTau)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsNeutrinoAny(conv::ReacMode_t m)
{
  return (m>=0 && m<kmCosmic);
}
//________________________________________________________________________________________
bool util::IsNuEToNuMu(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuE && kReacModeToFinalFlavour[m]==kNuMu)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsNuEToNuTau(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuE && kReacModeToFinalFlavour[m]==kNuTau)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsNuMuBarToNuEBar(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuMuBar && kReacModeToFinalFlavour[m]==kNuEBar)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsNuMuBarToNuTauBar(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuMuBar && kReacModeToFinalFlavour[m]==kNuTauBar)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsNuEBarToNuMuBar(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuEBar && kReacModeToFinalFlavour[m]==kNuMuBar)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsNuEBarToNuTauBar(conv::ReacMode_t m)
{
  if (kReacModeToInitFlavour[m]==kNuEBar && kReacModeToFinalFlavour[m]==kNuTauBar)
    return true;
  else
    return false; 
}
//________________________________________________________________________________________
bool util::IsCosmic(conv::ReacMode_t m)
{
  if (m == kmCosmic)
    return true;
  else
    return false; 
}

//________________________________________________________________________________________
bool util::IsDirt(conv::ReacMode_t m)
{
  if (m == kmDirt)
    return true;
  else
    return false; 
}

//________________________________________________________________________________________
bool util::IsCC(conv::ReacMode_t m)
{
  switch (m) {
  case(kmCohElastic )                   : return false;
  case(kmElScat     )                   : return false;
  case(kmIMDAnh     )                   : return false;
  case(kmIBD        )                   : return false;
  case(kmGlashow    )                   : return false;  
  case(kmAMNuGamma  )                   : return false;
  case(kmDiffractive)                   : return false;
  case(kmEM         )                   : return false;
  case(kmWeakMix    )                   : return false;
  case(kmCosmic     )                   : return false;
  case(kmDirt       )                   : return false;
  default                               : return true;
  }
  return true;
}

//________________________________________________________________________________________
ReacMode_t util::ReacModeFromInt(int m)
{
// Cast an integer to ReacMode_t, but with boundary check
// Ugh.. This is awkward. Piecemeal it is.
//             [first]                  [last]
//  if(m >= (int)kmNuMuCCQE && m <= (int) kmOther) return (ReacMode_t)m;
  if( ( std::abs(m) > 0 && std::abs(m) <= (int) kmNuENCMEC ) ||
      ( std::abs(m) >= (int) kmNuMuToNuECCQE && std::abs(m) <= (int) kmNuMuToNuENCMEC ) ||
      ( std::abs(m) >= (int) kmNuMuToNuTauCCQE && std::abs(m) <= (int) kmNuMuToNuTauNCMEC ) ||
      ( std::abs(m) >= (int) kmNuEToNuMuCCQE && std::abs(m) <= (int) kmNuEToNuMuNCMEC ) ||
      ( std::abs(m) >= (int) kmNuEToNuTauCCQE && std::abs(m) <= (int) kmNuEToNuTauNCMEC ) ) {
    return (ReacMode_t)m;
  }
  return kmUndefined;
}
//________________________________________________________________________________________
//aliases for systems that can't overload based on enum type (eg python)
string    util::ModeAsStringTeX (ReacMode_t m, bool root) {return util::AsStringTeX(m,root);}
string    util::ModeAsString    (ReacMode_t m) {return util::AsString(m);}
bool      util::ModeIsCC        (ReacMode_t m) {return util::IsCC(m);}
string    util::ReacTypeAsString(ReacType_t t) {return util::AsString(t);}
//________________________________________________________________________________________
ReacMode_t util::NuEModeToOscNuEMode(ReacMode_t m)
{
  // Deactivating this switch.
  return kmUndefined;
  /*
  switch (m) {
    case kmNuECCQE        : return kmNuMuToNuECCQE           ;
    case kmNuECCMEC       : return kmNuMuToNuECCMEC          ;
    case kmNuECC1piC      : return kmNuMuToNuECC1piC         ;
    case kmNuECC1pi0      : return kmNuMuToNuECC1pi0         ;
    case kmNuECC2piC      : return kmNuMuToNuECC2piC         ;
    case kmNuECC2pi0      : return kmNuMuToNuECC2pi0         ;
    case kmNuECCpiCpi0    : return kmNuMuToNuECCpiCpi0       ;
    case kmNuECCcoh       : return kmNuMuToNuECCcoh          ;
    case kmNuECCDIS       : return kmNuMuToNuECCDIS          ;
    case kmNuECCMpi       : return kmNuMuToNuECCMpi          ;
    case kmNuECCmisc      : return kmNuMuToNuECCmisc         ;
    case kmNuECCoth       : return kmNuMuToNuECCoth          ;
    case kmNuEELScat      : return kmNuMuToNuEELScat         ;
    case kmNuEBarCCQE     : return kmNuMuBarToNuEBarCCQE     ;
    case kmNuEBarCCMEC    : return kmNuMuBarToNuEBarCCMEC    ;
    case kmNuEBarCC1piC   : return kmNuMuBarToNuEBarCC1piC   ;
    case kmNuEBarCC1pi0   : return kmNuMuBarToNuEBarCC1pi0   ;
    case kmNuEBarCC2piC   : return kmNuMuBarToNuEBarCC2piC   ;
    case kmNuEBarCC2pi0   : return kmNuMuBarToNuEBarCC2pi0   ;
    case kmNuEBarCCpiCpi0 : return kmNuMuBarToNuEBarCCpiCpi0 ;
    case kmNuEBarCCcoh    : return kmNuMuBarToNuEBarCCcoh    ;
    case kmNuEBarCCIMD    : return kmNuMuBarToNuEBarCCIMD    ;
    case kmNuEBarCCDIS    : return kmNuMuBarToNuEBarCCDIS    ;
    case kmNuEBarCCMpi    : return kmNuMuBarToNuEBarCCMpi    ;
    case kmNuEBarCCmisc   : return kmNuMuBarToNuEBarCCmisc   ;
    case kmNuEBarCCoth    : return kmNuMuBarToNuEBarCCoth    ;
    case kmNuEBarELScat   : return kmNuMuBarToNuEBarELScat   ;
    
    default: return kmUndefined;
  }
  */
}
//________________________________________________________________________________________
conv::ReacMode_t util::ReacTypeToMode (conv::ReacType_t t, conv::NuFlavour_t nu_init, conv::NuFlavour_t nu_final)
{
  /*
  using namespace util;
  for (int im = 0 ; im < kNReacModes ; im++) {
    conv::ReacMode_t  m       =  util::ReacModeFromInt(im);
    conv::ReacType_t  m_t     =  util::ReacModeToType       (m);
    conv::NuFlavour_t m_init  =  util::ReacModeToInitFlavor (m);
    conv::NuFlavour_t m_final =  util::ReacModeToFinalFlavor(m);
    if (m_t == t && m_init == nu_init && nu_final == m_final) {
      return m;
    }
  }
  */
  return conv::kmUndefined;
}
//________________________________________________________________________________________
conv::ReacMode_t util::ReacModeFromString (std::string mode)
{
  /*
  string lc_mode = util::ToLowerCase(mode);
  for (int i = 0 ; i < kmOther ; i++ ) {
    ReacMode_t m = (ReacMode_t)i;
    string this_mode = util::ToLowerCase(util::AsString(m));
    if(lc_mode==this_mode && this_mode!="--") {
      return m;
    }
  }
  */
  return kmUndefined;
}
