//____________________________________________________________________________
/*!

\class     valor::RegistryItemTypeDef  (Copied/Adapted from GENIE.)

\brief     Definition of Registry item types

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Lab

\created   October 20, 2006

\copyright (c) 2003-2016, GENIE Neutrino MC Generator Collaboration

*/
//____________________________________________________________________________

#ifndef _valor_registry_item_type_def_h_
#define _valor_registry_item_type_def_h_

#include <map>
#include <string>
#include <ostream>

#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>

using std::pair;
using std::string;
using std::ostream;

typedef string       RgKey;
typedef int          RgInt;
typedef bool         RgBool;
typedef double       RgDbl;
typedef string       RgStr;
typedef const char * RgCChAr;
typedef TH1F *       RgH1F;
typedef TH2F *       RgH2F;
typedef TTree *      RgTree;
typedef TObject *    RgObject;

#endif // _valor_registry_item_type_def_h_
