#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include <pthread.h>

class MutexLock
{
private:
    pthread_mutex_t mutex;

public:
    MutexLock()
    {
        pthread_mutex_init(&mutex, nullptr);
    }
    ~MutexLock()
    {
        pthread_mutex_destroy(&mutex);
    }
    void lock()
    {
        pthread_mutex_lock(&mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(&mutex);
    }
    void trylock()
    {
        pthread_mutex_trylock(&mutex);
    }
    pthread_mutex_t *getMutex()
    {
        return &mutex;
    }
};

class MutexLockGuard
{
    private:
    MutexLock &_lock;
    public:
    MutexLockGuard(MutexLock& _mutex) : _lock(_mutex){
        _lock.lock();
    }
    ~MutexLockGuard(){
        _lock.unlock();
    }
};

#endif