//____________________________________________________________________________ 
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/ 
//____________________________________________________________________________

#include <cassert>
#include <cstring>
#include "valor/VArray/VALORArrayBin.h"

using namespace valor;

//____________________________________________________________________________
VALORArrayBin::VALORArrayBin()
 :
 bin1 (-1),
 bin2 (-1),
 bin3 (-1),
 bin4 (-1),
 bin5 (-1),
 bin6 (-1)
{
  return;
}
//____________________________________________________________________________
VALORArrayBin::VALORArrayBin(int b1, int b2, int b3, int b4, int b5, int b6)
 :
 bin1 (b1),
 bin2 (b2),
 bin3 (b3),
 bin4 (b4),
 bin5 (b5),
 bin6 (b6)
{
  return;
}
//____________________________________________________________________________
VALORArrayBin::VALORArrayBin(const int * bin_numbers)
 :
 bin1 (bin_numbers[0]),
 bin2 (bin_numbers[1]),
 bin3 (bin_numbers[2]),
 bin4 (bin_numbers[3]),
 bin5 (bin_numbers[4]),
 bin6 (bin_numbers[5])
{
  return;
}
//____________________________________________________________________________
namespace valor {
  bool operator==(const VALORArrayBin & lhs, const VALORArrayBin & rhs)
  {
    assert(sizeof(lhs)==6*sizeof(int));
    return !(memcmp(&lhs,&rhs,sizeof(lhs)));
  }
}
//____________________________________________________________________________
std::ostream& valor::operator<<(std::ostream& s,const VALORArrayBin & b)
{
  s<<"("<<b.bin1<<", "<<b.bin2<<", "<<b.bin3<<", "<<b.bin4<<", "<<b.bin5<<", "<<b.bin6<<")"<<std::endl;
  return s;
}
//____________________________________________________________________________
