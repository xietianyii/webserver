#ifndef LOCKER_H
#define LOCKER_H
//线程同步机制封装类
#include<pthread.h>
#include <semaphore.h>
class locker{
public:
    locker(){
        pthread_mutex_init(&m_mutex,NULL);
    }
    ~locker(){
        pthread_mutex_destroy(&m_mutex);
    }
    bool lock(){
        return pthread_mutex_lock(&m_mutex)==0;
    }
    bool unlock(){
        return pthread_mutex_unlock(&m_mutex)==0;
    }
    pthread_mutex_t *getmutex(){
        return &m_mutex;
    }
private:
    pthread_mutex_t m_mutex;
};
class cond{
public:
    cond(){
        pthread_cond_init(&c_cond,NULL);
    }
    ~cond(){
        pthread_cond_destroy(&c_cond);
    }
    bool wait(pthread_mutex_t *mutex){
        return pthread_cond_wait(&c_cond,mutex);
    }
    bool timewait(pthread_mutex_t *mutex,struct timespec time){
        return pthread_cond_timedwait(&c_cond,mutex,&time)==0;
    }
    bool signal(){
        return pthread_cond_signal(&c_cond)==0;
    }
    bool broadcast(){
        return pthread_cond_broadcast(&c_cond)==0;
    }
private:
    pthread_cond_t c_cond;
};
class sem{
public:
    sem(int num){
        sem_init(&s_sem,0,num);
    }
    ~sem(){
        sem_destroy(&s_sem);
    }
    bool semwait(){
        return sem_wait(&s_sem)==0;
    }
    bool sempost(){
        return sem_post(&s_sem)==0 ;
    }
    int getsemvalve(){
        int num;
        sem_getvalue(&s_sem,&num);
        return num;
    }
private:
    sem_t s_sem;
};
#endif