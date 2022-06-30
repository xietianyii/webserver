#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"locker.h"
#include"threadpool.h"
#include"httpconn.h"
#include<sys/epoll.h>
#include<signal.h>
#include<unistd.h>
#define max_eventnum 65536
#define max_httpnum 65536
extern void addfd(int epollnum,int fd,bool one_shot);
extern void delfd(int opollfd,int fd);
extern void modfd(int epollfd,int fd,int ev);
void addsig(int sig,void(handler)(int))
{
    struct sigaction sa;
    memset(&sa,'\0',sizeof sa);
    sa.sa_handler=handler;
    sigfillset(&sa.sa_mask);
    sigaction(sig,&sa,NULL);
}
int main(int argc,char*argv[])
{
    printf("%d\n",argc);
    if(argc<=1)
    {
        printf("无端口号\n");
        exit(-1);
    }
    addsig(SIGPIPE,SIG_IGN);//捕捉信号量
    int port=atoi(argv[1]);//端口号
    pthreadpool<httpconn>* pool=NULL;//线程池
    printf("port: %d\n",port);
    try{

    pool =new pthreadpool<httpconn>;
    }
    catch(...){
        printf("error pthreadpoo\n");
        return 1;
    }
    
    httpconn *users=new httpconn[max_httpnum];//用户链接保存数据
    int socknum=socket(AF_INET,SOCK_STREAM,0);//socket通信
    struct sockaddr_in addr;
    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    int optval=1;
    setsockopt(socknum,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));//端口复用
    bind(socknum,(struct sockaddr*)&addr,sizeof(addr));
    listen(socknum,8);
    int epollnum=epoll_create(2000);
    
    struct epoll_event events[max_eventnum];
    addfd(epollnum,socknum,false);
    while(true)
    {
        int num=epoll_wait(epollnum,events,max_eventnum,-1);
        for(int i=0;i<num;i++)
        {
            int fdnum=events[i].data.fd;
            if(fdnum==socknum)
            {
                printf("accept\n");
                struct sockaddr_in clientaddr;
                socklen_t len=sizeof(clientaddr);
                int cfd=accept(socknum,(struct sockaddr*)&clientaddr,&len);
                users[cfd].init(cfd,clientaddr);
            }
            else if(events[i].events&(EPOLLRDHUP|EPOLLHUP|EPOLLERR))
            {
                users[i].colse_conn();
            }
            else if(events[i].events&EPOLLIN)
            {
                users[i].read();
                pool->append(users+fdnum);
            }
            else if(events[i].events&EPOLLOUT)
            {
                users[i].write();
            }
        }
    }
    close(socknum);
    close(epollnum);
    delete []users;
    delete pool;
    return 0;
}