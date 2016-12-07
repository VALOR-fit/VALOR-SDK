#include "valor/Utilities/CacheItem.h"

#include <cstring>
#include "valor/Messenger/Messenger.h"

namespace valor
{
namespace util
{
   CacheItem::CacheItem() : _data(0), _size(0)
   {

   }

   CacheItem::CacheItem(char* data, unsigned long size): _data(0), _size(0)
   {
      SetData(data, size);
   }

   CacheItem::~CacheItem()
   {
      delete[] _data;
      for(std::vector<CacheItem*>::iterator it = _children.begin();
            it != _children.end(); ++it)
      {
         delete *it;
      }
      _children.clear();
   }

   void CacheItem::SetData(const char* data, unsigned long size)
   {
      delete[] _data;
      _data = new char[size];
      std::memcpy(_data, data, size);
      _size = size;
   }

   CacheItem* CacheItem::GetChild(unsigned int index)
   {
      if(index >= _children.size())
      {
         LOG("CacheItem", pFATAL) << "Attempt to retrieve child " << index <<
            ". Index of last child " << (_children.size() - 1);
         exit(1);
      }

      return _children[index];
   }

   unsigned long CacheItem::Build(const char* data)
   {
      unsigned long offset = 0;
      unsigned long ciSize = 0;
      // Read size of entire cache item (including children)
      std::memcpy(&ciSize, data, sizeof(ciSize)); offset += sizeof(ciSize);
      // Read size of data specific to this cache item
      std::memcpy(&_size, data + offset, sizeof(_size)); offset += sizeof(_size);
      // Retrieve the local data
      // Actual data starts after the header info
      SetData(data + offset, _size); offset += _size;

      // Retrieve the number of child cache items
      int numChildren = 0;
      std::memcpy(&numChildren, data + offset, sizeof(numChildren)); offset += sizeof(numChildren);
      // Retrieve the children (if any)
      for(int i = 0; i < numChildren; ++i)
      {
         CacheItem* child = new CacheItem();
         _children.push_back(child);
         offset += child->Build(data + offset);
      }

      return offset;
   }

   const char* CacheItem::Flatten()
   {
      // Determine the size of this cache item and all children
      unsigned long ciSize = GetDiskSize();
      char* data = new char[ciSize];

      unsigned long offset = 0;
      // Write the size of the entire cache item (including children)
         offset;
      std::memcpy(data, &ciSize, sizeof(ciSize)); offset += sizeof(ciSize);
      // Write size of data specific to this cache item
      std::memcpy(data + offset, &_size, sizeof(_size)); offset += sizeof(_size);
      // Write the local data
      std::memcpy(data + offset, _data, _size); offset += _size;

      // Write the number of children
      int numChildren = _children.size();
      std::memcpy(data + offset, &numChildren, sizeof(numChildren)); offset += sizeof(numChildren);
      // Write the children
      for(std::vector<CacheItem*>::iterator it = _children.begin();
            it != _children.end(); ++it)
      {
         CacheItem* child = *it;
         offset += child->Flatten(data + offset);
      }

      return data;
   }

   unsigned long CacheItem::Flatten(char* data)
   {
      unsigned long offset = 0;

      // Determine the size of this cache item and all children
      unsigned long ciSize = GetDiskSize();

      // Write the size of the entire cache item (including children)
      std::memcpy(data, &ciSize, sizeof(ciSize)); offset += sizeof(ciSize);
      // Write size of data specific to this cache item
      std::memcpy(data + offset, &_size, sizeof(_size)); offset += sizeof(_size);
      // Write the local data
      std::memcpy(data + offset, _data, _size); offset += _size;

      // Write the number of children
      int numChildren = _children.size();
      std::memcpy(data + offset, &numChildren, sizeof(numChildren)); offset += sizeof(numChildren);
      // Write the children
      for(std::vector<CacheItem*>::iterator it = _children.begin();
            it != _children.end(); ++it)
      {
         CacheItem* child = *it;
         offset += child->Flatten(data + offset);
      }

      return offset;
   }

   unsigned long CacheItem::GetDiskSize()
   {
      // Size of this cache item's data
      unsigned long size = GetDataSize();
      // Size of this cache items children
      for(std::vector<CacheItem*>::iterator it = _children.begin();
            it != _children.end(); ++it)
      {
         CacheItem* child = *it;
         size += child->GetDiskSize();
      }
      // Size of the descriptors stating total cache item size, local cache
      // item size and number of children
      size += 2 * sizeof(long) + sizeof(int);

      return size;
   }
}
}

