//____________________________________________________________________________
/*!

\class     valor::KinematicBinId

\brief     Simple struct for a kinematic bin.

\author    Steve Dennis <s.r.dennis \at University of Liverpool>
           University of Liverpool

\created   April 11, 2016

\copyright (c) 2010-2016, The VALOR group.

*/
//____________________________________________________________________________

#ifndef __valor_dune_kinematic_bin__
#define __valor_dune_kinematic_bin__

#include <iostream>

#include "dune/Conventions/DuneBuild.h"

namespace valor {
namespace util {

struct KinematicBin {

public:
  KinematicBin(void);
  
  int true1;
  int true2;
  int true3;
  int reco1;
  int reco2;
  int reco3;
  int zone ;

  friend std::ostream & operator << (std::ostream & stream, const KinematicBin & rfl);
}; //KinematicBin

std::ostream & operator << (std::ostream & stream, const KinematicBin & rfl);

}      // Namespace util
}      // valor namespace

#endif 
