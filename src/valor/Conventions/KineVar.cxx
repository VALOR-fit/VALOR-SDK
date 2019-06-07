//________________________________________________________________________________________
/*

   Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <algorithm>

//fmwk
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/KineVar.h"
#include "valor/Conventions/Constants.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(KineVar_t kv)
{
  switch(kv) {
    case (kvEtrue     ) : return "Etrue";     break;
    case (kvQ2true    ) : return "Q2true";    break;
    case (kvEreco     ) : return "Ereco";     break;
    case (kvEvis      ) : return "Evis";      break;
    case (kvYreco     ) : return "yreco";     break;
    case (kvThetareco ) : return "Thetareco"; break;
    case (kvq0reco    ) : return "q0reco";    break;
    case (kvmodqreco  ) : return "|q|reco";   break;
    default : 
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic variable: " << kv;
  }
  return "";
}
//________________________________________________________________________________________
string util::DSTBranchName(KineVar_t kv)
{
  switch(kv) {
    case (kvEtrue     ) : return "enu_true";     break;
    case (kvQ2true    ) : return "Q2_true";    break;
    case (kvEreco     ) : return "enu_reco";     break;
    case (kvEvis      ) : return "E_vis";      break;
    case (kvYreco     ) : return "y_reco";     break;
    case (kvThetareco ) : return "Theta_reco"; break;
    case (kvq0reco    ) : return "q0_reco";    break;
    case (kvmodqreco  ) : return "modq_reco";   break;
    default : 
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic variable: " << kv;
  }
  return "";
}
//________________________________________________________________________________________
string util::AsStringTeX(KineVar_t kv, bool root)
{
  string name = "";

  switch(kv) {
    case (kvEtrue     ) : name = "E_{true} (GeV)";                break;
    case (kvQ2true    ) : name = "Q^{2}_{true} (GeV^{2})";        break;
    case (kvEreco     ) : name = "E_{reco} (GeV)";                break;
    case (kvEvis      ) : name = "E_{vis} (GeV)";                 break;
    case (kvYreco     ) : name = "y_{reco}";                      break;
    case (kvThetareco ) : name = "\theta_{reco}";                 break;
    case (kvq0reco    ) : name = "q_{0 reco}";                    break;
    case (kvmodqreco  ) : name = "|\\overrightarrow{q}|_{reco}";  break;
    default : 
                          LOG("KineVar",pERROR) 
                            << "Unrecognized kinematic variable: " << kv;
  }

  if(root) {
    replace( name.begin(), name.end(), '\\', '#' ); // Make slashes hashes for RooTeX
    // Get rid of hashes preceding sin
    size_t ind = name.find("#sin",0);
    if (ind !=  string::npos) (name.replace(ind,4," sin"));
    return util::TrimSpaces(name);
  }

  return name;
}
//________________________________________________________________________________________
bool util::IsReco(KineVar_t kv)
{
  switch(kv) {
    case (kvEtrue     ) : return false;
    case (kvQ2true    ) : return false;
    case (kvEreco     ) : return true;
    case (kvEvis      ) : return true;
    case (kvYreco     ) : return true;
    case (kvThetareco ) : return true;
    case (kvq0reco    ) : return true;
    case (kvmodqreco  ) : return true;
    default:
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic variable: " << kv;
  }
  return false;
}
//________________________________________________________________________________________
bool util::IsTrue(KineVar_t kv)
{
  bool is_reco = util::IsReco(kv);
  return (!is_reco);
}
//________________________________________________________________________________________
KineVar_t util::KineVarFromInt(int k)
{
  // Cast an integer to KineVar_t, but with boundary check
  //            [first]                 [last]
  if((k >= (int)kvEtrue && k <= (int) kvmodqreco))
    return (KineVar_t)k;
  return kvUndefined;
}
//________________________________________________________________________________________
std::pair<double,double> util::PhysicalLimits(KineVar_t kv)
{
  switch(kv) {
    case (kvEtrue     ) : return std::make_pair(0.  , kALargeNum);
    case (kvQ2true    ) : return std::make_pair(0.  , kALargeNum);
    case (kvEreco     ) : return std::make_pair(0.  , kALargeNum);
    case (kvEvis      ) : return std::make_pair(0.  , kALargeNum);
    case (kvYreco     ) : return std::make_pair(0.  , 1.        );
    case (kvThetareco ) : return std::make_pair(0.  , 360.      );
    case (kvq0reco    ) : return std::make_pair(0.  , kALargeNum);
    case (kvmodqreco  ) : return std::make_pair(0.  , kALargeNum);
    default:
      LOG("KineVar",pFATAL)
        << "Unrecognized kinematic variable: " << kv;
      util::Die(1); return std::make_pair<double>(conv::kBadResult,conv::kBadResult);
  }
}
//________________________________________________________________________________________
conv::KineVar_t util::KineVarFromString(std::string kinevar)
{
  string lc_kinevar = util::ToLowerCase(kinevar);
  for (int i = 0 ; i < kNKineVarMax ; i++ ) {
    KineVar_t kv = (KineVar_t)i;
    string this_kv = util::ToLowerCase(util::AsString(kv));
    if(lc_kinevar==this_kv && this_kv!="--") {
      return kv;
    }
  }
  return kvUndefined;
}
//________________________________________________________________________________________
// aliases
string util::KineVarAsString(KineVar_t kv) { return util::AsString(kv); }
string util::KineVarAsStringTeX(KineVar_t kv, bool root) { return util::AsStringTeX(kv, root); }
//________________________________________________________________________________________
