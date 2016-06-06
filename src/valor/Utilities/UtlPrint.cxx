//____________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//____________________________________________________________________________

#include "TMatrixD.h"
#include "TVector.h"

#include <iomanip>
#include <cmath>
#include <cstdlib>

#include "valor/Conventions/Build.h"
#include "valor/Conventions/Constants.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/UtlPrint.h"

using namespace std;
using namespace valor;
using namespace valor::conv;

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
  stream.unsetf( ios::basefield | ios::floatfield);
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
  stream.unsetf( ios::basefield | ios::floatfield);
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
  stream.unsetf( ios::basefield | ios::floatfield);
}
//___________________________________________________________________________
void valor::util::PrintVector(
        const TVectorD & v, const PhysParamId_t * params, int w, int prec, Option_t * /*option*/, std::ostream & stream)
{
  //~ TString opt = option;
  
  int np = v.GetNoElements();
  stream << fixed;

  for(int ip = 0; ip < np; ip++) {
    PhysParamId_t p = params[ip];
    stream << "> param = " << setw(3) << ip;

    stream << setw(42) << NameRegistry::Instance()->AsString(p);
    stream << " ]";
    stream << " - value = " << setw(w) << setprecision(prec) << v[ip] << endl;
  }
  stream.unsetf( ios::basefield | ios::floatfield);
}
//___________________________________________________________________________
void valor::util::PrintVectorPair(
   const TVectorD & current, const TVectorD & nominal, const PhysParamId_t * params,
   int w, int prec, Option_t * /*option*/, std::ostream & stream)
{
  //~ TString opt = option;
  
  int np = current.GetNoElements();
  stream << fixed;

  for(int ip = 0; ip < np; ip++) {
    PhysParamId_t p = params[ip];
    stream << "> param = " << setw(3) << ip;
    stream << " [ ";

    stream << setw(30) << NameRegistry::Instance()->AsString(p);
    stream << " ]";
    
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
  stream.unsetf( ios::basefield | ios::floatfield);
}
//___________________________________________________________________________
#ifdef __VALOR_SLACK_TOKEN__
void valor::util::NotifySlack (std::string message, std::string channel, std::string username)
{
  static const TString request_pattern("curl -X POST --data-urlencode \
  \'payload={\"channel\": \"[CHANNEL]\", \"username\": \"[USERNAME]\", \"text\": \"[MESSAGE]\"}\' \
  https://hooks.slack.com/services/[TOKEN]");
  
  TString escaped_message(message);
  escaped_message.ReplaceAll("'","'\\''");
  
  TString request = request_pattern;
  request = request.ReplaceAll("[MESSAGE]" ,escaped_message.Data());
  request = request.ReplaceAll("[CHANNEL]" ,channel .c_str());
  request = request.ReplaceAll("[USERNAME]",username.c_str());
  request = request.ReplaceAll("[TOKEN]"   ,__VALOR_SLACK_TOKEN__);
  std::cerr<<request<<std::endl;
  system(request.Data());
}
#else
void valor::util::NotifySlack (std::string , std::string , std::string ) {}
#endif // __VALOR_SLACK_TOKEN__
