#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc <= 2) {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof (server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);

    int ret = bind(sockfd, (struct sockaddr*)&server_address, sizeof (server_address));
    assert(ret != -1);

    ret = listen(sockfd, 5);
    assert(ret != -1);

    // 用epoll_create()创建一个epoll实例
    int epfd = epoll_create(100);
    
    // 将监听的文件描述符添加到实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &epev);

    struct epoll_event epevs[1024];

    while (1) {
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        assert(ret != -1);
        printf("ret = %d\n", ret);
        for (int i = 0;i < ret; ++i) {
            int currfd = epevs[i].data.fd;
            if (currfd == sockfd) {
                // 监听的文件描述符有数据到达，有客户端连接
                struct sockaddr_in client_adddress;
                socklen_t client_length = sizeof(client_adddress);
                int connfd = accept(sockfd, (struct sockaddr*)&client_adddress, &client_length);
                epev.events = EPOLLIN;
                epev.data.fd = connfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &epev);
            } else {
                // 有数据到达，需要通信
                char buf[1024] = {'\0'};
                int len = read(currfd, buf, sizeof(buf));
                assert(len != -1);
                if (len == 0) {
                    printf("client close ...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, currfd, NULL);
                    close(currfd);
                } else {
                    printf("read buf = %s\n", buf);
                    write(currfd, buf, strlen(buf) + 1);
                }
            }
        }
    }
    close(sockfd);
    close(epfd);
    return 0;
}