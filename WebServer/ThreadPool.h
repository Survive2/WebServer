#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <cassert>
#include <queue>

#include "Condition.h"
#include "Lock.h"

using namespace std;

class ThreadPool{
public:
    enum quitMode{ WORKDONE, FORCEQUIT};
    ThreadPool(size_t threadNum,quitMode quit = WORKDONE,size_t maxSize=-1);
    ~ThreadPool();
    bool addTask(void (*function)(void*), void* args);
    static void* executeTask(void* args);
private:
    MutexLock _thread_lock;
    Condition _thread_cond;
    class ThreadPoolTask{
    public:
        void (*function)(void*);
        void* _args;
    };
    size_t _maxSize;
    vector<pthread_t> threads;
    queue<ThreadPoolTask> task_queue;
    quitMode _quit;
    size_t _threadNum;
};


#endif