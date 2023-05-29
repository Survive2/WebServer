#ifndef CONDITION_H
#define CONDITION_H

#include <cerrno>
#include <pthread.h>

#include "Lock.h"

class Condition{
private:
    MutexLock& _lock;
    pthread_cond_t _cond;
public:
    Condition(MutexLock& _mutex): _lock(_mutex){
        pthread_cond_init(&_cond,nullptr);
    }
    ~Condition(){
        pthread_cond_destroy(&_cond);
    }
    void notify(){
        pthread_cond_signal(&_cond);
    }
    void notifyAll(){
        pthread_cond_broadcast(&_cond);
    }
    void wait(){
        pthread_cond_wait(&_cond,_lock.getMutex());
    }
    bool timedwait(size_t __time_to_wait){
        timespec abstime;
        clock_gettime(CLOCK_REALTIME,&abstime);
        abstime.tv_sec += (time_t)__time_to_wait;
        return ETIMEDOUT != pthread_cond_timedwait(&_cond,_lock.getMutex(),&abstime);
    }
};

#endif