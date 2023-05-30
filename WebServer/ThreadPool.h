#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <cassert>
#include <queue>

#include "Condition.h"
#include "Lock.h"

using namespace std;

class ThreadPool{
public:
    enum quitMode {WORKDONE,FORCEQUIT};
    ThreadPool(size_t threadNum,size_t maxSize,quitMode quit);
    ~ThreadPool();
    bool addTask(void (*function)(void*),void* args);
    static void* executeTask(void * args);
private:
    MutexLock _thread_lock;
    Condition _thread_cond;
    struct ThreadTask
    {
        void (*function)(void*);
        void *args;
    };

    queue<ThreadTask> task_queue;
    vector<pthread_t> threads;
    size_t _threadNum;
    size_t _maxSize;
    quitMode _quit;
};

#endif