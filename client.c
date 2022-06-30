
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<sys/epoll.h>
#include<signal.h>
#include<unistd.h>
int main(int argc,char*argv[])
{
    int socknum=socket(AF_INET,SOCK_STREAM,0);//socket通信
    struct sockaddr_in addr;
    inet_pton(AF_INET,"124.222.151.103",&addr.sin_addr.s_addr);
    addr.sin_family=AF_INET;
    addr.sin_port=htons(13000);
    int ret=connect(socknum,(struct sockaddr*)&addr,sizeof(addr));
    printf("%d\n",ret);
    char * buf="i amclient";
    write(socknum,buf,strlen(buf));
}