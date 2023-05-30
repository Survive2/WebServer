#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadNum,size_t maxSize,quitMode quit):
_threadNum(threadNum),_maxSize(maxSize),_quit(quit),_thread_cond(_thread_lock){
    for(size_t i=0;i<threadNum;i++){
        pthread_t thread;
        if(!pthread_create(&thread,nullptr,executeTask,this)){
            threads.push_back(thread);
        }
    }
}

ThreadPool::~ThreadPool(){
    {
        MutexLockGuard guard(_thread_lock);
        if(_quit == FORCEQUIT){
            while (!task_queue.empty())
            {
                task_queue.pop();
            }
            
        }
        for(size_t i = 0;i<_threadNum;i++){
            auto killThread = [](void*){pthread_exit(0);};
            ThreadTask task = { killThread,nullptr};
            task_queue.push(task);
        }
        _thread_cond.notifyAll();
    }
    for(size_t i =0 ;i<_threadNum;i++){
        pthread_join(threads[i], nullptr);
    }
}

bool ThreadPool::addTask(void(* function)(void*),void* args){
    MutexLockGuard guard(_thread_lock);
    if(task_queue.size()>_maxSize){
        return false;
    }
    ThreadTask task = {function,args};
    task_queue.push(task);
    _thread_cond.notify();
    return true;
}

void* ThreadPool::executeTask(void* args){
    ThreadPool* pool = (ThreadPool*)args;
    ThreadTask task;
    while(1){
        MutexLockGuard guard(pool->_thread_lock);
        while(pool->task_queue.size() == 0){
            pool->_thread_cond.wait();
        }
        assert(pool->task_queue.size() != 0);
        task = pool->task_queue.front();
        pool->task_queue.pop();
        (task.function)(task.args);
    }
    assert(0 && "ThreadTask UNREACHABLE!");
    return nullptr;
}

