//________________________________________________________________________________________
/*

   Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <algorithm>

//fmwk
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"
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
    case (kspq0recomodqreco)   : return "q0reco,|q|reco";         break;
    case (kspErecoEvis)        : return "Ereco,Evis";             break;
    case (kspLvsEtrue)         : return "L,Etrue";                break;
    case (kspLoverEtrue)       : return "L/Etrue";                break;
    case (kspNone)             : return "None";                   break;  
  
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
    case (kspq0recomodqreco)   : name = "<q_{0 reco},|\\overrightarrow{q}|_{reco}>";      break;
    case (kspErecoEvis)        : name = "<E_{reco},E_{vis}>";                             break;      
    case (kspLvsEtrue)         : name = "<L,E_{true}>";                                   break;      
    case (kspLoverEtrue)       : name = "<L/E_{true}>";                                   break;      
    case (kspNone)             : name = "None";                                           break; 
  
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
    case (kspq0recomodqreco)   : return true;
    case (kspErecoEvis)        : return true;
    case (kspLvsEtrue)         : return false;
    case (kspLoverEtrue)       : return false;
    case (kspNone)             : return false; //Not sure this is the case
  
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
    case (kspq0recomodqreco)   : return false;
    case (kspErecoEvis)        : return false;       
    case (kspLvsEtrue)         : return true;
    case (kspLoverEtrue)       : return true;
    case (kspNone)             : return true;  //Not sure this is the case
	    
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
    case (kspq0recomodqreco)   : return false;
    case (kspErecoEvis)        : return false;      
    case (kspLvsEtrue)         : return false;
    case (kspLoverEtrue)       : return false;
    case (kspNone)             : return false; //Not sure this is the case

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
    case (kspq0recomodqreco)   : return 2;
    case (kspErecoEvis)        : return 2;      
    case (kspLvsEtrue)         : return 2;
    case (kspLoverEtrue)       : return 1;
    case (kspNone)             : return 0;                    
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
    case (kspq0recomodqreco)   : return 2;
    case (kspErecoEvis)        : return 2;      
    case (kspLvsEtrue)         : return 0;
    case (kspLoverEtrue)       : return 0;
    case (kspNone)             : return 0;                    
  
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
    case (kspq0recomodqreco)   : return 0;
    case (kspErecoEvis)        : return 0;            
    case (kspLvsEtrue)         : return 2;
    case (kspLoverEtrue)       : return 1;
    case (kspNone)             : return 0;                    
  
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
    case (kspq0recomodqreco)   : return kspq0recomodqreco;
    case (kspErecoEvis)        : return kspErecoEvis;            
    case (kspLvsEtrue)         : return kspUndefined;
    case (kspLoverEtrue)       : return kspUndefined;
    case (kspNone)             : return kspUndefined;
  
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
    case (kspq0recomodqreco)   : return kspUndefined;
    case (kspErecoEvis)        : return kspUndefined;                  
    case (kspLvsEtrue)         : return kspLvsEtrue;
    case (kspLoverEtrue)       : return kspLoverEtrue;
    case (kspNone)             : return kspNone;                  

    default:
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }
  return kspUndefined;
}
//________________________________________________________________________________________
std::vector<valor::conv::KineVar_t> util::KineVarSubset(conv::KineSpace_t original,
      conv::KineSpace_t target)
{
   std::vector<KineVar_t> originalKineVars = util::KineVar(original);
   std::vector<KineVar_t> targetKineVars   = util::KineVar(target);
   
   std::vector<KineVar_t> in_both;
   //~ std::vector<KineVar_t> in_original_but_not_target;
   std::vector<KineVar_t> in_target_but_not_original;
   
   //~ for (const auto & kv_o : originalKineVars) {
      //~ if (!util::KineSpaceContains(target,kv_o)) {
        //~ in_original_but_not_target.push_back(kv_o);
      //~ }
   //~ }
   
   for (const auto & kv_t : targetKineVars) {
     if (util::KineSpaceContains(original,kv_t)) {
       in_both.push_back(kv_t);
     }
     else {
       in_target_but_not_original.push_back(kv_t);
     }
   }
   
   if (in_target_but_not_original.size()) {
     LOG("KineSpace",pFATAL) << "Phase space "<<util::AsString(target)<<" is not a string subset of "<<util::AsString(original);
     util::Die(-1); return std::vector<KineVar_t>();
   }
   
   return in_both;
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
      break;
    case (kspq0recomodqreco) :
      vv.push_back(kvq0reco);
      vv.push_back(kvmodqreco);
      break;
    case (kspErecoEvis):
      vv.push_back(kvEreco);
      vv.push_back(kvEvis);
      break;
    case (kspLvsEtrue):
      vv.push_back(kvL);
      vv.push_back(kvEtrue);
      break;
    case (kspLoverEtrue):
      vv.push_back(kvLoverEtrue);
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
      break;
    case (kspq0recomodqreco) :
      vv.push_back(kvq0reco);
      vv.push_back(kvmodqreco);
      break;
    case (kspErecoEvis):
      vv.push_back(kvEreco);
      vv.push_back(kvEvis);
      break;
    case (kspLvsEtrue):
      break;
    case (kspLoverEtrue):
      break;      
    case (kspNone):
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
    case (kspq0recomodqreco) :
      break;
    case (kspErecoEvis):
      break;
    case (kspLvsEtrue):
      vv.push_back(kvL);
      vv.push_back(kvEtrue);
      break;
    case (kspLoverEtrue):
      vv.push_back(kvLoverEtrue);
      break;
     default:
      LOG("KineVar",pERROR) 
        << "Unrecognized kinematic space: " << ks;
  }

  return vv;
}
//________________________________________________________________________________________
bool util::KineSpaceContains(conv::KineSpace_t ks, conv::KineVar_t kv)
{
  std::vector<valor::conv::KineVar_t> vars(util::KineVar(ks));
  return std::find(vars.begin(), vars.end(), kv) != vars.end();
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
conv::KineSpace_t util::KineSpaceFromKineVar(const KineVar_t & kv)
{
  switch (kv) {
    case kvUndefined   : return kspUndefined;
    case kvEtrue       : return kspEtrue;
    case kvQ2true      : return kspUndefined;
    case kvEreco       : return kspEreco;
    case kvEvis        : return kspEvis;
    case kvYreco       : return kspYreco;
    case kvThetareco   : return kspThetareco;
    case kvq0reco      : return kspUndefined;
    case kvmodqreco    : return kspUndefined;
    case kvL           : return kspUndefined;
    case kvLoverEtrue  : return kspLoverEtrue;
  }
  return kspUndefined;
}
//________________________________________________________________________________________
conv::KineSpace_t util::KineSpaceFromKineVars(std::vector<valor::conv::KineVar_t> kvs)
{
  for (int i = 0 ; i < conv::kMaxKineSpaces; i++) {
    KineSpace_t sp = (KineSpace_t)i;
    if(util::NDim(sp) != kvs.size()) { continue; }
    int matching_dim = 0;
    for (const conv::KineVar_t kv : kvs) {
      if(kv == conv::kvUndefined) { return kspUndefined; }
      if(util::KineSpaceContains(sp, kv)) { matching_dim++; }
    }
    if(matching_dim == util::NDim(sp)) { return sp; }
  }

  return kspUndefined;
}
//________________________________________________________________________________________
// aliases
string util::KineSpaceAsString(KineSpace_t ks) { return util::AsString(ks); }
string util::KineSpaceAsStringTeX(KineSpace_t ks, bool root) { return util::AsStringTeX(ks, root); }
//________________________________________________________________________________________
