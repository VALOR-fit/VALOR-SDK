//________________________________________________________________________________________
/*

   Copyright (c) 2010-2016, The VALOR group. All rights reserved.

*/
//________________________________________________________________________________________

#include <cassert>
#include <fstream>
#include <stdlib.h>

#include <TSystem.h>

#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"
#include "valor/CmdLine/CmdLnArgList.h"
#include "valor/CmdLine/CmdLnArgParser.h"

using std::ifstream;
using namespace valor;

//________________________________________________________________________________________
CmdLnArgList::CmdLnArgList() :
  Registry("command_line_arg",false)
{

}
//________________________________________________________________________________________
CmdLnArgList::~CmdLnArgList()
{

}
//________________________________________________________________________________________
bool CmdLnArgList::Read(int argc, char ** argv, string arg_spec_files)
{
  CmdLnArgParser parser(argc,argv);

  LOG("CmdLnArgList",pINFO)
    << "Reading command-line argument specs from: " << arg_spec_files;
    
  bool failure = false;

  // list of argument spec files
  vector<string> fv;
  if(arg_spec_files.find(":") == string::npos) {
    fv.push_back(arg_spec_files); // there is only one
  } else {
    fv = util::Tokens(arg_spec_files, ":"); // there are many
  }

  LOG("CmdLnArgList",pNOTICE)
    << fv.size() << " argument specs files were specified";

  // loop over arg spec files
  vector<string>::const_iterator fvit = fv.begin();
  for( ; fvit != fv.end(); ++fvit) {
    // get filename
    string filename = *fvit;
    LOG("CmdLnArgList",pNOTICE)
      << "Reading command-line argument specs from file: " << filename;
    // replace $BLAH (if any) with gSystem->Getenv("BLAH")
    if(filename.find("$") != string::npos) {
      size_t start  = filename.find("$",0);
      size_t end    = filename.find("/", start);
      size_t length = end-start-1;
      string env_var = filename.substr(start+1,length);
      LOG("CmdLnArgList",pNOTICE)
        << "Looking up env. var: " << env_var;
      const char * env_var_value =  gSystem->Getenv(env_var.c_str());
      if(!env_var_value) {
        LOG("CmdLnArgList",pERROR)
          << "Undefined env. var: " << env_var;
        failure = true ; continue;;
      }
      string str_env_var_value = env_var_value;
      filename.replace(start,length+1,str_env_var_value);
      LOG("CmdLnArgList",pNOTICE)
        << "Actually, reading command-line argument specs from file: " << filename;
    }
    // open file
    ifstream file;     
    file.open(filename.c_str());
    if (! file.is_open() ) {
      LOG("CmdLnArgList",pERROR) << "Could not open file: "<<filename;
      continue;
    }
    // read file one line at a time
    while (!file.eof()) {
      string line = "";
      std::getline(file,line);
      LOG("CmdLnArgList",pDEBUG) << "Read line: " << line;
      if(line.find('#') != string::npos) {
        // comment line, continue
        // stricly speaking should check the first character other than space...
        continue;
      }
      if(line.size() == 0) {         
        continue;
      }
      LOG("CmdLnArgList",pDEBUG) 
        << "Line: [" << line << "]";

      // tokenize using | as delimeter
      vector<string> line_tokens = util::Tokens(line, "|");
      assert(line_tokens.size()==4);
      string argument = util::TrimSpaces(line_tokens[0]);
      string type     = util::TrimSpaces(line_tokens[1]);
      string required = util::TrimSpaces(line_tokens[2]);
      string deflt    = util::TrimSpaces(line_tokens[3]);
      LOG("CmdLnArgList",pINFO) 
        << "Argument: " << argument
        << " -> type: " << type
        << ", required: " << required
        << ", default value: " << deflt;

      bool is_int       = (type == "integer");
      bool is_dbl       = (type == "double");
      bool is_str       = (type == "string");
      bool is_bln       = (type == "boolean");
      bool is_blnlst    = (type == "boolean_list");
      bool is_optional  = (required == "optional");
      bool is_mandatory = (required == "mandatory");
      bool no_deflt     = (deflt == "-");
      bool is_found = parser.OptionExists(argument);

      //
      // Handle integer,double,string inputs
      //

      if(is_int || is_dbl || is_str) {

        // Error conditions 

        // Mandatory argument not found
        if(is_mandatory && !is_found) {
          LOG("CmdLnArgList",pERROR) 
            << "Mandatory argument: " << argument
            << " was not found!";
          failure = true ; continue;
        }
        // Optional argument not found, but no default either
        if(is_optional && !is_found && no_deflt) {
          LOG("CmdLnArgList",pERROR) 
            << "Optional argument: " << argument
            << " wasn't set and has no default value specified!";
          failure = true ; continue;
        }

        // All should be OK now, insert input value or default

        // Integers
        if(is_int) { 
          if(is_found) { // whether optional or mandatory
            int ival = parser.ArgAsInt(argument);
            this->Set(argument,ival);
          } else {
            if(is_optional) { // not found but has a given default
              int ival = atoi(deflt.c_str());
              this->Set(argument,ival);
            }
          }
        }
        // Doubles
        if(is_dbl) { 
          if(is_found) { // whether optional or mandatory
            double xval = parser.ArgAsDouble(argument);
            this->Set(argument,xval);
          } else {
            if(is_optional) { // not found but has a given default
              double xval = atof(deflt.c_str());
              this->Set(argument,xval);
            }
          }
        }
        // Strings
        if(is_str) { 
          if(is_found) { // whether optional or mandatory
            string sval = parser.ArgAsString(argument);
            this->Set(argument,sval);
          } else {
            if(is_optional) { // not found but has a given default
              string sval = deflt;
              this->Set(argument,sval);
            }
          }
        }

      } // int, or double, or string


      //
      // Handle boolean inputs
      //

      if(is_bln && !is_blnlst) {
        // can not be mandatory argument - see comments in header file
        if(is_mandatory) {
          LOG("CmdLnArgList",pERROR) 
            << "Argument: " << argument
            << " can not be mandatory!";
          failure = true ; continue;
        }
        this->Set(argument,is_found);
      }// boolean

      //
      // Handle boolean list inputs
      //

      if(is_blnlst) {

        // split input comma-delimited list
        vector<string> boolarglist = 
          util::Tokens(argument, ","); 

        // Error conditions 

        if(boolarglist.size() <= 1) {
          LOG("CmdLnArgList",pERROR) 
            << "Input: " << argument
            << " does not contain a list of multiple boolean arguments!";
          failure = true ; continue;
        }
        int nfound = 0;
        vector<string>::const_iterator boolargit = boolarglist.begin();
        for( ; boolargit != boolarglist.end(); ++boolargit) {
          string boolarg = util::TrimSpaces(*boolargit);
          if( parser.OptionExists(boolarg) ) { nfound++; }
        }        
        // More than one found (whether optional or mandatory) or
        // none found (if optional)
        if(is_mandatory && nfound==0) {
          LOG("CmdLnArgList",pERROR) 
            << "Mandatory argument from list: " << argument
            << " was not found!";
          failure = true ; continue;
        }
        if(nfound>1) {
          LOG("CmdLnArgList",pERROR) 
            << "Multiple argument from list: " << argument
            << " were not found but only one is expected!";
          failure = true ; continue;
        }
        // Optional argument not found, but no default either
        if(is_optional && nfound==0 && no_deflt) {
          LOG("CmdLnArgList",pERROR) 
            << "None of the arguments in list: " << argument
            << " was set, but no default value was specified either!";
          failure = true ; continue;
        }

        // All should be OK now, insert input value or default
        boolargit = boolarglist.begin();
        if(is_optional && nfound==0) {
          for( ; boolargit != boolarglist.end(); ++boolargit) {
            string boolarg = util::TrimSpaces(*boolargit);
            if(boolarg==deflt) {
              this->Set(boolarg,true);
            } else {
              this->Set(boolarg,false);
            }
          }
        }
        else {
          for( ; boolargit != boolarglist.end(); ++boolargit) {
            string boolarg = util::TrimSpaces(*boolargit);
            if(parser.OptionExists(boolarg)) {
              this->Set(boolarg,true);
            } else {
              this->Set(boolarg,false);
            }
          }
        }        
      }// boolean list


      //
      //
      // further processing
      //
      //

    }
    file.close();
  }

  if (!parser.RecognisedAllArgs()) {
    LOG("CmdLnArgList",pFATAL) << "Did not recognise all command line arguments";
    util::Die(-1);
    return false;
  }

  return !failure;
}
//________________________________________________________________________________________


