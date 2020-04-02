/*
 * get
 */

#include "csapp.h"
#include "ftpserver.h"

// Deleting the annoying \n from a string
void strip(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}

void get(int connfd)
{
    int n;
    char buf[MAXLINE];
    rio_t rio;
    // int fd;
    // char * fn;
    // FILE * f;

    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("%s Server received %u bytes.\n", PREFIX, (unsigned int)n);
        printf("> %s\n", buf);
        Rio_writen(connfd, buf, n);
    }

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

