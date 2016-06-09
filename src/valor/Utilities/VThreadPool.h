#ifndef VALOR_THREADPOOL_H
#define VALOR_THREADPOOL_H

#if __cplusplus > 199711L

#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <deque>
#include <list>
#include <functional>
#include <condition_variable>

namespace valor{
namespace util{
  
class VThreadPool {
  
  public:
    static VThreadPool * Instance();
    
    void SetNThreads(unsigned int n); // Will wait if deleting existing threads
    unsigned int GetNThreads(void) const;
    
    void Wait(); // Wait for all jobs to complete
    
    void AddTask(std::function<void(void)> work); // Add one job
    void AddTasks(std::vector<std::function<void(void)> > work); // Add many jobs
    void DoTasks (std::vector<std::function<void(void)> > work); // Same as above but waits for jobs to finish
    
    unsigned int GetNTasksInQueue(void);
    
    ~VThreadPool();
    
  private:
  
    VThreadPool();
    
    void AddThread(void);
    void ThreadBody();
    
    std::vector<std::thread> fThreads;
    std::deque<std::function<void(void)>> fQueue;
    
    unsigned int fNThreads;
    std::atomic_bool        fAbort;
    std::atomic_int         fNQueuedJobs;
    std::atomic_int         fNRunningJobs;
    std::condition_variable fJobAvailable;
    std::condition_variable fWaitVar;
    std::mutex              fWaitMutex;
    std::mutex              fQueueMutex;
    
    std::function<void(void)> GetTask();
    
    static VThreadPool * fInstance;
      struct Cleaner {
        void DummyMethodAndSilentCompiler() { } 
          ~Cleaner() {
            if (VThreadPool::fInstance !=0) {
              delete VThreadPool::fInstance;
              VThreadPool::fInstance = 0;
            }
          }
      };
    friend struct Cleaner;
};

} // namespace util
} // namespace valor

#else // No C++11
namespace valor{namespace util{class VThreadPool {};}}
#endif

#endif // VALOR_THREADPOOL_H
