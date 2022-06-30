#ifndef HTTPCONN_H
#define HTTPCONN_H
class httpconn{
public:
    httpconn();
    ~httpconn();
    static int epoll_num;
    static int user_count;
    void init(int fd,struct sockaddr_in& addr);
    void init();
    void read();
    void write();
    void colse_conn();
    void process();
    int cfd;
    sockaddr_in addr;
private:
    
};

#endif