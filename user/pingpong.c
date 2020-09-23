#include "kernel/types.h"
#include "user/user.h"


int main()
{
  int n1, n2, pid;
  int fd1[2], fd2[2];
  char buf1[100], buf2[100];
  
  // create a pipe, with two FDs in fds[0], fds[1].
  pipe(fd1);
  pipe(fd2);

  pid = fork();
  if (pid == 0) {
      close(fd1[0]);
      write(fd1[1], "received pong\n", 16);
      //sleep(1);
      n2 = read(fd2[0], buf2, sizeof(buf2));
      printf("%d: ", getpid());
      write(1, buf2, n2);
      sleep(3);

  } else {
      close(fd2[0]);
      write(fd2[1], "received ping\n", 14);
      sleep(1);
      n1 = read(fd1[0], buf1, sizeof(buf1));
      printf("%d: ", getpid());
      write(1, buf1, n1);
      sleep(3);
  }

  exit(0);
}