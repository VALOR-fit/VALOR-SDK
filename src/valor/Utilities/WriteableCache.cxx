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
//      _streams.reserve(_numFiles);
      for(int i = 0; i < _numFiles; ++i)
      {
         std::ostringstream sstream;
         sstream << name;// << "_" << i;
/*         _streams.push_back(new std::fstream(sstream.str().c_str(),
                  std::ios::out | std::ios::binary | std::ios::trunc |
                  std::ios::in));*/
         _stream = new std::fstream(sstream.str().c_str(), std::ios::out |
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
      if(_stream && _stream->is_open())
      {
         _stream->close();
      }
      delete _stream;
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

   void WriteableCache::Write(const char* data, const std::streamsize size)
   {
      // Temporarily only handling one stream
      _stream->write(data, size);
   }

   void WriteableCache::Seek(const std::streampos pos)
   {
      // Temporarily only handling one stream
      _stream->seekp(pos);
   }

   std::streampos WriteableCache::Position()
   {
      return _stream->tellp();
   }

   bool WriteableCache::Good()
   {
      return _stream->good();
   }
}
}

