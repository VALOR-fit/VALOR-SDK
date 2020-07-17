//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "TApplication.h"
#include "TROOT.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TPRegexp.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/Env.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Conventions/Build.h"
#include "valor/Conventions/Opt.h"

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
#ifdef __VALOR_FMWK_FAULT_ON_DIE__
  std::cerr<<"VALOR died, forcing segfault to get trace."<<std::endl;
  ((TSystem*)(NULL))->Abort();
#else
  std::cerr<<"VALOR died"<<std::endl;
#endif
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
  
  TPRegexp re("(lib[\\w.]*) => (.*)[\\s]+(?=\\(0[xX][0-9a-fA-F]+\\))");
  vector<string> out_lines;
  while (true) {
    TString line;
    if (!line.Gets(f)) {break;}
    TObjArray * arr = re.MatchS(line);
    if (arr->GetEntries()!=3) {util::Die(-1);}
    TObjString * os = dynamic_cast<TObjString*>(arr->At(2));
    if (!os) {util::Die(-1); return out_lines;}
    std::string out_line(os->GetString().Data());
    out_lines.push_back(out_line);
  }
  gSystem->ClosePipe(f);
  return out_lines;
}
//____________________________________________________________________________
std::string valor::util::CompleteConfigPath(std::string config_file)
{
  TString ts_file = util::RemoveAllWhitespace(config_file.c_str());
  if (ts_file.Length()==0) {
    LOG("UtlSystem",pERROR) << "Passed empty string to read as config path.";
    util::Die(-1) ; return "";
  }
  gSystem->ExpandPathName(ts_file);

  // Check for config file.
  // See if it works as a normal path
  FileStat_t dummy;
  if (gSystem->GetPathInfo(ts_file.Data(), dummy)) 
  {
    // If not, try the config folder
    ts_file = TString::Format("%s%s",Env::Instance()->TopLevelConfDir().c_str(),ts_file.Data());
    if (gSystem->GetPathInfo(ts_file.Data(), dummy)) {
      LOG("UtlSystem",pERROR) << "Config file not found: "<<config_file;
      util::Die(1); 
    }
  }
  return std::string(ts_file.Data());
}
//____________________________________________________________________________
