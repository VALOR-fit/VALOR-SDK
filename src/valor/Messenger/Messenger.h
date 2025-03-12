//____________________________________________________________________________
/*!

\class    valor::Messenger

\brief    A more convenient interface to the log4cpp Message Service
          Adappted from GENIE with permission from the authors.

\author   Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
          STFC, Rutherford Appleton Laboratory

\created  June 16, 2004

\cpright  Copyright (c) 2003-2020, The GENIE Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _VALOR_MESSENGER_H_
#define _VALOR_MESSENGER_H_

#include <iostream>
#include <cstring>
#include <string>
#include <map>


#if __cplusplus >= 201703L
    /* Hack to allow log4cpp with C++1z at FNAL*/
    #include <stdexcept>
    #include <string>
    #include <memory>

    /* Now remove the trow */
    #define throw(...)
    #include "log4cpp/Category.hh"
    #include "log4cpp/Appender.hh"
    #include "log4cpp/OstreamAppender.hh"
    #include "log4cpp/BasicLayout.hh"
    #include "log4cpp/SimpleLayout.hh"
    #include "log4cpp/Priority.hh"
    #undef throw /* reset */
#endif

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/SimpleLayout.hh"
#include "log4cpp/Priority.hh"

//#include "valor/Conventions/GBuild.h"

// comment defined priority levels for the document generator
/*! \def pFATAL  \brief Defines the FATAL priority level */
/*! \def pALERT  \brief Defines the ALERT priority level */
/*! \def pCRIT   \brief Defines the ALERT priority level */
/*! \def pERROR  \brief Defines the ALERT priority level */
/*! \def pWARN   \brief Defines the ALERT priority level */
/*! \def pNOTICE \brief Defines the ALERT priority level */
/*! \def pINFO   \brief Defines the ALERT priority level */
/*! \def pDEBUG  \brief Defines the ALERT priority level */

#define pFATAL  log4cpp::Priority::FATAL
#define pALERT  log4cpp::Priority::ALERT
#define pCRIT   log4cpp::Priority::CRIT
#define pERROR  log4cpp::Priority::ERROR
#define pWARN   log4cpp::Priority::WARN
#define pNOTICE log4cpp::Priority::NOTICE
#define pINFO   log4cpp::Priority::INFO
#define pDEBUG  log4cpp::Priority::DEBUG

/*! \def ENDL  \brief A shortcut for log4cpp's CategoryStream::ENDLINE or std manipulators*/

#ifdef __VALOR_FMWK_USES_LOG4CPP_VERSION__
  #if __VALOR_FMWK_USES_LOG4CPP_VERSION__==0
    #define ENDL log4cpp::CategoryStream::ENDLINE
  #else
    #define ENDL std::endl
  #endif
#else
  #define ENDL std::endl
#endif

// Switch off SLOG and LOG in AL9 because of an include conflict somewhere...

/*!
  \def   SLOG(stream, priority)
  \brief A macro that returns the requested log4cpp::Category
         appending a short string (using the __FUNCTION__ and __LINE__ macros)
         with information for the calling method [produces short message].
*/
#ifndef SLOG
#define SLOG(stream, priority) \
           (*valor::Messenger::Instance())(stream) \
               << priority << "[s] <" \
               << __FUNCTION__ << " (" << __LINE__ << ")> : "
#endif
*/

/*!
  \def   LOG(stream, priority)
  \brief A macro that returns the requested log4cpp::Category
         appending a string (using the __FILE__, __FUNCTION__ and __LINE__ macros)
         with information for the calling method [produces normal messages].
*/
#ifndef LOG
#define LOG(stream, priority) \
           (*valor::Messenger::Instance())(stream) \
               << priority << "[n] <" \
               << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ")> : "
#endif
*/



// Copies of the above two functions with "V" in front (for VALORLog).
// Can be used to prevent collisisons with GENIE logging in programs built against both.
#define VSLOG(stream, priority) \
           (*valor::Messenger::Instance())(stream) \
               << priority << "[s] <" \
               << __FUNCTION__ << " (" << __LINE__ << ")> : "
#define VLOG(stream, priority) \
           (*valor::Messenger::Instance())(stream) \
               << priority << "[s] <" \
               << __FUNCTION__ << " (" << __LINE__ << ")> : "
               

#ifndef LOG_FATAL //  This one will be used to assume all others below are already defined

#define LOG_FATAL(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::FATAL << "[n] <" \
               << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ")> : "

#define LOG_ALERT(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::ALERT << "[n] <" \
               << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ")> : "

#define LOG_CRIT(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::CRIT << "[n] <" \
               << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ")> : "

#define LOG_ERROR(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::ERROR << "[n] <" \
               << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ")> : "

#define LOG_WARN(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::WARN << "[n] <" \
               << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ")> : "

#define LOG_NOTICE(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::NOTICE << "[n] <" \
               << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ")> : "

#define LOG_INFO(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::INFO << "[n] <" \
               << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ")> : "

#define LOG_DEBUG(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::DEBUG << "[n] <" \
               << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ")> : "

#define VBLOG(stream, priority) \
	  (*valor::Messenger::Instance())(stream) << priority

#else // GENIE does #define these...

/*!
  \def   LLOG(stream, priority)
  \brief A macro that returns the requested log4cpp::Category
         appending a string (using the __PRETTY_FUNCTION__ and __LINE__ macros)
         with information for the calling method [produces long messages].
*/

#define LLOG(stream, priority) \
           (*valor::Messenger::Instance())(stream) \
               << priority << "[l] <" \
               << __PRETTY_FUNCTION__ << " (" << __LINE__ << ")> : "

#define LLOG_FATAL(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::FATAL << "[l] <" \
               << __PRETTY_FUNCTION__ << " (" << __LINE__ << ")> : "

#define LLOG_ALERT(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::ALERT << "[l] <" \
               << __PRETTY_FUNCTION__ << " (" << __LINE__ << ")> : "

#define LLOG_CRIT(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::CRIT << "[l] <" \
               << __PRETTY_FUNCTION__ << " (" << __LINE__ << ")> : "

#define LLOG_ERROR(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::ERROR << "[l] <" \
               << __PRETTY_FUNCTION__ << " (" << __LINE__ << ")> : "

#define LLOG_WARN(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::WARN << "'[l] <" \
               << __PRETTY_FUNCTION__ << " (" << __LINE__ << ")> : "

#define LLOG_NOTICE(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::NOTICE << "[l] <" \
               << __PRETTY_FUNCTION__ << " (" << __LINE__ << ")> : "

#define LLOG_INFO(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::INFO << "[l] <" \
               << __PRETTY_FUNCTION__ << " (" << __LINE__ << ")> : "

#define LLOG_DEBUG(stream) \
          (*valor::Messenger::Instance())(stream) \
               << log4cpp::Priority::DEBUG << "[l] <" \
               << __PRETTY_FUNCTION__ << " (" << __LINE__ << ")> : "

/*!
  \def   BLOG(stream, priority)
  \brief A macro that returns the requested log4cpp::Category appending no 
         additional information
*/

#define BLOG(stream, priority) \
	  (*valor::Messenger::Instance())(stream) << priority
       
#endif // Our check for VLOG_FATAL, to make sure we don't collide with GENIE

namespace valor {

class Messenger
{
public:
  static Messenger * Instance(void);

  log4cpp::Category & operator () (const char * stream);
  void SetPriorityLevel(const char * stream, log4cpp::Priority::Value p);
  void SetPriorityLevel(std::string stream, std::string priority);

  bool Configure(std::string filename);

private:
  Messenger();
  Messenger(const Messenger & m);
  virtual ~Messenger();

  static Messenger * fInstance;

  log4cpp::Priority::Value PriorityFromString(std::string priority);

  struct Cleaner {
      void DummyMethodAndSilentCompiler() { }
      ~Cleaner() {
         if (Messenger::fInstance !=0) {
            delete Messenger::fInstance;
            Messenger::fInstance = 0;
         }
      }
  };
  friend struct Cleaner;
};

}      // valor namespace
#endif // _MESSENGER_H_
