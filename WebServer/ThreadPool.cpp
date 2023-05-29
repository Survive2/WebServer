#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadNum,quitMode quit,size_t maxSize) :
 _threadNum(threadNum),_quit(quit),_maxSize(maxSize),_thread_cond(_thread_lock){
    while(threads.size()<_threadNum){
        pthread_t thread;
        if(!pthread_create(&thread,nullptr,executeTask,this)){
            threads.push_back(thread);
        }
    }
}