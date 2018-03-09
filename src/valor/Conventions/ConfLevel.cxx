//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include "valor/Conventions/ConfLevel.h"

using namespace valor;
using namespace valor::conv;

//____________________________________________________________________________
double valor::util::DeltaChisquare(
        ConfLevel_t cl, int nfitparams)
{
  switch(cl) {
    case (kCLUndefined) :
    {
      return -99999;
      break;
    }
    case (kCL68) :
    {
      if      (nfitparams == 1)  { return  1.00;  }
      else if (nfitparams == 2)  { return  2.30;  }
      else if (nfitparams == 3)  { return  3.53;  }
      else                       { return -99999; }
      break;
    }
    case (kCL90) :
    {
      if      (nfitparams == 1)  { return  2.71;  }
      else if (nfitparams == 2)  { return  4.61;  }
      else if (nfitparams == 3)  { return  6.25;  }
      else                       { return -99999; }
      break;
    }
    case (kCL95) :
    {
      if      (nfitparams == 1)  { return  3.84;  }
      else if (nfitparams == 2)  { return  5.99;  }
      else if (nfitparams == 3)  { return  7.82;  }
      else                       { return -99999; }
      break;
    }
    case (kCL99) :
    {
      if      (nfitparams == 1)  { return  6.63;  }
      else if (nfitparams == 2)  { return  9.21;  }
      else if (nfitparams == 3)  { return 11.34;  }
      else                       { return -99999; }
      break;
    }
    default:
      return -99999;
  }
  return -99999;
}
//____________________________________________________________________________
