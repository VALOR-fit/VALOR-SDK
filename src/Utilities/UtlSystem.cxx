//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "TApplication.h"
#include "TROOT.h"
#include "TString.h"
#include "TSystem.h"
#include "TPRegexp.h"

#include "Utilities/UtlStr.h"
#include "Utilities/UtlSystem.h"
#include "Conventions/Opt.h"

using namespace valor;

using namespace std;

#define USE_TERMINAL_COLOR

//____________________________________________________________________________
void valor::util::Die(int status_code)
{
  if (Opt::Instance()->gDoNotDie) {
#ifdef USE_TERMINAL_COLOR
    cerr<<"\033[1;31m";
#endif
    cerr<<"util/Die() >> Would have died with code "<<status_code<<". "
        <<"Choosing life."<<endl;
#ifdef USE_TERMINAL_COLOR
    cerr<<"\033[0m";
#endif
    return;
  }
  exit(status_code);
}
//____________________________________________________________________________
void valor::util::ChangeInterpreterPrompt(std::string new_prompt)
{
  TString appname = gApplication->ApplicationName();
  if (appname.Contains("Rint")) {
    TString command = TString::Format(
      "((TRint*)gApplication)->SetPrompt(\"%s\");",
      new_prompt.c_str());
    gROOT->ProcessLine(command.Data());
  }
}
//____________________________________________________________________________
std::vector<std::string> valor::util::GetLinkedLibraries(std::string executable_path, std::string pattern)
{
  TString command = TString::Format("ldd %s | grep -e %s",executable_path.c_str(),pattern.c_str());
  FILE * f = gSystem->OpenPipe(command.Data(),"r");
  //~ long size = ftell(f);
  //~ cerr<<"Size = "<<size<<"\t"<<strerror(errno)<<endl;
  //~ rewind(f);
  //~ cerr<<"Rewind done"<<endl;
  //~ char * buffer = new char[size]();
  //~ cerr<<"Alloc done"<<endl;
  //~ size_t result = fread(buffer,1,size,f);
  //~ cerr<<"Read done"<<endl;
  //~ gSystem->ClosePipe(f);
  //~ cerr<<"pipe closed"<<endl;
  //~ vector<string> lines = Tokens(buffer,"\n");
  //~ delete buffer;
  //~ cerr<<"buffer deleted"<<endl;
  
  TPRegexp re("(lib[\\w.]*) => (.*)[\\s]+(?=\\(0[xX][0-9a-fA-F]+\\))");
  vector<string> out_lines;
  while (true) {
    TString line;
    if (!line.Gets(f)) {break;}
    TObjArray * arr = re.MatchS(line);
    if (arr->GetEntries()!=3) {util::Die(-1);}
    TObjString * os = dynamic_cast<TObjString*>(arr->At(2));
    if (!os) {util::Die(-1);}
    std::string out_line(os->GetString().Data());
    out_lines.push_back(out_line);
  }
  gSystem->ClosePipe(f);
  return out_lines;
}
//____________________________________________________________________________
