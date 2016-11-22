//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

ChangeLog:

*/
//________________________________________________________________________________________

//fmwk
#include "valor/Utilities/UtlStr.h"
#include "valor/Messenger/Messenger.h"
//dune
#include "valor/Conventions/KineVar.h"

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
    case (kvEtrue     ) : name = "E_{true} (GeV)";         break;
    case (kvQ2true    ) : name = "Q^{2}_{true} (GeV^{2})"; break;
    case (kvEreco     ) : name = "E_{reco} (GeV)";         break;
    case (kvEvis      ) : name = "E_{vis} (GeV)";          break;
    case (kvYreco     ) : name = "y_{reco}";               break;
    case (kvThetareco ) : name = "\theta_{reco}";          break;            
    default : 
     LOG("KineVar",pERROR) 
        << "Unrecognized kinematic variable: " << kv;
  }

  if(root) return name;

  // change # with \ -- later

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
	
