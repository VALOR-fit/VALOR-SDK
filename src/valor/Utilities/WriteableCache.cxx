#include "valor/Utilities/WriteableCache.h"
#include "valor/Messenger/Messenger.h"
#include <sstream>
#include <TMath.h>

namespace valor
{
namespace util
{
   WriteableCache* WriteableCache::_instance = 0;

   WriteableCache::WriteableCache(std::string name, const int numToys,
         const int maxToysPerFile):
      _numToysPerFile(maxToysPerFile),
      _numFiles(TMath::Ceil(static_cast<double>(numToys) /
               static_cast<double>(maxToysPerFile)))
   {
      std::ostringstream headerName;
      headerName << name << ".head";
      _hStream = new std::fstream(headerName.str().c_str(), std::ios::out |
            std::ios::binary | std::ios::trunc | std::ios::in);
      // Number of toys can be written to the header immediately
      _hStream->write(reinterpret_cast<const char*>(&numToys), sizeof(numToys));
//      _streams.reserve(_numFiles);
      for(int i = 0; i < _numFiles; ++i)
      {
/*         _streams.push_back(new std::fstream(sstream.str().c_str(),
                  std::ios::out | std::ios::binary | std::ios::trunc |
                  std::ios::in));*/
         std::ostringstream toysName;
         toysName << name << ".toys";
         _tStream = new std::fstream(toysName.str().c_str(), std::ios::out |
               std::ios::binary | std::ios::trunc | std::ios::in);
      }

   }

   WriteableCache::~WriteableCache()
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
   }

   WriteableCache* WriteableCache::Instance()
   {
      if(!_instance)
      {
         LOG("WriteableCache", pFATAL) <<
            "Cache must be initalised with filename information";
         exit(1);
      }

      return _instance;
   }

   WriteableCache* WriteableCache::Instance(std::string name, const int numToys,
         const int maxToysPerFile)
   {
      if(!_instance)
      {
         _instance = new WriteableCache(name, numToys, maxToysPerFile);
      }
      else
      {
         LOG("WriteableCache", pNOTICE) <<
            "Ignoring attempt to reinitialise cache";
      }

      return _instance;
   }

   void WriteableCache::Write(CacheItem* item)
   {
      long size = item->GetDiskSize();
      long offset = _tStream->tellp();
      // The offset of the data being written
      _hStream->write(reinterpret_cast<char*>(&offset), sizeof(offset));
      // The amount of data to write to the toy file
      _hStream->write(reinterpret_cast<char*>(&size), sizeof(size));
      // Temporarily only handling one stream and one cache item
      const char* data = item->Flatten();
      _tStream->write(data, size);
      _hStream->flush();
      _tStream->flush();
   }
}
}

