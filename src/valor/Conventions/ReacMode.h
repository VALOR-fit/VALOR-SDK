//____________________________________________________________________________
/*!

\class     valor::conv::ReacMode_t

\brief     Enumeration of true reaction modes considered separately within 
           each DUNE analysis sample

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   April 15, 2014

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_reaction_modes_h
#define _valor_reaction_modes_h

#include <string>
#include <unordered_map>

//FWMK
#include "valor/Conventions/NuFlavour.h"

namespace valor {
namespace conv {

static const int kNReacModes = 191; //excluding undefined and other

typedef enum EReacMode 
{
   kmUndefined        = -999, 
   kmNuMuCCQE         =   1,       // numu    CC QE
   kmNuMuCCRESpPiP    =   2,       // numu    CC RES : v + p --> mu- + p + pi+
   kmNuMuCCRESnPi0    =   3,       // numu    CC RES : v + n --> mu- + p + pi0
   kmNuMuCCRESnPiP    =   4,       // numu    CC RES : v + n --> mu- + n + pi+
   kmNuMuCCDIS        =   5,       // numu    CC DIS
   kmNuMuCCCOH        =   6,       // numu    CC COH
   kmNuMuCCMEC        =   7,       // numu    CC MEC
   kmNuMuNCQE         =   8,       // numu    NC QE
   kmNuMuNCRESpPi0    =   9,       // numu    NC RES : v + p --> v + p + pi0
   kmNuMuNCRESpPiP    =  10,       // numu    NC RES : v + p --> v + n + pi+
   kmNuMuNCRESnPi0    =  11,       // numu    NC RES : v + n --> v + n + pi0
   kmNuMuNCRESnPiM    =  12,       // numu    NC RES : v + n --> v + p + pi-
   kmNuMuNCDIS        =  13,       // numu    NC DIS
   kmNuMuNCCOH        =  14,       // numu    NC COH
   kmNuMuNCMEC        =  15,       // numu    NC MEC
   kmNuMuBarCCQE      = - 1,       // numubar CC QE
   kmNuMuBarCCRESnPiM = - 2,       // numubar CC RES : vb + n --> mu+ + n + pi-
   kmNuMuBarCCRESpPi0 = - 3,       // numubar CC RES : vb + p --> mu+ + n + pi0
   kmNuMuBarCCRESpPiM = - 4,       // numubar CC RES : vb + p --> mu+ + p + pi-
   kmNuMuBarCCDIS     = - 5,       // numubar CC DIS
   kmNuMuBarCCCOH     = - 6,       // numubar CC COH
   kmNuMuBarCCMEC     = - 7,       // numubar CC MEC
   kmNuMuBarNCQE      = - 8,       // numubar NC QE
   kmNuMuBarNCRESpPi0 = - 9,       // numubar NC RES : vb + p --> vb + p + pi0
   kmNuMuBarNCRESpPiP = -10,       // numubar NC RES : vb + p --> vb + n + pi+
   kmNuMuBarNCRESnPi0 = -11,       // numubar NC RES : vb + n --> vb + n + pi0
   kmNuMuBarNCRESnPiM = -12,       // numubar NC RES : vb + n --> vb + p + pi-
   kmNuMuBarNCDIS     = -13,       // numubar NC DIS
   kmNuMuBarNCCOH     = -14,       // numubar NC COH
   kmNuMuBarNCMEC     = -15,       // numubar NC MEC
   kmNuECCQE          =  16,       // nue    CC QE
   kmNuECCRESpPiP     =  17,       // nue    CC RES : v + p --> e- + p + pi+
   kmNuECCRESnPi0     =  18,       // nue    CC RES : v + n --> e- + p + pi0
   kmNuECCRESnPiP     =  19,       // nue    CC RES : v + n --> e- + n + pi+
   kmNuECCDIS         =  20,       // nue    CC DIS
   kmNuECCCOH         =  21,       // nue    CC COH
   kmNuECCMEC         =  22,       // nue    CC MEC
   kmNuENCQE          =  23,       // nue    NC QE
   kmNuENCRESpPi0     =  24,       // nue    NC RES : v + p --> v + p + pi0
   kmNuENCRESpPiP     =  25,       // nue    NC RES : v + p --> v + n + pi+
   kmNuENCRESnPi0     =  26,       // nue    NC RES : v + n --> v + n + pi0
   kmNuENCRESnPiM     =  27,       // nue    NC RES : v + n --> v + p + pi-
   kmNuENCDIS         =  28,       // nue    NC DIS
   kmNuENCCOH         =  29,       // nue    NC COH
   kmNuENCMEC         =  30,       // nue    NC MEC
   kmNuEBarCCQE       = -16,       // nuebar CC QE
   kmNuEBarCCRESnPiM  = -17,       // nuebar CC RES : vb + n --> e+ + n + pi-
   kmNuEBarCCRESpPi0  = -18,       // nuebar CC RES : vb + p --> e+ + n + pi0
   kmNuEBarCCRESpPiM  = -19,       // nuebar CC RES : vb + p --> e+ + p + pi-
   kmNuEBarCCDIS      = -20,       // nuebar CC DIS
   kmNuEBarCCCOH      = -21,       // nuebar CC COH
   kmNuEBarCCMEC      = -22,       // nuebar CC MEC
   kmNuEBarNCQE       = -23,       // nuebar NC QE
   kmNuEBarNCRESpPi0  = -24,       // nuebar NC RES : vb + p --> vb + p + pi0
   kmNuEBarNCRESpPiP  = -25,       // nuebar NC RES : vb + p --> vb + n + pi+
   kmNuEBarNCRESnPi0  = -26,       // nuebar NC RES : vb + n --> vb + n + pi0
   kmNuEBarNCRESnPiM  = -27,       // nuebar NC RES : vb + n --> vb + p + pi-
   kmNuEBarNCDIS      = -28,       // nuebar NC DIS
   kmNuEBarNCCOH      = -29,       // nuebar NC COH
   kmNuEBarNCMEC      = -30,       // nuebar NC MEC
   // For some reason, I have to add oscillated modes separately. 
   // mu to e
   kmNuMuToNuECCQE         =  101, // mu->e CC QE
   kmNuMuToNuECCRESpPiP    =  102, // mu->e CC RES : v + p --> e- + p + pi+
   kmNuMuToNuECCRESnPi0    =  103, // mu->e CC RES : v + n --> e- + p + pi0
   kmNuMuToNuECCRESnPiP    =  104, // mu->e CC RES : v + n --> e- + n + pi+
   kmNuMuToNuECCDIS        =  105, // mu->e CC DIS
   kmNuMuToNuECCCOH        =  106, // mu->e CC COH
   kmNuMuToNuECCMEC        =  107, // mu->e CC MEC
   kmNuMuToNuENCQE         =  108, // mu->e NC QE
   kmNuMuToNuENCRESpPi0    =  109, // mu->e NC RES : v + p --> v + p + pi0
   kmNuMuToNuENCRESpPiP    =  110, // mu->e NC RES : v + p --> v + n + pi+
   kmNuMuToNuENCRESnPi0    =  111, // mu->e NC RES : v + n --> v + n + pi0
   kmNuMuToNuENCRESnPiM    =  112, // mu->e NC RES : v + n --> v + p + pi-
   kmNuMuToNuENCDIS        =  113, // mu->e NC DIS
   kmNuMuToNuENCCOH        =  114, // mu->e NC COH
   kmNuMuToNuENCMEC        =  115, // mu->e NC MEC
   // mu to tau
   kmNuMuToNuTauCCQE         =  201, // mu->tau CC QE
   kmNuMuToNuTauCCRESpPiP    =  202, // mu->tau CC RES : v + p --> tau- + p + pi+
   kmNuMuToNuTauCCRESnPi0    =  203, // mu->tau CC RES : v + n --> tau- + p + pi0
   kmNuMuToNuTauCCRESnPiP    =  204, // mu->tau CC RES : v + n --> tau- + n + pi+
   kmNuMuToNuTauCCDIS        =  205, // mu->tau CC DIS
   kmNuMuToNuTauCCCOH        =  206, // mu->tau CC COH
   kmNuMuToNuTauCCMEC        =  207, // mu->tau CC MEC
   kmNuMuToNuTauNCQE         =  208, // mu->tau NC QE
   kmNuMuToNuTauNCRESpPi0    =  209, // mu->tau NC RES : v + p --> v + p + pi0
   kmNuMuToNuTauNCRESpPiP    =  210, // mu->tau NC RES : v + p --> v + n + pi+
   kmNuMuToNuTauNCRESnPi0    =  211, // mu->tau NC RES : v + n --> v + n + pi0
   kmNuMuToNuTauNCRESnPiM    =  212, // mu->tau NC RES : v + n --> v + p + pi-
   kmNuMuToNuTauNCDIS        =  213, // mu->tau NC DIS
   kmNuMuToNuTauNCCOH        =  214, // mu->tau NC COH
   kmNuMuToNuTauNCMEC        =  215, // mu->tau NC MEC
   // mub to eb
   kmNuMuBarToNuEBarCCQE      = -101, // mub->eb CC QE
   kmNuMuBarToNuEBarCCRESnPiM = -102, // mub->eb CC RES : vb + n --> e+ + n + pi-
   kmNuMuBarToNuEBarCCRESpPi0 = -103, // mub->eb CC RES : vb + p --> e+ + n + pi0
   kmNuMuBarToNuEBarCCRESpPiM = -104, // mub->eb CC RES : vb + p --> e+ + p + pi-
   kmNuMuBarToNuEBarCCDIS     = -105, // mub->eb CC DIS
   kmNuMuBarToNuEBarCCCOH     = -106, // mub->eb CC COH
   kmNuMuBarToNuEBarCCMEC     = -107, // mub->eb CC MEC
   kmNuMuBarToNuEBarNCQE      = -108, // mub->eb NC QE
   kmNuMuBarToNuEBarNCRESpPi0 = -109, // mub->eb NC RES : vb + p --> vb + p + pi0
   kmNuMuBarToNuEBarNCRESpPiP = -110, // mub->eb NC RES : vb + p --> vb + n + pi+
   kmNuMuBarToNuEBarNCRESnPi0 = -111, // mub->eb NC RES : vb + n --> vb + n + pi0
   kmNuMuBarToNuEBarNCRESnPiM = -112, // mub->eb NC RES : vb + n --> vb + p + pi-
   kmNuMuBarToNuEBarNCDIS     = -113, // mub->eb NC DIS
   kmNuMuBarToNuEBarNCCOH     = -114, // mub->eb NC COH
   kmNuMuBarToNuEBarNCMEC     = -115, // mub->eb NC MEC
   // mub to taub
   kmNuMuBarToNuTauBarCCQE      = -201, // mub->taub CC QE
   kmNuMuBarToNuTauBarCCRESnPiM = -202, // mub->taub CC RES : vb + n --> e+ + n + pi-
   kmNuMuBarToNuTauBarCCRESpPi0 = -203, // mub->taub CC RES : vb + p --> e+ + n + pi0
   kmNuMuBarToNuTauBarCCRESpPiM = -204, // mub->taub CC RES : vb + p --> e+ + p + pi-
   kmNuMuBarToNuTauBarCCDIS     = -205, // mub->taub CC DIS
   kmNuMuBarToNuTauBarCCCOH     = -206, // mub->taub CC COH
   kmNuMuBarToNuTauBarCCMEC     = -207, // mub->taub CC MEC
   kmNuMuBarToNuTauBarNCQE      = -208, // mub->taub NC QE
   kmNuMuBarToNuTauBarNCRESpPi0 = -209, // mub->taub NC RES : vb + p --> vb + p + pi0
   kmNuMuBarToNuTauBarNCRESpPiP = -210, // mub->taub NC RES : vb + p --> vb + n + pi+
   kmNuMuBarToNuTauBarNCRESnPi0 = -211, // mub->taub NC RES : vb + n --> vb + n + pi0
   kmNuMuBarToNuTauBarNCRESnPiM = -212, // mub->taub NC RES : vb + n --> vb + p + pi-
   kmNuMuBarToNuTauBarNCDIS     = -213, // mub->taub NC DIS
   kmNuMuBarToNuTauBarNCCOH     = -214, // mub->taub NC COH
   kmNuMuBarToNuTauBarNCMEC     = -215, // mub->taub NC MEC
   // e to mu
   kmNuEToNuMuCCQE         =  301, // e->mu CC QE
   kmNuEToNuMuCCRESpPiP    =  302, // e->mu CC RES : v + p --> mu- + p + pi+
   kmNuEToNuMuCCRESnPi0    =  303, // e->mu CC RES : v + n --> mu- + p + pi0
   kmNuEToNuMuCCRESnPiP    =  304, // e->mu CC RES : v + n --> mu- + n + pi+
   kmNuEToNuMuCCDIS        =  305, // e->mu CC DIS
   kmNuEToNuMuCCCOH        =  306, // e->mu CC COH
   kmNuEToNuMuCCMEC        =  307, // e->mu CC MEC
   kmNuEToNuMuNCQE         =  308, // e->mu NC QE
   kmNuEToNuMuNCRESpPi0    =  309, // e->mu NC RES : v + p --> v + p + pi0
   kmNuEToNuMuNCRESpPiP    =  310, // e->mu NC RES : v + p --> v + n + pi+
   kmNuEToNuMuNCRESnPi0    =  311, // e->mu NC RES : v + n --> v + n + pi0
   kmNuEToNuMuNCRESnPiM    =  312, // e->mu NC RES : v + n --> v + p + pi-
   kmNuEToNuMuNCDIS        =  313, // e->mu NC DIS
   kmNuEToNuMuNCCOH        =  314, // e->mu NC COH
   kmNuEToNuMuNCMEC        =  315, // e->mu NC MEC
   // e to tau
   kmNuEToNuTauCCQE         =  401, // e->tau CC QE
   kmNuEToNuTauCCRESpPiP    =  402, // e->tau CC RES : v + p --> tau- + p + pi+
   kmNuEToNuTauCCRESnPi0    =  403, // e->tau CC RES : v + n --> tau- + p + pi0
   kmNuEToNuTauCCRESnPiP    =  404, // e->tau CC RES : v + n --> tau- + n + pi+
   kmNuEToNuTauCCDIS        =  405, // e->tau CC DIS
   kmNuEToNuTauCCCOH        =  406, // e->tau CC COH
   kmNuEToNuTauCCMEC        =  407, // e->tau CC MEC
   kmNuEToNuTauNCQE         =  408, // e->tau NC QE
   kmNuEToNuTauNCRESpPi0    =  409, // e->tau NC RES : v + p --> v + p + pi0
   kmNuEToNuTauNCRESpPiP    =  410, // e->tau NC RES : v + p --> v + n + pi+
   kmNuEToNuTauNCRESnPi0    =  411, // e->tau NC RES : v + n --> v + n + pi0
   kmNuEToNuTauNCRESnPiM    =  412, // e->tau NC RES : v + n --> v + p + pi-
   kmNuEToNuTauNCDIS        =  413, // e->tau NC DIS
   kmNuEToNuTauNCCOH        =  414, // e->tau NC COH
   kmNuEToNuTauNCMEC        =  415, // e->tau NC MEC
   // eb to mub
   kmNuEBarToNuMuBarCCQE      = -301, // mub->eb CC QE
   kmNuEBarToNuMuBarCCRESnPiM = -302, // mub->eb CC RES : vb + n --> mu+ + n + pi-
   kmNuEBarToNuMuBarCCRESpPi0 = -303, // mub->eb CC RES : vb + p --> mu+ + n + pi0
   kmNuEBarToNuMuBarCCRESpPiM = -304, // mub->eb CC RES : vb + p --> mu+ + p + pi-
   kmNuEBarToNuMuBarCCDIS     = -305, // mub->eb CC DIS
   kmNuEBarToNuMuBarCCCOH     = -306, // mub->eb CC COH
   kmNuEBarToNuMuBarCCMEC     = -307, // mub->eb CC MEC
   kmNuEBarToNuMuBarNCQE      = -308, // mub->eb NC QE
   kmNuEBarToNuMuBarNCRESpPi0 = -309, // mub->eb NC RES : vb + p --> vb + p + pi0
   kmNuEBarToNuMuBarNCRESpPiP = -310, // mub->eb NC RES : vb + p --> vb + n + pi+
   kmNuEBarToNuMuBarNCRESnPi0 = -311, // mub->eb NC RES : vb + n --> vb + n + pi0
   kmNuEBarToNuMuBarNCRESnPiM = -312, // mub->eb NC RES : vb + n --> vb + p + pi-
   kmNuEBarToNuMuBarNCDIS     = -313, // mub->eb NC DIS
   kmNuEBarToNuMuBarNCCOH     = -314, // mub->eb NC COH
   kmNuEBarToNuMuBarNCMEC     = -315, // mub->eb NC MEC
   // eb to taub
   kmNuEBarToNuTauBarCCQE      = -401, // mub->taub CC QE
   kmNuEBarToNuTauBarCCRESnPiM = -402, // mub->taub CC RES : vb + n --> tau+ + n + pi-
   kmNuEBarToNuTauBarCCRESpPi0 = -403, // mub->taub CC RES : vb + p --> tau+ + n + pi0
   kmNuEBarToNuTauBarCCRESpPiM = -404, // mub->taub CC RES : vb + p --> tau+ + p + pi-
   kmNuEBarToNuTauBarCCDIS     = -405, // mub->taub CC DIS
   kmNuEBarToNuTauBarCCCOH     = -406, // mub->taub CC COH
   kmNuEBarToNuTauBarCCMEC     = -407, // mub->taub CC MEC
   kmNuEBarToNuTauBarNCQE      = -408, // mub->taub NC QE
   kmNuEBarToNuTauBarNCRESpPi0 = -409, // mub->taub NC RES : vb + p --> vb + p + pi0
   kmNuEBarToNuTauBarNCRESpPiP = -410, // mub->taub NC RES : vb + p --> vb + n + pi+
   kmNuEBarToNuTauBarNCRESnPi0 = -411, // mub->taub NC RES : vb + n --> vb + n + pi0
   kmNuEBarToNuTauBarNCRESnPiM = -412, // mub->taub NC RES : vb + n --> vb + p + pi-
   kmNuEBarToNuTauBarNCDIS     = -413, // mub->taub NC DIS
   kmNuEBarToNuTauBarNCCOH     = -414, // mub->taub NC COH
   kmNuEBarToNuTauBarNCMEC     = -415, // mub->taub NC MEC
   // Miscellaneous modes
   kmCohElastic      = 931,       // any coh elastic
   kmElScat          = 932,       // any electron scattering
   kmIMDAnh          = 933,       // any inverse muon decay annihilation
   kmIBD             = 934,       // any inverse beta decay
   kmGlashow         = 935,       // any Glashow resonance
   kmAMNuGamma       = 936,       // any anomaly-mediated v+gamma
   kmDiffractive     = 937,       // any diffractive (COH on H)
   kmEM              = 938,       // any EM 
   kmWeakMix         = 939,       // any weak mix
   kmCosmic          = 940,       // cosmic
   kmDirt            = 941,       // dirt
} 
 ReacMode_t; 

// Same as above but not broken down by nu flavour
// careful: now there is a difference in the nu + e elastic scattering 
typedef enum EReacType
{
  kmtUndefined   = -999,
  kmtCCQE        =    1, // CC QE
  kmtCCRESvpPiP  =    2, // CC RES : v  + p --> l- + p + pi+
  kmtCCRESvnPi0  =    3, // CC RES : v  + n --> l- + p + pi0
  kmtCCRESvnPiP  =    4, // CC RES : v  + n --> l- + n + pi+
  kmtCCRESvbnPiM = -  2, // CC RES : vb + n --> l+ + n + pi-
  kmtCCRESvbpPi0 = -  3, // CC RES : vb + p --> l+ + n + pi0
  kmtCCRESvbpPiM = -  4, // CC RES : vb + p --> l+ + p + pi-
  kmtCCDIS       =    5, // CC DIS
  kmtCCCOH       =    6, // CC COH
  kmtCCMEC       =    7, // CC MEC
  kmtNCQE        =    8, // NC QE
  kmtNCRESvpPi0  =    9, // NC RES : v + p --> v + p + pi0
  kmtNCRESvpPiP  =   10, // NC RES : v + p --> v + n + pi+
  kmtNCRESvnPi0  =   11, // NC RES : v + n --> v + n + pi0
  kmtNCRESvnPiM  =   12, // NC RES : v + n --> v + p + pi-
  kmtNCRESvbpPi0 = -  9, // NC RES : vb + p --> vb + p + pi0
  kmtNCRESvbpPiP = - 10, // NC RES : vb + p --> vb + n + pi+
  kmtNCRESvbnPi0 = - 11, // NC RES : vb + n --> vb + n + pi0
  kmtNCRESvbnPiM = - 12, // NC RES : vb + n --> vb + p + pi-
  kmtNCDIS       =   13, // NC DIS
  kmtNCCOH       =   14, // NC COH
  kmtNCMEC       =   15, // NC MEC
  kmtCohElastic   =  931, // any coh elastic
  kmtElScat       =  932, // any electron scattering
  kmtIMDAnh      =  933, // any inverse muon decay annihilation
  kmtIBD         =  934, // any inverse beta decay
  kmtGlashow     =  935, // any Glashow resonance
  kmtAMNuGamma   =  936, // any anomaly-mediated v+gamma
  kmtDiffractive =  937, // any diffractive (COH on H)
  kmtEM          =  938, // any EM 
  kmtWeakMix     =  939, // any weak mix
  kmtCosmic      =  940, // cosmic
  kmtDirt        =  941, // dirt
}
ReacType_t;

// again, excluding undefined
static const int kNReacType = 33;
 
// Map the reaction modes to their type
static std::unordered_map<ReacMode_t, ReacType_t> kReacModeToType =
{
  { kmNuMuCCQE, kmtCCQE },
  { kmNuMuCCRESpPiP, kmtCCRESvpPiP },
  { kmNuMuCCRESnPi0, kmtCCRESvnPi0 },
  { kmNuMuCCRESnPiP, kmtCCRESvnPiP },
  { kmNuMuCCDIS, kmtCCDIS },
  { kmNuMuCCCOH, kmtCCCOH },
  { kmNuMuCCMEC, kmtCCMEC },
  { kmNuMuNCQE, kmtNCQE },
  { kmNuMuNCRESpPi0, kmtNCRESvpPi0 },
  { kmNuMuNCRESpPiP, kmtNCRESvpPiP },
  { kmNuMuNCRESnPi0, kmtNCRESvnPi0 },
  { kmNuMuNCRESnPiM, kmtNCRESvnPiM },
  { kmNuMuNCDIS, kmtNCDIS },
  { kmNuMuNCCOH, kmtNCCOH },
  { kmNuMuNCMEC, kmtNCMEC },
  { kmNuMuBarCCQE, kmtCCQE },
  { kmNuMuBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuMuBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuMuBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuMuBarCCDIS, kmtCCDIS },
  { kmNuMuBarCCCOH, kmtCCCOH },
  { kmNuMuBarCCMEC, kmtCCMEC },
  { kmNuMuBarNCQE, kmtNCQE },
  { kmNuMuBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuMuBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuMuBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuMuBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuMuBarNCDIS, kmtNCDIS },
  { kmNuMuBarNCCOH, kmtNCCOH },
  { kmNuMuBarNCMEC, kmtNCMEC },
  { kmNuECCQE, kmtCCQE },
  { kmNuECCRESpPiP, kmtCCRESvpPiP },
  { kmNuECCRESnPi0, kmtCCRESvnPi0 },
  { kmNuECCRESnPiP, kmtCCRESvnPiP },
  { kmNuECCDIS, kmtCCDIS },
  { kmNuECCCOH, kmtCCCOH },
  { kmNuECCMEC, kmtCCMEC },
  { kmNuENCQE, kmtNCQE },
  { kmNuENCRESpPi0, kmtNCRESvpPi0 },
  { kmNuENCRESpPiP, kmtNCRESvpPiP },
  { kmNuENCRESnPi0, kmtNCRESvnPi0 },
  { kmNuENCRESnPiM, kmtNCRESvnPiM },
  { kmNuENCDIS, kmtNCDIS },
  { kmNuENCCOH, kmtNCCOH },
  { kmNuENCMEC, kmtNCMEC },
  { kmNuEBarCCQE, kmtCCQE },
  { kmNuEBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuEBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuEBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuEBarCCDIS, kmtCCDIS },
  { kmNuEBarCCCOH, kmtCCCOH },
  { kmNuEBarCCMEC, kmtCCMEC },
  { kmNuEBarNCQE, kmtNCQE },
  { kmNuEBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuEBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuEBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuEBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuEBarNCDIS, kmtNCDIS },
  { kmNuEBarNCCOH, kmtNCCOH },
  { kmNuEBarNCMEC, kmtNCMEC },
  { kmNuMuToNuECCQE, kmtCCQE },
  { kmNuMuToNuECCRESpPiP, kmtCCRESvpPiP },
  { kmNuMuToNuECCRESnPi0, kmtCCRESvnPi0 },
  { kmNuMuToNuECCRESnPiP, kmtCCRESvnPiP },
  { kmNuMuToNuECCDIS, kmtCCDIS },
  { kmNuMuToNuECCCOH, kmtCCCOH },
  { kmNuMuToNuECCMEC, kmtCCMEC },
  { kmNuMuToNuENCQE, kmtNCQE },
  { kmNuMuToNuENCRESpPi0, kmtNCRESvpPi0 },
  { kmNuMuToNuENCRESpPiP, kmtNCRESvpPiP },
  { kmNuMuToNuENCRESnPi0, kmtNCRESvnPi0 },
  { kmNuMuToNuENCRESnPiM, kmtNCRESvnPiM },
  { kmNuMuToNuENCDIS, kmtNCDIS },
  { kmNuMuToNuENCCOH, kmtNCCOH },
  { kmNuMuToNuENCMEC, kmtNCMEC },
  { kmNuMuToNuTauCCQE, kmtCCQE },
  { kmNuMuToNuTauCCRESpPiP, kmtCCRESvpPiP },
  { kmNuMuToNuTauCCRESnPi0, kmtCCRESvnPi0 },
  { kmNuMuToNuTauCCRESnPiP, kmtCCRESvnPiP },
  { kmNuMuToNuTauCCDIS, kmtCCDIS },
  { kmNuMuToNuTauCCCOH, kmtCCCOH },
  { kmNuMuToNuTauCCMEC, kmtCCMEC },
  { kmNuMuToNuTauNCQE, kmtNCQE },
  { kmNuMuToNuTauNCRESpPi0, kmtNCRESvpPi0 },
  { kmNuMuToNuTauNCRESpPiP, kmtNCRESvpPiP },
  { kmNuMuToNuTauNCRESnPi0, kmtNCRESvnPi0 },
  { kmNuMuToNuTauNCRESnPiM, kmtNCRESvnPiM },
  { kmNuMuToNuTauNCDIS, kmtNCDIS },
  { kmNuMuToNuTauNCCOH, kmtNCCOH },
  { kmNuMuToNuTauNCMEC, kmtNCMEC },
  { kmNuMuBarToNuEBarCCQE, kmtCCQE },
  { kmNuMuBarToNuEBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuMuBarToNuEBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuMuBarToNuEBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuMuBarToNuEBarCCDIS, kmtCCDIS },
  { kmNuMuBarToNuEBarCCCOH, kmtCCCOH },
  { kmNuMuBarToNuEBarCCMEC, kmtCCMEC },
  { kmNuMuBarToNuEBarNCQE, kmtNCQE },
  { kmNuMuBarToNuEBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuMuBarToNuEBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuMuBarToNuEBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuMuBarToNuEBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuMuBarToNuEBarNCDIS, kmtNCDIS },
  { kmNuMuBarToNuEBarNCCOH, kmtNCCOH },
  { kmNuMuBarToNuEBarNCMEC, kmtNCMEC },
  { kmNuMuBarToNuTauBarCCQE, kmtCCQE },
  { kmNuMuBarToNuTauBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuMuBarToNuTauBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuMuBarToNuTauBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuMuBarToNuTauBarCCDIS, kmtCCDIS },
  { kmNuMuBarToNuTauBarCCCOH, kmtCCCOH },
  { kmNuMuBarToNuTauBarCCMEC, kmtCCMEC },
  { kmNuMuBarToNuTauBarNCQE, kmtNCQE },
  { kmNuMuBarToNuTauBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuMuBarToNuTauBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuMuBarToNuTauBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuMuBarToNuTauBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuMuBarToNuTauBarNCDIS, kmtNCDIS },
  { kmNuMuBarToNuTauBarNCCOH, kmtNCCOH },
  { kmNuMuBarToNuTauBarNCMEC, kmtNCMEC },
  { kmNuEToNuMuCCQE, kmtCCQE },
  { kmNuEToNuMuCCRESpPiP, kmtCCRESvpPiP },
  { kmNuEToNuMuCCRESnPi0, kmtCCRESvnPi0 },
  { kmNuEToNuMuCCRESnPiP, kmtCCRESvnPiP },
  { kmNuEToNuMuCCDIS, kmtCCDIS },
  { kmNuEToNuMuCCCOH, kmtCCCOH },
  { kmNuEToNuMuCCMEC, kmtCCMEC },
  { kmNuEToNuMuNCQE, kmtNCQE },
  { kmNuEToNuMuNCRESpPi0, kmtNCRESvpPi0 },
  { kmNuEToNuMuNCRESpPiP, kmtNCRESvpPiP },
  { kmNuEToNuMuNCRESnPi0, kmtNCRESvnPi0 },
  { kmNuEToNuMuNCRESnPiM, kmtNCRESvnPiM },
  { kmNuEToNuMuNCDIS, kmtNCDIS },
  { kmNuEToNuMuNCCOH, kmtNCCOH },
  { kmNuEToNuMuNCMEC, kmtNCMEC },
  { kmNuEToNuTauCCQE, kmtCCQE },
  { kmNuEToNuTauCCRESpPiP, kmtCCRESvpPiP },
  { kmNuEToNuTauCCRESnPi0, kmtCCRESvnPi0 },
  { kmNuEToNuTauCCRESnPiP, kmtCCRESvnPiP },
  { kmNuEToNuTauCCDIS, kmtCCDIS },
  { kmNuEToNuTauCCCOH, kmtCCCOH },
  { kmNuEToNuTauCCMEC, kmtCCMEC },
  { kmNuEToNuTauNCQE, kmtNCQE },
  { kmNuEToNuTauNCRESpPi0, kmtNCRESvpPi0 },
  { kmNuEToNuTauNCRESpPiP, kmtNCRESvpPiP },
  { kmNuEToNuTauNCRESnPi0, kmtNCRESvnPi0 },
  { kmNuEToNuTauNCRESnPiM, kmtNCRESvnPiM },
  { kmNuEToNuTauNCDIS, kmtNCDIS },
  { kmNuEToNuTauNCCOH, kmtNCCOH },
  { kmNuEToNuTauNCMEC, kmtNCMEC },
  { kmNuEBarToNuMuBarCCQE, kmtCCQE },
  { kmNuEBarToNuMuBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuEBarToNuMuBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuEBarToNuMuBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuEBarToNuMuBarCCDIS, kmtCCDIS },
  { kmNuEBarToNuMuBarCCCOH, kmtCCCOH },
  { kmNuEBarToNuMuBarCCMEC, kmtCCMEC },
  { kmNuEBarToNuMuBarNCQE, kmtNCQE },
  { kmNuEBarToNuMuBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuEBarToNuMuBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuEBarToNuMuBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuEBarToNuMuBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuEBarToNuMuBarNCDIS, kmtNCDIS },
  { kmNuEBarToNuMuBarNCCOH, kmtNCCOH },
  { kmNuEBarToNuMuBarNCMEC, kmtNCMEC },
  { kmNuEBarToNuTauBarCCQE, kmtCCQE },
  { kmNuEBarToNuTauBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuEBarToNuTauBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuEBarToNuTauBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuEBarToNuTauBarCCDIS, kmtCCDIS },
  { kmNuEBarToNuTauBarCCCOH, kmtCCCOH },
  { kmNuEBarToNuTauBarCCMEC, kmtCCMEC },
  { kmNuEBarToNuTauBarNCQE, kmtNCQE },
  { kmNuEBarToNuTauBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuEBarToNuTauBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuEBarToNuTauBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuEBarToNuTauBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuEBarToNuTauBarNCDIS, kmtNCDIS },
  { kmNuEBarToNuTauBarNCCOH, kmtNCCOH },
  { kmNuEBarToNuTauBarNCMEC, kmtNCMEC },
  { kmCohElastic, kmtCohElastic },
  { kmElScat, kmtElScat },
  { kmIMDAnh, kmtIMDAnh },
  { kmIBD, kmtIBD },
  { kmGlashow, kmtGlashow },
  { kmAMNuGamma, kmtAMNuGamma },
  { kmDiffractive, kmtDiffractive },
  { kmEM, kmtEM },
  { kmWeakMix, kmtWeakMix },
  { kmCosmic, kmtCosmic },
  { kmDirt, kmtDirt }
};

// Map the reaction modes to their final neutrino flavour 
static std::unordered_map<ReacMode_t, NuFlavour_t> kReacModeToFinalFlavour =
{
  { kmNuMuCCQE, kNuMu },
  { kmNuMuCCRESpPiP, kNuMu },
  { kmNuMuCCRESnPi0, kNuMu },
  { kmNuMuCCRESnPiP, kNuMu },
  { kmNuMuCCDIS, kNuMu },
  { kmNuMuCCCOH, kNuMu },
  { kmNuMuCCMEC, kNuMu },
  { kmNuMuNCQE, kNuMu },
  { kmNuMuNCRESpPi0, kNuMu },
  { kmNuMuNCRESpPiP, kNuMu },
  { kmNuMuNCRESnPi0, kNuMu },
  { kmNuMuNCRESnPiM, kNuMu },
  { kmNuMuNCDIS, kNuMu },
  { kmNuMuNCCOH, kNuMu },
  { kmNuMuNCMEC, kNuMu },
  { kmNuMuBarCCQE, kNuMuBar },
  { kmNuMuBarCCRESnPiM, kNuMuBar },
  { kmNuMuBarCCRESpPi0, kNuMuBar },
  { kmNuMuBarCCRESpPiM, kNuMuBar },
  { kmNuMuBarCCDIS, kNuMuBar },
  { kmNuMuBarCCCOH, kNuMuBar },
  { kmNuMuBarCCMEC, kNuMuBar },
  { kmNuMuBarNCQE, kNuMuBar },
  { kmNuMuBarNCRESpPi0, kNuMuBar },
  { kmNuMuBarNCRESpPiP, kNuMuBar },
  { kmNuMuBarNCRESnPi0, kNuMuBar },
  { kmNuMuBarNCRESnPiM, kNuMuBar },
  { kmNuMuBarNCDIS, kNuMuBar },
  { kmNuMuBarNCCOH, kNuMuBar },
  { kmNuMuBarNCMEC, kNuMuBar },
  { kmNuECCQE, kNuE },
  { kmNuECCRESpPiP, kNuE },
  { kmNuECCRESnPi0, kNuE },
  { kmNuECCRESnPiP, kNuE },
  { kmNuECCDIS, kNuE },
  { kmNuECCCOH, kNuE },
  { kmNuECCMEC, kNuE },
  { kmNuENCQE, kNuE },
  { kmNuENCRESpPi0, kNuE },
  { kmNuENCRESpPiP, kNuE },
  { kmNuENCRESnPi0, kNuE },
  { kmNuENCRESnPiM, kNuE },
  { kmNuENCDIS, kNuE },
  { kmNuENCCOH, kNuE },
  { kmNuENCMEC, kNuE },
  { kmNuEBarCCQE, kNuEBar },
  { kmNuEBarCCRESnPiM, kNuEBar },
  { kmNuEBarCCRESpPi0, kNuEBar },
  { kmNuEBarCCRESpPiM, kNuEBar },
  { kmNuEBarCCDIS, kNuEBar },
  { kmNuEBarCCCOH, kNuEBar },
  { kmNuEBarCCMEC, kNuEBar },
  { kmNuEBarNCQE, kNuEBar },
  { kmNuEBarNCRESpPi0, kNuEBar },
  { kmNuEBarNCRESpPiP, kNuEBar },
  { kmNuEBarNCRESnPi0, kNuEBar },
  { kmNuEBarNCRESnPiM, kNuEBar },
  { kmNuEBarNCDIS, kNuEBar },
  { kmNuEBarNCCOH, kNuEBar },
  { kmNuEBarNCMEC, kNuEBar },
  { kmNuMuToNuECCQE, kNuE },
  { kmNuMuToNuECCRESpPiP, kNuE },
  { kmNuMuToNuECCRESnPi0, kNuE },
  { kmNuMuToNuECCRESnPiP, kNuE },
  { kmNuMuToNuECCDIS, kNuE },
  { kmNuMuToNuECCCOH, kNuE },
  { kmNuMuToNuECCMEC, kNuE },
  { kmNuMuToNuENCQE, kNuE },
  { kmNuMuToNuENCRESpPi0, kNuE },
  { kmNuMuToNuENCRESpPiP, kNuE },
  { kmNuMuToNuENCRESnPi0, kNuE },
  { kmNuMuToNuENCRESnPiM, kNuE },
  { kmNuMuToNuENCDIS, kNuE },
  { kmNuMuToNuENCCOH, kNuE },
  { kmNuMuToNuENCMEC, kNuE },
  { kmNuMuToNuTauCCQE, kNuTau },
  { kmNuMuToNuTauCCRESpPiP, kNuTau },
  { kmNuMuToNuTauCCRESnPi0, kNuTau },
  { kmNuMuToNuTauCCRESnPiP, kNuTau },
  { kmNuMuToNuTauCCDIS, kNuTau },
  { kmNuMuToNuTauCCCOH, kNuTau },
  { kmNuMuToNuTauCCMEC, kNuTau },
  { kmNuMuToNuTauNCQE, kNuTau },
  { kmNuMuToNuTauNCRESpPi0, kNuTau },
  { kmNuMuToNuTauNCRESpPiP, kNuTau },
  { kmNuMuToNuTauNCRESnPi0, kNuTau },
  { kmNuMuToNuTauNCRESnPiM, kNuTau },
  { kmNuMuToNuTauNCDIS, kNuTau },
  { kmNuMuToNuTauNCCOH, kNuTau },
  { kmNuMuToNuTauNCMEC, kNuTau },
  { kmNuMuBarToNuEBarCCQE, kNuEBar },
  { kmNuMuBarToNuEBarCCRESnPiM, kNuEBar },
  { kmNuMuBarToNuEBarCCRESpPi0, kNuEBar },
  { kmNuMuBarToNuEBarCCRESpPiM, kNuEBar },
  { kmNuMuBarToNuEBarCCDIS, kNuEBar },
  { kmNuMuBarToNuEBarCCCOH, kNuEBar },
  { kmNuMuBarToNuEBarCCMEC, kNuEBar },
  { kmNuMuBarToNuEBarNCQE, kNuEBar },
  { kmNuMuBarToNuEBarNCRESpPi0, kNuEBar },
  { kmNuMuBarToNuEBarNCRESpPiP, kNuEBar },
  { kmNuMuBarToNuEBarNCRESnPi0, kNuEBar },
  { kmNuMuBarToNuEBarNCRESnPiM, kNuEBar },
  { kmNuMuBarToNuEBarNCDIS, kNuEBar },
  { kmNuMuBarToNuEBarNCCOH, kNuEBar },
  { kmNuMuBarToNuEBarNCMEC, kNuEBar },
  { kmNuMuBarToNuTauBarCCQE, kNuTauBar },
  { kmNuMuBarToNuTauBarCCRESnPiM, kNuTauBar },
  { kmNuMuBarToNuTauBarCCRESpPi0, kNuTauBar },
  { kmNuMuBarToNuTauBarCCRESpPiM, kNuTauBar },
  { kmNuMuBarToNuTauBarCCDIS, kNuTauBar },
  { kmNuMuBarToNuTauBarCCCOH, kNuTauBar },
  { kmNuMuBarToNuTauBarCCMEC, kNuTauBar },
  { kmNuMuBarToNuTauBarNCQE, kNuTauBar },
  { kmNuMuBarToNuTauBarNCRESpPi0, kNuTauBar },
  { kmNuMuBarToNuTauBarNCRESpPiP, kNuTauBar },
  { kmNuMuBarToNuTauBarNCRESnPi0, kNuTauBar },
  { kmNuMuBarToNuTauBarNCRESnPiM, kNuTauBar },
  { kmNuMuBarToNuTauBarNCDIS, kNuTauBar },
  { kmNuMuBarToNuTauBarNCCOH, kNuTauBar },
  { kmNuMuBarToNuTauBarNCMEC, kNuTauBar },
  { kmNuEToNuMuCCQE, kNuMu },
  { kmNuEToNuMuCCRESpPiP, kNuMu },
  { kmNuEToNuMuCCRESnPi0, kNuMu },
  { kmNuEToNuMuCCRESnPiP, kNuMu },
  { kmNuEToNuMuCCDIS, kNuMu },
  { kmNuEToNuMuCCCOH, kNuMu },
  { kmNuEToNuMuCCMEC, kNuMu },
  { kmNuEToNuMuNCQE, kNuMu },
  { kmNuEToNuMuNCRESpPi0, kNuMu },
  { kmNuEToNuMuNCRESpPiP, kNuMu },
  { kmNuEToNuMuNCRESnPi0, kNuMu },
  { kmNuEToNuMuNCRESnPiM, kNuMu },
  { kmNuEToNuMuNCDIS, kNuMu },
  { kmNuEToNuMuNCCOH, kNuMu },
  { kmNuEToNuMuNCMEC, kNuMu },
  { kmNuEToNuTauCCQE, kNuTau },
  { kmNuEToNuTauCCRESpPiP, kNuTau },
  { kmNuEToNuTauCCRESnPi0, kNuTau },
  { kmNuEToNuTauCCRESnPiP, kNuTau },
  { kmNuEToNuTauCCDIS, kNuTau },
  { kmNuEToNuTauCCCOH, kNuTau },
  { kmNuEToNuTauCCMEC, kNuTau },
  { kmNuEToNuTauNCQE, kNuTau },
  { kmNuEToNuTauNCRESpPi0, kNuTau },
  { kmNuEToNuTauNCRESpPiP, kNuTau },
  { kmNuEToNuTauNCRESnPi0, kNuTau },
  { kmNuEToNuTauNCRESnPiM, kNuTau },
  { kmNuEToNuTauNCDIS, kNuTau },
  { kmNuEToNuTauNCCOH, kNuTau },
  { kmNuEToNuTauNCMEC, kNuTau },
  { kmNuEBarToNuMuBarCCQE, kNuMuBar },
  { kmNuEBarToNuMuBarCCRESnPiM, kNuMuBar },
  { kmNuEBarToNuMuBarCCRESpPi0, kNuMuBar },
  { kmNuEBarToNuMuBarCCRESpPiM, kNuMuBar },
  { kmNuEBarToNuMuBarCCDIS, kNuMuBar },
  { kmNuEBarToNuMuBarCCCOH, kNuMuBar },
  { kmNuEBarToNuMuBarCCMEC, kNuMuBar },
  { kmNuEBarToNuMuBarNCQE, kNuMuBar },
  { kmNuEBarToNuMuBarNCRESpPi0, kNuMuBar },
  { kmNuEBarToNuMuBarNCRESpPiP, kNuMuBar },
  { kmNuEBarToNuMuBarNCRESnPi0, kNuMuBar },
  { kmNuEBarToNuMuBarNCRESnPiM, kNuMuBar },
  { kmNuEBarToNuMuBarNCDIS, kNuMuBar },
  { kmNuEBarToNuMuBarNCCOH, kNuMuBar },
  { kmNuEBarToNuMuBarNCMEC, kNuMuBar },
  { kmNuEBarToNuTauBarCCQE, kNuTauBar },
  { kmNuEBarToNuTauBarCCRESnPiM, kNuTauBar },
  { kmNuEBarToNuTauBarCCRESpPi0, kNuTauBar },
  { kmNuEBarToNuTauBarCCRESpPiM, kNuTauBar },
  { kmNuEBarToNuTauBarCCDIS, kNuTauBar },
  { kmNuEBarToNuTauBarCCCOH, kNuTauBar },
  { kmNuEBarToNuTauBarCCMEC, kNuTauBar },
  { kmNuEBarToNuTauBarNCQE, kNuTauBar },
  { kmNuEBarToNuTauBarNCRESpPi0, kNuTauBar },
  { kmNuEBarToNuTauBarNCRESpPiP, kNuTauBar },
  { kmNuEBarToNuTauBarNCRESnPi0, kNuTauBar },
  { kmNuEBarToNuTauBarNCRESnPiM, kNuTauBar },
  { kmNuEBarToNuTauBarNCDIS, kNuTauBar },
  { kmNuEBarToNuTauBarNCCOH, kNuTauBar },
  { kmNuEBarToNuTauBarNCMEC, kNuTauBar },
  { kmCohElastic, kNuFlavourUndefined },
  { kmElScat, kNuFlavourUndefined },
  { kmIMDAnh, kNuFlavourUndefined },
  { kmIBD, kNuFlavourUndefined },
  { kmGlashow, kNuFlavourUndefined },
  { kmAMNuGamma, kNuFlavourUndefined },
  { kmDiffractive, kNuFlavourUndefined },
  { kmEM, kNuFlavourUndefined },
  { kmWeakMix, kNuFlavourUndefined },
  { kmCosmic, kCosmic },
  { kmDirt, kDirt }
};

static std::unordered_map<ReacMode_t, NuFlavour_t> kReacModeToInitFlavour =
{
  { kmNuMuCCQE, kNuMu },
  { kmNuMuCCRESpPiP, kNuMu },
  { kmNuMuCCRESnPi0, kNuMu },
  { kmNuMuCCRESnPiP, kNuMu },
  { kmNuMuCCDIS, kNuMu },
  { kmNuMuCCCOH, kNuMu },
  { kmNuMuCCMEC, kNuMu },
  { kmNuMuNCQE, kNuMu },
  { kmNuMuNCRESpPi0, kNuMu },
  { kmNuMuNCRESpPiP, kNuMu },
  { kmNuMuNCRESnPi0, kNuMu },
  { kmNuMuNCRESnPiM, kNuMu },
  { kmNuMuNCDIS, kNuMu },
  { kmNuMuNCCOH, kNuMu },
  { kmNuMuNCMEC, kNuMu },
  { kmNuMuBarCCQE, kNuMuBar },
  { kmNuMuBarCCRESnPiM, kNuMuBar },
  { kmNuMuBarCCRESpPi0, kNuMuBar },
  { kmNuMuBarCCRESpPiM, kNuMuBar },
  { kmNuMuBarCCDIS, kNuMuBar },
  { kmNuMuBarCCCOH, kNuMuBar },
  { kmNuMuBarCCMEC, kNuMuBar },
  { kmNuMuBarNCQE, kNuMuBar },
  { kmNuMuBarNCRESpPi0, kNuMuBar },
  { kmNuMuBarNCRESpPiP, kNuMuBar },
  { kmNuMuBarNCRESnPi0, kNuMuBar },
  { kmNuMuBarNCRESnPiM, kNuMuBar },
  { kmNuMuBarNCDIS, kNuMuBar },
  { kmNuMuBarNCCOH, kNuMuBar },
  { kmNuMuBarNCMEC, kNuMuBar },
  { kmNuECCQE, kNuE },
  { kmNuECCRESpPiP, kNuE },
  { kmNuECCRESnPi0, kNuE },
  { kmNuECCRESnPiP, kNuE },
  { kmNuECCDIS, kNuE },
  { kmNuECCCOH, kNuE },
  { kmNuECCMEC, kNuE },
  { kmNuENCQE, kNuE },
  { kmNuENCRESpPi0, kNuE },
  { kmNuENCRESpPiP, kNuE },
  { kmNuENCRESnPi0, kNuE },
  { kmNuENCRESnPiM, kNuE },
  { kmNuENCDIS, kNuE },
  { kmNuENCCOH, kNuE },
  { kmNuENCMEC, kNuE },
  { kmNuEBarCCQE, kNuEBar },
  { kmNuEBarCCRESnPiM, kNuEBar },
  { kmNuEBarCCRESpPi0, kNuEBar },
  { kmNuEBarCCRESpPiM, kNuEBar },
  { kmNuEBarCCDIS, kNuEBar },
  { kmNuEBarCCCOH, kNuEBar },
  { kmNuEBarCCMEC, kNuEBar },
  { kmNuEBarNCQE, kNuEBar },
  { kmNuEBarNCRESpPi0, kNuEBar },
  { kmNuEBarNCRESpPiP, kNuEBar },
  { kmNuEBarNCRESnPi0, kNuEBar },
  { kmNuEBarNCRESnPiM, kNuEBar },
  { kmNuEBarNCDIS, kNuEBar },
  { kmNuEBarNCCOH, kNuEBar },
  { kmNuEBarNCMEC, kNuEBar },
  { kmNuMuToNuECCQE, kNuMu },
  { kmNuMuToNuECCRESpPiP, kNuMu },
  { kmNuMuToNuECCRESnPi0, kNuMu },
  { kmNuMuToNuECCRESnPiP, kNuMu },
  { kmNuMuToNuECCDIS, kNuMu },
  { kmNuMuToNuECCCOH, kNuMu },
  { kmNuMuToNuECCMEC, kNuMu },
  { kmNuMuToNuENCQE, kNuMu },
  { kmNuMuToNuENCRESpPi0, kNuMu },
  { kmNuMuToNuENCRESpPiP, kNuMu },
  { kmNuMuToNuENCRESnPi0, kNuMu },
  { kmNuMuToNuENCRESnPiM, kNuMu },
  { kmNuMuToNuENCDIS, kNuMu },
  { kmNuMuToNuENCCOH, kNuMu },
  { kmNuMuToNuENCMEC, kNuMu },
  { kmNuMuToNuTauCCQE, kNuMu },
  { kmNuMuToNuTauCCRESpPiP, kNuMu },
  { kmNuMuToNuTauCCRESnPi0, kNuMu },
  { kmNuMuToNuTauCCRESnPiP, kNuMu },
  { kmNuMuToNuTauCCDIS, kNuMu },
  { kmNuMuToNuTauCCCOH, kNuMu },
  { kmNuMuToNuTauCCMEC, kNuMu },
  { kmNuMuToNuTauNCQE, kNuMu },
  { kmNuMuToNuTauNCRESpPi0, kNuMu },
  { kmNuMuToNuTauNCRESpPiP, kNuMu },
  { kmNuMuToNuTauNCRESnPi0, kNuMu },
  { kmNuMuToNuTauNCRESnPiM, kNuMu },
  { kmNuMuToNuTauNCDIS, kNuMu },
  { kmNuMuToNuTauNCCOH, kNuMu },
  { kmNuMuToNuTauNCMEC, kNuMu },
  { kmNuMuBarToNuEBarCCQE, kNuMuBar },
  { kmNuMuBarToNuEBarCCRESnPiM, kNuMuBar },
  { kmNuMuBarToNuEBarCCRESpPi0, kNuMuBar },
  { kmNuMuBarToNuEBarCCRESpPiM, kNuMuBar },
  { kmNuMuBarToNuEBarCCDIS, kNuMuBar },
  { kmNuMuBarToNuEBarCCCOH, kNuMuBar },
  { kmNuMuBarToNuEBarCCMEC, kNuMuBar },
  { kmNuMuBarToNuEBarNCQE, kNuMuBar },
  { kmNuMuBarToNuEBarNCRESpPi0, kNuMuBar },
  { kmNuMuBarToNuEBarNCRESpPiP, kNuMuBar },
  { kmNuMuBarToNuEBarNCRESnPi0, kNuMuBar },
  { kmNuMuBarToNuEBarNCRESnPiM, kNuMuBar },
  { kmNuMuBarToNuEBarNCDIS, kNuMuBar },
  { kmNuMuBarToNuEBarNCCOH, kNuMuBar },
  { kmNuMuBarToNuEBarNCMEC, kNuMuBar },
  { kmNuMuBarToNuTauBarCCQE, kNuMuBar },
  { kmNuMuBarToNuTauBarCCRESnPiM, kNuMuBar },
  { kmNuMuBarToNuTauBarCCRESpPi0, kNuMuBar },
  { kmNuMuBarToNuTauBarCCRESpPiM, kNuMuBar },
  { kmNuMuBarToNuTauBarCCDIS, kNuMuBar },
  { kmNuMuBarToNuTauBarCCCOH, kNuMuBar },
  { kmNuMuBarToNuTauBarCCMEC, kNuMuBar },
  { kmNuMuBarToNuTauBarNCQE, kNuMuBar },
  { kmNuMuBarToNuTauBarNCRESpPi0, kNuMuBar },
  { kmNuMuBarToNuTauBarNCRESpPiP, kNuMuBar },
  { kmNuMuBarToNuTauBarNCRESnPi0, kNuMuBar },
  { kmNuMuBarToNuTauBarNCRESnPiM, kNuMuBar },
  { kmNuMuBarToNuTauBarNCDIS, kNuMuBar },
  { kmNuMuBarToNuTauBarNCCOH, kNuMuBar },
  { kmNuMuBarToNuTauBarNCMEC, kNuMuBar },
  { kmNuEToNuMuCCQE, kNuE },
  { kmNuEToNuMuCCRESpPiP, kNuE },
  { kmNuEToNuMuCCRESnPi0, kNuE },
  { kmNuEToNuMuCCRESnPiP, kNuE },
  { kmNuEToNuMuCCDIS, kNuE },
  { kmNuEToNuMuCCCOH, kNuE },
  { kmNuEToNuMuCCMEC, kNuE },
  { kmNuEToNuMuNCQE, kNuE },
  { kmNuEToNuMuNCRESpPi0, kNuE },
  { kmNuEToNuMuNCRESpPiP, kNuE },
  { kmNuEToNuMuNCRESnPi0, kNuE },
  { kmNuEToNuMuNCRESnPiM, kNuE },
  { kmNuEToNuMuNCDIS, kNuE },
  { kmNuEToNuMuNCCOH, kNuE },
  { kmNuEToNuMuNCMEC, kNuE },
  { kmNuEToNuTauCCQE, kNuE },
  { kmNuEToNuTauCCRESpPiP, kNuE },
  { kmNuEToNuTauCCRESnPi0, kNuE },
  { kmNuEToNuTauCCRESnPiP, kNuE },
  { kmNuEToNuTauCCDIS, kNuE },
  { kmNuEToNuTauCCCOH, kNuE },
  { kmNuEToNuTauCCMEC, kNuE },
  { kmNuEToNuTauNCQE, kNuE },
  { kmNuEToNuTauNCRESpPi0, kNuE },
  { kmNuEToNuTauNCRESpPiP, kNuE },
  { kmNuEToNuTauNCRESnPi0, kNuE },
  { kmNuEToNuTauNCRESnPiM, kNuE },
  { kmNuEToNuTauNCDIS, kNuE },
  { kmNuEToNuTauNCCOH, kNuE },
  { kmNuEToNuTauNCMEC, kNuE },
  { kmNuEBarToNuMuBarCCQE, kNuEBar },
  { kmNuEBarToNuMuBarCCRESnPiM, kNuEBar },
  { kmNuEBarToNuMuBarCCRESpPi0, kNuEBar },
  { kmNuEBarToNuMuBarCCRESpPiM, kNuEBar },
  { kmNuEBarToNuMuBarCCDIS, kNuEBar },
  { kmNuEBarToNuMuBarCCCOH, kNuEBar },
  { kmNuEBarToNuMuBarCCMEC, kNuEBar },
  { kmNuEBarToNuMuBarNCQE, kNuEBar },
  { kmNuEBarToNuMuBarNCRESpPi0, kNuEBar },
  { kmNuEBarToNuMuBarNCRESpPiP, kNuEBar },
  { kmNuEBarToNuMuBarNCRESnPi0, kNuEBar },
  { kmNuEBarToNuMuBarNCRESnPiM, kNuEBar },
  { kmNuEBarToNuMuBarNCDIS, kNuEBar },
  { kmNuEBarToNuMuBarNCCOH, kNuEBar },
  { kmNuEBarToNuMuBarNCMEC, kNuEBar },
  { kmNuEBarToNuTauBarCCQE, kNuEBar },
  { kmNuEBarToNuTauBarCCRESnPiM, kNuEBar },
  { kmNuEBarToNuTauBarCCRESpPi0, kNuEBar },
  { kmNuEBarToNuTauBarCCRESpPiM, kNuEBar },
  { kmNuEBarToNuTauBarCCDIS, kNuEBar },
  { kmNuEBarToNuTauBarCCCOH, kNuEBar },
  { kmNuEBarToNuTauBarCCMEC, kNuEBar },
  { kmNuEBarToNuTauBarNCQE, kNuEBar },
  { kmNuEBarToNuTauBarNCRESpPi0, kNuEBar },
  { kmNuEBarToNuTauBarNCRESpPiP, kNuEBar },
  { kmNuEBarToNuTauBarNCRESnPi0, kNuEBar },
  { kmNuEBarToNuTauBarNCRESnPiM, kNuEBar },
  { kmNuEBarToNuTauBarNCDIS, kNuEBar },
  { kmNuEBarToNuTauBarNCCOH, kNuEBar },
  { kmNuEBarToNuTauBarNCMEC, kNuEBar },
  { kmCohElastic, kNuFlavourUndefined },
  { kmElScat, kNuFlavourUndefined },
  { kmIMDAnh, kNuFlavourUndefined },
  { kmIBD, kNuFlavourUndefined },
  { kmGlashow, kNuFlavourUndefined },
  { kmAMNuGamma, kNuFlavourUndefined },
  { kmDiffractive, kNuFlavourUndefined },
  { kmEM, kNuFlavourUndefined },
  { kmWeakMix, kNuFlavourUndefined },
  { kmCosmic, kCosmic },
  { kmDirt, kDirt }
};


} // conv


//
// Associated utilities
//
namespace util {

  std::string        AsString             (conv::ReacMode_t m);
  std::string        AsStringTeX          (conv::ReacMode_t m, bool root = false);
  std::string        AsString             (conv::ReacType_t t);
  conv::ReacMode_t   ReacModeFromInt      (int m);
  conv::ReacMode_t   ReacModeFromString   (std::string s);
  conv::NuFlavour_t  ReacModeToInitFlavor (conv::ReacMode_t m);
  conv::NuFlavour_t  ReacModeToFinalFlavor(conv::ReacMode_t m);
  conv::ReacType_t   ReacModeToType       (conv::ReacMode_t m);
  conv::ReacMode_t   ReacTypeToMode       (conv::ReacType_t t, conv::NuFlavour_t nu_init, conv::NuFlavour_t nu_final);
  
  bool               IsCC               (conv::ReacMode_t m);
  bool               IsNuMu             (conv::ReacMode_t m);
  bool               IsNuMuBar          (conv::ReacMode_t m);
  bool               IsNuE              (conv::ReacMode_t m);
  bool               IsNuEBar           (conv::ReacMode_t m);
  bool               IsNuMuToNuE        (conv::ReacMode_t m);
  bool               IsNuMuToNuTau      (conv::ReacMode_t m);
  bool               IsNuEToNuMu        (conv::ReacMode_t m);
  bool               IsNuEToNuTau       (conv::ReacMode_t m);
  bool               IsNuEToNuMu        (conv::ReacMode_t m);
  bool               IsNuMuBarToNuEBar  (conv::ReacMode_t m);
  bool               IsNuMuBarToNuTauBar(conv::ReacMode_t m);
  bool               IsNuEBarToNuMuBar  (conv::ReacMode_t m);
  bool               IsNuEBarToNuTauBar (conv::ReacMode_t m);
  bool               IsCosmic           (conv::ReacMode_t m);
  bool               IsDirt             (conv::ReacMode_t m);
  bool               IsNeutrinoAny      (conv::ReacMode_t m);

  conv::ReacMode_t   NuEModeToOscNuEMode(conv::ReacMode_t m);

  // Aliases for above
  std::string      ModeAsString        (conv::ReacMode_t m);
  std::string      ModeAsStringTeX     (conv::ReacMode_t m, bool root = false);
  bool             ModeIsCC            (conv::ReacMode_t m);  
  std::string      ReacTypeAsString    (conv::ReacType_t t);

} // util

} // valor
  
#endif 

