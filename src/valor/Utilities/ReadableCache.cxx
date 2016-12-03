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
               static_cast<double>(maxToysPerFile)))
   {
//      _streams.reserve(_numFiles);
      LOG("ReadableCache", pNOTICE) << "Num files: " << _numFiles;
      for(int i = 0; i < _numFiles; ++i)
      {
         std::ostringstream sstream;
         sstream << name;// << "_" << i;
         LOG("ReadableCache", pNOTICE) << "Filename " << sstream.str();
//         _streams.push_back(new std::fstream(sstream.str().c_str(),
//                  std::ios::binary | std::ios::in));
         _stream = new std::fstream(sstream.str().c_str(), std::ios::binary |
               std::ios::in);
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
      if(_stream && _stream->is_open())
      {
         _stream->close();
      }
      delete _stream;
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

   void ReadableCache::Read(char* data, const std::streamsize size)
   {
      // Temporarily only handling one stream
      _stream->read(data, size);
   }

   void ReadableCache::Seek(const std::streampos pos)
   {
      // Temporarily only handling one stream
      _stream->seekg(pos);
   }

   std::streampos ReadableCache::Position()
   {
      return _stream->tellg();
   }

   bool ReadableCache::Good()
   {
      return _stream->good();
   }
}
}

