#include "kernel/types.h"
#include "user/user.h"

void source() {
  int i;
  for (i = 2; i < 36; i++) {
    write(1, &i, sizeof(i));
  }
}
 
void cull(int p) {
  int n;
  while (read(0, &n, sizeof(n))) {
    if (n % p != 0) {
      write(1, &n, sizeof(n));
    }
  }
}
 
void redirect(int k, int pd[]) {
  close(k);
  dup(pd[k]);
  close(pd[0]);
  close(pd[1]);
}
 
void sink() {
  int pd[2];
  int p;
  if (read(0, &p, sizeof(p))) {
    printf("prime %d\n", p);
    pipe(pd);
    if (fork()) {
      redirect(0, pd);
      sink();
    } else {
      redirect(1, pd);
      cull(p);
    }
  }
}
 
int main( ) {
  int pd[2];
  pipe(pd);
  if (fork()>0) {  // 父进程
    // 重定向标准输入文件到pd[0]
    redirect(0, pd);
    sink();
  } else {
    //子进程重定向标准输出文件到pd[1]
    redirect(1, pd);
    source();
  }
  exit(0);
}
