//____________________________________________________________________________
/*!

\class      valor::CmdLnArgParser

\brief      Command line argument parser taken from $GENIE/src/Utils

\author     Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
            University of Liverpool & STFC Rutherford Appleton Lab

\created    July 23, 2010

\copyright  Copyright (c) 2003-2016, GENIE Neutrino MC Generator Collaboration

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

  bool          OptionExists (char opt) const;   ///< was option set?
  const char *  Arg          (char opt) const;   ///< return argument following -`opt'

  string         ArgAsString       (char opt) const;
  vector<string> ArgAsStringTokens (char opt, string delimeter) const;
  
  vector<string> ArgAsStringTokens (string opt, string delimeter) const;
  
  double                ArgAsDouble       (char opt) const;
  vector<double>        ArgAsDoubleTokens (char opt, string delimeter) const;
  int                   ArgAsInt          (char opt) const;
  vector<int>           ArgAsIntTokens    (char opt, string delimeter) const;
  unsigned int          ArgAsUInt         (char opt) const;
  vector<unsigned int>  ArgAsUIntTokens   (char opt, string delimeter) const;
  long                  ArgAsLong         (char opt) const;
  vector<long>          ArgAsLongTokens   (char opt, string delimeter) const;

  // As above, but supporting multi-character switches (eg --with-x-file )

  bool    OptionExists (string opt) const; ///< was option set?
  const char *  Arg    (string opt) const; ///< return argument following --`opt'

  string         ArgAsString       (string opt) const;
  double         ArgAsDouble       (string opt) const;
  int            ArgAsInt          (string opt) const;
  unsigned int   ArgAsUInt         (string opt) const;
  long           ArgAsLong         (string opt) const;

private:

  int     fArgc;
  char ** fArgv;
  
};

}      // valor namespace

#endif // _CMD_LINE_ARG_PARSER_H_
