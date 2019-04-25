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
  
  double Calculate2FSterileOsc(int init_pdg, int final_pdg, double lOverE, double sinsq2thee, double sinsq2thmumu, double dmsq);
  double Calculate2FSterileOscMuE(int init_pdg, int final_pdg, double lOverE, double sinsq2thmue, double sinsqthmumu, double dmsq);
  
  double FlipOctantAroundMaxDisap(double sinsqth23, double sinsqth13);
  
} // util
} // valor

#endif
