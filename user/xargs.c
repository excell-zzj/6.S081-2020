#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main (int argc, char *argv[]) {
    if (argc < 2) {
        printf("wrong input!\n");
        exit(1);
    }

    int argv_len;
    char buf[128];
    char *max_argv[MAXARG];
    for (int i = 1; i < argc; ++i) {
        max_argv[i-1] = argv[i];
    }

    while (gets(buf, sizeof(buf))) {
        int buf_len = strlen(buf);
        if (buf_len < 1) break;
        buf[buf_len - 1] = 0;

        argv_len = argc-1;
        char *value = buf;
        while(*value) {
            while(*value && (*value == ' ')) *value++ = 0;
            if (*value) max_argv[argv_len++] = value;
            while(*value && (*value != ' ')) value++;
        }

        if (argv_len < 1) {
            printf("too few\n");
            exit(1);
        }
        if (argv_len > MAXARG) {
            printf("too many value\n");
            exit(1);
        }

        max_argv[argv_len] = 0;
        if (fork() > 0) {
            sleep(2);
        } else {
            exec(max_argv[0], max_argv);
            exit(0);
        }

    }
    exit(0);
}