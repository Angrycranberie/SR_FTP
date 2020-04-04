/*
 * ftpclient.c
 */
#include "csapp.h"

#define PREFIX "\033[1;32mftp>\033[0m"  // Préfixe de la console du client
#define PORT 2003                       // Port du serveur sur lequel se connectera le client



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
    clientfd = Open_clientfd(host, PORT);

    /*
     * At this stage, the connection is established between the client
     * and the server OS ... but it is possible that the server application
     * has not yet called "Accept" for this connection
     */
    printf("%s Client connected to server [%s:%d].\n", PREFIX, host, PORT);
    printf("%s ", PREFIX);

    Rio_readinitb(&rio, clientfd);

    // Fgets(buf, MAXLINE, stdin);
    // Rio_writen(clientfd, buf, strlen(buf));
    // while (Rio_readlineb(&rio, buf, MAXLINE) > 0) Fputs(buf, stdout);


    // while (Fgets(buf, MAXLINE, stdin) != NULL) {
    //     Rio_writen(clientfd, buf, strlen(buf));
    //     if (Rio_readlineb(&rio, buf, MAXLINE) > 0) {
    //         printf("%s Command '%s' received by server.\n", PREFIX, buf);
    //     } else { /* the server has prematurely closed the connection */
    //         break;
    //     }
    //     printf("%s ", PREFIX);
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
            //      printf("%s Command '%s' received by server.\n", PREFIX, buf);
            // } else { /* the server has prematurely closed the connection */
            //     break;
            // }
            // free(msglen);
        }
        printf("%s ", PREFIX);
    }

    Close(clientfd);
    printf("\n%s Client disconnected from the server [%s:%d].\n", PREFIX, host, PORT);
    exit(0);
}
