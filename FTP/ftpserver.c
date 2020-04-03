/*
 * ftpserver.c
 *
 * Mathias DEPLANQUE
 * Alexis YVON
 * Groupe 2
 *
 */

#include "csapp.h"
#include "ftpserver.h"

/* Création du pool de processus esclaves. */
pid_t pool[NPROC];

void shutchildren(int sig) {
    int i;
    for(i=0; i<NPROC; i++) Kill(pool[i], SIGKILL);
    printf("\n%s Server shutting down gracefully...\n", PREFIX);
    exit(0);
}

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

void get(int connfd);

/* Programme principal du serveur FTP. */
int main(int argc, char **argv) {
    int listenfd, connfd, i/*, n, outlen*/;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN]; // 
    char client_hostname[MAX_NAME_LEN];
    char buf[MAXLINE];
    // char * args[MAX_CMD_SIZE];
    // size_t cmdsize;
    rio_t rio;
    char inlen[MAXLINE];

    /* Sauvegarde du PID du maître. */
    pid_t ppid = getpid();

    /* Récupération du signal d'arrêt du serveur pour le transmettre du maître aux esclaves. */
    Signal(SIGINT, shutchildren);

    clientlen = (socklen_t)sizeof(clientaddr);

    listenfd = Open_listenfd(PORT);

    printf("%s Server opened on port %d.\n", PREFIX, PORT);

    /* Création des esclaves du pool de processus.
    * Un esclave s'occupe d'un unique client. */
    for (i=0; i<NPROC; i++) {
        if (getpid() == ppid) {
            // printf("%s Creation of server slave n°%d...\n", PREFIX, i+1);
            pool[i] = Fork();
        }
    }

    /*  */
    while (1) {
        if (getpid() != ppid) {
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

            /* Récupération du nom du client */
            Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAX_NAME_LEN, 0, 0, 0);

            /* Détermination de la représentation textuelle de l'adresse IP du client */
            Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string, INET_ADDRSTRLEN);

            printf("%s Server connected to %s (%s).\n", PREFIX, client_hostname, client_ip_string);

            Rio_readinitb(&rio, connfd);

            // while (1) {
            while (Rio_readlineb(&rio, inlen, MAXLINE) != 0) {
                // Rio_readlineb(&rio, inlen, MAXLINE);
                Rio_readlineb(&rio, buf, atoi(inlen));
                printf("%s %s\n", PREFIX, buf);
                // printf("%s Server received %u bytes.\n", PREFIX, (unsigned int)n);
                // cmdsize = parsecmd(args, buf);
                // ml = strlen(args[0]);
                // printf("%s Command '%s' sent by client %s (%s).\n", PREFIX, args[0], client_hostname, client_ip_string);
                // printf("> %d\n", ml);
                // Rio_writen(connfd, &ml, sizeof(int));
                // Rio_writen(connfd, args[0], strlen(args[0]));
                // freecmd(args, cmdsize);
            }

            // get(connfd);

            Close(connfd);
            printf("%s Client %s (%s) disconnected from the server.\n", PREFIX, client_hostname, client_ip_string);
        }
    }

    exit(0);
    return 0;
}

