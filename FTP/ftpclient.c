/*
 * ftpclient.c
 */
#include "csapp.h"
#include "ftp.h"
#include "cmd.h"

/* Programme principal du client FTP. */
int main(int argc, char **argv)
{
    int clientfd;
    char *host, buf[MAXLINE];
    rio_t rio;
    char outlen[MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <host>\n", argv[0]);
        exit(0);
    }
    host = argv[1];

    /*
     * Note that the 'host' can be a name or an IP address.
     * If necessary, Open_clientfd will perform the name resolution
     * to obtain the IP address.
     */
    clientfd = Open_clientfd(host, CL_PORT);

    /*
     * At this stage, the connection is established between the client
     * and the server OS ... but it is possible that the server application
     * has not yet called "Accept" for this connection
     */
    printf("%s Client connected to server [%s:%d].\n", CL_PFX, host, CL_PORT);
    printf("%s ", CL_PFX);

    Rio_readinitb(&rio, clientfd);

    // Fgets(buf, MAXLINE, stdin);
    // Rio_writen(clientfd, buf, strlen(buf));
    // while (Rio_readlineb(&rio, buf, MAXLINE) > 0) Fputs(buf, stdout);


    // while (Fgets(buf, MAXLINE, stdin) != NULL) {
    //     Rio_writen(clientfd, buf, strlen(buf));
    //     if (Rio_readlineb(&rio, buf, MAXLINE) > 0) {
    //         printf("%s Command '%s' received by server.\n", CL_PFX, buf);
    //     } else { /* the server has prematurely closed the connection */
    //         break;
    //     }
    //     printf("%s ", CL_PFX);
    // }


    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        strip(buf);
        if (strcmp(buf, "\n")) {
            sprintf(outlen, "%05d", (int)strlen(buf));
            Rio_writen(clientfd, outlen, 5);
            Rio_writen(clientfd, buf, atoi(outlen));
            printf("%s\n", outlen);
            printf("%s\n", buf);
            // Rio_writen(clientfd, buf, strlen(buf));
            // Rio_readlineb(&rio, msglen, sizeof(int));
            // printf("> %d\n", *msglen);
            // if (Rio_readlineb(&rio, buf, atoi(msglen)) > 0) {
            //      printf("%s Command '%s' received by server.\n", CL_PFX, buf);
            // } else { /* the server has prematurely closed the connection */
            //     break;
            // }
            // free(msglen);
        }
        printf("%s ", CL_PFX);
    }

    Close(clientfd);
    printf("\n%s Client disconnected from the server [%s:%d].\n", CL_PFX, host, CL_PORT);
    exit(0);
}
