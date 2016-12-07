#include "valor/Utilities/ReadableCache.h"
#include "valor/Messenger/Messenger.h"
#include <sstream>
#include <TMath.h>

namespace valor
{
namespace util
{
   ReadableCache* ReadableCache::_instance = 0;

   ReadableCache::ReadableCache(std::string name, const int numToys,
         const int maxToysPerFile):
      _numToysPerFile(maxToysPerFile),
      _numFiles(TMath::Ceil(static_cast<double>(numToys) /
               static_cast<double>(maxToysPerFile))),
      _cache(0)
   {
      std::ostringstream headerName;
      headerName << name << ".head";
      _hStream = new std::fstream(headerName.str().c_str(), std::ios::binary |
            std::ios::in);
      int numToysFromFile = 0;
      _hStream->read(reinterpret_cast<char*>(&numToysFromFile),
            sizeof(numToysFromFile));
      for(int i = 0; i < _numFiles; ++i)
      {
//         _streams.push_back(new std::fstream(sstream.str().c_str(),
//                  std::ios::binary | std::ios::in));
         std::ostringstream toysName;
         toysName << name << ".toys";
         _tStream = new std::fstream(toysName.str().c_str(), std::ios::binary |
               std::ios::in);
         if(!_tStream)
         {
            LOG("ReadableCache", pFATAL) << "Failed to open file: " <<
               toysName.str();
            exit(1);
         }
         else
         {
            LOG("ReadableCache", pNOTICE) << "Opened file, at position" <<
               _tStream->tellg();
         }
      }
   }

   ReadableCache::~ReadableCache()
   {
/*      for(std::vector<std::fstream*>::iterator it = _streams.begin();
            it != _streams.end(); ++it)
      {
         std::fstream* stream = *it;
         if(stream)
         {
            stream->close();
            delete stream;
         }
      }
      _streams.clear();*/
      if(_hStream && _hStream->is_open())
      {
         _hStream->close();
      }
      delete _hStream;
      if(_tStream && _tStream->is_open())
      {
         _tStream->close();
      }
      delete _tStream;
      delete _cache;
      _cache = 0;
   }

   ReadableCache* ReadableCache::Instance()
   {
      if(!_instance)
      {
         LOG("ReadableCache", pFATAL) <<
            "Cache must be initalised with filename information";
         exit(1);
      }

      return _instance;
   }

   ReadableCache* ReadableCache::Instance(std::string name, const int numToys,
         const int maxToysPerFile)
   {
      if(!_instance)
      {
         _instance = new ReadableCache(name, numToys, maxToysPerFile);
      }
      else
      {
         LOG("ReadableCache", pNOTICE) <<
            "Ignoring attempt to reinitialise cache";
      }

      return _instance;
   }

   CacheItem* ReadableCache::Read()
   {
      long offset = 0;
      long size = 0;
      // Read in the location of the data within the toy file
      _hStream->read(reinterpret_cast<char*>(&offset), sizeof(offset));
      // Determine how much data to read from the toy file
      _hStream->read(reinterpret_cast<char*>(&size), sizeof(size));

      // Temporarily only handling one stream
      char data[size];
      _tStream->read(data, size);

      // Temporarily only handling one cached item
      delete _cache;
      _cache = new CacheItem();
      _cache->Build(data);

      return _cache;
   }

   void ReadableCache::Reset()
   {
      if(_hStream && _hStream->is_open())
      {
         _hStream->seekg(sizeof(int));
      }
      if(_tStream && _tStream->is_open())
      {
         _tStream->seekg(0);
      }
   }
}
}

