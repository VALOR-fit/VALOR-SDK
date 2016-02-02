//____________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//____________________________________________________________________________

#include "TMatrixD.h"
#include "TVector.h"

#include <iomanip>

#include "Conventions/Constants.h"
#include "Utilities/UtlPrint.h"

using namespace std;

//____________________________________________________________________________
void valor::util::PrintVector(
        const TVectorD & v, int w, int prec, Option_t * option, std::ostream & stream)
{
  TString opt = option;
  
  int np = v.GetNoElements();
  stream << fixed;

  for(int ip = 0; ip < np; ip++) {
     stream << "> param = " << setw(3) << ip;
     
     stream << " - value = " << setw(w) << setprecision(prec) << v[ip] << endl;
  }
  stream << scientific;
}
//___________________________________________________________________________
void valor::util::PrintVectorPair(
   const TVectorD & current, const TVectorD & nominal, 
   int w, int prec, Option_t * option, std::ostream & stream)
{
  TString opt = option;
  
  int np = current.GetNoElements();
  stream << fixed;

  for(int ip = 0; ip < np; ip++) {
     stream << "> param = " << setw(3) << ip;
     
     stream << " - current = " 
          << setw(w) << setprecision(prec) << current[ip];
     stream << " - nominal = " 
          << setw(w) << setprecision(prec) << nominal[ip];
     if(fabs(nominal[ip]) > conv::kASmallNum) {
       stream << " - current/nominal = " 
            << setw(w) << setprecision(prec) << current[ip]/nominal[ip];
     }
     stream << endl;
  }
  stream << scientific;
}
//___________________________________________________________________________
void valor::util::PrintMatrix(
  const TMatrixD & m, int w, int prec, std::ostream & stream)
{
  int nc = m.GetNcols();
  int nr = m.GetNrows();
  stream << fixed;
  for(int ir=0; ir<nr; ir++) {
    if(ir==0) {
       stream << setw(w) << setfill(' ') << "r\\c:" << " | "; 
       for(int ic=0; ic<nc; ic++) {
         stream << setw(w) << setfill(' ') << setprecision(0) << ic << " | ";
       }
       stream << endl;
    }
    stream << setw(w) << setfill(' ') << setprecision(0) << ir << " | "; 
    for(int ic=0; ic<nc; ic++) {
         stream << setw(w) << setfill(' ') << setprecision(prec) 
              << m[ir][ic] << " | ";
    }//rows
    stream << endl;
  }//columns
  stream << scientific;
}
//___________________________________________________________________________
