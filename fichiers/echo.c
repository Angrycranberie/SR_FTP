/*
 * echo - read and echo text lines until client closes connection
 */
#include "csapp.h"

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

void echo(int connfd)
{
    //size_t n;
    size_t m;
    char buf[MAXLINE];
    rio_t rio;
    int fd;
    char * filename;

    Rio_readinitb(&rio, connfd);
    // while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    //     printf("server received %u bytes\n", (unsigned int)n);
    //     printf("%s\n", buf);
    //     Rio_writen(connfd, buf, n);
    // }

    // n = Rio_readlineb(&rio, buf, MAXLINE);
    Rio_readlineb(&rio, buf, MAXLINE);
    filename = buf;
    strip(filename);
    printf("%s\n", filename);
    // Rio_writen(connfd, buf, n);

    fd = Open(filename, O_RDONLY, S_IRUSR);
    if (fd) {
        while((m = Read(fd,buf,MAXLINE)) != 0){
            Rio_writen(connfd, buf, m);
        }
    }
}

