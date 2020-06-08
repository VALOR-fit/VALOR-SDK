//____________________________________________________________________________
/*!

\class      valor::CmdLnArgParser

\brief      Command line argument parser taken from $GENIE/src/Utils

\author     Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
            University of Liverpool & STFC Rutherford Appleton Lab

\created    July 23, 2010

\copyright  Copyright (c) 2003-2020, The GENIE Collaboration

*/
//____________________________________________________________________________

#ifndef _CMD_LINE_ARG_PARSER_H_
#define _CMD_LINE_ARG_PARSER_H_

#include <string>
#include <vector>

namespace valor {

class CmdLnArgParser {

public:
  CmdLnArgParser(int argc, const char* const* argv);
 ~CmdLnArgParser();

  // Methods to check the existence of single character switches (eg -f, -s)
  // and retrieve the command-line argument following the switch

  bool                      OptionExists (char opt, bool set_used = true) const;   ///< was option set?
  
  std::string               ArgAsString       (char opt) const;
  std::vector<std::string>  ArgAsStringTokens (char opt, std::string delimeter) const;
  std::vector<std::string>  ArgAsStringTokens (std::string opt, std::string delimeter) const;
  
  double                    ArgAsDouble       (char opt) const;
  std::vector<double>       ArgAsDoubleTokens (char opt, std::string delimeter) const;
  int                       ArgAsInt          (char opt) const;
  std::vector<int>          ArgAsIntTokens    (char opt, std::string delimeter) const;
  unsigned int              ArgAsUInt         (char opt) const;
  std::vector<unsigned int> ArgAsUIntTokens   (char opt, std::string delimeter) const;
  long                      ArgAsLong         (char opt) const;
  std::vector<long>         ArgAsLongTokens   (char opt, std::string delimeter) const;

  // As above, but supporting multi-character switches (eg --with-x-file )
  
  bool    OptionExists (std::string opt, bool set_used = true) const; ///< was option set?
  
  std::string         ArgAsString       (std::string opt) const;
  double              ArgAsDouble       (std::string opt) const;
  int                 ArgAsInt          (std::string opt) const;
  unsigned int        ArgAsUInt         (std::string opt) const;
  long                ArgAsLong         (std::string opt) const;
  std::vector<double> ArgAsDoubleTokens (std::string opt, std::string delimeter) const;

  
  // Method to make sure all arguments given been identified
  bool RecognisedAllArgs() const;

private:

  int                 fArgc;
  const char* const*  fArgv;
  
  // Saves whether each argument has been used or not.
  mutable std::vector<bool> fArgUsed;
  
};

}      // valor namespace

#endif // _CMD_LINE_ARG_PARSER_H_
