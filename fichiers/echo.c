/*
 * echo - read and echo text lines until client closes connection
 */
#include "csapp.h"

void echo(int connfd)
{
    size_t n,m;
    char buf[MAXLINE];
    char fbuf[MAXLINE];
    rio_t rio;

    int fd;

    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {



    	printf("%s\n", buf );
    	fd = Open(buf, O_RDONLY, S_IRUSR);
    	while((m = Read(fd,fbuf,MAXLINE)) != 0){
    		Rio_writen(connfd, fbuf, m);
    	}

        printf("server received %u bytes\n", (unsigned int)n);
        Rio_writen(connfd, buf, n);
    }
}

