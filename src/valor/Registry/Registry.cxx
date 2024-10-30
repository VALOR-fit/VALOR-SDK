//____________________________________________________________________________
/*
 Copyright (c) 2003-2020, The GENIE Collaboration

 Author: Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
         University of Liverpool & STFC Rutherford Appleton Lab 
*/
//____________________________________________________________________________

#include <cassert>
#include <cstdlib>
#include <sstream>
#include <iomanip>

#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TObjString.h>

#include "valor/Messenger/Messenger.h"
#include "valor/Registry/Registry.h"
#include "valor/Registry/RegistryItemTypeId.h"

using namespace valor;

using std::string;
using std::setw;
using std::setfill;
using std::ostream;
using std::istream;
using std::cout;
using std::endl;
using std::ostringstream;

//____________________________________________________________________________
namespace valor {
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 template<> void SetRegistryItem(Registry * r, RgKey key, RgDblVec item)
 {
    string itemtype = typeid(item).name();
    
    ostringstream item_content;
    item_content << "[";
    for (size_t i = 0 ; i < item.size() ; i++) {
      item_content << (i?",":"")<<item[i];
    }
    item_content<<"]";
    VLOG("Reg", pINFO)
       << "Set item [" << itemtype << "]: key = "
       << key << " --> value = "<<item_content.str();
    bool lock = r->ItemIsLocked(key); // store, could be true but inhibited
    RegistryItem<RgDblVec> * reg_item = new RegistryItem<RgDblVec>(item,lock);
    RgIMapPair config_entry(key, reg_item);
    r->Set(config_entry);
 }
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 template<class T> void SetRegistryItem(Registry * r, RgKey key, T item)
 {
    string itemtype = typeid(item).name();
    VLOG("Reg", pINFO)
       << "Set item [" << itemtype << "]: key = "
       << key << " --> value = " << item;
    bool lock = r->ItemIsLocked(key); // store, could be true but inhibited
    RegistryItem<T> * reg_item = new RegistryItem<T>(item,lock);
    RgIMapPair config_entry(key, reg_item);
    r->Set(config_entry);
 }
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 template<class T> T GetValueOrUseDefault(
                          Registry * r, RgKey key, T def, bool set_def)
 {
    // Return the requested registry item. If it does not exist return
    // the input default value (in this case, if set_def is true it can 
    // override a lock and add the input default as a new registry item)
    
    T value;
    if(r->Exists(key)) { 
       if(r->ItemIsLocal(key)) {
          r->Get(key,value); 
          return value;
       }
    }
    value = def;
    bool was_locked = r->IsLocked();
    if(was_locked) r->UnLock();
    
    if(set_def) {
        r->Set(key, value);
        r->LinkToGlobalDef(key);
     }
     if(was_locked) r->Lock();
     return value;
 }
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ostream & operator << (ostream & stream, const Registry & registry)
 {
   registry.Print(stream);
   return stream;
 }
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}
//____________________________________________________________________________
Registry::Registry() 
{
  this->Init();
}
//____________________________________________________________________________
Registry::Registry(string name, bool isReadOnly) :
fName             ( name       ),
fIsReadOnly       ( isReadOnly ),
fInhibitItemLocks ( false      )
{

}
//____________________________________________________________________________
Registry::Registry(const Registry & registry) :
fIsReadOnly(false)
{
  this->Copy(registry);
}
//____________________________________________________________________________
Registry::~Registry()
{
  this->Clear(true);
}
//____________________________________________________________________________
void Registry::operator() (RgKey key, RgInt item)
{
  this->Set(key, item);
}
//____________________________________________________________________________
void Registry::operator() (RgKey key, RgBool item)
{
  this->Set(key, item);
}
//____________________________________________________________________________
void Registry::operator() (RgKey key, RgDbl item)
{
  this->Set(key, item);
}
//____________________________________________________________________________
void Registry::operator() (RgKey key, RgCChAr item)
{
  RgStr item2 = RgStr(item); // "const char *" -> "string"
  this->Set(key,item2);
}
//____________________________________________________________________________
void Registry::operator() (RgKey key, RgStr item)
{
  this->Set(key, item);
}
//____________________________________________________________________________
Registry & Registry::operator = (const Registry & reg)
{
  this->Copy(reg);
  return (*this);
}
//____________________________________________________________________________
Registry & Registry::operator += (const Registry & reg)
{
  this->Append(reg);
  return (*this);
}
//____________________________________________________________________________
void Registry::Lock(void)
{
  fIsReadOnly = true;
}
//____________________________________________________________________________
void Registry::UnLock(void)
{
  fIsReadOnly = false;
}
//____________________________________________________________________________
bool Registry::IsLocked(void) const
{
  return fIsReadOnly;
}
//____________________________________________________________________________
void Registry::InhibitItemLocks(void)
{
  fInhibitItemLocks = true;
}
//____________________________________________________________________________
void Registry::RestoreItemLocks(void)
{
  fInhibitItemLocks = false;
}
//____________________________________________________________________________
bool Registry::ItemLocksAreActive(void) const
{
  return !fInhibitItemLocks;
}
//____________________________________________________________________________
bool Registry::ItemIsLocal(RgKey key) const
{
  if( this->Exists(key) ) {
     RgIMapConstIter entry = fRegistry.find(key);
     bool is_local = entry->second->IsLocal();
     return is_local;
  } 
  return false;
}
//____________________________________________________________________________
void Registry::OverrideGlobalDef(RgKey key)
{
  if( this->Exists(key) ) {
     RgIMapConstIter entry = fRegistry.find(key);
     entry->second->SetLocal(true);
  } else {
     VLOG("Reg", pWARN)
        << "*** Can't give 'local' status to  non-existem item [" 
        << key << "]"; 
  }
}
//____________________________________________________________________________
void Registry::LinkToGlobalDef(RgKey key)
{
  if( this->Exists(key) ) {
     RgIMapConstIter entry = fRegistry.find(key);
     entry->second->SetLocal(false);
  } else {
     VLOG("Reg", pWARN)
        << "*** Can't give 'global' status to  non-existem item [" 
        << key << "]"; 
  }
}
//____________________________________________________________________________
bool Registry::ItemIsLocked(RgKey key) const
{
  if( this->Exists(key) ) {
     RgIMapConstIter entry = fRegistry.find(key);
     bool is_locked = entry->second->IsLocked();
     return is_locked;
  } 
  return false;
}
//____________________________________________________________________________
void Registry::LockItem(RgKey key)
{
  if( this->Exists(key) ) {
     RgIMapConstIter entry = fRegistry.find(key);
     entry->second->Lock();
  } else {
     VLOG("Reg", pWARN)
           << "*** Can't lock non-existem item [" << key << "]";
  }
}
//____________________________________________________________________________
void Registry::UnLockItem(RgKey key)
{
  if( this->Exists(key) ) {
     RgIMapConstIter entry = fRegistry.find(key);
     entry->second->UnLock();
  } else {
    VLOG("Reg", pWARN)
       << "*** Can't unlock non-existem item [" << key << "]";
  }
}
//____________________________________________________________________________
bool Registry::CanSetItem(RgKey key) const
{
  bool locked_item       = this->ItemIsLocked(key);
  bool active_item_locks = this->ItemLocksAreActive();
  bool locked_registry   = this->IsLocked();

  bool can_set = !locked_registry && ( !locked_item || !active_item_locks );

  return can_set;
}
//____________________________________________________________________________
void Registry::Set(RgIMapPair entry)
{
  RgKey key = entry.first;
  if( this->CanSetItem(key) ) {
    this->DeleteEntry(key);
    fRegistry.insert(entry);
  } else {
     VLOG("Reg", pWARN)
       << "*** Registry item [" << key << "] can not be set";
  }
}
//____________________________________________________________________________
void Registry::Set(RgKey key, RgBool item)
{
  SetRegistryItem(this, key, item); // call templated set method
}
//____________________________________________________________________________
void Registry::Set(RgKey key, RgInt item)
{
  SetRegistryItem(this, key, item); // call templated set method
}
//____________________________________________________________________________
void Registry::Set(RgKey key, RgDbl item)
{
  SetRegistryItem(this, key, item); // call templated set method
}
//____________________________________________________________________________
void Registry::Set(RgKey key, RgCChAr item)
{
  RgStr item2 = RgStr(item); // "const char *" -> "string"
  this->Set(key, item2);
}
//____________________________________________________________________________
void Registry::Set(RgKey key, RgStr item)
{
  SetRegistryItem(this, key, item); // call templated set method
}
//____________________________________________________________________________
void Registry::Set(RgKey key, RgH1F item)
{
  SetRegistryItem(this, key, item); // call templated set method
}
//____________________________________________________________________________
void Registry::Set(RgKey key, RgH2F item)
{
  SetRegistryItem(this, key, item); // call templated set method
}
//____________________________________________________________________________
void Registry::Set(RgKey key, RgTree item)
{
  SetRegistryItem(this, key, item); // call templated set method
}
//____________________________________________________________________________
void Registry::Set(RgKey key, RgObject item)
{
  SetRegistryItem(this, key, item); // call templated set method
}
//____________________________________________________________________________
void Registry::Set(RgKey key, RgDblVec item)
{
  SetRegistryItem(this, key, item); // call templated set method
}
//____________________________________________________________________________
void Registry::Get(RgKey key, const RegistryItemI * & item) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  item = entry->second;
}
//____________________________________________________________________________
void Registry::Get(RgKey key, RgBool & item) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgBool> * ri = dynamic_cast<RegistryItem<RgBool>*> (rib);
  item = ri->Data();
}
//____________________________________________________________________________
void Registry::Get(RgKey key, RgInt & item) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgInt> * ri = dynamic_cast< RegistryItem<RgInt> * > (rib);
  item = ri->Data();
}
//____________________________________________________________________________
void Registry::Get(RgKey key, RgDbl & item) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgDbl> * ri = dynamic_cast<RegistryItem<RgDbl>*> (rib);
  item = ri->Data();
}
//____________________________________________________________________________
void Registry::Get(RgKey key, RgStr & item) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgStr> * ri = dynamic_cast<RegistryItem<RgStr>*> (rib);
  item = ri->Data();
}
//____________________________________________________________________________
void Registry::Get(RgKey key, RgH1F & item) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgH1F> *ri = dynamic_cast<RegistryItem<RgH1F>*> (rib);
  item = ri->Data();

  if(!item) {
    VLOG("Reg", pWARN) << "Returned NULL ptr for TH1F param = " << key;
  }
}
//____________________________________________________________________________
void Registry::Get(RgKey key, RgH2F & item) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgH2F> *ri = dynamic_cast<RegistryItem<RgH2F>*> (rib);
  item = ri->Data();

  if(!item) {
    VLOG("Reg", pWARN) << "Returned NULL ptr for TH2F param = " << key;
  }
}
//____________________________________________________________________________
void Registry::Get(RgKey key, RgTree & item) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgTree> *ri =  dynamic_cast<RegistryItem<RgTree>*> (rib);
  item = ri->Data();

  if(!item) {
    VLOG("Reg", pWARN) << "Returned NULL ptr for TTree param = " << key;
  }
}
//____________________________________________________________________________
void Registry::Get(RgKey key, RgObject & item) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgObject> *ri =  dynamic_cast<RegistryItem<RgObject>*> (rib);
  item = ri->Data();

  if(!item) {
    VLOG("Reg", pWARN) << "Returned NULL ptr for TObject param = " << key;
  }
}
//____________________________________________________________________________
void Registry::Get(RgKey key, RgDblVec & item) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgDblVec> * ri = dynamic_cast<RegistryItem<RgDblVec>*> (rib);
  item = ri->Data();
}
//____________________________________________________________________________
RgBool Registry::GetBool(RgKey key) const
{
  RgBool value;
  this->Get(key, value);
  return value;
}
//____________________________________________________________________________
RgInt Registry::GetInt(RgKey key) const
{
  RgInt value;
  this->Get(key, value);
  return value;
}
//____________________________________________________________________________
RgDbl Registry::GetDouble(RgKey key) const
{
  RgDbl value;
  this->Get(key, value);
  return value;
}
//____________________________________________________________________________
RgStr Registry::GetString(RgKey key) const
{
  RgStr value;
  this->Get(key, value);
  return value;
}
//____________________________________________________________________________
RgH1F Registry::GetH1F(RgKey key) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgH1F> *ri = dynamic_cast<RegistryItem<RgH1F>*> (rib);

  RgH1F item = ri->Data();
  return item;
}
//____________________________________________________________________________
RgH2F Registry::GetH2F(RgKey key) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgH2F> *ri = dynamic_cast<RegistryItem<RgH2F>*> (rib);

  RgH2F item = ri->Data();
  return item;
}
//____________________________________________________________________________
RgTree Registry::GetTree(RgKey key) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgTree> *ri = dynamic_cast<RegistryItem<RgTree>*> (rib);

  RgTree item = ri->Data();
  return item;
}
//____________________________________________________________________________
RgObject Registry::GetObject(RgKey key) const
{
  RgIMapConstIter entry = this->SafeFind(key);
  RegistryItemI * rib = entry->second;
  RegistryItem<RgObject> *ri = dynamic_cast<RegistryItem<RgObject>*> (rib);

  RgObject item = ri->Data();
  return item;
}
//____________________________________________________________________________
RgDblVec Registry::GetDoubleVec(RgKey key) const
{
  RgDblVec value;
  this->Get(key, value);
  return value;
}
//____________________________________________________________________________
RgBool Registry::GetBoolDef(RgKey key, RgBool def_opt, bool set_def) 
{
  return GetValueOrUseDefault(this, key, def_opt, set_def);
}
//____________________________________________________________________________
int Registry::GetIntDef(RgKey key, int def_opt, bool set_def)
{
  return GetValueOrUseDefault(this, key, def_opt, set_def);
}
//____________________________________________________________________________
double Registry::GetDoubleDef(RgKey key, double def_opt, bool set_def) 
{
  return GetValueOrUseDefault(this, key, def_opt, set_def);
}
//____________________________________________________________________________
string Registry::GetStringDef(RgKey key, string def_opt, bool set_def) 
{
  return GetValueOrUseDefault(this, key, def_opt, set_def);
}
//____________________________________________________________________________
RgIMapConstIter Registry::SafeFind(RgKey key) const
{
  RgIMapConstIter entry = fRegistry.find(key);
  if (entry!=fRegistry.end()) {
    return entry;
  }
  VLOG("Reg", pFATAL)
       << "*** Key: " << key
       << " does not exist in registry: " << this->Name();
  exit(1);    
}
//____________________________________________________________________________
bool Registry::Exists(RgKey key) const
{
  RgIMapConstIter entry = fRegistry.find(key);
  return (entry!=fRegistry.end());
}
//____________________________________________________________________________
bool Registry::DeleteEntry(RgKey key)
{
  if(!fIsReadOnly && Exists(key)) {
      RgIMapIter entry = fRegistry.find(key);
      RegistryItemI * item = entry->second;
      delete item;
      item = 0;
      fRegistry.erase(entry);
      return true;
  }
  return false;
}
//____________________________________________________________________________
int Registry::NEntries(void) const
{
  RgIMapSizeType reg_size = fRegistry.size();
  return (int) reg_size;
}
//____________________________________________________________________________
void Registry::SetName(string name)
{
  if(! fIsReadOnly) fName = name;
  else {
     VLOG("Reg", pWARN)
        << "*** Registry is locked - Can not change its name";
  }
}
//____________________________________________________________________________
string Registry::Name(void) const
{
  return fName;
}
//____________________________________________________________________________
void Registry::AssertExistence(RgKey key0) const
{
  if ( ! this->Exists(key0) ) {
     VLOG("Reg", pERROR) << (*this);
     VLOG("Reg", pFATAL)
        << "*** Key: " << key0
        << " does not exist in registry: " << this->Name();
     exit(1);    
  }
}
//____________________________________________________________________________
void Registry::AssertExistence(RgKey key0, RgKey key1) const
{
  this->AssertExistence(key0);
  this->AssertExistence(key1);
}
//____________________________________________________________________________
void Registry::AssertExistence(RgKey key0, RgKey key1, RgKey key2) const
{
  this->AssertExistence(key0);
  this->AssertExistence(key1);
  this->AssertExistence(key2);
}
//____________________________________________________________________________
void Registry::Print(ostream & stream) const
{
// Prints the registry to the specified stream
//
   stream << endl;
   stream << "[-] Registry name: [" << Name() << "]";

   stream << " - Write Status: ";
   if(fIsReadOnly)       { stream << "[locked]";   }
   else                  { stream << "[unlocked]"; }

   stream << " - Inhibited Item Locking: ";
   if(fInhibitItemLocks) { stream << "[on]";       }
   else                  { stream << "[off]";      }

   stream << " - # entries: " << setfill(' ') << setw(3) << fRegistry.size()
          << endl;

   RgIMapConstIter rcit = fRegistry.begin();
   for( ; rcit != fRegistry.end(); rcit++) {

     RgKey           key   = rcit->first;
     RegistryItemI * ritem = rcit->second;
     if(ritem) {
        RgType_t type  = ritem->TypeInfo();
        string   stype = RgType::AsString(type);

        string key_lbl  = string("> ") + key;
        string type_lbl = string("[") + stype + string("] ");
        stream << " |" << setfill('-') << setw(50) << key_lbl
               << setfill(' ') << setw(10) << type_lbl;
        ritem->Print(stream);
        stream << endl;
     } else {
        VLOG("Reg", pERROR) << "Null RegistryItemI with key = " << key;
     }
   }// registry iterator
}
//____________________________________________________________________________
void Registry::Copy(const Registry & registry)
{
// Copies the input registry
//
  VLOG("Reg", pINFO) 
     << "Copying registry " << registry.Name() << " to " << this->Name();

  if(this->IsLocked()) {
   VLOG("Reg", pWARN) << "Registry is locked. Can't copy input entries!";
   return;
  }

  this->Init();
  this->Clear();
  this->Append(registry);

  fName             = registry.fName;
  fIsReadOnly       = registry.fIsReadOnly;
  fInhibitItemLocks = registry.fInhibitItemLocks;
}
//____________________________________________________________________________
void Registry::Append(const Registry & registry, RgKey prefix)
{
// Appends the input registry entries (& their locks)

  VLOG("Reg", pINFO) 
     << "Appending registry " << registry.Name() << " to " << this->Name();

  if(this->IsLocked()) {
   VLOG("Reg", pWARN) << "Registry is locked. Can't copy input entries!";
   return;
  }

  this->InhibitItemLocks();

  RgIMapConstIter reg_iter;
  for(reg_iter = registry.fRegistry.begin();
                      reg_iter != registry.fRegistry.end(); reg_iter++) {

     RgKey name     = reg_iter->first;
     RgKey new_name = prefix + name;

     RegistryItemI * ri  = reg_iter->second;

     bool     ilk   = ri->IsLocked();
     RgType_t type  = ri->TypeInfo();
     string   stype = RgType::AsString(type);

     VLOG("Reg", pINFO)
         << "Copying [" << stype << "] item named = " 
         << name << " as " << new_name;

     RegistryItemI * cri = 0; // cloned registry item
     if (type == kRgBool)
           cri = new RegistryItem<RgBool>(registry.GetBool(name),ilk);
     else if (type == kRgDbl)
           cri = new RegistryItem<RgDbl> (registry.GetDouble(name),ilk);
     else if (type == kRgInt)
           cri = new RegistryItem<RgInt> (registry.GetInt(name),ilk);
     else if (type == kRgStr)
           cri = new RegistryItem<RgStr> (registry.GetString(name),ilk);
     else if (type == kRgDblVec)
           cri = new RegistryItem<RgDblVec> (registry.GetDoubleVec(name),ilk);
     else if (type == kRgH1F) {
           RgH1F histo = registry.GetH1F(name);
           if(histo) {
               RgH1F chisto = new TH1F(*histo);
               VLOG("Reg", pDEBUG) << chisto->GetName();
               cri = new RegistryItem<RgH1F>(chisto,ilk);
           } else {
             VLOG("Reg", pERROR)
               << "Null TH1F with key = " << name << " - not copied";
           }
     } else if (type == kRgH2F) {
           RgH2F histo = registry.GetH2F(name);
           if(histo) {
               RgH2F chisto = new TH2F(*histo);
               VLOG("Reg", pDEBUG) << chisto->GetName();
               cri = new RegistryItem<RgH2F>(chisto,ilk);
           } else {
             VLOG("Reg", pERROR)
               << "Null TH2F with key = " << name << " - not copied";
           }
     } else if (type == kRgTree) {
           RgTree tree = registry.GetTree(name);
           if(tree) {
               //TTree * ctree = new TTree(*tree);
               TTree * ctree = tree->CopyTree("1");
               VLOG("Reg", pDEBUG) << ctree->GetName();
               cri = new RegistryItem<RgTree>(ctree,ilk);
           } else {
             VLOG("Reg", pERROR)
               << "Null TTree with key = " << name << " - not copied";
           }
     } else if (type == kRgObject) {
           RgObject obj = registry.GetObject(name);
           if(obj) {
             TObject * cobj = obj->Clone();
             VLOG("Reg", pDEBUG) << cobj->GetName();
             cri = new RegistryItem<RgObject>(cobj,ilk);
           } else {
             VLOG("Reg", pERROR)
               << "Null TObject with key = " << name << " - not copied";
           }
     } else {}

     RgIMapPair reg_entry(new_name, cri);
     fRegistry.insert(reg_entry);
   }
}
//____________________________________________________________________________
RgType_t Registry::ItemType(RgKey key) const
{
  RgIMapConstIter reg_iter = fRegistry.find(key);
  if(reg_iter != fRegistry.end()) {
     RegistryItemI * ri = reg_iter->second;
     RgType_t type  = ri->TypeInfo();
     return type;
  }
  return kRgUndefined;
}
//____________________________________________________________________________
RgKeyList Registry::FindKeys(RgKey key_part) const
{
  RgKeyList klist;

  RgIMapConstIter reg_iter = fRegistry.begin();
  for( ; reg_iter != fRegistry.end(); reg_iter++) {
    RgKey key = reg_iter->first;
    if (key.find(key_part) != string::npos) {
      klist.push_back(key);
    }
  }

  return klist;
}
//____________________________________________________________________________
void Registry::Init(void)
{
// initialize registry properties

  fName              = "NoName";
  fIsReadOnly        = false;
  fInhibitItemLocks  = false;
}
//____________________________________________________________________________
void Registry::Clear(bool force)
{
// clean all registry entries

  //~ VLOG("Reg", pINFO) 
      //~ << "Cleaning-up [force unlock = " << ((force)?"true":"false")
      //~ << "] registry: " << this->Name();
  if(!force) {
   if(this->IsLocked()) {
      VLOG("Reg", pWARN) << "Registry is locked. Can't clear its entries";
      return;
    }
  }
  RgIMapIter rit;
  for(rit = fRegistry.begin(); rit != fRegistry.end(); rit++) {
     RgKey           name = rit->first;
     RegistryItemI * item = rit->second;
     if(!item) {
       VLOG("Reg", pWARN) << "Item with key = " << name << " is null!";
     }
     delete item;
     item = 0;
  }
  fRegistry.clear();
}
//____________________________________________________________________________

