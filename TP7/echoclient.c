/*
 * echoclient.c - An echo client
 */
#include "csapp.h"

int main(int argc, char **argv)
{
    int clientfd, port;
    char *host, buf[MAXLINE];
    rio_t rio;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);

    /*
     * Note that the 'host' can be a name or an IP address.
     * If necessary, Open_clientfd will perform the name resolution
     * to obtain the IP address.
     */
    clientfd = Open_clientfd(host, port);

    /*
     * At this stage, the connection is established between the client
     * and the server OS ... but it is possible that the server application
     * has not yet called "Accept" for this connection
     */
    printf("client connected to server %s:%d\n",host,port);

    Rio_readinitb(&rio, clientfd);

    Fgets(buf, MAXLINE, stdin);
    Rio_writen(clientfd, buf, strlen(buf));
    while (Rio_readlineb(&rio, buf, MAXLINE) > 0) Fputs(buf, stdout);

    // while (Fgets(buf, MAXLINE, stdin) != NULL) {
    //     Rio_writen(clientfd, buf, strlen(buf));
    //     if (Rio_readlineb(&rio, buf, MAXLINE) > 0) {
    //         Fputs(buf, stdout);
    //     } else { /* the server has prematurely closed the connection */
    //         break;
    //     }
    // }

    Close(clientfd);
    exit(0);
}
