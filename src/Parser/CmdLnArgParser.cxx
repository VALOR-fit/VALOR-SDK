//____________________________________________________________________________
/*
 Copyright (c) 2010-2016, The VALOR group. 

 Copyright (c) 2003-2016, GENIE Neutrino MC Generator Collaboration
 For the full text of the license visit http://copyright.genie-mc.org
 or see $GENIE/LICENSE

 Author: Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
         University of Liverpool & STFC Rutherford Appleton Lab 

 For documentation see the corresponding header file.

 Important revisions after taking from GENIE, 14 Aug 2015:

*/
//____________________________________________________________________________

#include <cctype>
#include <cstdlib>

#include "Parser/CmdLnArgParser.h"
#include "Messenger/Messenger.h"
#include "Utilities/UtlStr.h"
#include "Utilities/UtlStyle.h"
#include "Utilities/Env.h"

using namespace valor;

//____________________________________________________________________________
CmdLnArgParser::CmdLnArgParser(int argc, char **argv) :
fArgc(argc),
fArgv(argv)
{
  TString conf_dir  = valor::Env::Instance()->TopLevelConfDir();
  TString verbosity = conf_dir + "/verbosity.xml";
  
  valor::util::Verbosity(verbosity.Data());
}
//____________________________________________________________________________
CmdLnArgParser::~CmdLnArgParser()
{

}
//____________________________________________________________________________
const char * CmdLnArgParser::Arg(char op) const
{
  const int buf_size = 2048;

  //bool    set       = false;
  char *  argument  = new char[buf_size];
  strcpy(argument, "");

  int     argc = fArgc;
  char ** argv = fArgv;

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
            strcpy(argument,&argv[1][2]);
            //set = true;
            LOG("Parser", pINFO) 
               << "Set opt = [" << op << "] to val = [" << argument << "]";

         } else if( (argc>2) && 
                    !(argv[2][0]=='-' && isalpha(argv[2][1])) ) {

            LOG("Parser", pDEBUG) 
              << "argc>2 and next arg not a '-' followed by an alpha char";

            argc--;
            argv++;
            strcpy(argument,&argv[1][0]);
            //set = true;
            LOG("Parser", pINFO) 
               << "Set opt = [" << op << "] to val = [" << argument << "]";
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
bool CmdLnArgParser::OptionExists(char op) const
{
  bool set = false;

  int     argc = fArgc;
  char ** argv = fArgv;

  while(argc>1) {
    if(argv[1][0] == '-') {
      if (argv[1][1] == op) set = true;
    }
    argc--;
    argv++;
  }

  return set;
}
//____________________________________________________________________________
string CmdLnArgParser::ArgAsString(char op) const
{
  const char * argument = this->Arg(op);
  string value = string(argument);
  delete [] argument;

  return value;
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
  const char * argument = this->Arg(op);
  double value = atof(argument);
  delete [] argument;

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
    tokens.push_back(atof(arg.c_str()));
  }
  return tokens;
}
//____________________________________________________________________________
int CmdLnArgParser::ArgAsInt(char op) const
{
  const char * argument = this->Arg(op);
  int value = atoi(argument);
  delete [] argument;

  return value;
}
//____________________________________________________________________________
unsigned int CmdLnArgParser::ArgAsUInt(char op) const
{
  const char * argument = this->Arg(op);
  unsigned int value = strtoul(argument,NULL,0);
  delete [] argument;

  return value;
}
//____________________________________________________________________________
vector<int> CmdLnArgParser::ArgAsIntTokens(char op, string delimeter) const
{
  vector<string> strtokens = this->ArgAsStringTokens(op, delimeter);
  vector<int> tokens;
  vector<string>::const_iterator iter = strtokens.begin();
  for( ; iter != strtokens.end(); ++iter) {
    string arg = *iter;
    tokens.push_back(atoi(arg.c_str()));
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
    string arg = *iter;
    tokens.push_back(strtoul(arg.c_str(),NULL,0));
  }
  return tokens;
}
//____________________________________________________________________________
long CmdLnArgParser::ArgAsLong(char op) const
{
  const char * argument = this->Arg(op);
  long value = atol(argument);
  delete [] argument;

  return value;
}
//____________________________________________________________________________
vector<long> CmdLnArgParser::ArgAsLongTokens(char op, string delimeter) const
{
  vector<string> strtokens = this->ArgAsStringTokens(op, delimeter);
  vector<long> tokens;
  vector<string>::const_iterator iter = strtokens.begin();
  for( ; iter != strtokens.end(); ++iter) {
    string arg = *iter;
    tokens.push_back(atol(arg.c_str()));
  }
  return tokens;
}
//____________________________________________________________________________
const char * CmdLnArgParser::Arg(string op) const
{
  const int buf_size = 2048;
  //bool    set        = false;
  char *  argument   = new char[buf_size];
  strcpy(argument, "");

  int     argc  = fArgc;
  char ** argv  = fArgv;

  while(argc>2)
  {
    LOG("Parser", pDEBUG) << "Getting next argument in argument-list";
    LOG("Parser", pDEBUG) << "Current argc = " << argc;

    if (argv[1][0] == '-' && argv[1][1] == '-') {      
      //char * op_cur = strndup(argv[1]+2,strlen(argv[1]));
      char op_cur[buf_size];
      strcpy(op_cur,&argv[1][2]);
      LOG("Parser", pDEBUG) 
        << "Got string following '--' : " << op_cur;
      if (strcmp(op.c_str(),op_cur)==0) {
  	 LOG("Parser", pDEBUG) << "Input option: " << op << " was matched";

         if (strlen(&argv[2][0]) ) {
            strcpy(argument,&argv[2][0]);
            //set = true;
            LOG("Parser", pINFO) 
               << "Set opt = [" << op << "] to val = [" << argument << "]";

         } else if( (argc>2) && 
                    !(argv[2][0]=='-' &&argv[2][1]=='-' && isalpha(argv[2][2])) ) {
            LOG("Parser", pDEBUG) 
              << "argc>2 and next arg not a '--' followed by an alpha char";

            argc--;
            argv++;
            strcpy(argument,&argv[1][0]);
            //set = true;
            LOG("Parser", pINFO) 
               << "Set opt = [" << op << "] to val = [" << argument << "]";
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
bool CmdLnArgParser::OptionExists(string op) const
{
  const int buf_size = 2048;
  bool set = false;

  int     argc = fArgc;
  char ** argv = fArgv;

  while(argc>1) {
    if(argv[1][0] == '-' && argv[1][1] == '-') {
        //char * op_cur =  strndup(argv[1]+2,strlen(argv[1]));
        char op_cur[buf_size];
        strcpy(op_cur,&argv[1][2]);
        if (strcmp(op.c_str(),op_cur)==0) set = true;
    }
    argc--;
    argv++;
  }

  return set;
}
//____________________________________________________________________________
string CmdLnArgParser::ArgAsString(string op) const
{
  const char * argument = this->Arg(op);
  string value = string(argument);
  delete [] argument;

  return value;
}
//____________________________________________________________________________
double CmdLnArgParser::ArgAsDouble(string op) const
{
  const char * argument = this->Arg(op);
  double value = atof(argument);
  delete [] argument;

  return value;
}
//____________________________________________________________________________
int CmdLnArgParser::ArgAsInt(string op) const
{
  const char * argument = this->Arg(op);
  int value = atoi(argument);
  delete [] argument;

  return value;
}
//____________________________________________________________________________
unsigned int CmdLnArgParser::ArgAsUInt(string op) const
{
  const char * argument = this->Arg(op);
  int value = strtoul(argument,NULL,0);
  delete [] argument;

  return value;
}
//____________________________________________________________________________
long CmdLnArgParser::ArgAsLong(string op) const
{
  const char * argument = this->Arg(op);
  long value = atol(argument);
  delete [] argument;

  return value;
}
//____________________________________________________________________________
