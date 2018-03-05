//____________________________________________________________________________
/*!

\class     valor::VALORArrayBin

\brief     Simple header format for writing VALORArrays as blocks.
           No dependence on stored data type - so it should be common 
           to all VALORArray types.

\author    Steve Dennis <s.r.dennis@liverpool.ac.uk>
           University of Liverpool

\created   April 2016

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_array_bin_h_
#define _valor_array_bin_h_

#include <cstddef>
#include <iostream>

#include "valor/Conventions/Build.h"

namespace valor { 

struct VALORArrayBin
{
public:
  VALORArrayBin();
  VALORArrayBin(int b1 ,int b2, int b3, int b4, int b5, int b6);
  VALORArrayBin(const int * bin_numbers);
  
  int bin1;
  int bin2;
  int bin3;
  int bin4;
  int bin5;
  int bin6;
  
  friend std::ostream & operator << (std::ostream & stream, const VALORArrayBin & rfl);
  
}; // VALORArrayBin

bool operator==(const VALORArrayBin & lhs, const VALORArrayBin & rhs);
std::ostream & operator << (std::ostream & stream, const VALORArrayBin & rfl);

} //valor

#endif
