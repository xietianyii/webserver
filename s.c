#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<signal.h>
#include<unistd.h>
int main(int argc,char*argv[])
{
    int socknum=socket(AF_INET,SOCK_STREAM,0);//socket通信
    struct sockaddr_in addr;
    addr.sin_addr.s_addr=INADDR_ANY;
    //addr.sin_family=AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    addr.sin_port=htons(13000);

    bind(socknum,(struct sockaddr*)&addr,sizeof(addr));
    listen(socknum,8);
    while(1)
    {
        
                struct sockaddr_in clientaddr;
                socklen_t len=sizeof(clientaddr);
                int cfd=accept(socknum,(struct sockaddr*)&clientaddr,&len);
                char buf[1024];
                read(cfd,buf,sizeof(buf));
                printf("%s\n",buf);
    }
    
}
