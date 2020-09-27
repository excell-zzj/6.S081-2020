#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  //static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

    return p;
}

void
find(char *path, char *name)
{
   char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if (strcmp(name, fmtname(path)) == 0) {
        printf("%s\n", path);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    char *init = p;
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        p = init;
        if(de.inum == 0)
            continue;
        for(int i = 0; de.name[i] != 0; i++)
                *p++ = de.name[i];
        *p++ = 0;
        if(stat(buf, &st) < 0)
            continue;
        if(strcmp(fmtname(buf),".") != 0 && strcmp(fmtname(buf),"..") != 0)
            find(buf,name);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  //int i;

  if(argc < 2){
    fprintf(2, "Usage : find path filename\n");
    exit(1);
  }

    // const char *path = argv[1];
    // const char *filename = argv[2];
    find(argv[1], argv[2]);

    exit(0);
}
