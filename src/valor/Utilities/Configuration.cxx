#include "valor/Utilities/Configuration.h"

namespace valor
{
namespace util
{

Configuration* Configuration::_instance = nullptr;

Configuration::Configuration(): Registry()
{

}

Configuration::~Configuration()
{
}

Configuration* Configuration::Instance()
{
   if(not(_instance))
   {
      _instance = new Configuration();
   }

   return _instance;
}

}
}
