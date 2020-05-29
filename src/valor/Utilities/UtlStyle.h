//____________________________________________________________________________
/*!

\class     valor::utils

\brief     Utilities for formatting and style

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_style_utils_h_
#define _valor_style_utils_h_

#include <iostream>
#include <string>
#include <vector>

#include <TVectorD.h>
#include <TMatrixD.h>
#include <TString.h>

class TGraph;
class TH1;

using std::string;
using std::vector;
using std::cout;

namespace valor {
namespace util {

  void SetDefaultStyle(bool black_n_white=false);

  void Format(TGraph* gr,
        int lcol, int lsty, int lwid, int mcol, int msty, double msiz);
  void Format(TH1* hst,
        int lcol, int lsty, int lwid, int mcol, int msty, double msiz);

  void PrintBanner(string filename, UInt_t wait_msec);

  void Verbosity(string filename);

//  void PrintCompact    (const TVectorD & v, int w=15, int prec=9, Option_t * opt="", ostream & stream = cout);
//  void PrintCompact    (const TVectorD & current, const TVectorD & nominal, int w, int prec, Option_t * option, ostream & stream = cout);
//  void PrintCompact    (const TMatrixD & m, int w=15, int prec=9, ostream & stream = cout);
//  void PrintCompactTex (const TMatrixD & m, int w=20, int prec=9, ostream & stream = cout, TString opt = "");

} // util
} // valor
  
#endif


