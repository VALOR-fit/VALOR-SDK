//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <vector>
#include <iostream>

#include "TObjArray.h"
#include "TString.h"
#include "TMath.h"

// valor/dune
#include "valor/Conventions/PDG.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"

#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/Sample.h"

#include "valor/GENIEUtilities/GENIEIncludes.h"
#include "valor/GENIEUtilities/UtlEventRec.h"

using namespace valor;

bool valor::util::GetHadronicSystems(const genie::EventRecord * event, vector<int> & prim_had_syst, vector<int> & final_had_syst) 
{
  // Function adapted from GENIE app gNtpConv (GST).
  prim_had_syst .clear();
  final_had_syst.clear();
  
  // Process id
  const genie::ProcessInfo &  proc_info = event->Summary()->ProcInfo();
  bool is_qel     = proc_info.IsQuasiElastic();
  bool is_res     = proc_info.IsResonant();
  bool is_dis     = proc_info.IsDeepInelastic();
  bool is_dfr     = proc_info.IsDiffractive();
  bool is_singlek = proc_info.IsSingleKaon();    
  bool is_mec     = proc_info.IsMEC();
  
#if __GENIE_RELEASE_CODE__ >= GRELCODE(3,4,0)
  bool is_coh     = proc_info.IsCoherentProduction();
#else
  bool is_coh     = proc_info.IsCoherent();
#endif

  // Extract more info on the hadronic system
  // Only for QEL/RES/DIS/COH/MEC events
  //
  bool study_hadsyst = (is_qel || is_res || is_dis || is_coh || is_dfr || is_mec || is_singlek);
  if (!study_hadsyst) {return false;}

  //
  TObjArrayIter piter(event);
  genie::GHepParticle * p = 0;
  int ip=-1;

  //
  // Extract the final state system originating from the hadronic vertex 
  // (after the intranuclear rescattering step)
  //

  LOG("UtlEventRec", pDEBUG) << "Extracting final state hadronic system";

  while( (p = (genie::GHepParticle *) piter.Next()) && study_hadsyst)
  {
    ip++;
    // don't count final state lepton as part hadronic system 
    //if(!is_coh && event->Particle(ip)->FirstMother()==0) continue;
    if(event->Particle(ip)->FirstMother()==0) continue;
    if(genie::pdg::IsPseudoParticle(p->Pdg())) continue;
    int pdgc = p->Pdg();
    int ist  = p->Status();
    if(ist==genie::kIStStableFinalState) {
      if (pdgc == genie::kPdgGamma || pdgc == genie::kPdgElectron || pdgc == genie::kPdgPositron)  {
        int igmom = p->FirstMother();
        if(igmom!=-1) {
          // only count e+'s e-'s or gammas not from decay of pi0
          if(event->Particle(igmom)->Pdg() != genie::kPdgPi0) { final_had_syst.push_back(ip); }
        }
      } else {
        final_had_syst.push_back(ip);
      }
    }
    // now add pi0's that were decayed as short lived particles
    else if(pdgc == genie::kPdgPi0){
      int ifd = p->FirstDaughter();
      int fd_pdgc = event->Particle(ifd)->Pdg();
      // just require that first daughter is one of gamma, e+ or e-  
      if(fd_pdgc == genie::kPdgGamma || fd_pdgc == genie::kPdgElectron || fd_pdgc == genie::kPdgPositron){
        final_had_syst.push_back(ip);
      }
    }
  }//particle-loop

  //~ if( count(final_had_syst.begin(), final_had_syst.end(), -1) > 0) {
    //~ mcrec->Clear();
    //~ continue;
  //~ }

  //
  // Extract info on the primary hadronic system (before any intranuclear rescattering)
  // looking for particles with status_code == kIStHadronInTheNucleus 
  // An exception is the coherent production and scattering off free nucleon targets 
  // (no intranuclear rescattering) in which case primary hadronic system is set to be 
  // 'identical' with the final  state hadronic system
  //

  LOG("UtlEventRec", pDEBUG) << "Extracting primary hadronic system";

  ip = -1;
  TObjArrayIter piter_prim(event);
  
  if(study_hadsyst) {
    // if coherent or free nucleon target set primary states equal to final states
    genie::GHepParticle * target = event->Particle(1);
    if(!genie::pdg::IsIon(target->Pdg()) || (is_coh)) {
      vector<int>::const_iterator hiter = final_had_syst.begin();
      for( ; hiter != final_had_syst.end(); ++hiter) {
        prim_had_syst.push_back(*hiter);
      }
    } 
    // otherwise loop over all particles and store indices of those which are hadrons
    // created within the nucleus
    else {
      while( (p = (genie::GHepParticle *) piter_prim.Next()) ){
        ip++;      
        int ist_comp  = p->Status();
        if(ist_comp==genie::kIStHadronInTheNucleus) {
          prim_had_syst.push_back(ip); 
        }
      }//particle-loop   
      //
      // also include gammas from nuclear de-excitations (appearing in the daughter list of the 
      // hit nucleus, earlier than the primary hadronic system extracted above)
      for(int i = target->FirstDaughter(); i <= target->LastDaughter(); i++) {
        if(i<0) continue;
        if(event->Particle(i)->Status()==genie::kIStStableFinalState) { prim_had_syst.push_back(i); }
      }      
    }//freenuc?
  }//study_hadsystem?
  
  return study_hadsyst;
}
//_________________________________________________________________________________
bool valor::util::EventContainsQuarkFlavour(const genie::EventRecord * event, int quark_pdg)
{
  //
  TObjArrayIter piter(event);
  genie::GHepParticle * p = 0;
  
  bool result = false;
  
  while( (p = (genie::GHepParticle *) piter.Next())) {
    int pdg = std::abs(p->Pdg());
    pdg = std::abs(pdg);
    
    // Check if PDG code contains the quark PDG
    if (genie::pdg::IsHadron(pdg)) {
      int magnitude = 1000;
      while(magnitude) {
        int digit = (pdg / magnitude) % 10;
        if (digit==std::abs(quark_pdg)) {result = true;}
        magnitude/=10;
      }
    }
  } //piter
  return result;
}
//_________________________________________________________________________________
conv::ReacMode_t valor::util::DetermineReactionMode(const genie::EventRecord * event, bool use_final_state)
{
  int  nupdg       = event->Probe()->Pdg();
  bool is_nue      = (nupdg == genie::kPdgNuE);
  bool is_numu     = (nupdg == genie::kPdgNuMu);
  bool is_nuebar   = (nupdg == genie::kPdgAntiNuE);
  bool is_numubar  = (nupdg == genie::kPdgAntiNuMu);

  bool is_CC       = event->Summary()->ProcInfo().IsWeakCC();
  bool is_NC       = event->Summary()->ProcInfo().IsWeakNC();
  bool is_charm   = util::EventContainsQuarkFlavour(event, genie::kPdgCQuark);
  bool is_strange = util::EventContainsQuarkFlavour(event, genie::kPdgSQuark);
  bool is_QE       = event->Summary()->ProcInfo().IsQuasiElastic() && !is_strange && !is_charm;
  bool is_MEC      = event->Summary()->ProcInfo().IsMEC();
  bool is_NuEEl    = event->Summary()->ProcInfo().IsNuElectronElastic();
  bool is_IMD      = event->Summary()->ProcInfo().IsInverseMuDecay();
  bool is_IMDanh   = event->Summary()->ProcInfo().IsIMDAnnihilation();
  
#if __GENIE_RELEASE_CODE__ >= GRELCODE(3,4,0)
  bool is_Coherent = event->Summary()->ProcInfo().IsCoherentProduction();
#else
  bool is_Coherent  = event->Summary()->ProcInfo().IsCoherent();
#endif

  /*  if (is_NuEEl) {
    LOG("UtlEventRec",pWARN)<<"Ignoring NuEEl event.";
    return conv::kmUndefined;
    }*/
  
  vector<int> primary_hadr_system;
  vector<int> final_hadr_system;
  
  GetHadronicSystems(event,primary_hadr_system,final_hadr_system);
  
  // Set up variables for counting particles
  int nP        = 0;
  int nN        = 0;
  int nPip      = 0;
  int nPim      = 0;
  int nPi0      = 0;
  int nKp       = 0;
  int nKm       = 0;
  int nK0       = 0;
  int nEM       = 0;
  int nOtherHad = 0;
  int nNucleus  = 0;
  
  std::vector<const genie::GHepParticle*> unknown_particles;
  
  const vector<int> & hadr_system = (use_final_state ? final_hadr_system : primary_hadr_system);
  // We exclude states with the right number of pions but other mesons.
  

  
  for (unsigned int ip = 0 ; ip < hadr_system.size(); ip++) {
    const genie::GHepParticle * p = event->Particle(hadr_system[ip]);
    int pdg = p->Pdg();
    if      (pdg == genie::kPdgProton  || pdg == genie::kPdgAntiProton)                           { nP++;       }
    else if (pdg == genie::kPdgNeutron || pdg == genie::kPdgAntiNeutron)                          { nN++;       }
    else if (pdg == genie::kPdgPiP)                                                               { nPip++;     }
    else if (pdg == genie::kPdgPiM)                                                               { nPim++;     }
    else if (pdg == genie::kPdgPi0)                                                               { nPi0++;     }
    else if (pdg == genie::kPdgKP)                                                                { nKp++;      }
    else if (pdg == genie::kPdgKM)                                                                { nKm++;      }
    else if (pdg == genie::kPdgK0    || pdg == genie::kPdgAntiK0)                                 { nK0++;      }
    else if (pdg == genie::kPdgGamma || pdg == genie::kPdgElectron || pdg == genie::kPdgPositron) { nEM++;      }
    else if (genie::pdg::IsHadron(pdg))                                                           { nOtherHad++;}
    else if (genie::pdg::IsIon   (pdg))                                                           { nNucleus++; }
    else { unknown_particles.push_back(p); }
    
  }
  
  if (unknown_particles.size()) {
    std::ostringstream s;
    for (unsigned int ip = 0 ; ip < unknown_particles.size() ; ip++) {
      const genie::GHepParticle * p = unknown_particles[ip];
      s<<*p<<std::endl;
    }
    LOG("UtlEventRec",pWARN)<<unknown_particles.size()<<" unidentified particles in event:\n"<<s.str();
  }
  
  int nPiC = nPip+nPim;
  int nK   = nKp+nKm+nK0;
  
  bool is_1piC     = (!is_Coherent) && (nPiC==1) && (nPi0==0) && (nK==0) && (nOtherHad==0);
  bool is_1pi0     = (!is_Coherent) && (nPiC==0) && (nPi0==1) && (nK==0) && (nOtherHad==0); 
  bool is_2piC     = (!is_Coherent) && (nPiC==2) && (nPi0==0) && (nK==0) && (nOtherHad==0); 
  bool is_2pi0     = (!is_Coherent) && (nPiC==0) && (nPi0==2) && (nK==0) && (nOtherHad==0); 
  bool is_1piC1pi0 = (!is_Coherent) && (nPiC==1) && (nPi0==1) && (nK==0) && (nOtherHad==0); 
  
  bool is_ccnuel   = (is_NuEEl && is_CC);
  bool is_ncnuel   = (is_NuEEl && is_NC);
  
  vector<conv::ReacMode_t> modes;
  
  // First do exclusive modes
  if(is_numu) {
    if(is_CC) {
      if (is_QE)       { modes.push_back(conv::kmNuMuCCQE    ); } // numu    CC QE
      if (is_MEC)      { modes.push_back(conv::kmNuMuCCMEC   ); } // numu    CC MEC
      if (is_1piC)     { modes.push_back(conv::kmNuMuCC1piC  ); } // numu    CC 1 pi+/-
      if (is_1pi0)     { modes.push_back(conv::kmNuMuCC1pi0  ); } // numu    CC 1 pi0
      if (is_2piC)     { modes.push_back(conv::kmNuMuCC2piC  ); } // numu    CC 2 pi+/-
      if (is_2pi0)     { modes.push_back(conv::kmNuMuCC2pi0  ); } // numu    CC 2 pi0
      if (is_1piC1pi0) { modes.push_back(conv::kmNuMuCCpiCpi0); } // numu    CC 1 pi0 + 1 pi+/-
      if (is_Coherent) { modes.push_back(conv::kmNuMuCCcoh   ); } // numu    CC coherent
      if (is_IMD)      { modes.push_back(conv::kmNuMuCCIMD   ); } // numu    CC IMD
    }
    if(is_NC) {
      if (is_1piC)     { modes.push_back(conv::kmNuMuNC1piC  ); } // numu    NC 1 pi+/-
      if (is_1pi0)     { modes.push_back(conv::kmNuMuNC1pi0  ); } // numu    NC 1 pi0
      if (is_Coherent) { modes.push_back(conv::kmNuMuNCcoh   ); } // numu    NC coherent
    }
    if (is_ccnuel) { modes.push_back(conv::kmNuMuCCELScat   ); } // numu + electron CC
    if (is_ncnuel) { modes.push_back(conv::kmNuMuNCELScat   ); } // numu + electron NC
  }

  if(is_numubar) {
    if(is_CC) {
      if (is_QE)       { modes.push_back(conv::kmNuMuBarCCQE    ); } // numubar CC QE
      if (is_MEC)      { modes.push_back(conv::kmNuMuBarCCMEC   ); } // numubar CC MEC
      if (is_1piC)     { modes.push_back(conv::kmNuMuBarCC1piC  ); } // numubar CC 1 pi+/-
      if (is_1pi0)     { modes.push_back(conv::kmNuMuBarCC1pi0  ); } // numubar CC 1 pi0
      if (is_2piC)     { modes.push_back(conv::kmNuMuBarCC2piC  ); } // numubar CC 2 pi+/-
      if (is_2pi0)     { modes.push_back(conv::kmNuMuBarCC2pi0  ); } // numubar CC 2 pi0
      if (is_1piC1pi0) { modes.push_back(conv::kmNuMuBarCCpiCpi0); } // numubar CC 1 pi0 + 1 pi+/-
      if (is_Coherent) { modes.push_back(conv::kmNuMuBarCCcoh   ); } // numubar CC coherent
    }
    if(is_NC) {
      if (is_1piC)     { modes.push_back(conv::kmNuMuBarNC1piC ); } // numubar NC 1 pi+/-
      if (is_1pi0)     { modes.push_back(conv::kmNuMuBarNC1pi0 ); } // numubar NC 1 pi0
      if (is_Coherent) { modes.push_back(conv::kmNuMuBarNCcoh  ); } // numubar NC coherent
    }
    if (is_ccnuel) { modes.push_back(conv::kmNuMuBarCCELScat   ); } // numubar + electron CC                                                 
    if (is_ncnuel) { modes.push_back(conv::kmNuMuBarNCELScat   ); } // numubar + electron NC    
  }

  //
  if(is_nue) {
    if(is_CC) {
      if (is_QE)       { modes.push_back(conv::kmNuECCQE       );}     // nue     CC QE
      if (is_MEC)      { modes.push_back(conv::kmNuECCMEC      );}     // nue     CC MEC
      if (is_1piC)     { modes.push_back(conv::kmNuECC1piC     );}     // nue     CC 1 pi+/-
      if (is_1pi0)     { modes.push_back(conv::kmNuECC1pi0     );}     // nue     CC 1 pi0
      if (is_2piC)     { modes.push_back(conv::kmNuECC2piC     );}     // nue     CC 2 pi+/-
      if (is_2pi0)     { modes.push_back(conv::kmNuECC2pi0     );}     // nue     CC 2 pi0
      if (is_1piC1pi0) { modes.push_back(conv::kmNuECCpiCpi0   );}     // nue     CC 1 pi0 + 1 pi+/-
      if (is_Coherent) { modes.push_back(conv::kmNuECCcoh      );}     // nue     CC coherent
    }
    if(is_NC) {
      if (is_1piC)     { modes.push_back(conv::kmNuENC1piC     );}     // nue     NC 1 pi+/-
      if (is_1pi0)     { modes.push_back(conv::kmNuENC1pi0     );}     // nue     NC 1 pi0
      if (is_Coherent) { modes.push_back(conv::kmNuENCcoh      );}     // nue     NC coherent
    }
    if(is_NuEEl) { modes.push_back(conv::kmNuEELScat     );}     // nue + electron scattering (NC + CC + interference)
  }
  
  //
  if(is_nuebar) {
    if(is_CC) {
      if (is_QE)       { modes.push_back(conv::kmNuEBarCCQE    );}     // nuebar  CC QE
      if (is_MEC)      { modes.push_back(conv::kmNuEBarCCMEC   );}     // nuebar  CC MEC
      if (is_1piC)     { modes.push_back(conv::kmNuEBarCC1piC  );}     // nuebar  CC 1 pi+/-
      if (is_1pi0)     { modes.push_back(conv::kmNuEBarCC1pi0  );}     // nuebar  CC 1 pi0
      if (is_2piC)     { modes.push_back(conv::kmNuEBarCC2piC  );}     // nuebar  CC 2 pi+/-
      if (is_2pi0)     { modes.push_back(conv::kmNuEBarCC2pi0  );}     // nuebar  CC 2 pi0
      if (is_1piC1pi0) { modes.push_back(conv::kmNuEBarCCpiCpi0);}     // nuebar  CC 1 pi0 + 1 pi+/-
      if (is_Coherent) { modes.push_back(conv::kmNuEBarCCcoh   );}     // nuebar  CC coherent
      if (is_IMDanh)   { modes.push_back(conv::kmNuEBarCCIMD   );}     // nuebar  CC IMD annihilation 
    }
    if(is_NC) {
      if (is_1piC)     { modes.push_back(conv::kmNuEBarNC1piC );}     // nuebar  NC 1 pi+/-
      if (is_1pi0)     { modes.push_back(conv::kmNuEBarNC1pi0 );}     // nuebar  NC 1 pi0
      if (is_Coherent) { modes.push_back(conv::kmNuEBarNCcoh  );}     // nuebar  NC coherent
    }
    if(is_NuEEl) { modes.push_back(conv::kmNuEBarELScat     );}     // nuebar + electron scattering (NC + CC + interference)
  }
  
  // If we've got one exclusive mode, we did well.
  if (modes.size() == 1) {
    return modes[0];
  }
  // Otherwise look for inclusive
  else if (modes.size() == 0) {
    if (is_numu) {
      if (is_CC) {return conv::kmNuMuCCoth;}
      if (is_NC) {return conv::kmNuMuNCoth;}
    }
    if (is_numubar) {
      if (is_CC) {return conv::kmNuMuBarCCoth;}
      if (is_NC) {return conv::kmNuMuBarNCoth;}
    }
    if (is_nue) {
      if (is_CC) {return conv::kmNuECCoth;}
      if (is_NC) {return conv::kmNuENCoth;}
    }
    if (is_nuebar) {
      if (is_CC) {return conv::kmNuEBarCCoth;}
      if (is_NC) {return conv::kmNuEBarNCoth;}
    }
  }
  else {
    std::ostringstream mode_names;
    for (unsigned int ii = 0 ; ii < modes.size(); ii++) {
      mode_names << "\n "<<util::AsString(modes[ii]);
    }
    LOG("UtlEventRec",pERROR)<<"Event filed into multiple exclusive modes: "
      <<mode_names.str()
      <<"\nFull record: "<<*event;
      util::Die(-1);
  }

  return conv::kmUndefined;
}
//_________________________________________________________________________________

std::vector<double> valor::util::GetTrueKinematics(const genie::EventRecord* evrec,
      conv::KineSpace_t kv)
{
   // Get the interaction and kinematics from the event record
   genie::Interaction* interaction = evrec->Summary();
   const genie::Kinematics kine = interaction->Kine();

   std::vector<double> vars;

   switch(kv)
   {
      case conv::kspEtrue:
      {  // Retrieve the true energy
         double Enu = evrec->Probe()->Energy();
         vars.push_back(Enu);
         return vars;
      }
      case conv::kspEtrueQ2true:
      {  // Retrieve the true energy and momentum transfer
         double Enu = evrec->Probe()->Energy();
         double Q2 = kine.Q2(true);
         vars.push_back(Enu);
         vars.push_back(Q2);
         return vars;
      }
      default:
      {  // Only interested in true E and Q2 for now, ignore anything else
         return vars;
      }
   }
}
