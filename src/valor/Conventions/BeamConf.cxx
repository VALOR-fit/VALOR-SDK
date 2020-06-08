//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

//fmwk
#include "valor/Utilities/UtlStr.h"
//dune
#include "valor/Conventions/BeamConf.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(BeamConf_t b, Option_t * opt)
{
  string name = "";

  switch(b) {
    case (kbLBNF_FHC   ) : name = "LBNF_FHC";    break;
    case (kbLBNF_RHC   ) : name = "LBNF_RHC";    break;        
    case (kbJPARC_FHC  ) : name = "JPARC_FHC";   break; 
    case (kbJPARC_RHC  ) : name = "JPARC_RHC";   break; 
    case (kbNuMI_FHC   ) : name = "NUMI_FHC";    break;
    case (kbNuMI_RHC   ) : name = "NUMI_RHC";    break; 
    case (kbBNB_FHC    ) : name = "BNB_FHC";     break;
    case (kbBNB_RHC    ) : name = "BNB_RHC";     break; 
    default : name = "--";  
  }

  if(opt) {
    string name2  = name;
    string optstr = opt;
    if(optstr.find("lowercase") != string::npos) {
       name2 = util::ToLowerCase(name);
    }
    return name2;
  }

  return name;
}
//________________________________________________________________________________________
string util::BeamConfAsString(BeamConf_t b, Option_t * opt) {return util::AsString(b,opt);}
//________________________________________________________________________________________
BeamConf_t util::BeamConfFromInt(int b)
{
// Cast an integer to BeamConf_t, but with boundary check
//            [first]              [last]
  if(b >= (int)kbLBNF_FHC && b <= (int) kbBNB_RHC) return (BeamConf_t)b;
  return kbUndefined;
}
//________________________________________________________________________________________
BeamConf_t util::BeamConfFromString(string beam)
{
  string lc_beam = util::ToLowerCase(beam);
  for (int i = 0 ; i <= (int)kbBNB_RHC ; i++ ) {
    BeamConf_t s = (BeamConf_t)i;
    string this_beam = util::ToLowerCase(util::AsString(s));
    if(lc_beam==this_beam && this_beam!="--") {
      return s;
    }
  }
    
  return kbUndefined;
}
//________________________________________________________________________________________
