#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc <= 2) {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    int ret = bind(sockfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sockfd, 5);
    assert(ret != -1);

    // 创建fd_set集合，存放需要检测的文件描述符
    fd_set rdset;
    FD_ZERO(&rdset);
    FD_SET(sockfd, &rdset);
    struct timeval T;
    T.tv_sec = 1;
    int maxfd = sockfd;
    // 创建fd_set集合，存放需要检测的文件描述符
    fd_set rdset, tmp;
    FD_ZERO(&rdset);
    FD_SET(sockfd, &rdset);
    struct timeval T;
    T.tv_sec = 1;
    int maxfd = sockfd;
    while (1) {
        tmp = rdset;
        ret = select(maxfd + 1, &tmp, NULL, NULL, &T);
        assert(ret != -1);
        if (ret == 0) {
            printf("none connect\n");
        } else {
            printf("connect\n");
            if (FD_ISSET(sockfd, &tmp)) {
                struct sockaddr_in client;
                socklen_t client_address = sizeof(client);
                int fd = accept(sockfd, (sockaddr*)&client, &client_address);
                FD_SET(fd, &rdset);
                maxfd = maxfd > fd ? maxfd : fd;
            }
            for (int i = sockfd + 1; i <= maxfd; ++i) {
                if (FD_ISSET(i, &tmp)) {
                    char buf[1024];
                    int len = read(i, buf, sizeof(buf));
                    assert(len != -1);
                    if (!len) {
                        printf("client close...");
                        close(i);
                        FD_CLR(i, &tmp);
                    }
                    printf("read buf = %s\n", buf);
                    write(i, buf, strlen(buf) + 1);
                }
            }
        }
    }

    return 0;
}