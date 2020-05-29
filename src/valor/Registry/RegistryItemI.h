//____________________________________________________________________________
/*!

\class     valor::RegistryItemI
           (Copied/Adapted from GENIE.)

\brief     Registry item pABC

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Lab

\created   May 04, 2004

\copyright Copyright (c) 2003-2020, The GENIE Collaboration

*/
//____________________________________________________________________________

#ifndef _valor_registry_item_pabc_h_
#define _valor_registry_item_pabc_h_

#include <iostream>

#include "valor/Registry/RegistryItemTypeId.h"

namespace valor {

class RegistryItemI 
{
public:
  virtual ~RegistryItemI() { }

  virtual RegistryItemI * Clone    (void)           const = 0;
  virtual RgType_t        TypeInfo (void)           const = 0;
  virtual bool            IsLocked (void)           const = 0;
  virtual void            Lock     (void)                 = 0;
  virtual void            UnLock   (void)                 = 0;
  virtual bool            IsLocal  (void)           const = 0;
  virtual void            SetLocal (bool)                 = 0;
  virtual void            Print    (std::ostream &) const = 0;

protected:

  RegistryItemI() { }
};

}      // valor namespace

#endif // _valor_registry_item_pabc_h_
