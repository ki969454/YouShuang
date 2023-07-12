# YouShuang
Linux 高性能服务器编程

ch05

网络字节序：
    大段字节序：一个整数的高位字节存储在内存的低地址处，低位字节存储在内存的高地址处；
    小段字节序：一个整数的高位字节存储在内存的高地址处，低位字节存储在内存的低地址处。
    现代PC多采用小段字节序，小端字节序又被称为主机字节序；大段字节序成为网络字节序。

创建socket：
    int socket(int domain, int type, int protocol);

命名socket：
    将一个socket与socket地址绑定成为给socket命名。
    int bind(int sockfd, const struct sockaddr* my_addr, socklen_t addrlen);

监听socket：
    int listen(int sockfd, int backlog);

接受连接：
    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

发起连接：
    int connect(int sockfd, const struct sockaddr *serv_addr, socklend_t addrlen);

数据读写：
    ssize_t recv(int sockfd, void* buf, size_t len, int flags);
    ssize_t send(int sockfd, const void *buf, size_t len, int flags);

I/O多路复用：
I/O 多路复用使得程序能同时监听多个文件描述符，能够提高程序的性能，Linux 下实现 I/O 多路复用的
系统调用主要有 select、poll 和 epoll

select:
poll:
epoll: