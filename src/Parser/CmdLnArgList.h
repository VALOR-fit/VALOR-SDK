//____________________________________________________________________________
/*!

\class     valor::CmdLnArgList

\brief     Holds command-line arguments.
           
         # argument   |   type       |  require?   |   default for optional
         # ...................................................................
           ntoys      |   integer    |  optional   |   1000
           input-file |   string     |  mandatory  |
           fluctuate  |   boolean    |  optional   |   true

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   April 08, 2016

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef _valor_cmd_line_arg_list_h_
#define _valor_cmd_line_arg_list_h_

#include <string>
#include <vector>

#include <TObject.h>

#include "Registry/Registry.h"

using std::string;
using std::vector;

namespace valor {

class CmdLnArgList: public TObject {

public:
  CmdLnArgList();
 ~CmdLnArgList();

  bool Configure (string file_list);
  bool Parse     (int argc, char ** argv);

private:

  vector<string> fConfigFileContent;
  Registry       fArgs;
};

}

#endif
