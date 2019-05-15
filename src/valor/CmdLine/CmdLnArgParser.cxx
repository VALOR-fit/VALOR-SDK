//____________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

*/
//____________________________________________________________________________

#include <cctype>
#include <cstdlib>
#include <string>

#include "TMath.h"

#include "valor/CmdLine/CmdLnArgParser.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlStyle.h"
#include "valor/Utilities/Env.h"

using namespace valor;
using std::string;
using std::vector;

//____________________________________________________________________________
CmdLnArgParser::CmdLnArgParser(int argc, const char* const* argv) :
  fArgc(argc),
  fArgv(argv),
  fArgUsed(fArgc,false)
{
  TString conf_dir  = valor::Env::Instance()->TopLevelConfDir();
  TString verbosity = conf_dir + "/verbosity.xml";

  valor::util::Verbosity(verbosity.Data());
  
  for (int i = 1 ; i < fArgc; i++) {
    fArgUsed[i] = false;
  }
  
  fArgUsed[0] = true;
}
//____________________________________________________________________________
CmdLnArgParser::~CmdLnArgParser()
{
}
//____________________________________________________________________________
std::string CmdLnArgParser::ArgAsString(char op) const
{
  
  std::string argument;

  int                 argc = fArgc;
  const char* const*  argv = fArgv;

  while(argc>2)
  {
    LOG("Parser", pDEBUG) << "Getting next argument in argument-list";
    LOG("Parser", pDEBUG) << "Current argc = " << argc;

    if (argv[1][0] == '-') {      
      LOG("Parser", pDEBUG) 
        << "Got char (argv[1][1]) following argv[1][0]='-' : " << argv[1][1];

      if (argv[1][1] == op) {
        LOG("Parser", pDEBUG) << "Input option: " << op << " was matched";

        if (strlen(&argv[1][2]) ) {
          argument = &argv[1][2];
          //set = true;
          LOG("Parser", pINFO) 
            << "Set opt = [" << op << "] to val = [" << argument << "] "<<" from "<<argv[1][2]<<"(args "<<fArgc-argc+1<<","<<fArgc-argc+2<<")";
          
        }
        else if( (argc>2) && 
            !(argv[2][0]=='-' && isalpha(argv[2][1])) ) {

          LOG("Parser", pDEBUG) 
            << "argc>2 and next arg not a '-' followed by an alpha char";

          argc--;
          argv++;
          argument = &argv[1][0];
          //set = true;
          LOG("Parser", pINFO) 
            << "Set opt = [" << op << "] to val = [" << argument << "] (args "<<fArgc-argc+1<<","<<fArgc-argc+2<<")";
        }
      }
    }
    argc--;
    argv++;
    if(argc>2) {
      LOG("Parser", pDEBUG) << "Next argv[1][0] = " << argv[1][0];
    }
  }

  return argument;
}
//____________________________________________________________________________
bool CmdLnArgParser::OptionExists(char op, bool set_used) const
{
  bool set = false;

  int                argc = fArgc;
  const char* const* argv = fArgv;

  while(argc>1) {
    if(argv[1][0] == '-') {
      if (argv[1][1] == op) {
        set = true;
        if (set_used) {fArgUsed[fArgc-argc+1] = true;}
      }
    }
    argc--;
    argv++;
  }

  return set;
}
//____________________________________________________________________________
vector<string> CmdLnArgParser::ArgAsStringTokens(char op, string delimeter) const
{
  string argument = this->ArgAsString(op);

  vector<string> tokens = util::Tokens(argument, delimeter);
  return tokens;
}
//____________________________________________________________________________
vector<string> CmdLnArgParser::ArgAsStringTokens(string op, string delimeter) const
{
  string argument = this->ArgAsString(op);
  //string argument = op;

  vector<string> tokens = util::Tokens(argument, delimeter);
  return tokens;
}
//____________________________________________________________________________
double CmdLnArgParser::ArgAsDouble(char op) const
{
  std::string argument = this->ArgAsString(op);
  double value = std::atof(argument.c_str());

  return value;
}
//____________________________________________________________________________
vector<double> CmdLnArgParser::ArgAsDoubleTokens(char op, string delimeter) const
{
  vector<string> strtokens = this->ArgAsStringTokens(op, delimeter);
  vector<double> tokens;
  vector<string>::const_iterator iter = strtokens.begin();
  for( ; iter != strtokens.end(); ++iter) {
    string arg = *iter;
    tokens.push_back(std::atof(arg.c_str()));
  }
  return tokens;
}
//____________________________________________________________________________
vector<double> CmdLnArgParser::ArgAsDoubleTokens(string op, string delimeter) const
{
  vector<string> strtokens = this->ArgAsStringTokens(op, delimeter);
  vector<double> tokens;
  vector<string>::const_iterator iter = strtokens.begin();
  for( ; iter != strtokens.end(); ++iter) {
    string arg = *iter;
    tokens.push_back(std::atof(arg.c_str()));
  }
  return tokens;
}
//____________________________________________________________________________
int CmdLnArgParser::ArgAsInt(char op) const
{
  std::string argument = this->ArgAsString(op);
  int value = std::atoi(argument.c_str());
  // If there's an e in it, treat it as a double
  if (argument.find('e') != std::string::npos || argument.find('E') != std::string::npos) {
    value = TMath::Nint(std::atof(argument.c_str()));
  }
  return value;
}
//____________________________________________________________________________
unsigned int CmdLnArgParser::ArgAsUInt(char op) const
{
  std::string argument = this->ArgAsString(op);
  unsigned int value = std::strtoul(argument.c_str(),NULL,0);
  // If there's an e in it, treat it as a double
  if (argument.find('e') != std::string::npos || argument.find('E') != std::string::npos) {
    double as_d = std::atof(argument.c_str());
    value = TMath::Nint(as_d);
    if (as_d < 0) { LOG("Parser",pWARN) << "Warning: unsigned int argument has value "<<as_d<<" converted to "<<value;}
  }
  return value;
}
//____________________________________________________________________________
vector<int> CmdLnArgParser::ArgAsIntTokens(char op, string delimeter) const
{
  vector<string> strtokens = this->ArgAsStringTokens(op, delimeter);
  vector<int> tokens;
  vector<string>::const_iterator iter = strtokens.begin();
  for( ; iter != strtokens.end(); ++iter) {
    string argument = *iter;
    int value = std::atoi(argument.c_str());
    if (argument.find('e') != std::string::npos || argument.find('E') != std::string::npos) {
      double as_d = std::atof(argument.c_str());
      value = TMath::Nint(as_d);
      if (as_d < 0) { LOG("Parser",pWARN) << "Warning: unsigned int argument has value "<<as_d<<" converted to "<<value;}
    }
    tokens.push_back(value);
  }
  return tokens;
}
//____________________________________________________________________________
vector<unsigned int> CmdLnArgParser::ArgAsUIntTokens(char op, string delimeter) const
{
  vector<string> strtokens = this->ArgAsStringTokens(op, delimeter);
  vector<unsigned int> tokens;
  vector<string>::const_iterator iter = strtokens.begin();
  for( ; iter != strtokens.end(); ++iter) {
    string argument = *iter;
    unsigned int value = std::strtoul(argument.c_str(),NULL,0);
    // If there's an e in it, treat it as a double
    if (argument.find('e') != std::string::npos || argument.find('E') != std::string::npos) {
      double as_d = std::atof(argument.c_str());
      value = TMath::Nint(as_d);
      if (as_d < 0) { LOG("Parser",pWARN) << "Warning: unsigned int argument has value "<<as_d<<" converted to "<<value;}
    }
    tokens.push_back(value);
  }
  return tokens;
}
//____________________________________________________________________________
long CmdLnArgParser::ArgAsLong(char op) const
{
  std::string argument = this->ArgAsString(op);
  long value = std::atol(argument.c_str());
  // If there's an e in it, treat it as a double
  if (argument.find('e') != std::string::npos || argument.find('E') != std::string::npos) {
    value = TMath::Nint(std::atof(argument.c_str()));
  }
  return value;
}
//____________________________________________________________________________
vector<long> CmdLnArgParser::ArgAsLongTokens(char op, string delimeter) const
{
  vector<string> strtokens = this->ArgAsStringTokens(op, delimeter);
  vector<long> tokens;
  vector<string>::const_iterator iter = strtokens.begin();
  for( ; iter != strtokens.end(); ++iter) {
    string argument = *iter;
    long value = std::atol(argument.c_str());
    // If there's an e in it, treat it as a double
    if (argument.find('e') != std::string::npos || argument.find('E') != std::string::npos) {
      value = TMath::Nint(std::atof(argument.c_str()));
    }
    tokens.push_back(value);
  }
  return tokens;
}
//____________________________________________________________________________
std::string CmdLnArgParser::ArgAsString(string op) const
{
  std::string argument;

  int                argc  = fArgc;
  const char* const* argv  = fArgv;

  while(argc>2)
  {
    LOG("Parser", pDEBUG) << "Getting next argument in argument-list";
    LOG("Parser", pDEBUG) << "Current argc = " << argc;

    if (argv[1][0] == '-' && argv[1][1] == '-') {      
      //char * op_cur = strndup(argv[1]+2,strlen(argv[1]));
      const char * op_cur = &argv[1][2];
      LOG("Parser", pDEBUG) 
        << "Got string following '--' : " << op_cur;
      if (strcmp(op.c_str(),op_cur)==0) {
        LOG("Parser", pDEBUG) << "Input option: " << op << " was matched";

        if (strlen(&argv[2][0]) ) {
          argument = &argv[2][0];
          //set = true;
          LOG("Parser", pINFO) 
            << "Set opt = [" << op << "] to val = [" << argument << "] (args "<<fArgc-argc+1<<","<<fArgc-argc+2<<")";

        }
        else if( (argc>2) && 
            !(argv[2][0]=='-' &&argv[2][1]=='-' && isalpha(argv[2][2])) ) {
          LOG("Parser", pDEBUG) 
            << "argc>2 and next arg not a '--' followed by an alpha char";

          argc--;
          argv++;
          argument = &argv[1][0];
          //set = true;
          LOG("Parser", pINFO) 
            << "Set opt = [" << op << "] to val = [" << argument << "] (args "<<fArgc-argc+1<<","<<fArgc-argc+2<<")";
        }
        fArgUsed[fArgc-argc+1] = true;
        fArgUsed[fArgc-argc+2] = true;
      } //option_matched
    }
    argc--;
    argv++;
    if(argc>2) {
      LOG("Parser", pDEBUG) << "Next argv[1][0] = " << argv[1][0];
    }
  }

  return argument;
}
//____________________________________________________________________________
bool CmdLnArgParser::OptionExists(string op, bool set_used) const
{
  bool set = false;

  int                 argc = fArgc;
  const char* const*  argv = fArgv;

  while(argc>1) {
    if(argv[1][0] == '-' && argv[1][1] == '-') {
      if (strcmp(op.c_str(),&argv[1][2])==0) {
        set = true;
        if (set_used) {fArgUsed[fArgc-argc+1] = true;}
      }
    }
    argc--;
    argv++;
  }

  return set;
}
//____________________________________________________________________________
double CmdLnArgParser::ArgAsDouble(string op) const
{
  std::string argument = this->ArgAsString(op);
  double value = std::atof(argument.c_str());
  return value;
}
//____________________________________________________________________________
int CmdLnArgParser::ArgAsInt(string op) const
{
  std::string argument = this->ArgAsString(op);
  int value = std::atoi(argument.c_str());
  // If there's an e in it, treat it as a double
  if (argument.find('e') != std::string::npos || argument.find('E') != std::string::npos) {
    value = TMath::Nint(std::atof(argument.c_str()));
  }
  return value;
}
//____________________________________________________________________________
unsigned int CmdLnArgParser::ArgAsUInt(string op) const
{
  std::string argument = this->ArgAsString(op);
  unsigned int value = std::strtoul(argument.c_str(),NULL,0);
  // If there's an e in it, treat it as a double
  if (argument.find('e') != std::string::npos || argument.find('E') != std::string::npos) {
    double as_d = std::atof(argument.c_str());
    value = TMath::Nint(as_d);
    if (as_d < 0) { LOG("Parser",pWARN) << "Warning: unsigned int argument has value "<<as_d<<" converted to "<<value;}
  }
  return value;
}
//____________________________________________________________________________
long CmdLnArgParser::ArgAsLong(string op) const
{
  std::string argument = this->ArgAsString(op);
  long value = std::atol(argument.c_str());
  // If there's an e in it, treat it as a double
  if (argument.find('e') != std::string::npos || argument.find('E') != std::string::npos) {
    value = TMath::Nint(std::atof(argument.c_str()));
  }
  return value;
}
//____________________________________________________________________________
bool CmdLnArgParser::RecognisedAllArgs() const
{
  bool status = true;
  for (int iarg = 0 ; iarg < fArgc; iarg++) {
    if (!(fArgUsed[iarg])) {
      LOG("Parser",pWARN) << "Argument not recognised "<<fArgv[iarg];
      status = false;
    }
    else {
      LOG("Parser",pDEBUG) << "Argument recognised "<<fArgv[iarg];
    }
  }
  return status;
}
//____________________________________________________________________________
