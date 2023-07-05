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