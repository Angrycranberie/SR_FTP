/*
 * echoserveri.c - An iterative echo server
 */

#include "csapp.h"

#define MAX_NAME_LEN 256
#define NPROC 10

pid_t proc[NPROC];

void shutchildren(int sig) {
    int i;
    for(i=0; i<NPROC; i++){
        Kill(proc[i], SIGKILL);
    }
    exit(0);
}

void echo(int connfd);

/*
 * Note that this code only works with IPv4 addresses
 * (IPv6 is not supported)
 */
int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
    pid_t perePid = getpid();
    Signal(SIGINT, shutchildren);

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);

    clientlen = (socklen_t)sizeof(clientaddr);

    listenfd = Open_listenfd(port);

    int i;
    for(i=0; i<NPROC; i++){
        if (getpid() == perePid) { /* parent */
        proc[i] = Fork();
    }
}

while (1) {
    if(getpid() != perePid){
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

    /* determine the name of the client */
        Getnameinfo((SA *) &clientaddr, clientlen,
            client_hostname, MAX_NAME_LEN, 0, 0, 0);

    /* determine the textual representation of the client's IP address */
        Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
            INET_ADDRSTRLEN);

        printf("server connected to %s (%s)\n", client_hostname,
            client_ip_string);

        echo(connfd);
        Close(connfd);
    }
}

exit(0);

return 0;
}

