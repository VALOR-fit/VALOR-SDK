//____________________________________________________________________________
/*!

\class     valor::KinematicPoint

\brief     Simple struct for a point in kinematic space.

\author    Steve Dennis <s.r.dennis \at University of Liverpool>
           University of Liverpool

\created   April 20, 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef __valor_kinematic_point__
#define __valor_kinematic_point__

#include <iostream>

namespace valor {
namespace util {

struct KinematicPoint {

public:
  KinematicPoint(void);
  
  double true1;
  double true2;
  double true3;
  double reco1;
  double reco2;
  double reco3;
  
  friend std::ostream & operator << (std::ostream & stream, const KinematicPoint & rfl);

}; //KinematicPoint

std::ostream & operator << (std::ostream & stream, const KinematicPoint & rfl);

}      // Namespace util
}      // valor namespace

#endif 
