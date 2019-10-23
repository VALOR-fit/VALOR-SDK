//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

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

    case (kmNuMuCCQE        ): return "numuCCQE";     break;
    case (kmNuMuCCMEC       ): return "numuCCMEC";    break;        
    case (kmNuMuCC1piC      ): return "numuCC1piC";   break;        
    case (kmNuMuCC1pi0      ): return "numuCC1pi0";   break;        
    case (kmNuMuCC2piC      ): return "numuCC2piC";   break;        
    case (kmNuMuCC2pi0      ): return "numuCC2pi0";   break;        
    case (kmNuMuCCpiCpi0    ): return "numuCCpiCpi0"; break;         
    case (kmNuMuCCcoh       ): return "numuCCcoh";    break;       
    case (kmNuMuCCIMD       ): return "numuCCIMD";    break;
    case (kmNuMuCCELScat    ): return "numuCCELscat"; break;        
    case (kmNuMuCCoth       ): return "numuCCoth";    break;        
//  case (kmNuMuNCEL        ): return ""; break;        
//  case (kmNuMuNCMEC       ): return ""; break;        
    case (kmNuMuNC1piC      ): return "numuNC1piC"; break;        
    case (kmNuMuNC1pi0      ): return "numuNC1pi0"; break;        
//  case (kmNuMuNC2piC      ): return ""; break;        
//  case (kmNuMuNC2pi0      ): return ""; break;        
//  case (kmNuMuNCpiCpi0    ): return ""; break;       
    case (kmNuMuNCcoh       ): return "numuNCcoh";  break;        
    case (kmNuMuNCELScat    ): return "numuNCELscat"; break;        
    case (kmNuMuNCoth       ): return "numuNCoth";  break;        

    case (kmNuMuBarCCQE     ): return "numubarCCQE";     break;        
    case (kmNuMuBarCCMEC    ): return "numubarCCMEC";    break;        
    case (kmNuMuBarCC1piC   ): return "numubarCC1piC";   break;       
    case (kmNuMuBarCC1pi0   ): return "numubarCC1pi0";   break;       
    case (kmNuMuBarCC2piC   ): return "numubarCC2piC";   break;      
    case (kmNuMuBarCC2pi0   ): return "numubarCC2pi0";   break;       
    case (kmNuMuBarCCpiCpi0 ): return "numubarCCpiCpi0"; break;   
    case (kmNuMuBarCCcoh    ): return "numubarCCcoh";    break;      
    case (kmNuMuBarCCELScat ): return "numubarCCELscat"; break;        
    case (kmNuMuBarCCoth    ): return "numubarCCoth";    break;    
//  case (kmNuMuBarNCEL     ): return ""; break;    
//  case (kmNuMuBarNCMEC    ): return ""; break;    
    case (kmNuMuBarNC1piC   ): return "numubarNC1piC"; break;   
    case (kmNuMuBarNC1pi0   ): return "numubarNC1pi0"; break;    
//  case (kmNuMuBarNC2piC   ): return ""; break;   
//  case (kmNuMuBarNC2pi0   ): return ""; break;    
//  case (kmNuMuBarNCpiCpi0 ): return ""; break;   
    case (kmNuMuBarNCcoh    ): return "numubarNCcoh";  break;  
    case (kmNuMuBarNCELScat ): return "numubarNCELscat"; break;         
    case (kmNuMuBarNCoth    ): return "numubarNCoth";  break;      

    case (kmNuECCQE         ): return "nueCCQE";     break;             
    case (kmNuECCMEC        ): return "nueCCMEC";    break;         
    case (kmNuECC1piC       ): return "nueCC1piC";   break; 
    case (kmNuECC1pi0       ): return "nueCC1pi0";   break;    
    case (kmNuECC2piC       ): return "nueCC2piC";   break;    
    case (kmNuECC2pi0       ): return "nueCC2pi0";   break;    
    case (kmNuECCpiCpi0     ): return "nueCCpiCpi0"; break;  
    case (kmNuECCcoh        ): return "nueCCcoh";    break;  
    case (kmNuECCoth        ): return "nueCCoth";    break;    
//  case (kmNuENCEL         ): return ""; break;    
//  case (kmNuENCMEC        ): return ""; break;    
    case (kmNuENC1piC       ): return "nueNC1piC"; break;    
    case (kmNuENC1pi0       ): return "nueNC1pi0"; break;    
//  case (kmNuENC2piC       ): return ""; break;   
//  case (kmNuENC2pi0       ): return ""; break;    
//  case (kmNuENCpiCpi0     ): return ""; break;   
    case (kmNuENCcoh        ): return "nueNCcoh";  break; 
    case (kmNuEELScat       ): return "nueELscat"; break;        
    case (kmNuENCoth        ): return "nueNCoth";  break;    

    case (kmNuEBarCCQE      ): return "nuebarCCQE";     break;    
    case (kmNuEBarCCMEC     ): return "nuebarCCMEC";    break;    
    case (kmNuEBarCC1piC    ): return "nuebarCC1piC";   break;   
    case (kmNuEBarCC1pi0    ): return "nuebarCC1pi0";   break;    
    case (kmNuEBarCC2piC    ): return "nuebarCC2piC";   break;   
    case (kmNuEBarCC2pi0    ): return "nuebarCC2pi0";   break;    
    case (kmNuEBarCCpiCpi0  ): return "nuebarCCpiCpi0"; break;    
    case (kmNuEBarCCcoh     ): return "nuebarCCcoh";    break;    
    case (kmNuEBarCCIMD     ): return "nuebarCCIMD";    break;
    case (kmNuEBarCCoth     ): return "nuebarCCoth";    break;    
//  case (kmNuEBarNCEL      ): return ""; break;   
//  case (kmNuEBarNCMEC     ): return ""; break;    
    case (kmNuEBarNC1piC    ): return "nuebarNC1piC"; break;   
    case (kmNuEBarNC1pi0    ): return "nuebarNC1pi0"; break;    
//  case (kmNuEBarNC2piC    ): return ""; break;   
//  case (kmNuEBarNC2pi0    ): return ""; break;    
//  case (kmNuEBarNCpiCpi0  ): return ""; break;  
    case (kmNuEBarNCcoh     ): return "nuebarNCcoh";  break;   
    case (kmNuEBarELScat    ): return "nuebarELscat"; break;        
    case (kmNuEBarNCoth     ): return "nuebarNCoth";  break;     

  case (kmNuMuToNuECCQE        ): return "numutonueCCQE";     break;
  case (kmNuMuToNuECCMEC       ): return "numutonueCCMEC";    break;        
  case (kmNuMuToNuECC1piC      ): return "numutonueCC1piC";   break;        
  case (kmNuMuToNuECC1pi0      ): return "numutonueCC1pi0";   break;        
  case (kmNuMuToNuECC2piC      ): return "numutonueCC2piC";   break;        
  case (kmNuMuToNuECC2pi0      ): return "numutonueCC2pi0";   break;        
  case (kmNuMuToNuECCpiCpi0    ): return "numutonueCCpiCpi0"; break;         
  case (kmNuMuToNuECCcoh       ): return "numutonueCCcoh";    break;       
  case (kmNuMuToNuEELScat      ): return "numutonueELscat";   break;  
  case (kmNuMuToNuECCoth       ): return "numutonueCCoth";    break;        

  case (kmNuMuToNuTauCCQE        ): return "numutonutauCCQE";     break;
  case (kmNuMuToNuTauCCMEC       ): return "numutonutauCCMEC";    break;        
  case (kmNuMuToNuTauCC1piC      ): return "numutonutauCC1piC";   break;        
  case (kmNuMuToNuTauCC1pi0      ): return "numutonutauCC1pi0";   break;        
  case (kmNuMuToNuTauCC2piC      ): return "numutonutauCC2piC";   break;        
  case (kmNuMuToNuTauCC2pi0      ): return "numutonutauCC2pi0";   break;        
  case (kmNuMuToNuTauCCpiCpi0    ): return "numutonutauCCpiCpi0"; break;         
  case (kmNuMuToNuTauCCcoh       ): return "numutonutauCCcoh";    break;       
  case (kmNuMuToNuTauCCELScat    ): return "numutonutauCCELscat"; break;  
  case (kmNuMuToNuTauCCoth       ): return "numutonutauCCoth";    break;        

  case (kmNuEToNuMuCCQE         ): return "nuetonumuCCQE";     break;             
  case (kmNuEToNuMuCCMEC        ): return "nuetonumuCCMEC";    break;         
  case (kmNuEToNuMuCC1piC       ): return "nuetonumuCC1piC";   break; 
  case (kmNuEToNuMuCC1pi0       ): return "nuetonumuCC1pi0";   break;    
  case (kmNuEToNuMuCC2piC       ): return "nuetonumuCC2piC";   break;    
  case (kmNuEToNuMuCC2pi0       ): return "nuetonumuCC2pi0";   break;    
  case (kmNuEToNuMuCCpiCpi0     ): return "nuetonumuCCpiCpi0"; break;  
  case (kmNuEToNuMuCCcoh        ): return "nuetonumuCCcoh";    break;  
  case (kmNuEToNuMuCCIMD        ): return "nuetonumuCCIMD";    break;
  case (kmNuEToNuMuCCELScat     ): return "nuetonumuCCELscat"; break;  
  case (kmNuEToNuMuCCoth        ): return "nuetonumuCCoth";    break;    

  case (kmNuEToNuTauCCQE         ): return "nuetonutauCCQE";     break;             
  case (kmNuEToNuTauCCMEC        ): return "nuetonutauCCMEC";    break;         
  case (kmNuEToNuTauCC1piC       ): return "nuetonutauCC1piC";   break; 
  case (kmNuEToNuTauCC1pi0       ): return "nuetonutauCC1pi0";   break;    
  case (kmNuEToNuTauCC2piC       ): return "nuetonutauCC2piC";   break;    
  case (kmNuEToNuTauCC2pi0       ): return "nuetonutauCC2pi0";   break;    
  case (kmNuEToNuTauCCpiCpi0     ): return "nuetonutauCCpiCpi0"; break;  
  case (kmNuEToNuTauCCcoh        ): return "nuetonutauCCcoh";    break;  
  case (kmNuEToNuTauCCELScat     ): return "nuetonutauCCELscat"; break;  
  case (kmNuEToNuTauCCoth        ): return "nuetonutauCCoth";    break;    

  case (kmNuMuBarToNuEBarCCQE        ): return "numubartonuebarCCQE";     break;
  case (kmNuMuBarToNuEBarCCMEC       ): return "numubartonuebarCCMEC";    break;        
  case (kmNuMuBarToNuEBarCC1piC      ): return "numubartonuebarCC1piC";   break;        
  case (kmNuMuBarToNuEBarCC1pi0      ): return "numubartonuebarCC1pi0";   break;        
  case (kmNuMuBarToNuEBarCC2piC      ): return "numubartonuebarCC2piC";   break;        
  case (kmNuMuBarToNuEBarCC2pi0      ): return "numubartonuebarCC2pi0";   break;        
  case (kmNuMuBarToNuEBarCCpiCpi0    ): return "numubartonuebarCCpiCpi0"; break;         
  case (kmNuMuBarToNuEBarCCcoh       ): return "numubartonuebarCCcoh";    break;       
  case (kmNuMuBarToNuEBarCCIMD       ): return "numubartonuebarCCIMD";    break;  
  case (kmNuMuBarToNuEBarELScat      ): return "numubartonuebarELscat";   break;  
  case (kmNuMuBarToNuEBarCCoth       ): return "numubartonuebarCCoth";    break;        

  case (kmNuMuBarToNuTauBarCCQE        ): return "numubartonutaubarCCQE";     break;
  case (kmNuMuBarToNuTauBarCCMEC       ): return "numubartonutaubarCCMEC";    break;        
  case (kmNuMuBarToNuTauBarCC1piC      ): return "numubartonutaubarCC1piC";   break;        
  case (kmNuMuBarToNuTauBarCC1pi0      ): return "numubartonutaubarCC1pi0";   break;        
  case (kmNuMuBarToNuTauBarCC2piC      ): return "numubartonutaubarCC2piC";   break;        
  case (kmNuMuBarToNuTauBarCC2pi0      ): return "numubartonutaubarCC2pi0";   break;        
  case (kmNuMuBarToNuTauBarCCpiCpi0    ): return "numubartonutaubarCCpiCpi0"; break;         
  case (kmNuMuBarToNuTauBarCCcoh       ): return "numubartonutaubarCCcoh";    break;       
  case (kmNuMuBarToNuTauBarCCELScat    ): return "numubartonutaubarCCELscat"; break;  
  case (kmNuMuBarToNuTauBarCCoth       ): return "numubartonutaubarCCoth";    break;        

  case (kmNuEBarToNuMuBarCCQE         ): return "nuebartonumubarCCQE";     break;             
  case (kmNuEBarToNuMuBarCCMEC        ): return "nuebartonumubarCCMEC";    break;         
  case (kmNuEBarToNuMuBarCC1piC       ): return "nuebartonumubarCC1piC";   break; 
  case (kmNuEBarToNuMuBarCC1pi0       ): return "nuebartonumubarCC1pi0";   break;    
  case (kmNuEBarToNuMuBarCC2piC       ): return "nuebartonumubarCC2piC";   break;    
  case (kmNuEBarToNuMuBarCC2pi0       ): return "nuebartonumubarCC2pi0";   break;    
  case (kmNuEBarToNuMuBarCCpiCpi0     ): return "nuebartonumubarCCpiCpi0"; break;  
  case (kmNuEBarToNuMuBarCCcoh        ): return "nuebartonumubarCCcoh";    break;  
  case (kmNuEBarToNuMuBarCCELScat     ): return "nuebartonumubarCCELscat"; break;  
  case (kmNuEBarToNuMuBarCCoth        ): return "nuebartonumubarCCoth";    break;    

  case (kmNuEBarToNuTauBarCCQE         ): return "nuebartonutaubarCCQE";     break;             
  case (kmNuEBarToNuTauBarCCMEC        ): return "nuebartonutaubarCCMEC";    break;         
  case (kmNuEBarToNuTauBarCC1piC       ): return "nuebartonutaubarCC1piC";   break; 
  case (kmNuEBarToNuTauBarCC1pi0       ): return "nuebartonutaubarCC1pi0";   break;    
  case (kmNuEBarToNuTauBarCC2piC       ): return "nuebartonutaubarCC2piC";   break;    
  case (kmNuEBarToNuTauBarCC2pi0       ): return "nuebartonutaubarCC2pi0";   break;    
  case (kmNuEBarToNuTauBarCCpiCpi0     ): return "nuebartonutaubarCCpiCpi0"; break;  
  case (kmNuEBarToNuTauBarCCcoh        ): return "nuebartonutaubarCCcoh";    break;  
  case (kmNuEBarToNuTauBarCCELScat     ): return "nuebartonutaubarCCELscat"; break;      
  case (kmNuEBarToNuTauBarCCoth        ): return "nuebartonutaubarCCoth";    break;

    case (kmOther           ): return "other";  break;     
    default                  : return "--"; 
  }
  return "";
}
//________________________________________________________________________________________
string util::AsStringTeX(ReacMode_t m, bool root)
{
  string name = "";

  switch(m) {

    case (kmNuMuCCQE        ): name = "#nu_{#mu} CCQE";                   break;
    case (kmNuMuCCMEC       ): name = "#nu_{#mu} CCMEC";                  break;        
    case (kmNuMuCC1piC      ): name = "#nu_{#mu} CC1#pi^{#pm}";           break;        
    case (kmNuMuCC1pi0      ): name = "#nu_{#mu} CC1#pi^{0}";             break;        
    case (kmNuMuCC2piC      ): name = "#nu_{#mu} CC2#pi^{#pm}";           break;        
    case (kmNuMuCC2pi0      ): name = "#nu_{#mu} CC2#pi^{0}";             break;        
    case (kmNuMuCCpiCpi0    ): name = "#nu_{#mu} CC#pi^{#pm}#pi^{0}";     break;         
    case (kmNuMuCCcoh       ): name = "#nu_{#mu} CCcoh";                  break;
    case (kmNuMuCCIMD       ): name = "#nu_{#mu} CC IMD";                 break;
    case (kmNuMuCCELScat    ): name = "#nu_{#mu} CC e^{-} elastic scat";  break;        
    case (kmNuMuCCoth       ): name = "#nu_{#mu} CCoth";                  break;        
//  case (kmNuMuNCEL        ): name = "";                                 break;        
//  case (kmNuMuNCMEC       ): name = "";                                 break;        
    case (kmNuMuNC1piC      ): name = "#nu_{#mu} NC1#pi^{#pm}";           break;        
    case (kmNuMuNC1pi0      ): name = "#nu_{#mu} NC1#pi^{0}";             break;        
//  case (kmNuMuNC2piC      ): name = "";                                 break;        
//  case (kmNuMuNC2pi0      ): name = "";                                 break;        
//  case (kmNuMuNCpiCpi0    ): name = "";                                 break;       
    case (kmNuMuNCcoh       ): name = "#nu_{#mu} NCcoh";                  break;        
    case (kmNuMuNCELScat    ): name = "#nu_{#mu} NC e^{-} elastic scat";  break;        
    case (kmNuMuNCoth       ): name = "#nu_{#mu} NCoth";                  break;        

    case (kmNuMuBarCCQE     ): name = "#bar{#nu}_{#mu} CCQE";             break;        
    case (kmNuMuBarCCMEC    ): name = "#bar{#nu}_{#mu} CCMEC";            break;        
    case (kmNuMuBarCC1piC   ): name = "#bar{#nu}_{#mu} CC1#pi^{#pm}";     break;       
    case (kmNuMuBarCC1pi0   ): name = "#bar{#nu}_{#mu} CC1#pi^{0}";       break;       
    case (kmNuMuBarCC2piC   ): name = "#bar{#nu}_{#mu} CC2#pi^{#pm}";     break;      
    case (kmNuMuBarCC2pi0   ): name = "#bar{#nu}_{#mu} CC2#pi^{0}";       break;       
    case (kmNuMuBarCCpiCpi0 ): name = "#bar{#nu}_{#mu} CC#pi^{#pm}#pi0";  break;   
    case (kmNuMuBarCCcoh    ): name = "#bar{#nu}_{#mu} CCcoh";            break;      
    case (kmNuMuBarCCELScat ): name = "#bar{#nu}_{#mu} CC e^{-} elastic scat";  break;        
    case (kmNuMuBarCCoth    ): name = "#bar{#nu}_{#mu} CCoth";            break;    
//  case (kmNuMuBarNCEL     ): name = "";                                 break;    
//  case (kmNuMuBarNCMEC    ): name = "";                                 break;    
    case (kmNuMuBarNC1piC   ): name = "#bar{#nu}_{#mu} NC1#pi^{#pm}";     break;   
    case (kmNuMuBarNC1pi0   ): name = "#bar{#nu}_{#mu} NC1#pi^{0}";       break;    
//  case (kmNuMuBarNC2piC   ): name = "";                                 break;   
//  case (kmNuMuBarNC2pi0   ): name = "";                                 break;    
//  case (kmNuMuBarNCpiCpi0 ): name = "";                                 break;   
    case (kmNuMuBarNCcoh    ): name = "#bar{#nu}_{#mu} NCcoh";            break;  
    case (kmNuMuBarNCELScat ): name = "#bar{#nu}_{#mu} NC e^{-} elastic scat";  break;        
    case (kmNuMuBarNCoth    ): name = "#bar{#nu}_{#mu} NCoth";            break;       

    case (kmNuECCQE         ): name = "#nu_{e} CCQE";                     break;             
    case (kmNuECCMEC        ): name = "#nu_{e} CCMEC";                    break;         
    case (kmNuECC1piC       ): name = "#nu_{e} CC1#pi^{#pm}";             break; 
    case (kmNuECC1pi0       ): name = "#nu_{e} CC1#pi^{0}";               break;    
    case (kmNuECC2piC       ): name = "#nu_{e} CC2#pi^{#pm}";             break;    
    case (kmNuECC2pi0       ): name = "#nu_{e} CC2#pi^{0}";               break;    
    case (kmNuECCpiCpi0     ): name = "#nu_{e} CC#pi^{#pm}#pi^{0}";       break;  
    case (kmNuECCcoh        ): name = "#nu_{e} CCcoh";                    break;  
    case (kmNuECCoth        ): name = "#nu_{e} CCoth";                    break;    
//  case (kmNuENCEL         ): name = "";                                 break;    
//  case (kmNuENCMEC        ): name = "";                                 break;    
    case (kmNuENC1piC       ): name = "#nu_{e} NC1#pi^{#pm}";             break;    
    case (kmNuENC1pi0       ): name = "#nu_{e} NC1#pi^{0}";               break;    
//  case (kmNuENC2piC       ): name = "";                                 break;   
//  case (kmNuENC2pi0       ): name = "";                                 break;    
//  case (kmNuENCpiCpi0     ): name = "";                                 break;   
    case (kmNuENCcoh        ): name = "#nu_{e} NCcoh";                    break; 
    case (kmNuEELScat       ): name = "#nu_{e} e^{-} elastic scat";       break;          
    case (kmNuENCoth        ): name = "#nu_{e} NCoth";                    break;    

    case (kmNuEBarCCQE      ): name = "#bar{#nu}_{e} CCQE";               break;    
    case (kmNuEBarCCMEC     ): name = "#bar{#nu}_{e} CCMEC";              break;    
    case (kmNuEBarCC1piC    ): name = "#bar{#nu}_{e} CC1#pi^{#pm}";       break;   
    case (kmNuEBarCC1pi0    ): name = "#bar{#nu}_{e} CC1#pi^{0}";         break;    
    case (kmNuEBarCC2piC    ): name = "#bar{#nu}_{e} CC2#pi^{#pm}";       break;   
    case (kmNuEBarCC2pi0    ): name = "#bar{#nu}_{e} CC2#pi^{0}";         break;    
    case (kmNuEBarCCpiCpi0  ): name = "#bar{#nu}_{e} CC#pi^{#pm}#pi^{0}"; break;    
    case (kmNuEBarCCcoh     ): name = "#bar{#nu}_{e} CCcoh";              break;    
    case (kmNuEBarCCIMD     ): name = "#bar{#nu}_{e} CC IMD";             break;  
    case (kmNuEBarCCoth     ): name = "#bar{#nu}_{e} CCoth";              break;    
//  case (kmNuEBarNCEL      ): name = "";                                 break;   
//  case (kmNuEBarNCMEC     ): name = "";                                 break;    
    case (kmNuEBarNC1piC    ): name = "#bar{#nu}_{e} NC1#pi^{#pm}";       break;   
    case (kmNuEBarNC1pi0    ): name = "#bar{#nu}_{e} NC1#pi^{0}";         break;    
//  case (kmNuEBarNC2piC    ): name = "";                                 break;   
//  case (kmNuEBarNC2pi0    ): name = "";                                 break;    
//  case (kmNuEBarNCpiCpi0  ): name = "";                                 break;  
    case (kmNuEBarNCcoh     ): name = "#bar{#nu}_{e} NCcoh";              break;   
    case (kmNuEBarELScat    ): name = "#bar{#nu}_{e} e^{-} elastic scat"; break;          
    case (kmNuEBarNCoth     ): name = "#bar{#nu}_{e} NCoth";              break;     

    case (kmNuMuToNuECCQE        ): name = "#nu_{#mu} #rightarrow #nu_{e} CCQE";                   break;
    case (kmNuMuToNuECCMEC       ): name = "#nu_{#mu} #rightarrow #nu_{e} CCMEC";                  break;        
    case (kmNuMuToNuECC1piC      ): name = "#nu_{#mu} #rightarrow #nu_{e} CC1#pi^{#pm}";           break;        
    case (kmNuMuToNuECC1pi0      ): name = "#nu_{#mu} #rightarrow #nu_{e} CC1#pi^{0}";             break;        
    case (kmNuMuToNuECC2piC      ): name = "#nu_{#mu} #rightarrow #nu_{e} CC2#pi^{#pm}";           break;        
    case (kmNuMuToNuECC2pi0      ): name = "#nu_{#mu} #rightarrow #nu_{e} CC2#pi^{0}";             break;        
    case (kmNuMuToNuECCpiCpi0    ): name = "#nu_{#mu} #rightarrow #nu_{e} CC#pi^{#pm}#pi^{0}";     break;         
    case (kmNuMuToNuECCcoh       ): name = "#nu_{#mu} #rightarrow #nu_{e} CCcoh";                  break;       
    case (kmNuMuToNuEELScat      ): name = "#nu_{#mu} #rightarrow #nu_{e} e^{-} elastic scat";     break;        
    case (kmNuMuToNuECCoth       ): name = "#nu_{#mu} #rightarrow #nu_{e} CCoth";                  break;        

    case (kmNuMuToNuTauCCQE        ): name = "#nu_{#mu} #rightarrow #nu_{#tau} CCQE";                   break;
    case (kmNuMuToNuTauCCMEC       ): name = "#nu_{#mu} #rightarrow #nu_{#tau} CCMEC";                  break;        
    case (kmNuMuToNuTauCC1piC      ): name = "#nu_{#mu} #rightarrow #nu_{#tau} CC1#pi^{#pm}";           break;        
    case (kmNuMuToNuTauCC1pi0      ): name = "#nu_{#mu} #rightarrow #nu_{#tau} CC1#pi^{0}";             break;        
    case (kmNuMuToNuTauCC2piC      ): name = "#nu_{#mu} #rightarrow #nu_{#tau} CC2#pi^{#pm}";           break;        
    case (kmNuMuToNuTauCC2pi0      ): name = "#nu_{#mu} #rightarrow #nu_{#tau} CC2#pi^{0}";             break;        
    case (kmNuMuToNuTauCCpiCpi0    ): name = "#nu_{#mu} #rightarrow #nu_{#tau} CC#pi^{#pm}#pi^{0}";     break;         
    case (kmNuMuToNuTauCCcoh       ): name = "#nu_{#mu} #rightarrow #nu_{#tau} CCcoh";                  break;       
    case (kmNuMuToNuTauCCELScat    ): name = "#nu_{#mu} #rightarrow #nu_{#tau} CC e^{-} elastic scat";  break;        
    case (kmNuMuToNuTauCCoth       ): name = "#nu_{#mu} #rightarrow #nu_{#tau} CCoth";                  break;   

    case (kmNuEToNuMuCCQE        ): name = "#nu_{e} #rightarrow #nu_{#mu} CCQE";                   break;
    case (kmNuEToNuMuCCMEC       ): name = "#nu_{e} #rightarrow #nu_{#mu} CCMEC";                  break;        
    case (kmNuEToNuMuCC1piC      ): name = "#nu_{e} #rightarrow #nu_{#mu} CC1#pi^{#pm}";           break;        
    case (kmNuEToNuMuCC1pi0      ): name = "#nu_{e} #rightarrow #nu_{#mu} CC1#pi^{0}";             break;        
    case (kmNuEToNuMuCC2piC      ): name = "#nu_{e} #rightarrow #nu_{#mu} CC2#pi^{#pm}";           break;        
    case (kmNuEToNuMuCC2pi0      ): name = "#nu_{e} #rightarrow #nu_{#mu} CC2#pi^{0}";             break;        
    case (kmNuEToNuMuCCpiCpi0    ): name = "#nu_{e} #rightarrow #nu_{#mu} CC#pi^{#pm}#pi^{0}";     break;         
    case (kmNuEToNuMuCCcoh       ): name = "#nu_{e} #rightarrow #nu_{#mu} CCcoh";                  break;       
    case (kmNuEToNuMuCCIMD       ): name = "#nu_{e} #rightarrow #nu_{#mu} CC IMD";                 break;  
    case (kmNuEToNuMuCCELScat    ): name = "#nu_{e} #rightarrow #nu_{#mu} CC e^{-} elastic scat";  break;        
    case (kmNuEToNuMuCCoth       ): name = "#nu_{e} #rightarrow #nu_{#mu} CCoth";                  break;             

    case (kmNuEToNuTauCCQE        ): name = "#nu_{e} #rightarrow #nu_{#tau} CCQE";                   break;
    case (kmNuEToNuTauCCMEC       ): name = "#nu_{e} #rightarrow #nu_{#tau} CCMEC";                  break;        
    case (kmNuEToNuTauCC1piC      ): name = "#nu_{e} #rightarrow #nu_{#tau} CC1#pi^{#pm}";           break;        
    case (kmNuEToNuTauCC1pi0      ): name = "#nu_{e} #rightarrow #nu_{#tau} CC1#pi^{0}";             break;        
    case (kmNuEToNuTauCC2piC      ): name = "#nu_{e} #rightarrow #nu_{#tau} CC2#pi^{#pm}";           break;        
    case (kmNuEToNuTauCC2pi0      ): name = "#nu_{e} #rightarrow #nu_{#tau} CC2#pi^{0}";             break;        
    case (kmNuEToNuTauCCpiCpi0    ): name = "#nu_{e} #rightarrow #nu_{#tau} CC#pi^{#pm}#pi^{0}";     break;         
    case (kmNuEToNuTauCCcoh       ): name = "#nu_{e} #rightarrow #nu_{#tau} CCcoh";                  break;       
    case (kmNuEToNuTauCCELScat    ): name = "#nu_{e} #rightarrow #nu_{#tau} CC e^{-} elastic scat";  break;        
    case (kmNuEToNuTauCCoth       ): name = "#nu_{e} #rightarrow #nu_{#tau} CCoth";                  break;             

    case (kmNuMuBarToNuEBarCCQE        ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e} CCQE";                   break;
    case (kmNuMuBarToNuEBarCCMEC       ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e} CCMEC";                  break;        
    case (kmNuMuBarToNuEBarCC1piC      ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e} CC1#pi^{#pm}";           break;        
    case (kmNuMuBarToNuEBarCC1pi0      ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e} CC1#pi^{0}";             break;        
    case (kmNuMuBarToNuEBarCC2piC      ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e} CC2#pi^{#pm}";           break;        
    case (kmNuMuBarToNuEBarCC2pi0      ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e} CC2#pi^{0}";             break;        
    case (kmNuMuBarToNuEBarCCpiCpi0    ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e} CC#pi^{#pm}#pi^{0}";     break;         
    case (kmNuMuBarToNuEBarCCcoh       ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e} CCcoh";                  break;       
    case (kmNuMuBarToNuEBarCCIMD       ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e} CC IMD";                 break;  
    case (kmNuMuBarToNuEBarELScat      ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e} e^{-} elastic scat";     break;        
    case (kmNuMuBarToNuEBarCCoth       ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e} CCoth";                  break;        

    case (kmNuMuBarToNuTauBarCCQE        ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#tau} CCQE";                   break;
    case (kmNuMuBarToNuTauBarCCMEC       ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#tau} CCMEC";                  break;   
    case (kmNuMuBarToNuTauBarCC1piC      ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#tau} CC1#pi^{#pm}";           break;    
    case (kmNuMuBarToNuTauBarCC1pi0      ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#tau} CC1#pi^{0}";             break;    
    case (kmNuMuBarToNuTauBarCC2piC      ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#tau} CC2#pi^{#pm}";           break;    
    case (kmNuMuBarToNuTauBarCC2pi0      ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#tau} CC2#pi^{0}";             break;    
    case (kmNuMuBarToNuTauBarCCpiCpi0    ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#tau} CC#pi^{#pm}#pi^{0}";     break;    
    case (kmNuMuBarToNuTauBarCCcoh       ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#tau} CCcoh";                  break;    
    case (kmNuMuBarToNuTauBarCCELScat    ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#tau} CC e^{-} elastic scat";  break;  
    case (kmNuMuBarToNuTauBarCCoth       ): name = "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#tau} CCoth";                  break;   

    case (kmNuEBarToNuMuBarCCQE        ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#mu} CCQE";                   break;
    case (kmNuEBarToNuMuBarCCMEC       ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#mu} CCMEC";                  break;        
    case (kmNuEBarToNuMuBarCC1piC      ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#mu} CC1#pi^{#pm}";           break;        
    case (kmNuEBarToNuMuBarCC1pi0      ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#mu} CC1#pi^{0}";             break;        
    case (kmNuEBarToNuMuBarCC2piC      ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#mu} CC2#pi^{#pm}";           break;        
    case (kmNuEBarToNuMuBarCC2pi0      ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#mu} CC2#pi^{0}";             break;        
    case (kmNuEBarToNuMuBarCCpiCpi0    ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#mu} CC#pi^{#pm}#pi^{0}";     break;         
    case (kmNuEBarToNuMuBarCCcoh       ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#mu} CCcoh";                  break;       
    case (kmNuEBarToNuMuBarCCELScat    ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#mu} CC e^{-} elastic scat";  break;        
    case (kmNuEBarToNuMuBarCCoth       ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#mu} CCoth";                  break;        

    case (kmNuEBarToNuTauBarCCQE        ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#tau} CCQE";                   break;
    case (kmNuEBarToNuTauBarCCMEC       ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#tau} CCMEC";                  break;        
    case (kmNuEBarToNuTauBarCC1piC      ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#tau} CC1#pi^{#pm}";           break;        
    case (kmNuEBarToNuTauBarCC1pi0      ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#tau} CC1#pi^{0}";             break;        
    case (kmNuEBarToNuTauBarCC2piC      ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#tau} CC2#pi^{#pm}";           break;        
    case (kmNuEBarToNuTauBarCC2pi0      ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#tau} CC2#pi^{0}";             break;        
    case (kmNuEBarToNuTauBarCCpiCpi0    ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#tau} CC#pi^{#pm}#pi^{0}";     break;    
    case (kmNuEBarToNuTauBarCCcoh       ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#tau} CCcoh";                  break;       
    case (kmNuEBarToNuTauBarCCELScat    ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#tau} CC e^{-} elastic scat";  break;        
    case (kmNuEBarToNuTauBarCCoth       ): name = "#bar{#nu}_{e} #rightarrow #bar{#nu}_{#tau} CCoth";                  break; 
 
    case (kmOther           ): name = "Other";                            break;     
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
    case (kmtCCQE     ) : return "CCQE";
    case (kmtCCMEC    ) : return "CCMEC";
    case (kmtCC1piC   ) : return "CC1piC";
    case (kmtCC1pi0   ) : return "CC1pi0";
    case (kmtCC2piC   ) : return "CC2piC";
    case (kmtCC2pi0   ) : return "CC2pi0";
    case (kmtCCpiCpi0 ) : return "CC1pi01piC";
    case (kmtCCcoh    ) : return "CCcoherent";
    case (kmtCCIMD    ) : return "CCIMD";
    case (kmtCCELScat ) : return "CCnuEEl";
    case (kmtCCoth    ) : return "CCother";
    case (kmtNCEL     ) : return "NCEL";
    case (kmtNCMEC    ) : return "NCMEC";
    case (kmtNC1piC   ) : return "NC1piC";
    case (kmtNC1pi0   ) : return "NC1pi0";
    case (kmtNC2piC   ) : return "NC2piC";
    case (kmtNC2pi0   ) : return "NC2pi0";
    case (kmtNCpiCpi0 ) : return "NC1pi01piC";
    case (kmtNCcoh    ) : return "NCcoherent";
    case (kmtNCELScat ) : return "NCnuEEl";
    case (kmtNCoth    ) : return "NCother";
    case (kmtELScat   ) : return "nuEEl";
    case (kmtOther    ) : return "misc";
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
bool util::IsCC(conv::ReacMode_t m)
{
  switch (m) {
    case (kmNuMuCCQE       )  : return true;
    case (kmNuMuCCMEC      )  : return true;
    case (kmNuMuCC1piC     )  : return true;
    case (kmNuMuCC1pi0     )  : return true;
    case (kmNuMuCC2piC     )  : return true;
    case (kmNuMuCC2pi0     )  : return true;
    case (kmNuMuCCpiCpi0   )  : return true;
    case (kmNuMuCCcoh      )  : return true;
    case (kmNuMuCCIMD      )  : return true;
    case (kmNuMuCCELScat   )  : return true;    
    case (kmNuMuCCoth      )  : return true;
    case (kmNuMuBarCCQE    )  : return true;
    case (kmNuMuBarCCMEC   )  : return true;
    case (kmNuMuBarCC1piC  )  : return true;
    case (kmNuMuBarCC1pi0  )  : return true;
    case (kmNuMuBarCC2piC  )  : return true;
    case (kmNuMuBarCC2pi0  )  : return true;
    case (kmNuMuBarCCpiCpi0)  : return true;
    case (kmNuMuBarCCcoh   )  : return true;
    case (kmNuMuBarCCELScat)  : return true;
    case (kmNuMuBarCCoth   )  : return true;
    case (kmNuECCQE        )  : return true;
    case (kmNuECCMEC       )  : return true;
    case (kmNuECC1piC      )  : return true;
    case (kmNuECC1pi0      )  : return true;
    case (kmNuECC2piC      )  : return true;
    case (kmNuECC2pi0      )  : return true;
    case (kmNuECCpiCpi0    )  : return true;
    case (kmNuECCcoh       )  : return true;
    case (kmNuEELScat      )  : return true; // CAVEAT: this one is also added as it is CC+NC
    case (kmNuECCoth       )  : return true;
    case (kmNuEBarCCQE     )  : return true;
    case (kmNuEBarCCMEC    )  : return true;
    case (kmNuEBarCC1piC   )  : return true;
    case (kmNuEBarCC1pi0   )  : return true;
    case (kmNuEBarCC2piC   )  : return true;
    case (kmNuEBarCC2pi0   )  : return true;
    case (kmNuEBarCCpiCpi0 )  : return true;
    case (kmNuEBarCCcoh    )  : return true;
    case (kmNuEBarCCIMD    )  : return true;  
    case (kmNuEBarELScat   )  : return true; // CAVEAT: this one is also added as it is CC+NC
    case (kmNuEBarCCoth    )  : return true;
    case (kmNuMuToNuECCQE       )  : return true;
    case (kmNuMuToNuECCMEC      )  : return true;
    case (kmNuMuToNuECC1piC     )  : return true;
    case (kmNuMuToNuECC1pi0     )  : return true;
    case (kmNuMuToNuECC2piC     )  : return true;
    case (kmNuMuToNuECC2pi0     )  : return true;
    case (kmNuMuToNuECCpiCpi0   )  : return true;
    case (kmNuMuToNuECCcoh      )  : return true;
    case (kmNuMuToNuEELScat     )  : return true; // CAVEAT: this one is also added as it is CC+NC
    case (kmNuMuToNuECCoth      )  : return true;
    case (kmNuMuToNuTauCCQE       )  : return true;
    case (kmNuMuToNuTauCCMEC      )  : return true;
    case (kmNuMuToNuTauCC1piC     )  : return true;
    case (kmNuMuToNuTauCC1pi0     )  : return true;
    case (kmNuMuToNuTauCC2piC     )  : return true;
    case (kmNuMuToNuTauCC2pi0     )  : return true;
    case (kmNuMuToNuTauCCpiCpi0   )  : return true;
    case (kmNuMuToNuTauCCcoh      )  : return true;
    case (kmNuMuToNuTauCCELScat   )  : return true;
    case (kmNuMuToNuTauCCoth      )  : return true;
    case (kmNuEToNuMuCCQE       )  : return true;
    case (kmNuEToNuMuCCMEC      )  : return true;
    case (kmNuEToNuMuCC1piC     )  : return true;
    case (kmNuEToNuMuCC1pi0     )  : return true;
    case (kmNuEToNuMuCC2piC     )  : return true;
    case (kmNuEToNuMuCC2pi0     )  : return true;
    case (kmNuEToNuMuCCpiCpi0   )  : return true;
    case (kmNuEToNuMuCCcoh      )  : return true;
    case (kmNuEToNuMuCCIMD      )  : return true;  
    case (kmNuEToNuMuCCELScat   )  : return true;
    case (kmNuEToNuMuCCoth      )  : return true;
    case (kmNuEToNuTauCCQE       )  : return true;
    case (kmNuEToNuTauCCMEC      )  : return true;
    case (kmNuEToNuTauCC1piC     )  : return true;
    case (kmNuEToNuTauCC1pi0     )  : return true;
    case (kmNuEToNuTauCC2piC     )  : return true;
    case (kmNuEToNuTauCC2pi0     )  : return true;
    case (kmNuEToNuTauCCpiCpi0   )  : return true;
    case (kmNuEToNuTauCCcoh      )  : return true;
    case (kmNuEToNuTauCCELScat   )  : return true;
    case (kmNuEToNuTauCCoth      )  : return true;
    case (kmNuMuBarToNuEBarCCQE       )  : return true;
    case (kmNuMuBarToNuEBarCCMEC      )  : return true;
    case (kmNuMuBarToNuEBarCC1piC     )  : return true;
    case (kmNuMuBarToNuEBarCC1pi0     )  : return true;
    case (kmNuMuBarToNuEBarCC2piC     )  : return true;
    case (kmNuMuBarToNuEBarCC2pi0     )  : return true;
    case (kmNuMuBarToNuEBarCCpiCpi0   )  : return true;
    case (kmNuMuBarToNuEBarCCcoh      )  : return true;
    case (kmNuMuBarToNuEBarCCIMD      )  : return true;
    case (kmNuMuBarToNuEBarELScat     )  : return true; // CAVEAT: this one is also added as it is CC+NC
    case (kmNuMuBarToNuEBarCCoth      )  : return true;
    case (kmNuMuBarToNuTauBarCCQE       )  : return true;
    case (kmNuMuBarToNuTauBarCCMEC      )  : return true;
    case (kmNuMuBarToNuTauBarCC1piC     )  : return true;
    case (kmNuMuBarToNuTauBarCC1pi0     )  : return true;
    case (kmNuMuBarToNuTauBarCC2piC     )  : return true;
    case (kmNuMuBarToNuTauBarCC2pi0     )  : return true;
    case (kmNuMuBarToNuTauBarCCpiCpi0   )  : return true;
    case (kmNuMuBarToNuTauBarCCcoh      )  : return true;
    case (kmNuMuBarToNuTauBarCCELScat   )  : return true;
    case (kmNuMuBarToNuTauBarCCoth      )  : return true;
    case (kmNuEBarToNuMuBarCCQE       )  : return true;
    case (kmNuEBarToNuMuBarCCMEC      )  : return true;
    case (kmNuEBarToNuMuBarCC1piC     )  : return true;
    case (kmNuEBarToNuMuBarCC1pi0     )  : return true;
    case (kmNuEBarToNuMuBarCC2piC     )  : return true;
    case (kmNuEBarToNuMuBarCC2pi0     )  : return true;
    case (kmNuEBarToNuMuBarCCpiCpi0   )  : return true;
    case (kmNuEBarToNuMuBarCCcoh      )  : return true;
    case (kmNuEBarToNuMuBarCCELScat   )  : return true;
    case (kmNuEBarToNuMuBarCCoth      )  : return true;
    case (kmNuEBarToNuTauBarCCQE       )  : return true;
    case (kmNuEBarToNuTauBarCCMEC      )  : return true;
    case (kmNuEBarToNuTauBarCC1piC     )  : return true;
    case (kmNuEBarToNuTauBarCC1pi0     )  : return true;
    case (kmNuEBarToNuTauBarCC2piC     )  : return true;
    case (kmNuEBarToNuTauBarCC2pi0     )  : return true;
    case (kmNuEBarToNuTauBarCCpiCpi0   )  : return true;
    case (kmNuEBarToNuTauBarCCcoh      )  : return true;
    case (kmNuEBarToNuTauBarCCELScat   )  : return true;
    case (kmNuEBarToNuTauBarCCoth      )  : return true;
    default                   : return false;
  }
}

//________________________________________________________________________________________
ReacMode_t util::ReacModeFromInt(int m)
{
// Cast an integer to ReacMode_t, but with boundary check
//             [first]                  [last]
  if(m >= (int)kmNuMuCCQE && m <= (int) kmOther) return (ReacMode_t)m;
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
  switch (m) {
    case kmNuECCQE        : return kmNuMuToNuECCQE           ;
    case kmNuECCMEC       : return kmNuMuToNuECCMEC          ;
    case kmNuECC1piC      : return kmNuMuToNuECC1piC         ;
    case kmNuECC1pi0      : return kmNuMuToNuECC1pi0         ;
    case kmNuECC2piC      : return kmNuMuToNuECC2piC         ;
    case kmNuECC2pi0      : return kmNuMuToNuECC2pi0         ;
    case kmNuECCpiCpi0    : return kmNuMuToNuECCpiCpi0       ;
    case kmNuECCcoh       : return kmNuMuToNuECCcoh          ;
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
    case kmNuEBarCCoth    : return kmNuMuBarToNuEBarCCoth    ;
    case kmNuEBarELScat   : return kmNuMuBarToNuEBarELScat   ;
    
    default: return kmUndefined;
  }
}
//________________________________________________________________________________________
conv::ReacMode_t util::ReacTypeToMode (conv::ReacType_t t, conv::NuFlavour_t nu_init, conv::NuFlavour_t nu_final)
{
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
  return conv::kmUndefined;
}
//________________________________________________________________________________________
conv::ReacMode_t util::ReacModeFromString (std::string mode)
{
  string lc_mode = util::ToLowerCase(mode);
  for (int i = 0 ; i < kmOther ; i++ ) {
    ReacMode_t m = (ReacMode_t)i;
    string this_mode = util::ToLowerCase(util::AsString(m));
    if(lc_mode==this_mode && this_mode!="--") {
      return m;
    }
  }

  return kmUndefined;
}
