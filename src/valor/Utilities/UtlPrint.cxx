//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

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
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/UtlPrint.h"
#include "valor/Utilities/UtlStr.h"

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
  LOG("UtlPrint",pNOTICE)<<"Notifying slack: "<<"\n"<<request;
  system(request.Data());
}
#else
void valor::util::NotifySlack (std::string , std::string , std::string ) {}
#endif // __VALOR_SLACK_TOKEN__
//___________________________________________________________________________
template <class T> void valor::util::PrintVectorComparison(const std::vector<T> & v1, const std::vector<T> & v2, std::ostream & out)
{
  size_t large_size = std::max(v1.size(),v2.size());
  for (size_t i = 0; i < large_size ; i++) {
    bool v1_good = i < v1.size();
    bool v2_good = i < v2.size();
    bool same = false;
    if (v1_good && v2_good) {
      same = v1[i]==v2[i];
    }
    out<<(same?"  ":"* ")<<i<<"\t"
       <<(v1_good?util::StreamIntoString(v1[i]).c_str():"EMPTY")<<"\t"
       <<(v2_good?util::StreamIntoString(v2[i]).c_str():"EMPTY")<<std::endl;
  }
  return;
}
template void valor::util::PrintVectorComparison (const std::vector<double> & v1, const std::vector<double> & v2, std::ostream & out);
template void valor::util::PrintVectorComparison (const std::vector<string> & v1, const std::vector<string> & v2, std::ostream & out);
//___________________________________________________________________________
