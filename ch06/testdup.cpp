#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd = open("a.txt", O_RDWR | O_CREAT, 0664);

    int fd1 = dup(fd);

    assert(fd1 != -1);

    printf("fd: %d fd1: %d\n", fd, fd1);

    close(fd);

    char *str = "fd1 write!!!!";
    int ret = write(fd1, str, strlen(str));
    assert(ret != -1);

    close(fd1);
}