#include "valor/Utilities/ReadableCache.h"
#include "valor/Messenger/Messenger.h"
#include <sstream>
#include <algorithm>
#include <TMath.h>

namespace valor
{
namespace util
{
   ReadableCache* ReadableCache::_instance = 0;

   ReadableCache::ReadableCache(std::string name, const int numToys,
         const int cacheSize):
      _numToys(numToys),
      _cacheSize(cacheSize),
      _firstToy(-1),
      _lastToy(-1),
      _currentToy(-1),
      _toySize(0),
      _offsets(new long[numToys]),
      _raw(0),
      _cache(0)
   {
      std::ostringstream headerName;
      headerName << name << ".head";
      _hStream = new std::fstream(headerName.str().c_str(), std::ios::binary |
            std::ios::in);
      // We might want to be able to read a smaller subset of toys than are
      // actually stored in the file, but if we don't we can get rid of the
      // numToys parameter passed to Instance and the constructor.
      int numToysFromFile = 0;
      _hStream->read(reinterpret_cast<char*>(&numToysFromFile),
            sizeof(numToysFromFile));

      if(_numToys <= numToysFromFile)
      {
         // Read in the offsets of each toy and the size of each toy
         for(int i = 0; i < _numToys; ++i)
         {
            _hStream->read(reinterpret_cast<char*>(&_offsets[i]),
                  sizeof(_offsets[i]));
            _hStream->read(reinterpret_cast<char*>(&_toySize), sizeof(_toySize));
         }
         _raw = new char[_numToys * _toySize];
         // Done with the header file
         _hStream->close();
      }
      else
      {
         LOG("ReadableCache", pFATAL) << "Number of run-time toys (" <<
            numToys << ") more than number of toys in file (" <<
            numToysFromFile << ")";
         exit(1);
      }

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
   }

   ReadableCache::~ReadableCache()
   {
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
      delete[] _offsets;
      _offsets = 0;
      delete[] _raw;
      _raw = 0;
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
         const int cacheSize)
   {
      if(!_instance)
      {
         _instance = new ReadableCache(name, numToys, cacheSize);
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
      // Move to the next toy
      ++_currentToy;
      if(_currentToy > _lastToy)
      {  // Read in the next block of toys
         _firstToy = _currentToy;
         _lastToy = _firstToy + (_cacheSize - 1);
         _tStream->seekg(_offsets[_firstToy]);
         if(_lastToy < _numToys)
         {
            _tStream->read(_raw, _cacheSize * _toySize);
         }
         else
         {  // Ensure block doesn't extend beyond final toy in file
            _lastToy = _numToys - 1;
            _tStream->read(_raw, (1 + _lastToy - _firstToy) * _toySize);
         }
      }

      long relativeOffset = _offsets[_currentToy] - _offsets[_firstToy];
      char* data = &_raw[relativeOffset];

      // Temporarily only handling one cached item
      delete _cache;
      _cache = new CacheItem();
      _cache->Build(data);

      return _cache;
   }

   void ReadableCache::Reset()
   {
      if(_tStream && _tStream->is_open())
      {
         _tStream->seekg(0);
      }
      _currentToy = -1;
      _firstToy = -1;
      _lastToy = -1;
   }
}
}

