//____________________________________________________________________________
/*!

\class     valor::utils

\brief     Two flavour oscillation calculations

\author    Steve Dennis <stephen.dennis \t warwick.ac.uk
           University of Warwick & STFC Rutherford Appleton Laboratory
          
\created   Jan 2017

\copyright (c) 2010-2017, The VALOR group

*/
//____________________________________________________________________________

#ifndef valor_osc_simple_h
#define valor_osc_simple_h

namespace valor {

namespace util {
  
  double Calculate2FSterileOsc(int init_pdg, int final_pdg, double lOverE, double sinsq2thee, double sinsqthmumu, double dmsq);
  
  double FlipOctantAroundMaxDisap(double sinsqth23, double sinsqth13);
  
} // util
} // valor

#endif
