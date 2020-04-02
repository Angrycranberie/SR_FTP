/*
 * echo - read and echo text lines until client closes connection
 */
#include "csapp.h"

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

void echo(int connfd)
{
    char buf[MAXLINE];
    rio_t rio;
    int fd;
    char * fn;
    FILE * f;

    Rio_readinitb(&rio, connfd);
    // while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    //     printf("server received %u bytes\n", (unsigned int)n);
    //     printf("%s\n", buf);
    //     Rio_writen(connfd, buf, n);
    // }

    Rio_readlineb(&rio, buf, MAXLINE);
    fn = buf;
    strip(fn);
    printf("%s\n", fn);

    fd = Open(fn, O_RDONLY, S_IRUSR);
    f = Fdopen(fd,"r");
    if (fd && f != NULL) {
        // while((m = Read(fd,buf,MAXLINE)) != 0){
        //     Rio_writen(connfd, buf, m);
        // }
        while (Fgets(buf, MAXLINE, f) != NULL) {
            printf("%s\n", buf);
            Rio_writen(connfd, buf, strlen(buf));
        }
    }
    Fclose(f);
}

