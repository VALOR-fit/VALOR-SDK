//________________________________________________________________________________________
/*

   Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

//fmwk
#include "valor/Utilities/UtlStr.h"
#include "valor/Messenger/Messenger.h"
//dune
#include "valor/Conventions/KineSpace.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(KineSpace_t ks)
{
  switch(ks) {
    case (kspEtrue)            : return "Etrue";                  break;
    case (kspEreco)            : return "Ereco";                  break;
    case (kspEvis)             : return "Evis";                   break;
    case (kspYreco)            : return "yreco";                  break;
    case (kspThetareco)        : return "Thetareco";              break;
    case (kspErecoYreco)       : return "Ereco,yreco";            break;
    case (kspEtrueQ2true)      : return "Etrue,Q2true";           break;
    case (kspErecoThetareco)   : return "Ereco,Thetareco";        break;
    case (kspq0recoq3reco)     : return "q0reco,q3reco";          break;
    case (kspq0recomodqreco)   : return "q0reco,|q|reco";         break;
    default : 
     LOG("KineSpace",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }
  return "";
}
//________________________________________________________________________________________
string util::AsStringTeX(KineSpace_t ks, bool root)
{
  string name = "";

  switch(ks) {
    case (kspEtrue)            : name = "<E_{true}>";                                     break;
    case (kspEreco)            : name = "<E_{reco}>";                                     break;
    case (kspEvis )            : name = "<E_{vis}>";                                      break;
    case (kspYreco)            : name = "<y_{reco}>";                                     break;
    case (kspThetareco)        : name = "\theta_{reco}";                                  break;
    case (kspErecoYreco)       : name = "<E_{reco},y_{reco}>";                            break;
    case (kspEtrueQ2true)      : name = "<E_{true},Q^2_{true}>";                          break;
    case (kspErecoThetareco)   : name = "<E_{reco},\theta_{reco}";                        break;
    case (kspq0recoq3reco)     : name = "<q_{0 reco},q_{3 reco}>";       				          break;
    case (kspq0recomodqreco)   : name = "<q_{0 reco},|\\overrightarrow{q}|_{reco}>";      break;
    default : 
     LOG("KineSpace",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }

  if(root) return name;

  // change # with \ -- later

  return name;
}
//________________________________________________________________________________________
bool util::IsPureReco(KineSpace_t ks)
{
  switch(ks) {
    case (kspEtrue)            : return false;
    case (kspEreco)            : return true;
    case (kspEvis )            : return true;
    case (kspYreco)            : return true;
    case (kspThetareco)        : return true;
    case (kspErecoYreco)       : return true;
    case (kspEtrueQ2true)      : return false;
    case (kspErecoThetareco)   : return true;
    case (kspq0recoq3reco)     : return true;
    case (kspq0recomodqreco)   : return true;
    default:
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }
  return false;
}
//________________________________________________________________________________________
bool util::IsPureTrue(KineSpace_t ks)
{
  switch(ks) {
    case (kspEtrue)            : return true;
    case (kspEreco)            : return false;
    case (kspEvis )            : return false;
    case (kspYreco)            : return false;
    case (kspThetareco)        : return false;
    case (kspErecoYreco)       : return false;
    case (kspEtrueQ2true)      : return true;
    case (kspErecoThetareco)   : return false;
    case (kspq0recoq3reco)     : return false;
    case (kspq0recomodqreco)   : return false;
    default:
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }
  return false;
}
//________________________________________________________________________________________
bool util::IsMixedRecoTrue(KineSpace_t ks)
{
  switch(ks) {
    case (kspEtrue)            : return false;
    case (kspEreco)            : return false;
    case (kspEvis )            : return false;
    case (kspYreco)            : return false;
    case (kspThetareco)        : return false;
    case (kspErecoYreco)       : return false;
    case (kspEtrueQ2true)      : return false;
    case (kspErecoThetareco)   : return false;
    case (kspq0recoq3reco)     : return false;
    case (kspq0recomodqreco)   : return false;
    default:
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }
  return false;
}
//________________________________________________________________________________________
int util::NDim(KineSpace_t ks)
{
  switch(ks) {
    case (kspEtrue)            : return 1;
    case (kspEreco)            : return 1;
    case (kspEvis )            : return 1;
    case (kspYreco)            : return 1;
    case (kspThetareco)        : return 1;
    case (kspErecoYreco)       : return 2;
    case (kspEtrueQ2true)      : return 2;
    case (kspErecoThetareco)   : return 2;
    case (kspq0recoq3reco)     : return 2;
    case (kspq0recomodqreco)   : return 2;
    default:
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }
  return 0;
}
//________________________________________________________________________________________
int util::NDimReco(KineSpace_t ks)
{
  switch(ks) {
    case (kspEtrue)            : return 0;
    case (kspEreco)            : return 1;
    case (kspEvis )            : return 1;
    case (kspYreco)            : return 1;
    case (kspThetareco)        : return 1;
    case (kspErecoYreco)       : return 2;
    case (kspEtrueQ2true)      : return 0;
    case (kspErecoThetareco)   : return 2;
    case (kspq0recoq3reco)     : return 2;
    case (kspq0recomodqreco)   : return 2;
    default:
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }
  return 0;
}
//________________________________________________________________________________________
int util::NDimTrue(KineSpace_t ks)
{
  switch(ks) {
    case (kspEtrue)            : return 1;
    case (kspEreco)            : return 0;
    case (kspEvis )            : return 0;
    case (kspYreco)            : return 0;
    case (kspThetareco)        : return 0;
    case (kspErecoYreco)       : return 0;
    case (kspEtrueQ2true)      : return 2;
    case (kspErecoThetareco)   : return 0;
    case (kspq0recoq3reco)     : return 0;
    case (kspq0recomodqreco)   : return 0;
    default:
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }
  return 0;
}
//________________________________________________________________________________________
conv::KineSpace_t util::SubSpaceReco(conv::KineSpace_t ks)
{
  switch(ks) {
    case (kspEtrue)            : return kspUndefined;
    case (kspEreco)            : return kspEreco;
    case (kspEvis )            : return kspEvis;
    case (kspYreco)            : return kspYreco;
    case (kspThetareco)        : return kspThetareco;
    case (kspErecoYreco)       : return kspErecoYreco;
    case (kspEtrueQ2true)      : return kspUndefined;
    case (kspErecoThetareco)   : return kspErecoThetareco;
    case (kspq0recoq3reco)     : return kspq0recoq3reco;
    case (kspq0recomodqreco)   : return kspq0recoq3reco;
    default:
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }
  return kspUndefined;
}
//________________________________________________________________________________________
conv::KineSpace_t util::SubSpaceTrue(conv::KineSpace_t ks)
{
  switch(ks) {
    case (kspEtrue)            : return kspEtrue;
    case (kspEreco)            : return kspUndefined;
    case (kspEvis )            : return kspUndefined;
    case (kspYreco)            : return kspUndefined;
    case (kspThetareco)        : return kspUndefined;
    case (kspErecoYreco)       : return kspUndefined;
    case (kspEtrueQ2true)      : return kspEtrueQ2true;
    case (kspErecoThetareco)   : return kspUndefined;
    case (kspq0recoq3reco)     : return kspUndefined;
    case (kspq0recomodqreco)   : return kspUndefined;
    default:
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }
  return kspUndefined;
}
//________________________________________________________________________________________
vector<KineVar_t> util::KineVar(conv::KineSpace_t ks)
{
  vector<KineVar_t> vv;

  switch(ks) {
    case (kspEtrue) : 
      vv.push_back(kvEtrue);
      break;
    case (kspEreco) : 
      vv.push_back(kvEreco);
      break;
    case (kspEvis) : 
      vv.push_back(kvEvis);
      break;
    case (kspYreco) : 
      vv.push_back(kvYreco);
      break;
    case (kspThetareco) :
      vv.push_back(kvThetareco);
      break;
    case (kspErecoYreco) : 
      vv.push_back(kvEreco);
      vv.push_back(kvYreco);
      break;
    case (kspEtrueQ2true) :
      vv.push_back(kvEtrue);
      vv.push_back(kvQ2true);
      break;
    case (kspErecoThetareco)   :
      vv.push_back(kvEreco);
      vv.push_back(kvThetareco);
    case (kspq0recoq3reco) :
      vv.push_back(kvq0reco);
      vv.push_back(kvq3reco);
      break;
    case (kspq0recomodqreco) :
      vv.push_back(kvq0reco);
      vv.push_back(kvmodqreco);
      break;
    default:
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }

  return vv;
}
//________________________________________________________________________________________
vector<KineVar_t> util::KineVarReco(conv::KineSpace_t ks)
{
  vector<KineVar_t> vv;

  switch(ks) {
    case (kspEtrue) : 
      break;
    case (kspEreco) : 
      vv.push_back(kvEreco);
      break;
    case (kspEvis) : 
      vv.push_back(kvEvis);
      break;
    case (kspYreco) : 
      vv.push_back(kvYreco);
      break;
    case (kspThetareco) :
      vv.push_back(kvThetareco);
      break;
    case (kspErecoYreco) : 
      vv.push_back(kvEreco);
      vv.push_back(kvYreco);
      break;
    case (kspEtrueQ2true) :
      break;
    case (kspErecoThetareco)   :
      vv.push_back(kvEreco);
      vv.push_back(kvThetareco);
    case (kspq0recoq3reco) :
      vv.push_back(kvq0reco);
      vv.push_back(kvq3reco);
      break;
    case (kspq0recomodqreco) :
      vv.push_back(kvq0reco);
      vv.push_back(kvmodqreco);
      break;
    default:
      LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }

  return vv;
}
//________________________________________________________________________________________
vector<KineVar_t> util::KineVarTrue(conv::KineSpace_t ks)
{
  vector<KineVar_t> vv;

  switch(ks) {
    case (kspEtrue) : 
      vv.push_back(kvEtrue);
      break;
    case (kspEreco) : 
      break;
    case (kspEvis) : 
      break;
    case (kspYreco) : 
      break;
    case (kspThetareco) :
      break;
    case (kspErecoYreco) : 
      break;
    case (kspEtrueQ2true) :
      vv.push_back(kvEtrue);
      vv.push_back(kvQ2true);
      break;
    case (kspErecoThetareco)   :
      break;
    case (kspq0recoq3reco) :
      break;
    case (kspq0recomodqreco) :
      break;
    default:
      LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }

  return vv;
}
//________________________________________________________________________________________
conv::KineSpace_t util::KineSpaceFromInt(int i_sp)
{
  // Cast an integer to KineSpace_t, but with boundary check
  //                [first]                   [last]
  if(i_sp >= (int)kspEtrue && i_sp <= (int) kspq0recomodqreco) return (KineSpace_t)i_sp;
  return kspUndefined;
}
//________________________________________________________________________________________
conv::KineSpace_t util::KineSpaceFromString(string str_sp)
{
  string lc_str_sp = util::ToLowerCase(str_sp);
  for (int i = 0 ; i < conv::kMaxKineSpaces; i++) {
    KineSpace_t sp = (KineSpace_t)i;
    string this_lc_str_sp = util::ToLowerCase(util::AsString(sp));
    if(lc_str_sp==this_lc_str_sp && this_lc_str_sp!="") {
      return sp;
    }
  }

  return kspUndefined;
}
//________________________________________________________________________________________
// aliases
string util::KineSpaceAsString(KineSpace_t ks) { return util::AsString(ks); }
string util::KineSpaceAsStringTeX(KineSpace_t ks, bool root) { return util::AsStringTeX(ks, root); }
//________________________________________________________________________________________
