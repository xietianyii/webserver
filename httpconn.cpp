#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"locker.h"
#include"threadpool.h"
#include"httpconn.h"
#include<sys/epoll.h>
#include<signal.h>
#include<unistd.h>
#include<fcntl.h>
int httpconn::epoll_num=-1;
int httpconn::user_count=0;
void noblock(int fd)
{
    int ofd=fcntl(fd,F_GETFL);
    ofd|=O_NONBLOCK;
    fcntl(fd,F_SETFL,ofd);
}
void addfd(int epollfd,int fd,bool one_shot){
    epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN|EPOLLRDHUP;
    if(one_shot)
    event.events|=EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    noblock(fd);
}
void delf(int epollfd,int fd){
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}
void modfd(int epollfd,int fd,int ev)
{
    epoll_event event;
    event.data.fd=fd;
    event.events=ev|EPOLLIN|EPOLLONESHOT|EPOLLRDHUP;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}
void httpconn::init(int fd,struct sockaddr_in &clientaddr)
{
    addr=clientaddr;
    cfd=fd;
    int ret=1;
    setsockopt(cfd,SOL_SOCKET,SO_REUSEADDR,&ret,sizeof(ret));
    user_count++;
    init();
}
void httpconn::init()
{

}
void httpconn::read()
{
    printf("一次读完所有数据\n");
}
void httpconn::write()
{
    printf("一次写完所有数据\n");
}
void httpconn::colse_conn()
{
    cfd=-1;
    user_count--;

}
void httpconn::process()
{
    printf("i am server\n");
}
httpconn::httpconn()
{

}
httpconn::~httpconn()
{
    
}