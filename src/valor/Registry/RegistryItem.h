//____________________________________________________________________________
/*!

\class     valor::RegistryItem  (Copied/Adapted from GENIE.)

\brief     A templated concrete implementation of the RegistryItemI interface.
           Provides an arbitrary basic type (bool, int, double, string) value
           for RegistryI-type containers.

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Lab

\created   May 06, 2004

\copyright Copyright (c) 2003-2020, The GENIE Collaboration

*/
//____________________________________________________________________________

#ifndef _valor_registry_item_h_
#define _valor_registry_item_h_

#include <map>
#include <string>
#include <ostream>

#include "valor/Registry/RegistryItemI.h"
#include "valor/Registry/RegistryItemTypeDef.h"

namespace valor {

template<typename T> class RegistryItem;
template<typename T>
  std::ostream & operator << (std::ostream & stream, const RegistryItem<T> & rec);

template<typename T> class RegistryItem : public RegistryItemI {

public:
  RegistryItem() { };
  RegistryItem(T item, bool locked=false, bool local=true);
  RegistryItem(const RegistryItem * ri);
  ~RegistryItem();

  RegistryItemI * Clone    (void) const;
  RgType_t        TypeInfo (void) const;
  const T &       Data     (void) const {  return fItem;       }
  bool            IsLocked (void) const {  return fIsLocked;   }
  void            Lock     (void)       {  fIsLocked = true;   }
  void            UnLock   (void)       {  fIsLocked = false;  }
  bool            IsLocal  (void) const {  return fIsLocal;    }
  void            SetLocal (bool isloc) {  fIsLocal  = isloc;  }

  void Print(std::ostream& stream) const;

  friend std::ostream & operator <<
              <T>(std::ostream & stream, const RegistryItem<T> & rec);

private:

  T    fItem;
  bool fIsLocked;
  bool fIsLocal;
};

}      // valor namespace

#endif // _valor_registry_item_h_
