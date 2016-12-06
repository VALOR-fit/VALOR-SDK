#ifndef CACHEITEM_H
#define CACHEITEM_H

#include <vector>

namespace valor
{
namespace util
{
   class CacheItem
   {
   public:
      CacheItem();

      /**
         Constructor.

         \param data The data to be stored.
         \param size The size (in bytes) of the data to be stored.
      */
      CacheItem(char* data, unsigned long size);

      virtual ~CacheItem();

      /**
         Returns the data for this cache item.

         \return True if the cache status is good, false otherwise.
      */
      char* GetData(){ return _data; }

      /**
         Sets the data for this cache.

         \param data The data to be stored.
         \param size The size (in bytes) of the data to be stored.
      */
      void SetData(const char* data, unsigned long size);

      /**
         Retrieves the size of the data for this cache. This does not include
         the size of child cache items.
      */
      unsigned long GetDataSize(){ return _size; }

      /**
         Retrieve a child cache item from this cache item.

         \param index The index of the child item to retrieve.
      */
      CacheItem* GetChild(unsigned int index);

      /**
         Add a child cache item to this cache item/

         \param child The cache item to be added.
      */
      void AddChild(CacheItem* child){ _children.push_back(child); }

      /**
         Retrieve the number of children for this cache item.
      */
      int GetNumChildren(){ return _children.size(); }

      /**
         Builds a cache item from a block of raw data.

         \param data The data from which the item is to be built.

         \return The number of bytes processed for the build.
      */
      unsigned long Build(const char* data);

      /**
         Flattens a cache item into a block of raw data.

         \return The number of bytes processed for the build.
      */
      const char* Flatten();

      /**
         Returns the size of the cache item on disk, including all children and
         descriptor variables.

         \return The size of the cache on disk.
      */
      unsigned long GetDiskSize();

   private:
      /**
         Flattens a cache item into a block of raw data.

         \param data The data block into which the cache item should be flattened.

         \return The number of bytes flattened.
      */
      unsigned long Flatten(char* data);

   private:
      char* _data;
      unsigned long _size;
      std::vector<CacheItem*> _children;
   };
}
}

#endif
