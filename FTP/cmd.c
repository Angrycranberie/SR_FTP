#include "csapp.h"
#include "ftp.h"
#include "cmd.h"

size_t parsecmd(char * args[], char cmd[]) {
    char * delim = " ";
    int i = 0;
    char *ptr = strtok(cmd, delim);
    while(ptr != NULL) {
        args[i] = malloc(strlen(ptr) + 1);
        strcpy(args[i], ptr);
        i++;
        ptr = strtok(NULL, delim);
    }
    return (i+1);
}

void freecmd(char * args[], size_t n) {
    int i;
    for (i=0; i < n; i++) Free(args[i]);
    return;
}

// ATTENTION MAUVAIS
void get(int connfd)
{
    int n;
    char buf[MAXLINE];
    rio_t rio;
    // char * args[MAX_CMD_SIZE];
    // size_t cmdsize;
    // int fd;
    // char * fn;
    // FILE * f;

    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        // printf("%s Server received %u bytes.\n", PREFIX, (unsigned int)n);
        // cmdsize = parsecmd(args, buf);
        // printf("%s Command '%s' sent by client.\n", PREFIX, args[0]);
        // Rio_writen(connfd, args[0], strlen(args[0]));
        printf("> %s\n", buf);
        Rio_writen(connfd, buf, n);
    }

    // freecmd(args, cmdsize);

    // Rio_readlineb(&rio, buf, MAXLINE);
    // fn = buf;
    // strip(fn);
    // printf("%s\n", fn);

    // fd = Open(fn, O_RDONLY, S_IRUSR);
    // f = Fdopen(fd,"r");
    // if (fd && f != NULL) {
    //     // while((m = Read(fd,buf,MAXLINE)) != 0){
    //     //     Rio_writen(connfd, buf, m);
    //     // }
    //     while (Fgets(buf, MAXLINE, f) != NULL) {
    //         printf("%s\n", buf);
    //         Rio_writen(connfd, buf, strlen(buf));
    //     }
    // }
    // Fclose(f);
}