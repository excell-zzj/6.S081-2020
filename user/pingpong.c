#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"

#define MAX_LINE 80

int main(int argc, char *argv[]) {
    int fd[2], fd2[2];
    pipe(fd);
    pipe(fd2);
    char buf[MAX_LINE];

    if (fork() == 0) {
        close(fd[0]);
        close(fd2[1]);
        read(fd2[0], buf, 13);
        printf("%d: %s\n", getpid(), buf);
        write(fd[1], "received ping", 13);
    }else {
        close(fd[1]);
        close(fd2[0]);
        write(fd2[1], "received pong", 13);
        read(fd[0], buf, 13);
        printf("%d: %s\n", getpid(), buf);
        
    }
    exit(0);
}