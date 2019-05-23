//____________________________________________________________________________
/*!

\class     valor::RegistryItemTypeId (Copied/Adapted from GENIE.)

\brief     An enumeration of Registry item types

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Lab

\created   October 20, 2006
 
\copyright (c) 2003-2016, GENIE Neutrino MC Generator Collaboration

*/
//____________________________________________________________________________

#ifndef _valor_registry_item_type_id_h_
#define _valor_registry_item_type_id_h_

#include <string>
#include <vector>

namespace valor {

typedef enum ERgType {

  kRgUndefined = 0,
  kRgBool,
  kRgInt,
  kRgDbl,
  kRgStr,
  kRgH1F,
  kRgH2F,
  kRgTree,
  kRgObject,
  kRgDblVec

} RgType_t;

class RgType {

public:
  static std::string AsString(RgType_t rt) 
  {
    switch (rt) {
      case (kRgUndefined) : return "undefined";    break;
      case (kRgBool)      : return "bool";         break;
      case (kRgInt)       : return "int";          break;
      case (kRgDbl)       : return "double";       break;
      case (kRgStr)       : return "string";       break;
      case (kRgH1F)       : return "h1f";          break;
      case (kRgH2F)       : return "h2f";          break;
      case (kRgTree)      : return "tree";         break;
      case (kRgObject)    : return "object";       break;
      case (kRgDblVec)    : return "double_vector";break;
      default             : return "undefined";
    }
  }
};

}        // valor namespace
#endif   // _valor_registry_item_type_id_h_
