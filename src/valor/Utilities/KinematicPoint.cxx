//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//________________________________________________________________________________________

#include "valor/Utilities/KinematicPoint.h"

using namespace valor;
using namespace valor::util;

//________________________________________________________________________________________
KinematicPoint::KinematicPoint(void)
 :
 true1 (0.),
 true2 (0.),
 true3 (0.),
 reco1 (0.),
 reco2 (0.),
 reco3 (0.)
{
  
}
//________________________________________________________________________________________
std::ostream& valor::util::operator<<(std::ostream& s,const KinematicPoint & b)
{
  s<<"True("<<b.true1<<","<<b.true2<<","<<b.true3<<")\tReco("<<b.reco1<<","<<b.reco2<<","<<b.reco3<<")"<<std::endl;
  return s;
}
//________________________________________________________________________________________
