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

static const int kNReacModes = 203; //excluding undefined and other

typedef enum EReacMode 
{
   kmUndefined        = -999, 
   kmNuMuCCQE         =   1,       // numu    CC QE
   kmNuMuCCRESpPiP    =   2,       // numu    CC RES : v + p --> mu- + p + pi+
   kmNuMuCCRESnPi0    =   3,       // numu    CC RES : v + n --> mu- + p + pi0
   kmNuMuCCRESnPiP    =   4,       // numu    CC RES : v + n --> mu- + n + pi+
   kmNuMuCCRESSPP     =   5,       // numu    CC RES : other single pion production
   kmNuMuCCDIS        =   6,       // numu    CC DIS
   kmNuMuCCCOH        =   7,       // numu    CC COH
   kmNuMuCCMEC        =   8,       // numu    CC MEC
   kmNuMuNCQE         =   9,       // numu    NC QE
   kmNuMuNCRESpPi0    =  10,       // numu    NC RES : v + p --> v + p + pi0
   kmNuMuNCRESpPiP    =  11,       // numu    NC RES : v + p --> v + n + pi+
   kmNuMuNCRESnPi0    =  12,       // numu    NC RES : v + n --> v + n + pi0
   kmNuMuNCRESnPiM    =  13,       // numu    NC RES : v + n --> v + p + pi-
   kmNuMuNCRESSPP     =  14,       // numu    NC RES : other single pion production
   kmNuMuNCDIS        =  15,       // numu    NC DIS
   kmNuMuNCCOH        =  16,       // numu    NC COH
   kmNuMuNCMEC        =  17,       // numu    NC MEC
   kmNuMuBarCCQE      = - 1,       // numubar CC QE
   kmNuMuBarCCRESnPiM = - 2,       // numubar CC RES : vb + n --> mu+ + n + pi-
   kmNuMuBarCCRESpPi0 = - 3,       // numubar CC RES : vb + p --> mu+ + n + pi0
   kmNuMuBarCCRESpPiM = - 4,       // numubar CC RES : vb + p --> mu+ + p + pi-
   kmNuMuBarCCRESSPP  = - 5,       // numubar CC RES : other single pion production
   kmNuMuBarCCDIS     = - 6,       // numubar CC DIS
   kmNuMuBarCCCOH     = - 7,       // numubar CC COH
   kmNuMuBarCCMEC     = - 8,       // numubar CC MEC
   kmNuMuBarNCQE      = - 9,       // numubar NC QE
   kmNuMuBarNCRESpPi0 = -10,       // numubar NC RES : vb + p --> vb + p + pi0
   kmNuMuBarNCRESpPiP = -11,       // numubar NC RES : vb + p --> vb + n + pi+
   kmNuMuBarNCRESnPi0 = -12,       // numubar NC RES : vb + n --> vb + n + pi0
   kmNuMuBarNCRESnPiM = -13,       // numubar NC RES : vb + n --> vb + p + pi-
   kmNuMuBarNCRESSPP  = -14,       // numubar NC RES : other single pion production
   kmNuMuBarNCDIS     = -15,       // numubar NC DIS
   kmNuMuBarNCCOH     = -16,       // numubar NC COH
   kmNuMuBarNCMEC     = -17,       // numubar NC MEC
   kmNuECCQE          =  18,       // nue    CC QE
   kmNuECCRESpPiP     =  19,       // nue    CC RES : v + p --> e- + p + pi+
   kmNuECCRESnPi0     =  20,       // nue    CC RES : v + n --> e- + p + pi0
   kmNuECCRESnPiP     =  21,       // nue    CC RES : v + n --> e- + n + pi+
   kmNuECCRESSPP      =  22,       // nue    CC RES : other single pion production
   kmNuECCDIS         =  23,       // nue    CC DIS
   kmNuECCCOH         =  24,       // nue    CC COH
   kmNuECCMEC         =  25,       // nue    CC MEC
   kmNuENCQE          =  26,       // nue    NC QE
   kmNuENCRESpPi0     =  27,       // nue    NC RES : v + p --> v + p + pi0
   kmNuENCRESpPiP     =  28,       // nue    NC RES : v + p --> v + n + pi+
   kmNuENCRESnPi0     =  29,       // nue    NC RES : v + n --> v + n + pi0
   kmNuENCRESnPiM     =  30,       // nue    NC RES : v + n --> v + p + pi-
   kmNuENCRESSPP      =  31,       // nue    NC RES : other single pion production
   kmNuENCDIS         =  32,       // nue    NC DIS
   kmNuENCCOH         =  33,       // nue    NC COH
   kmNuENCMEC         =  34,       // nue    NC MEC
   kmNuEBarCCQE       = -18,       // nuebar CC QE
   kmNuEBarCCRESnPiM  = -19,       // nuebar CC RES : vb + n --> e+ + n + pi-
   kmNuEBarCCRESpPi0  = -20,       // nuebar CC RES : vb + p --> e+ + n + pi0
   kmNuEBarCCRESpPiM  = -21,       // nuebar CC RES : vb + p --> e+ + p + pi-
   kmNuEBarCCRESSPP   = -22,       // nuebar CC RES : other single pion production
   kmNuEBarCCDIS      = -23,       // nuebar CC DIS
   kmNuEBarCCCOH      = -24,       // nuebar CC COH
   kmNuEBarCCMEC      = -25,       // nuebar CC MEC
   kmNuEBarNCQE       = -26,       // nuebar NC QE
   kmNuEBarNCRESpPi0  = -27,       // nuebar NC RES : vb + p --> vb + p + pi0
   kmNuEBarNCRESpPiP  = -28,       // nuebar NC RES : vb + p --> vb + n + pi+
   kmNuEBarNCRESnPi0  = -29,       // nuebar NC RES : vb + n --> vb + n + pi0
   kmNuEBarNCRESnPiM  = -30,       // nuebar NC RES : vb + n --> vb + p + pi-
   kmNuEBarNCRESSPP   = -31,       // nuebar NC RES : other single pion production
   kmNuEBarNCDIS      = -32,       // nuebar NC DIS
   kmNuEBarNCCOH      = -33,       // nuebar NC COH
   kmNuEBarNCMEC      = -34,       // nuebar NC MEC
   // For some reason, I have to add oscillated modes separately. 
   // mu to e
   kmNuMuToNuECCQE         =  101, // mu->e CC QE
   kmNuMuToNuECCRESpPiP    =  102, // mu->e CC RES : v + p --> e- + p + pi+
   kmNuMuToNuECCRESnPi0    =  103, // mu->e CC RES : v + n --> e- + p + pi0
   kmNuMuToNuECCRESnPiP    =  104, // mu->e CC RES : v + n --> e- + n + pi+
   kmNuMuToNuECCRESSPP     =  105, // mu->e CC RES : other single pion production
   kmNuMuToNuECCDIS        =  106, // mu->e CC DIS
   kmNuMuToNuECCCOH        =  107, // mu->e CC COH
   kmNuMuToNuECCMEC        =  108, // mu->e CC MEC
   kmNuMuToNuENCQE         =  109, // mu->e NC QE
   kmNuMuToNuENCRESpPi0    =  110, // mu->e NC RES : v + p --> v + p + pi0
   kmNuMuToNuENCRESpPiP    =  111, // mu->e NC RES : v + p --> v + n + pi+
   kmNuMuToNuENCRESnPi0    =  112, // mu->e NC RES : v + n --> v + n + pi0
   kmNuMuToNuENCRESnPiM    =  113, // mu->e NC RES : v + n --> v + p + pi-
   kmNuMuToNuENCRESSPP     =  114, // mu->e NC RES : other single pion production
   kmNuMuToNuENCDIS        =  115, // mu->e NC DIS
   kmNuMuToNuENCCOH        =  116, // mu->e NC COH
   kmNuMuToNuENCMEC        =  117, // mu->e NC MEC
   // mu to tau
   kmNuMuToNuTauCCQE         =  201, // mu->tau CC QE
   kmNuMuToNuTauCCRESpPiP    =  202, // mu->tau CC RES : v + p --> tau- + p + pi+
   kmNuMuToNuTauCCRESnPi0    =  203, // mu->tau CC RES : v + n --> tau- + p + pi0
   kmNuMuToNuTauCCRESnPiP    =  204, // mu->tau CC RES : v + n --> tau- + n + pi+
   kmNuMuToNuTauCCRESSPP     =  205, // mu->tau CC RES : other single pion production
   kmNuMuToNuTauCCDIS        =  206, // mu->tau CC DIS
   kmNuMuToNuTauCCCOH        =  207, // mu->tau CC COH
   kmNuMuToNuTauCCMEC        =  208, // mu->tau CC MEC
   kmNuMuToNuTauNCQE         =  209, // mu->tau NC QE
   kmNuMuToNuTauNCRESpPi0    =  210, // mu->tau NC RES : v + p --> v + p + pi0
   kmNuMuToNuTauNCRESpPiP    =  211, // mu->tau NC RES : v + p --> v + n + pi+
   kmNuMuToNuTauNCRESnPi0    =  212, // mu->tau NC RES : v + n --> v + n + pi0
   kmNuMuToNuTauNCRESnPiM    =  213, // mu->tau NC RES : v + n --> v + p + pi-
   kmNuMuToNuTauNCRESSPP     =  214, // mu->tau NC RES : other single pion production
   kmNuMuToNuTauNCDIS        =  215, // mu->tau NC DIS
   kmNuMuToNuTauNCCOH        =  216, // mu->tau NC COH
   kmNuMuToNuTauNCMEC        =  217, // mu->tau NC MEC
   // mub to eb
   kmNuMuBarToNuEBarCCQE      = -101, // mub->eb CC QE
   kmNuMuBarToNuEBarCCRESnPiM = -102, // mub->eb CC RES : vb + n --> e+ + n + pi-
   kmNuMuBarToNuEBarCCRESpPi0 = -103, // mub->eb CC RES : vb + p --> e+ + n + pi0
   kmNuMuBarToNuEBarCCRESpPiM = -104, // mub->eb CC RES : vb + p --> e+ + p + pi-
   kmNuMuBarToNuEBarCCRESSPP  = -105, // mub->eb CC RES : other single pion production
   kmNuMuBarToNuEBarCCDIS     = -106, // mub->eb CC DIS
   kmNuMuBarToNuEBarCCCOH     = -107, // mub->eb CC COH
   kmNuMuBarToNuEBarCCMEC     = -108, // mub->eb CC MEC
   kmNuMuBarToNuEBarNCQE      = -109, // mub->eb NC QE
   kmNuMuBarToNuEBarNCRESpPi0 = -110, // mub->eb NC RES : vb + p --> vb + p + pi0
   kmNuMuBarToNuEBarNCRESpPiP = -111, // mub->eb NC RES : vb + p --> vb + n + pi+
   kmNuMuBarToNuEBarNCRESnPi0 = -112, // mub->eb NC RES : vb + n --> vb + n + pi0
   kmNuMuBarToNuEBarNCRESnPiM = -113, // mub->eb NC RES : vb + n --> vb + p + pi-
   kmNuMuBarToNuEBarNCRESSPP  = -114, // mub->eb NC RES : other single pion production
   kmNuMuBarToNuEBarNCDIS     = -115, // mub->eb NC DIS
   kmNuMuBarToNuEBarNCCOH     = -116, // mub->eb NC COH
   kmNuMuBarToNuEBarNCMEC     = -117, // mub->eb NC MEC
   // mub to taub
   kmNuMuBarToNuTauBarCCQE      = -201, // mub->taub CC QE
   kmNuMuBarToNuTauBarCCRESnPiM = -202, // mub->taub CC RES : vb + n --> e+ + n + pi-
   kmNuMuBarToNuTauBarCCRESpPi0 = -203, // mub->taub CC RES : vb + p --> e+ + n + pi0
   kmNuMuBarToNuTauBarCCRESpPiM = -204, // mub->taub CC RES : vb + p --> e+ + p + pi-
   kmNuMuBarToNuTauBarCCRESSPP  = -205, // mub->taub CC RES : other single pion production
   kmNuMuBarToNuTauBarCCDIS     = -206, // mub->taub CC DIS
   kmNuMuBarToNuTauBarCCCOH     = -207, // mub->taub CC COH
   kmNuMuBarToNuTauBarCCMEC     = -208, // mub->taub CC MEC
   kmNuMuBarToNuTauBarNCQE      = -209, // mub->taub NC QE
   kmNuMuBarToNuTauBarNCRESpPi0 = -210, // mub->taub NC RES : vb + p --> vb + p + pi0
   kmNuMuBarToNuTauBarNCRESpPiP = -211, // mub->taub NC RES : vb + p --> vb + n + pi+
   kmNuMuBarToNuTauBarNCRESnPi0 = -212, // mub->taub NC RES : vb + n --> vb + n + pi0
   kmNuMuBarToNuTauBarNCRESnPiM = -213, // mub->taub NC RES : vb + n --> vb + p + pi-
   kmNuMuBarToNuTauBarNCRESSPP  = -214, // mub->taub NC RES : other single pion production
   kmNuMuBarToNuTauBarNCDIS     = -215, // mub->taub NC DIS
   kmNuMuBarToNuTauBarNCCOH     = -216, // mub->taub NC COH
   kmNuMuBarToNuTauBarNCMEC     = -217, // mub->taub NC MEC
   // e to mu
   kmNuEToNuMuCCQE         =  301, // e->mu CC QE
   kmNuEToNuMuCCRESpPiP    =  302, // e->mu CC RES : v + p --> mu- + p + pi+
   kmNuEToNuMuCCRESnPi0    =  303, // e->mu CC RES : v + n --> mu- + p + pi0
   kmNuEToNuMuCCRESnPiP    =  304, // e->mu CC RES : v + n --> mu- + n + pi+
   kmNuEToNuMuCCRESSPP     =  305, // e->mu CC RES : other single pion production
   kmNuEToNuMuCCDIS        =  306, // e->mu CC DIS
   kmNuEToNuMuCCCOH        =  307, // e->mu CC COH
   kmNuEToNuMuCCMEC        =  308, // e->mu CC MEC
   kmNuEToNuMuNCQE         =  309, // e->mu NC QE
   kmNuEToNuMuNCRESpPi0    =  310, // e->mu NC RES : v + p --> v + p + pi0
   kmNuEToNuMuNCRESpPiP    =  311, // e->mu NC RES : v + p --> v + n + pi+
   kmNuEToNuMuNCRESnPi0    =  312, // e->mu NC RES : v + n --> v + n + pi0
   kmNuEToNuMuNCRESnPiM    =  313, // e->mu NC RES : v + n --> v + p + pi-
   kmNuEToNuMuNCRESSPP     =  314, // e->mu NC RES : other single pion production
   kmNuEToNuMuNCDIS        =  315, // e->mu NC DIS
   kmNuEToNuMuNCCOH        =  316, // e->mu NC COH
   kmNuEToNuMuNCMEC        =  317, // e->mu NC MEC
   // e to tau
   kmNuEToNuTauCCQE         =  401, // e->tau CC QE
   kmNuEToNuTauCCRESpPiP    =  402, // e->tau CC RES : v + p --> tau- + p + pi+
   kmNuEToNuTauCCRESnPi0    =  403, // e->tau CC RES : v + n --> tau- + p + pi0
   kmNuEToNuTauCCRESnPiP    =  404, // e->tau CC RES : v + n --> tau- + n + pi+
   kmNuEToNuTauCCRESSPP     =  405, // e->tau CC RES : other single pion production
   kmNuEToNuTauCCDIS        =  406, // e->tau CC DIS
   kmNuEToNuTauCCCOH        =  407, // e->tau CC COH
   kmNuEToNuTauCCMEC        =  408, // e->tau CC MEC
   kmNuEToNuTauNCQE         =  409, // e->tau NC QE
   kmNuEToNuTauNCRESpPi0    =  410, // e->tau NC RES : v + p --> v + p + pi0
   kmNuEToNuTauNCRESpPiP    =  411, // e->tau NC RES : v + p --> v + n + pi+
   kmNuEToNuTauNCRESnPi0    =  412, // e->tau NC RES : v + n --> v + n + pi0
   kmNuEToNuTauNCRESnPiM    =  413, // e->tau NC RES : v + n --> v + p + pi-
   kmNuEToNuTauNCRESSPP     =  414, // e->tau NC RES : other single pion production
   kmNuEToNuTauNCDIS        =  415, // e->tau NC DIS
   kmNuEToNuTauNCCOH        =  416, // e->tau NC COH
   kmNuEToNuTauNCMEC        =  417, // e->tau NC MEC
   // eb to mub
   kmNuEBarToNuMuBarCCQE      = -301, // eb->mub CC QE
   kmNuEBarToNuMuBarCCRESnPiM = -302, // eb->mub CC RES : vb + n --> mu+ + n + pi-
   kmNuEBarToNuMuBarCCRESpPi0 = -303, // eb->mub CC RES : vb + p --> mu+ + n + pi0
   kmNuEBarToNuMuBarCCRESpPiM = -304, // eb->mub CC RES : vb + p --> mu+ + p + pi-
   kmNuEBarToNuMuBarCCRESSPP  = -305, // eb->mub CC RES : other single pion production
   kmNuEBarToNuMuBarCCDIS     = -306, // eb->mub CC DIS
   kmNuEBarToNuMuBarCCCOH     = -307, // eb->mub CC COH
   kmNuEBarToNuMuBarCCMEC     = -308, // eb->mub CC MEC
   kmNuEBarToNuMuBarNCQE      = -309, // eb->mub NC QE
   kmNuEBarToNuMuBarNCRESpPi0 = -310, // eb->mub NC RES : vb + p --> vb + p + pi0
   kmNuEBarToNuMuBarNCRESpPiP = -311, // eb->mub NC RES : vb + p --> vb + n + pi+
   kmNuEBarToNuMuBarNCRESnPi0 = -312, // eb->mub NC RES : vb + n --> vb + n + pi0
   kmNuEBarToNuMuBarNCRESnPiM = -313, // eb->mub NC RES : vb + n --> vb + p + pi-
   kmNuEBarToNuMuBarNCRESSPP  = -314, // eb->mub NC RES : other single pion production
   kmNuEBarToNuMuBarNCDIS     = -315, // eb->mub NC DIS
   kmNuEBarToNuMuBarNCCOH     = -316, // eb->mub NC COH
   kmNuEBarToNuMuBarNCMEC     = -317, // eb->mub NC MEC
   // eb to taub
   kmNuEBarToNuTauBarCCQE      = -401, // eb->taub CC QE
   kmNuEBarToNuTauBarCCRESnPiM = -402, // eb->taub CC RES : vb + n --> tau+ + n + pi-
   kmNuEBarToNuTauBarCCRESpPi0 = -403, // eb->taub CC RES : vb + p --> tau+ + n + pi0
   kmNuEBarToNuTauBarCCRESpPiM = -404, // eb->taub CC RES : vb + p --> tau+ + p + pi-
   kmNuEBarToNuTauBarCCRESSPP  = -405, // eb->taub CC RES : other single pion production
   kmNuEBarToNuTauBarCCDIS     = -406, // eb->taub CC DIS
   kmNuEBarToNuTauBarCCCOH     = -407, // eb->taub CC COH
   kmNuEBarToNuTauBarCCMEC     = -408, // eb->taub CC MEC
   kmNuEBarToNuTauBarNCQE      = -409, // eb->taub NC QE
   kmNuEBarToNuTauBarNCRESpPi0 = -410, // eb->taub NC RES : vb + p --> vb + p + pi0
   kmNuEBarToNuTauBarNCRESpPiP = -411, // eb->taub NC RES : vb + p --> vb + n + pi+
   kmNuEBarToNuTauBarNCRESnPi0 = -412, // eb->taub NC RES : vb + n --> vb + n + pi0
   kmNuEBarToNuTauBarNCRESnPiM = -413, // eb->taub NC RES : vb + n --> vb + p + pi-
   kmNuEBarToNuTauBarNCRESSPP  = -414, // eb->taub NC RES : other single pion production
   kmNuEBarToNuTauBarNCDIS     = -415, // eb->taub NC DIS
   kmNuEBarToNuTauBarNCCOH     = -416, // eb->taub NC COH
   kmNuEBarToNuTauBarNCMEC     = -417, // eb->taub NC MEC
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
  kmtCCRESvSPP   =    5, // CC RES : SPP by neutrino
  kmtCCRESvbnPiM = -  2, // CC RES : vb + n --> l+ + n + pi-
  kmtCCRESvbpPi0 = -  3, // CC RES : vb + p --> l+ + n + pi0
  kmtCCRESvbpPiM = -  4, // CC RES : vb + p --> l+ + p + pi-
  kmtCCRESvbSPP  = -  5, // CC RES : SPP by antineutrino
  kmtCCDIS       =    6, // CC DIS
  kmtCCCOH       =    7, // CC COH
  kmtCCMEC       =    8, // CC MEC
  kmtNCQE        =    9, // NC QE
  kmtNCRESvpPi0  =   10, // NC RES : v + p --> v + p + pi0
  kmtNCRESvpPiP  =   11, // NC RES : v + p --> v + n + pi+
  kmtNCRESvnPi0  =   12, // NC RES : v + n --> v + n + pi0
  kmtNCRESvnPiM  =   13, // NC RES : v + n --> v + p + pi-
  kmtNCRESvSPP   =   14, // NC RES : SPP by neutrino
  kmtNCRESvbpPi0 = - 10, // NC RES : vb + p --> vb + p + pi0
  kmtNCRESvbpPiP = - 11, // NC RES : vb + p --> vb + n + pi+
  kmtNCRESvbnPi0 = - 12, // NC RES : vb + n --> vb + n + pi0
  kmtNCRESvbnPiM = - 13, // NC RES : vb + n --> vb + p + pi-
  kmtNCRESvbSPP  = - 14, // NC RES : SPP by antineutrino
  kmtNCDIS       =   15, // NC DIS
  kmtNCCOH       =   16, // NC COH
  kmtNCMEC       =   17, // NC MEC
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
static const int kNReacType = 37;
 
// Map the reaction modes to their type
static std::unordered_map<ReacMode_t, ReacType_t> kReacModeToType =
{
  { kmNuMuCCQE, kmtCCQE },
  { kmNuMuCCRESpPiP, kmtCCRESvpPiP },
  { kmNuMuCCRESnPi0, kmtCCRESvnPi0 },
  { kmNuMuCCRESnPiP, kmtCCRESvnPiP },
  { kmNuMuCCRESSPP,  kmtCCRESvSPP },
  { kmNuMuCCDIS, kmtCCDIS },
  { kmNuMuCCCOH, kmtCCCOH },
  { kmNuMuCCMEC, kmtCCMEC },
  { kmNuMuNCQE, kmtNCQE },
  { kmNuMuNCRESpPi0, kmtNCRESvpPi0 },
  { kmNuMuNCRESpPiP, kmtNCRESvpPiP },
  { kmNuMuNCRESnPi0, kmtNCRESvnPi0 },
  { kmNuMuNCRESnPiM, kmtNCRESvnPiM },
  { kmNuMuNCRESSPP,  kmtNCRESvSPP },
  { kmNuMuNCDIS, kmtNCDIS },
  { kmNuMuNCCOH, kmtNCCOH },
  { kmNuMuNCMEC, kmtNCMEC },
  { kmNuMuBarCCQE, kmtCCQE },
  { kmNuMuBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuMuBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuMuBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuMuBarCCRESSPP,  kmtCCRESvbSPP },
  { kmNuMuBarCCDIS, kmtCCDIS },
  { kmNuMuBarCCCOH, kmtCCCOH },
  { kmNuMuBarCCMEC, kmtCCMEC },
  { kmNuMuBarNCQE, kmtNCQE },
  { kmNuMuBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuMuBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuMuBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuMuBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuMuBarNCRESSPP,  kmtNCRESvbSPP },
  { kmNuMuBarNCDIS, kmtNCDIS },
  { kmNuMuBarNCCOH, kmtNCCOH },
  { kmNuMuBarNCMEC, kmtNCMEC },
  { kmNuECCQE, kmtCCQE },
  { kmNuECCRESpPiP, kmtCCRESvpPiP },
  { kmNuECCRESnPi0, kmtCCRESvnPi0 },
  { kmNuECCRESnPiP, kmtCCRESvnPiP },
  { kmNuECCRESSPP,  kmtCCRESvSPP },
  { kmNuECCDIS, kmtCCDIS },
  { kmNuECCCOH, kmtCCCOH },
  { kmNuECCMEC, kmtCCMEC },
  { kmNuENCQE, kmtNCQE },
  { kmNuENCRESpPi0, kmtNCRESvpPi0 },
  { kmNuENCRESpPiP, kmtNCRESvpPiP },
  { kmNuENCRESnPi0, kmtNCRESvnPi0 },
  { kmNuENCRESnPiM, kmtNCRESvnPiM },
  { kmNuENCRESSPP,  kmtNCRESvSPP },
  { kmNuENCDIS, kmtNCDIS },
  { kmNuENCCOH, kmtNCCOH },
  { kmNuENCMEC, kmtNCMEC },
  { kmNuEBarCCQE, kmtCCQE },
  { kmNuEBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuEBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuEBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuEBarCCRESSPP,  kmtCCRESvSPP },
  { kmNuEBarCCDIS, kmtCCDIS },
  { kmNuEBarCCCOH, kmtCCCOH },
  { kmNuEBarCCMEC, kmtCCMEC },
  { kmNuEBarNCQE, kmtNCQE },
  { kmNuEBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuEBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuEBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuEBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuEBarNCRESSPP,  kmtNCRESvSPP },
  { kmNuEBarNCDIS, kmtNCDIS },
  { kmNuEBarNCCOH, kmtNCCOH },
  { kmNuEBarNCMEC, kmtNCMEC },
  { kmNuMuToNuECCQE, kmtCCQE },
  { kmNuMuToNuECCRESpPiP, kmtCCRESvpPiP },
  { kmNuMuToNuECCRESnPi0, kmtCCRESvnPi0 },
  { kmNuMuToNuECCRESnPiP, kmtCCRESvnPiP },
  { kmNuMuToNuECCRESSPP,  kmtCCRESvSPP },
  { kmNuMuToNuECCDIS, kmtCCDIS },
  { kmNuMuToNuECCCOH, kmtCCCOH },
  { kmNuMuToNuECCMEC, kmtCCMEC },
  { kmNuMuToNuENCQE, kmtNCQE },
  { kmNuMuToNuENCRESpPi0, kmtNCRESvpPi0 },
  { kmNuMuToNuENCRESpPiP, kmtNCRESvpPiP },
  { kmNuMuToNuENCRESnPi0, kmtNCRESvnPi0 },
  { kmNuMuToNuENCRESnPiM, kmtNCRESvnPiM },
  { kmNuMuToNuENCRESSPP,  kmtNCRESvSPP },
  { kmNuMuToNuENCDIS, kmtNCDIS },
  { kmNuMuToNuENCCOH, kmtNCCOH },
  { kmNuMuToNuENCMEC, kmtNCMEC },
  { kmNuMuToNuTauCCQE, kmtCCQE },
  { kmNuMuToNuTauCCRESpPiP, kmtCCRESvpPiP },
  { kmNuMuToNuTauCCRESnPi0, kmtCCRESvnPi0 },
  { kmNuMuToNuTauCCRESnPiP, kmtCCRESvnPiP },
  { kmNuMuToNuTauCCRESSPP,  kmtCCRESvSPP },
  { kmNuMuToNuTauCCDIS, kmtCCDIS },
  { kmNuMuToNuTauCCCOH, kmtCCCOH },
  { kmNuMuToNuTauCCMEC, kmtCCMEC },
  { kmNuMuToNuTauNCQE, kmtNCQE },
  { kmNuMuToNuTauNCRESpPi0, kmtNCRESvpPi0 },
  { kmNuMuToNuTauNCRESpPiP, kmtNCRESvpPiP },
  { kmNuMuToNuTauNCRESnPi0, kmtNCRESvnPi0 },
  { kmNuMuToNuTauNCRESnPiM, kmtNCRESvnPiM },
  { kmNuMuToNuTauNCRESSPP,  kmtNCRESvSPP },
  { kmNuMuToNuTauNCDIS, kmtNCDIS },
  { kmNuMuToNuTauNCCOH, kmtNCCOH },
  { kmNuMuToNuTauNCMEC, kmtNCMEC },
  { kmNuMuBarToNuEBarCCQE, kmtCCQE },
  { kmNuMuBarToNuEBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuMuBarToNuEBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuMuBarToNuEBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuMuBarToNuEBarCCRESSPP,  kmtCCRESvbSPP },
  { kmNuMuBarToNuEBarCCDIS, kmtCCDIS },
  { kmNuMuBarToNuEBarCCCOH, kmtCCCOH },
  { kmNuMuBarToNuEBarCCMEC, kmtCCMEC },
  { kmNuMuBarToNuEBarNCQE, kmtNCQE },
  { kmNuMuBarToNuEBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuMuBarToNuEBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuMuBarToNuEBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuMuBarToNuEBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuMuBarToNuEBarNCRESSPP,  kmtNCRESvbSPP },
  { kmNuMuBarToNuEBarNCDIS, kmtNCDIS },
  { kmNuMuBarToNuEBarNCCOH, kmtNCCOH },
  { kmNuMuBarToNuEBarNCMEC, kmtNCMEC },
  { kmNuMuBarToNuTauBarCCQE, kmtCCQE },
  { kmNuMuBarToNuTauBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuMuBarToNuTauBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuMuBarToNuTauBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuMuBarToNuTauBarCCRESSPP,  kmtCCRESvbSPP },
  { kmNuMuBarToNuTauBarCCDIS, kmtCCDIS },
  { kmNuMuBarToNuTauBarCCCOH, kmtCCCOH },
  { kmNuMuBarToNuTauBarCCMEC, kmtCCMEC },
  { kmNuMuBarToNuTauBarNCQE, kmtNCQE },
  { kmNuMuBarToNuTauBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuMuBarToNuTauBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuMuBarToNuTauBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuMuBarToNuTauBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuMuBarToNuTauBarNCRESSPP,  kmtNCRESvbSPP },
  { kmNuMuBarToNuTauBarNCDIS, kmtNCDIS },
  { kmNuMuBarToNuTauBarNCCOH, kmtNCCOH },
  { kmNuMuBarToNuTauBarNCMEC, kmtNCMEC },
  { kmNuEToNuMuCCQE, kmtCCQE },
  { kmNuEToNuMuCCRESpPiP, kmtCCRESvpPiP },
  { kmNuEToNuMuCCRESnPi0, kmtCCRESvnPi0 },
  { kmNuEToNuMuCCRESnPiP, kmtCCRESvnPiP },
  { kmNuEToNuMuCCRESSPP,  kmtCCRESvSPP },
  { kmNuEToNuMuCCDIS, kmtCCDIS },
  { kmNuEToNuMuCCCOH, kmtCCCOH },
  { kmNuEToNuMuCCMEC, kmtCCMEC },
  { kmNuEToNuMuNCQE, kmtNCQE },
  { kmNuEToNuMuNCRESpPi0, kmtNCRESvpPi0 },
  { kmNuEToNuMuNCRESpPiP, kmtNCRESvpPiP },
  { kmNuEToNuMuNCRESnPi0, kmtNCRESvnPi0 },
  { kmNuEToNuMuNCRESnPiM, kmtNCRESvnPiM },
  { kmNuEToNuMuNCRESSPP,  kmtNCRESvSPP },
  { kmNuEToNuMuNCDIS, kmtNCDIS },
  { kmNuEToNuMuNCCOH, kmtNCCOH },
  { kmNuEToNuMuNCMEC, kmtNCMEC },
  { kmNuEToNuTauCCQE, kmtCCQE },
  { kmNuEToNuTauCCRESpPiP, kmtCCRESvpPiP },
  { kmNuEToNuTauCCRESnPi0, kmtCCRESvnPi0 },
  { kmNuEToNuTauCCRESnPiP, kmtCCRESvnPiP },
  { kmNuEToNuTauCCRESSPP,  kmtCCRESvSPP },
  { kmNuEToNuTauCCDIS, kmtCCDIS },
  { kmNuEToNuTauCCCOH, kmtCCCOH },
  { kmNuEToNuTauCCMEC, kmtCCMEC },
  { kmNuEToNuTauNCQE, kmtNCQE },
  { kmNuEToNuTauNCRESpPi0, kmtNCRESvpPi0 },
  { kmNuEToNuTauNCRESpPiP, kmtNCRESvpPiP },
  { kmNuEToNuTauNCRESnPi0, kmtNCRESvnPi0 },
  { kmNuEToNuTauNCRESnPiM, kmtNCRESvnPiM },
  { kmNuEToNuTauNCRESSPP,  kmtNCRESvSPP },
  { kmNuEToNuTauNCDIS, kmtNCDIS },
  { kmNuEToNuTauNCCOH, kmtNCCOH },
  { kmNuEToNuTauNCMEC, kmtNCMEC },
  { kmNuEBarToNuMuBarCCQE, kmtCCQE },
  { kmNuEBarToNuMuBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuEBarToNuMuBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuEBarToNuMuBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuEBarToNuMuBarCCRESSPP,  kmtCCRESvbSPP },
  { kmNuEBarToNuMuBarCCDIS, kmtCCDIS },
  { kmNuEBarToNuMuBarCCCOH, kmtCCCOH },
  { kmNuEBarToNuMuBarCCMEC, kmtCCMEC },
  { kmNuEBarToNuMuBarNCQE, kmtNCQE },
  { kmNuEBarToNuMuBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuEBarToNuMuBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuEBarToNuMuBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuEBarToNuMuBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuEBarToNuMuBarNCRESSPP,  kmtNCRESvbSPP },
  { kmNuEBarToNuMuBarNCDIS, kmtNCDIS },
  { kmNuEBarToNuMuBarNCCOH, kmtNCCOH },
  { kmNuEBarToNuMuBarNCMEC, kmtNCMEC },
  { kmNuEBarToNuTauBarCCQE, kmtCCQE },
  { kmNuEBarToNuTauBarCCRESnPiM, kmtCCRESvbnPiM },
  { kmNuEBarToNuTauBarCCRESpPi0, kmtCCRESvbpPi0 },
  { kmNuEBarToNuTauBarCCRESpPiM, kmtCCRESvbpPiM },
  { kmNuEBarToNuTauBarCCRESSPP,  kmtCCRESvbSPP },
  { kmNuEBarToNuTauBarCCDIS, kmtCCDIS },
  { kmNuEBarToNuTauBarCCCOH, kmtCCCOH },
  { kmNuEBarToNuTauBarCCMEC, kmtCCMEC },
  { kmNuEBarToNuTauBarNCQE, kmtNCQE },
  { kmNuEBarToNuTauBarNCRESpPi0, kmtNCRESvbpPi0 },
  { kmNuEBarToNuTauBarNCRESpPiP, kmtNCRESvbpPiP },
  { kmNuEBarToNuTauBarNCRESnPi0, kmtNCRESvbnPi0 },
  { kmNuEBarToNuTauBarNCRESnPiM, kmtNCRESvbnPiM },
  { kmNuEBarToNuTauBarNCRESSPP,  kmtNCRESvbSPP },
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
  { kmNuMuCCRESSPP, kNuMu },
  { kmNuMuCCDIS, kNuMu },
  { kmNuMuCCCOH, kNuMu },
  { kmNuMuCCMEC, kNuMu },
  { kmNuMuNCQE, kNuMu },
  { kmNuMuNCRESpPi0, kNuMu },
  { kmNuMuNCRESpPiP, kNuMu },
  { kmNuMuNCRESnPi0, kNuMu },
  { kmNuMuNCRESnPiM, kNuMu },
  { kmNuMuNCRESSPP, kNuMu },
  { kmNuMuNCDIS, kNuMu },
  { kmNuMuNCCOH, kNuMu },
  { kmNuMuNCMEC, kNuMu },
  { kmNuMuBarCCQE, kNuMuBar },
  { kmNuMuBarCCRESnPiM, kNuMuBar },
  { kmNuMuBarCCRESpPi0, kNuMuBar },
  { kmNuMuBarCCRESpPiM, kNuMuBar },
  { kmNuMuBarCCRESSPP, kNuMuBar },
  { kmNuMuBarCCDIS, kNuMuBar },
  { kmNuMuBarCCCOH, kNuMuBar },
  { kmNuMuBarCCMEC, kNuMuBar },
  { kmNuMuBarNCQE, kNuMuBar },
  { kmNuMuBarNCRESpPi0, kNuMuBar },
  { kmNuMuBarNCRESpPiP, kNuMuBar },
  { kmNuMuBarNCRESnPi0, kNuMuBar },
  { kmNuMuBarNCRESnPiM, kNuMuBar },
  { kmNuMuBarNCRESSPP, kNuMuBar },
  { kmNuMuBarNCDIS, kNuMuBar },
  { kmNuMuBarNCCOH, kNuMuBar },
  { kmNuMuBarNCMEC, kNuMuBar },
  { kmNuECCQE, kNuE },
  { kmNuECCRESpPiP, kNuE },
  { kmNuECCRESnPi0, kNuE },
  { kmNuECCRESnPiP, kNuE },
  { kmNuECCRESSPP, kNuE },
  { kmNuECCDIS, kNuE },
  { kmNuECCCOH, kNuE },
  { kmNuECCMEC, kNuE },
  { kmNuENCQE, kNuE },
  { kmNuENCRESpPi0, kNuE },
  { kmNuENCRESpPiP, kNuE },
  { kmNuENCRESnPi0, kNuE },
  { kmNuENCRESnPiM, kNuE },
  { kmNuENCRESSPP, kNuE },
  { kmNuENCDIS, kNuE },
  { kmNuENCCOH, kNuE },
  { kmNuENCMEC, kNuE },
  { kmNuEBarCCQE, kNuEBar },
  { kmNuEBarCCRESnPiM, kNuEBar },
  { kmNuEBarCCRESpPi0, kNuEBar },
  { kmNuEBarCCRESpPiM, kNuEBar },
  { kmNuEBarCCRESSPP, kNuEBar },
  { kmNuEBarCCDIS, kNuEBar },
  { kmNuEBarCCCOH, kNuEBar },
  { kmNuEBarCCMEC, kNuEBar },
  { kmNuEBarNCQE, kNuEBar },
  { kmNuEBarNCRESpPi0, kNuEBar },
  { kmNuEBarNCRESpPiP, kNuEBar },
  { kmNuEBarNCRESnPi0, kNuEBar },
  { kmNuEBarNCRESnPiM, kNuEBar },
  { kmNuEBarNCRESSPP, kNuEBar },
  { kmNuEBarNCDIS, kNuEBar },
  { kmNuEBarNCCOH, kNuEBar },
  { kmNuEBarNCMEC, kNuEBar },
  { kmNuMuToNuECCQE, kNuE },
  { kmNuMuToNuECCRESpPiP, kNuE },
  { kmNuMuToNuECCRESnPi0, kNuE },
  { kmNuMuToNuECCRESnPiP, kNuE },
  { kmNuMuToNuECCRESSPP, kNuE },
  { kmNuMuToNuECCDIS, kNuE },
  { kmNuMuToNuECCCOH, kNuE },
  { kmNuMuToNuECCMEC, kNuE },
  { kmNuMuToNuENCQE, kNuE },
  { kmNuMuToNuENCRESpPi0, kNuE },
  { kmNuMuToNuENCRESpPiP, kNuE },
  { kmNuMuToNuENCRESnPi0, kNuE },
  { kmNuMuToNuENCRESnPiM, kNuE },
  { kmNuMuToNuENCRESSPP, kNuE },
  { kmNuMuToNuENCDIS, kNuE },
  { kmNuMuToNuENCCOH, kNuE },
  { kmNuMuToNuENCMEC, kNuE },
  { kmNuMuToNuTauCCQE, kNuTau },
  { kmNuMuToNuTauCCRESpPiP, kNuTau },
  { kmNuMuToNuTauCCRESnPi0, kNuTau },
  { kmNuMuToNuTauCCRESnPiP, kNuTau },
  { kmNuMuToNuTauCCRESSPP, kNuTau },
  { kmNuMuToNuTauCCDIS, kNuTau },
  { kmNuMuToNuTauCCCOH, kNuTau },
  { kmNuMuToNuTauCCMEC, kNuTau },
  { kmNuMuToNuTauNCQE, kNuTau },
  { kmNuMuToNuTauNCRESpPi0, kNuTau },
  { kmNuMuToNuTauNCRESpPiP, kNuTau },
  { kmNuMuToNuTauNCRESnPi0, kNuTau },
  { kmNuMuToNuTauNCRESnPiM, kNuTau },
  { kmNuMuToNuTauNCRESSPP, kNuTau },
  { kmNuMuToNuTauNCDIS, kNuTau },
  { kmNuMuToNuTauNCCOH, kNuTau },
  { kmNuMuToNuTauNCMEC, kNuTau },
  { kmNuMuBarToNuEBarCCQE, kNuEBar },
  { kmNuMuBarToNuEBarCCRESnPiM, kNuEBar },
  { kmNuMuBarToNuEBarCCRESpPi0, kNuEBar },
  { kmNuMuBarToNuEBarCCRESpPiM, kNuEBar },
  { kmNuMuBarToNuEBarCCRESSPP, kNuEBar },
  { kmNuMuBarToNuEBarCCDIS, kNuEBar },
  { kmNuMuBarToNuEBarCCCOH, kNuEBar },
  { kmNuMuBarToNuEBarCCMEC, kNuEBar },
  { kmNuMuBarToNuEBarNCQE, kNuEBar },
  { kmNuMuBarToNuEBarNCRESpPi0, kNuEBar },
  { kmNuMuBarToNuEBarNCRESpPiP, kNuEBar },
  { kmNuMuBarToNuEBarNCRESnPi0, kNuEBar },
  { kmNuMuBarToNuEBarNCRESnPiM, kNuEBar },
  { kmNuMuBarToNuEBarNCRESSPP, kNuEBar },
  { kmNuMuBarToNuEBarNCDIS, kNuEBar },
  { kmNuMuBarToNuEBarNCCOH, kNuEBar },
  { kmNuMuBarToNuEBarNCMEC, kNuEBar },
  { kmNuMuBarToNuTauBarCCQE, kNuTauBar },
  { kmNuMuBarToNuTauBarCCRESnPiM, kNuTauBar },
  { kmNuMuBarToNuTauBarCCRESpPi0, kNuTauBar },
  { kmNuMuBarToNuTauBarCCRESpPiM, kNuTauBar },
  { kmNuMuBarToNuTauBarCCRESSPP, kNuTauBar },
  { kmNuMuBarToNuTauBarCCDIS, kNuTauBar },
  { kmNuMuBarToNuTauBarCCCOH, kNuTauBar },
  { kmNuMuBarToNuTauBarCCMEC, kNuTauBar },
  { kmNuMuBarToNuTauBarNCQE, kNuTauBar },
  { kmNuMuBarToNuTauBarNCRESpPi0, kNuTauBar },
  { kmNuMuBarToNuTauBarNCRESpPiP, kNuTauBar },
  { kmNuMuBarToNuTauBarNCRESnPi0, kNuTauBar },
  { kmNuMuBarToNuTauBarNCRESnPiM, kNuTauBar },
  { kmNuMuBarToNuTauBarNCRESSPP, kNuTauBar },
  { kmNuMuBarToNuTauBarNCDIS, kNuTauBar },
  { kmNuMuBarToNuTauBarNCCOH, kNuTauBar },
  { kmNuMuBarToNuTauBarNCMEC, kNuTauBar },
  { kmNuEToNuMuCCQE, kNuMu },
  { kmNuEToNuMuCCRESpPiP, kNuMu },
  { kmNuEToNuMuCCRESnPi0, kNuMu },
  { kmNuEToNuMuCCRESnPiP, kNuMu },
  { kmNuEToNuMuCCRESSPP, kNuMu },
  { kmNuEToNuMuCCDIS, kNuMu },
  { kmNuEToNuMuCCCOH, kNuMu },
  { kmNuEToNuMuCCMEC, kNuMu },
  { kmNuEToNuMuNCQE, kNuMu },
  { kmNuEToNuMuNCRESpPi0, kNuMu },
  { kmNuEToNuMuNCRESpPiP, kNuMu },
  { kmNuEToNuMuNCRESnPi0, kNuMu },
  { kmNuEToNuMuNCRESnPiM, kNuMu },
  { kmNuEToNuMuNCRESSPP, kNuMu },
  { kmNuEToNuMuNCDIS, kNuMu },
  { kmNuEToNuMuNCCOH, kNuMu },
  { kmNuEToNuMuNCMEC, kNuMu },
  { kmNuEToNuTauCCQE, kNuTau },
  { kmNuEToNuTauCCRESpPiP, kNuTau },
  { kmNuEToNuTauCCRESnPi0, kNuTau },
  { kmNuEToNuTauCCRESnPiP, kNuTau },
  { kmNuEToNuTauCCRESSPP, kNuTau },
  { kmNuEToNuTauCCDIS, kNuTau },
  { kmNuEToNuTauCCCOH, kNuTau },
  { kmNuEToNuTauCCMEC, kNuTau },
  { kmNuEToNuTauNCQE, kNuTau },
  { kmNuEToNuTauNCRESpPi0, kNuTau },
  { kmNuEToNuTauNCRESpPiP, kNuTau },
  { kmNuEToNuTauNCRESnPi0, kNuTau },
  { kmNuEToNuTauNCRESnPiM, kNuTau },
  { kmNuEToNuTauNCRESSPP, kNuTau },
  { kmNuEToNuTauNCDIS, kNuTau },
  { kmNuEToNuTauNCCOH, kNuTau },
  { kmNuEToNuTauNCMEC, kNuTau },
  { kmNuEBarToNuMuBarCCQE, kNuMuBar },
  { kmNuEBarToNuMuBarCCRESnPiM, kNuMuBar },
  { kmNuEBarToNuMuBarCCRESpPi0, kNuMuBar },
  { kmNuEBarToNuMuBarCCRESpPiM, kNuMuBar },
  { kmNuEBarToNuMuBarCCRESSPP, kNuMuBar },
  { kmNuEBarToNuMuBarCCDIS, kNuMuBar },
  { kmNuEBarToNuMuBarCCCOH, kNuMuBar },
  { kmNuEBarToNuMuBarCCMEC, kNuMuBar },
  { kmNuEBarToNuMuBarNCQE, kNuMuBar },
  { kmNuEBarToNuMuBarNCRESpPi0, kNuMuBar },
  { kmNuEBarToNuMuBarNCRESpPiP, kNuMuBar },
  { kmNuEBarToNuMuBarNCRESnPi0, kNuMuBar },
  { kmNuEBarToNuMuBarNCRESnPiM, kNuMuBar },
  { kmNuEBarToNuMuBarNCRESSPP, kNuMuBar },
  { kmNuEBarToNuMuBarNCDIS, kNuMuBar },
  { kmNuEBarToNuMuBarNCCOH, kNuMuBar },
  { kmNuEBarToNuMuBarNCMEC, kNuMuBar },
  { kmNuEBarToNuTauBarCCQE, kNuTauBar },
  { kmNuEBarToNuTauBarCCRESnPiM, kNuTauBar },
  { kmNuEBarToNuTauBarCCRESpPi0, kNuTauBar },
  { kmNuEBarToNuTauBarCCRESpPiM, kNuTauBar },
  { kmNuEBarToNuTauBarCCRESSPP, kNuTauBar },
  { kmNuEBarToNuTauBarCCDIS, kNuTauBar },
  { kmNuEBarToNuTauBarCCCOH, kNuTauBar },
  { kmNuEBarToNuTauBarCCMEC, kNuTauBar },
  { kmNuEBarToNuTauBarNCQE, kNuTauBar },
  { kmNuEBarToNuTauBarNCRESpPi0, kNuTauBar },
  { kmNuEBarToNuTauBarNCRESpPiP, kNuTauBar },
  { kmNuEBarToNuTauBarNCRESnPi0, kNuTauBar },
  { kmNuEBarToNuTauBarNCRESnPiM, kNuTauBar },
  { kmNuEBarToNuTauBarNCRESSPP, kNuTauBar },
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
  { kmNuMuCCRESSPP, kNuMu },
  { kmNuMuCCDIS, kNuMu },
  { kmNuMuCCCOH, kNuMu },
  { kmNuMuCCMEC, kNuMu },
  { kmNuMuNCQE, kNuMu },
  { kmNuMuNCRESpPi0, kNuMu },
  { kmNuMuNCRESpPiP, kNuMu },
  { kmNuMuNCRESnPi0, kNuMu },
  { kmNuMuNCRESnPiM, kNuMu },
  { kmNuMuNCRESSPP, kNuMu },
  { kmNuMuNCDIS, kNuMu },
  { kmNuMuNCCOH, kNuMu },
  { kmNuMuNCMEC, kNuMu },
  { kmNuMuBarCCQE, kNuMuBar },
  { kmNuMuBarCCRESnPiM, kNuMuBar },
  { kmNuMuBarCCRESpPi0, kNuMuBar },
  { kmNuMuBarCCRESpPiM, kNuMuBar },
  { kmNuMuBarCCRESSPP, kNuMuBar },
  { kmNuMuBarCCDIS, kNuMuBar },
  { kmNuMuBarCCCOH, kNuMuBar },
  { kmNuMuBarCCMEC, kNuMuBar },
  { kmNuMuBarNCQE, kNuMuBar },
  { kmNuMuBarNCRESpPi0, kNuMuBar },
  { kmNuMuBarNCRESpPiP, kNuMuBar },
  { kmNuMuBarNCRESnPi0, kNuMuBar },
  { kmNuMuBarNCRESnPiM, kNuMuBar },
  { kmNuMuBarNCRESSPP, kNuMuBar },
  { kmNuMuBarNCDIS, kNuMuBar },
  { kmNuMuBarNCCOH, kNuMuBar },
  { kmNuMuBarNCMEC, kNuMuBar },
  { kmNuECCQE, kNuE },
  { kmNuECCRESpPiP, kNuE },
  { kmNuECCRESnPi0, kNuE },
  { kmNuECCRESnPiP, kNuE },
  { kmNuECCRESSPP, kNuE },
  { kmNuECCDIS, kNuE },
  { kmNuECCCOH, kNuE },
  { kmNuECCMEC, kNuE },
  { kmNuENCQE, kNuE },
  { kmNuENCRESpPi0, kNuE },
  { kmNuENCRESpPiP, kNuE },
  { kmNuENCRESnPi0, kNuE },
  { kmNuENCRESnPiM, kNuE },
  { kmNuENCRESSPP, kNuE },
  { kmNuENCDIS, kNuE },
  { kmNuENCCOH, kNuE },
  { kmNuENCMEC, kNuE },
  { kmNuEBarCCQE, kNuEBar },
  { kmNuEBarCCRESnPiM, kNuEBar },
  { kmNuEBarCCRESpPi0, kNuEBar },
  { kmNuEBarCCRESpPiM, kNuEBar },
  { kmNuEBarCCRESSPP, kNuEBar },
  { kmNuEBarCCDIS, kNuEBar },
  { kmNuEBarCCCOH, kNuEBar },
  { kmNuEBarCCMEC, kNuEBar },
  { kmNuEBarNCQE, kNuEBar },
  { kmNuEBarNCRESpPi0, kNuEBar },
  { kmNuEBarNCRESpPiP, kNuEBar },
  { kmNuEBarNCRESnPi0, kNuEBar },
  { kmNuEBarNCRESnPiM, kNuEBar },
  { kmNuEBarNCRESSPP, kNuEBar },
  { kmNuEBarNCDIS, kNuEBar },
  { kmNuEBarNCCOH, kNuEBar },
  { kmNuEBarNCMEC, kNuEBar },
  { kmNuMuToNuECCQE, kNuMu },
  { kmNuMuToNuECCRESpPiP, kNuMu },
  { kmNuMuToNuECCRESnPi0, kNuMu },
  { kmNuMuToNuECCRESnPiP, kNuMu },
  { kmNuMuToNuECCRESSPP, kNuMu },
  { kmNuMuToNuECCDIS, kNuMu },
  { kmNuMuToNuECCCOH, kNuMu },
  { kmNuMuToNuECCMEC, kNuMu },
  { kmNuMuToNuENCQE, kNuMu },
  { kmNuMuToNuENCRESpPi0, kNuMu },
  { kmNuMuToNuENCRESpPiP, kNuMu },
  { kmNuMuToNuENCRESnPi0, kNuMu },
  { kmNuMuToNuENCRESnPiM, kNuMu },
  { kmNuMuToNuENCRESSPP, kNuMu },
  { kmNuMuToNuENCDIS, kNuMu },
  { kmNuMuToNuENCCOH, kNuMu },
  { kmNuMuToNuENCMEC, kNuMu },
  { kmNuMuToNuTauCCQE, kNuMu },
  { kmNuMuToNuTauCCRESpPiP, kNuMu },
  { kmNuMuToNuTauCCRESnPi0, kNuMu },
  { kmNuMuToNuTauCCRESnPiP, kNuMu },
  { kmNuMuToNuTauCCRESSPP, kNuMu },
  { kmNuMuToNuTauCCDIS, kNuMu },
  { kmNuMuToNuTauCCCOH, kNuMu },
  { kmNuMuToNuTauCCMEC, kNuMu },
  { kmNuMuToNuTauNCQE, kNuMu },
  { kmNuMuToNuTauNCRESpPi0, kNuMu },
  { kmNuMuToNuTauNCRESpPiP, kNuMu },
  { kmNuMuToNuTauNCRESnPi0, kNuMu },
  { kmNuMuToNuTauNCRESnPiM, kNuMu },
  { kmNuMuToNuTauNCRESSPP, kNuMu },
  { kmNuMuToNuTauNCDIS, kNuMu },
  { kmNuMuToNuTauNCCOH, kNuMu },
  { kmNuMuToNuTauNCMEC, kNuMu },
  { kmNuMuBarToNuEBarCCQE, kNuMuBar },
  { kmNuMuBarToNuEBarCCRESnPiM, kNuMuBar },
  { kmNuMuBarToNuEBarCCRESpPi0, kNuMuBar },
  { kmNuMuBarToNuEBarCCRESpPiM, kNuMuBar },
  { kmNuMuBarToNuEBarCCRESSPP, kNuMuBar },
  { kmNuMuBarToNuEBarCCDIS, kNuMuBar },
  { kmNuMuBarToNuEBarCCCOH, kNuMuBar },
  { kmNuMuBarToNuEBarCCMEC, kNuMuBar },
  { kmNuMuBarToNuEBarNCQE, kNuMuBar },
  { kmNuMuBarToNuEBarNCRESpPi0, kNuMuBar },
  { kmNuMuBarToNuEBarNCRESpPiP, kNuMuBar },
  { kmNuMuBarToNuEBarNCRESnPi0, kNuMuBar },
  { kmNuMuBarToNuEBarNCRESnPiM, kNuMuBar },
  { kmNuMuBarToNuEBarNCRESSPP, kNuMuBar },
  { kmNuMuBarToNuEBarNCDIS, kNuMuBar },
  { kmNuMuBarToNuEBarNCCOH, kNuMuBar },
  { kmNuMuBarToNuEBarNCMEC, kNuMuBar },
  { kmNuMuBarToNuTauBarCCQE, kNuMuBar },
  { kmNuMuBarToNuTauBarCCRESnPiM, kNuMuBar },
  { kmNuMuBarToNuTauBarCCRESpPi0, kNuMuBar },
  { kmNuMuBarToNuTauBarCCRESpPiM, kNuMuBar },
  { kmNuMuBarToNuTauBarCCRESSPP, kNuMuBar },
  { kmNuMuBarToNuTauBarCCDIS, kNuMuBar },
  { kmNuMuBarToNuTauBarCCCOH, kNuMuBar },
  { kmNuMuBarToNuTauBarCCMEC, kNuMuBar },
  { kmNuMuBarToNuTauBarNCQE, kNuMuBar },
  { kmNuMuBarToNuTauBarNCRESpPi0, kNuMuBar },
  { kmNuMuBarToNuTauBarNCRESpPiP, kNuMuBar },
  { kmNuMuBarToNuTauBarNCRESnPi0, kNuMuBar },
  { kmNuMuBarToNuTauBarNCRESnPiM, kNuMuBar },
  { kmNuMuBarToNuTauBarNCRESSPP, kNuMuBar },
  { kmNuMuBarToNuTauBarNCDIS, kNuMuBar },
  { kmNuMuBarToNuTauBarNCCOH, kNuMuBar },
  { kmNuMuBarToNuTauBarNCMEC, kNuMuBar },
  { kmNuEToNuMuCCQE, kNuE },
  { kmNuEToNuMuCCRESpPiP, kNuE },
  { kmNuEToNuMuCCRESnPi0, kNuE },
  { kmNuEToNuMuCCRESnPiP, kNuE },
  { kmNuEToNuMuCCRESSPP, kNuE },
  { kmNuEToNuMuCCDIS, kNuE },
  { kmNuEToNuMuCCCOH, kNuE },
  { kmNuEToNuMuCCMEC, kNuE },
  { kmNuEToNuMuNCQE, kNuE },
  { kmNuEToNuMuNCRESpPi0, kNuE },
  { kmNuEToNuMuNCRESpPiP, kNuE },
  { kmNuEToNuMuNCRESnPi0, kNuE },
  { kmNuEToNuMuNCRESnPiM, kNuE },
  { kmNuEToNuMuNCRESSPP, kNuE },
  { kmNuEToNuMuNCDIS, kNuE },
  { kmNuEToNuMuNCCOH, kNuE },
  { kmNuEToNuMuNCMEC, kNuE },
  { kmNuEToNuTauCCQE, kNuE },
  { kmNuEToNuTauCCRESpPiP, kNuE },
  { kmNuEToNuTauCCRESnPi0, kNuE },
  { kmNuEToNuTauCCRESnPiP, kNuE },
  { kmNuEToNuTauCCRESSPP, kNuE },
  { kmNuEToNuTauCCDIS, kNuE },
  { kmNuEToNuTauCCCOH, kNuE },
  { kmNuEToNuTauCCMEC, kNuE },
  { kmNuEToNuTauNCQE, kNuE },
  { kmNuEToNuTauNCRESpPi0, kNuE },
  { kmNuEToNuTauNCRESpPiP, kNuE },
  { kmNuEToNuTauNCRESnPi0, kNuE },
  { kmNuEToNuTauNCRESnPiM, kNuE },
  { kmNuEToNuTauNCRESSPP, kNuE },
  { kmNuEToNuTauNCDIS, kNuE },
  { kmNuEToNuTauNCCOH, kNuE },
  { kmNuEToNuTauNCMEC, kNuE },
  { kmNuEBarToNuMuBarCCQE, kNuEBar },
  { kmNuEBarToNuMuBarCCRESnPiM, kNuEBar },
  { kmNuEBarToNuMuBarCCRESpPi0, kNuEBar },
  { kmNuEBarToNuMuBarCCRESpPiM, kNuEBar },
  { kmNuEBarToNuMuBarCCRESSPP, kNuEBar },
  { kmNuEBarToNuMuBarCCDIS, kNuEBar },
  { kmNuEBarToNuMuBarCCCOH, kNuEBar },
  { kmNuEBarToNuMuBarCCMEC, kNuEBar },
  { kmNuEBarToNuMuBarNCQE, kNuEBar },
  { kmNuEBarToNuMuBarNCRESpPi0, kNuEBar },
  { kmNuEBarToNuMuBarNCRESpPiP, kNuEBar },
  { kmNuEBarToNuMuBarNCRESnPi0, kNuEBar },
  { kmNuEBarToNuMuBarNCRESnPiM, kNuEBar },
  { kmNuEBarToNuMuBarNCRESSPP, kNuEBar },
  { kmNuEBarToNuMuBarNCDIS, kNuEBar },
  { kmNuEBarToNuMuBarNCCOH, kNuEBar },
  { kmNuEBarToNuMuBarNCMEC, kNuEBar },
  { kmNuEBarToNuTauBarCCQE, kNuEBar },
  { kmNuEBarToNuTauBarCCRESnPiM, kNuEBar },
  { kmNuEBarToNuTauBarCCRESpPi0, kNuEBar },
  { kmNuEBarToNuTauBarCCRESpPiM, kNuEBar },
  { kmNuEBarToNuTauBarCCRESSPP, kNuEBar },
  { kmNuEBarToNuTauBarCCDIS, kNuEBar },
  { kmNuEBarToNuTauBarCCCOH, kNuEBar },
  { kmNuEBarToNuTauBarCCMEC, kNuEBar },
  { kmNuEBarToNuTauBarNCQE, kNuEBar },
  { kmNuEBarToNuTauBarNCRESpPi0, kNuEBar },
  { kmNuEBarToNuTauBarNCRESpPiP, kNuEBar },
  { kmNuEBarToNuTauBarNCRESnPi0, kNuEBar },
  { kmNuEBarToNuTauBarNCRESnPiM, kNuEBar },
  { kmNuEBarToNuTauBarNCRESSPP, kNuEBar },
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

