//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include "valor/Utilities/KinematicBin.h"

using namespace valor;
using namespace valor::util;

//________________________________________________________________________________________
KinematicBin::KinematicBin(void)
 :
 true1 (0),
 true2 (0),
 true3 (0),
 reco1 (0),
 reco2 (0),
 reco3 (0),
 zone  (0)
{
  
}
//________________________________________________________________________________________
std::ostream& valor::util::operator<<(std::ostream& s,const KinematicBin & b)
{
  s<<"True("<<b.true1<<","<<b.true2<<","<<b.true3<<")\tReco("<<b.reco1<<","<<b.reco2<<","<<b.reco3<<")\tZone"<<b.zone<<std::endl;
  return s;
}
//________________________________________________________________________________________
