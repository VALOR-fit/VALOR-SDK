//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>

#include "valor/Utilities/VThreadPool.h"

#if __cplusplus > 199711L

using namespace valor;

//____________________________________________________________________________
valor::util::VThreadPool * valor::util::VThreadPool::fInstance = 0;
//____________________________________________________________________________
valor::util::VThreadPool::VThreadPool()
:
  fNThreads(std::thread::hardware_concurrency()),
  fAbort(false),
  fNQueuedJobs(0),
  fNRunningJobs(0)
{
  fInstance = 0;
  while (fThreads.size() < fNThreads) {
    this->AddThread();
  }
}
//____________________________________________________________________________
valor::util::VThreadPool * valor::util::VThreadPool::Instance() 
{
  if(fInstance == 0) {
    static valor::util::VThreadPool::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new valor::util::VThreadPool;
  }
  return fInstance;
}
//____________________________________________________________________________
valor::util::VThreadPool::~VThreadPool()
{
  fAbort = true;
  fJobAvailable.notify_all();
  for( auto &t : fThreads ) {
    if( t.joinable() ) {t.join();}
  }
}
//____________________________________________________________________________
void valor::util::VThreadPool::AddThread()
{
  fThreads.emplace_back(std::move( std::thread( [this]{ this->ThreadBody(); } ) ));
}
//____________________________________________________________________________
void valor::util::VThreadPool::ThreadBody(void)
{
  while( !fAbort ) {
    std::function<void(void)> f = this->GetTask();
    fNRunningJobs++;
    f();
    fNRunningJobs--;
    fWaitVar.notify_one();
  }
}
//____________________________________________________________________________
std::function<void(void)> valor::util::VThreadPool::GetTask()
{
  std::unique_lock<std::mutex> job_lock( fQueueMutex );

  // Wait for job to become available
  fJobAvailable.wait( job_lock, [this]{ return (fAbort || fNQueuedJobs); });
  
  std::function<void(void)> f = []{};
  if (!fAbort) {
    f = fQueue.front();
    fQueue.pop_front();
    fNQueuedJobs--;
  }
  return f;
}
//____________________________________________________________________________
unsigned int util::VThreadPool::GetNTasksInQueue(void)
{
  return fNQueuedJobs;
}
//____________________________________________________________________________
unsigned int util::VThreadPool::GetNThreads(void) const
{
  return fNThreads;
}
//____________________________________________________________________________
void util::VThreadPool::SetNThreads(unsigned int n)
{
  if (n < fNThreads) {
    // Clear the threads if we have too many
    fJobAvailable.notify_all();
    this->Wait();
    fAbort = true;
    for( auto &t : fThreads ) {
      if( t.joinable() ) {t.join();}
    }
  }
  fThreads.clear();
  
  // Initialise any required new threads
  fAbort = false;
  fNThreads = n;
  while (fThreads.size() < fNThreads) {
    this->AddThread();
  }
}
//____________________________________________________________________________
void util::VThreadPool::AddTask(std::function<void(void)> work)
{
  std::lock_guard<std::mutex> lock( fQueueMutex );
  fQueue.emplace_back( work );
  fNQueuedJobs++;
  fJobAvailable.notify_one();
}
//____________________________________________________________________________
void util::VThreadPool::AddTasks(std::vector<std::function<void(void)> > work)
{
  for (unsigned int i = 0 ; i < work.size() ; i++) {
    this->AddTask(work[i]);
  }
}
//____________________________________________________________________________
void util::VThreadPool::DoTasks(std::vector<std::function<void(void)> > work)
{
  this->AddTasks(work);
  this->Wait();
}
//____________________________________________________________________________
void util::VThreadPool::Wait(bool finish)
{
  std::function<bool(void)> finished =[&]{ return !(fNQueuedJobs||(finish&&fNRunningJobs));};
  while(!finished()) {
    std::unique_lock<std::mutex> lock( fWaitMutex );
    fWaitVar.wait_for( lock, std::chrono::seconds(10), finished);
    lock.unlock();
  }
}
//____________________________________________________________________________


#endif //__cplusplus > 199711L
