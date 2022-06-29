#ifndef THREADPOLL_H
#define THREADPOLL_H
#include"locker.h"
#include<stdio.h>
#include<pthread.h>
#include<list>
template<typename T>
class pthreadpool{
public:
    pthreadpool(int pthreadnum=8,int requestnum=10000);
    ~pthreadpool();
    void run();
    int append(T*);
    static void* work(void * arg);
private:
    //线程池
    pthread_t  * pthreads;
    int pthread_num;
    // 请求队列
    std::list<T*>requestlist;
    int request_maxnum;
    //线程同步
    locker mutex;
    sem m_request;
    bool stop;
};
template <typename T>
pthreadpool<T>::pthreadpool(int pthread_num,int request_num)
{
    pthreadnum=pthread_num;
    request_maxnum=request_num;
    stop=false;
    pthreads=new pthread_t[pthreadnum];
    for(int i=0;i<pthreadnum;i++)
    {
        pthread_create(&pthreads[i],NULL,work,this);
        pthread_detach(pthreads[i]);
    }
}
template <typename T>
pthreadpool<T>::~pthreadpool()
{
    delete []pthreads;
    stop=true;
}
template<typename T>
void* pthreadpool<T>::work(void * agr)
{
    pthreadpoll * pthread=(struct pthreadpoll *)agr;
    pthread->run();
    return NULL;
}
template <typename T>
void pthreadpool<T>::run()
{
    while(stop)
    {
        if(requestlist.size()!=0)
        {
            m_request.semwait();
            mutex.lock();
            T * request=requestlist.front();
            requestlist.pop_front();
            mutex.unlock();
            request->process();
        }
    }
}
template<typename T>
int pthreadpool<T>::append(T* add)
{
    if(requestlist.size()>=request_maxnum)
    {
        return 0;
    }
    mutex.lock();
    m_request.sempost();
    requestlist.push_back(T);
    mutex.unlock();
    return 1;
}

#endif