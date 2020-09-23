#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

int wanted_file(char *path, char *filename) {
    char *p;
    for (p = path+strlen(path); p>=path && *p != '/'; p--) {

    }
    p++;
    if (strcmp(p, filename)==0) return 1;
    else return 0;
}

void find(char* path, char *filename)
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
    // printf("file\n");
    // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    if (wanted_file(path, filename)) {
        printf("%s\n", path);
    }
    break;

  case T_DIR:
  //printf("dir\n");
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      
    
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, filename);
    //   if (strcmp(filename, de.name) == 0) {
    //     printf("find\n");
    //     printf("./%s\n", de.name);
    //   }
    //   if(stat(buf, &st) < 0){
    //     printf("ls: cannot stat %s\n", buf);
    //     continue;
    //   }
    //   if(buf == filename)
    //     printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  

  if(argc < 3){
    write(1, "input wrong!\n", 13);
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
