//____________________________________________________________________________
/*!

\class      valor::CmdLnArgParser

\brief      Command line argument parser
            Taken from $GENIE/src/Utils

\author     Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
            University of Liverpool & STFC Rutherford Appleton Lab

\created    July 23, 2010

\cpright    Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
            For the full text of the license visit http://copyright.genie-mc.org
            or see $GENIE/LICENSE

\copyright (c) 2010-2014, The VALOR group. 

*/
//____________________________________________________________________________

#ifndef _CMD_LINE_ARG_PARSER_H_
#define _CMD_LINE_ARG_PARSER_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace valor {

class CmdLnArgParser {

public:
  CmdLnArgParser(int argc, char **argv);
 ~CmdLnArgParser();

  // Methods to check the existence of single character switches (eg -f, -s)
  // and retrieve the command-line argument following the switch

  bool    OptionExists (char opt);   ///< was option set?
  char *  Arg          (char opt);   ///< return argument following -`opt'

  string         ArgAsString       (char opt);
  vector<string> ArgAsStringTokens (char opt, string delimeter);
  
  vector<string> ArgAsStringTokens (string opt, string delimeter);
  
  double         ArgAsDouble       (char opt);
  vector<double> ArgAsDoubleTokens (char opt, string delimeter);
  int            ArgAsInt          (char opt);
  vector<int>    ArgAsIntTokens    (char opt, string delimeter);
  long           ArgAsLong         (char opt);
  vector<long>   ArgAsLongTokens   (char opt, string delimeter);

  // As above, but supporting multi-character switches (eg --with-x-file )

  bool    OptionExists (string opt); ///< was option set?
  char *  Arg          (string opt); ///< return argument following --`opt'

  string         ArgAsString       (string opt);
  double         ArgAsDouble       (string opt);
  int            ArgAsInt          (string opt);
  long           ArgAsLong         (string opt);

private:

  int    fArgc;
  char **fArgv;
  
};

}      // valor namespace

#endif // _CMD_LINE_ARG_PARSER_H_
