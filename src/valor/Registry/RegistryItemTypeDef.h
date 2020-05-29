//____________________________________________________________________________
/*!

\class     valor::RegistryItemTypeDef  (Copied/Adapted from GENIE.)

\brief     Definition of Registry item types

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Lab

\created   October 20, 2006

\copyright Copyright (c) 2003-2020, The GENIE Collaboration

*/
//____________________________________________________________________________

#ifndef _valor_registry_item_type_def_h_
#define _valor_registry_item_type_def_h_

#include <map>
#include <string>
#include <ostream>
#include <vector>

#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>

typedef std::string  RgKey;
typedef int          RgInt;
typedef bool         RgBool;
typedef double       RgDbl;
typedef std::string  RgStr;
typedef const char * RgCChAr;
typedef TH1F       * RgH1F;
typedef TH2F       * RgH2F;
typedef TTree      * RgTree;
typedef TObject    * RgObject;
typedef std::vector<double> RgDblVec;

#endif // _valor_registry_item_type_def_h_
