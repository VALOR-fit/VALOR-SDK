//____________________________________________________________________________
/*!

\class     valor::CmdLnArgList

\brief     Holds command-line arguments.

           It is configured with argument specification files formatted as
           follows (lines starting with # are neglected):

           # argument       |   type         |  require?   |   default 
           # ..........................................................
             ntoys          |   integer      |  optional   |   1000
             dm32sq         |   double       |  optional   |   0.00232
             mixing32       |   doublevec    |  mandatory  |   0.5,0.513
             input-file     |   string       |  mandatory  |   -
             with-xyz       |   boolean      |  optional   |   -         (a)
             red,green,blue |   boolean_list |  mandatory  |   -         (b)
             red,green,blue |   boolean_list |  optional   |   red       (c)

           (a) No default needed / can only be optional.
               If `--with-xyz' is not specified, then without xyz is  used.
           (b) One of the three _must_ be specified.
               The list will contain all `red',`green',`blue' entries.
               One will be true, while the other two will be false.
           (c) No need to specify.
               The list will contain all `red',`green',`blue' entries.
               One will be true, while the other two will be false.
               If none is specified, red->true, green->false, blue->false

           Multiple argument specification files can be supplied using a 
           colon separated list (e.g. "seeds.arg:toys.arg:fitter.arg")
           to allow granular organisation of command-line arguments.

           NOTE: In future could expand argument specifications to hold
                 integer_list, double_list, string_list, or 
                 root objects (th1f, th2f, ttree, tfile, tgraph,...)

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   April 08, 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_cmd_line_arg_list_h_
#define _valor_cmd_line_arg_list_h_

#include <string>
#include <vector>

#include "valor/Registry/Registry.h"

using std::string;
using std::vector;

namespace valor {

class CmdLnArgList: public Registry {

public:
  CmdLnArgList();
 ~CmdLnArgList();

  bool Read (int argc, const char* const* argv, string arg_spec_files);
};

}

#endif
