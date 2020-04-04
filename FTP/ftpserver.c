/*
 * ftpserver.c
 *
 * Mathias DEPLANQUE
 * Alexis YVON
 * Groupe 2
 *
 */

#include "csapp.h"
#include "ftp.h"
#include "cmd.h"

/* Création du pool de processus esclaves. */
pid_t pool[SV_NPROC];

void shutchildren(int sig) {
    int i;
    for(i=0; i<SV_NPROC; i++) Kill(pool[i], SIGKILL);
    printf("\n%s Server shutting down gracefully...\n", SV_PFX);
    exit(0);
}

// void get(int connfd);

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

    listenfd = Open_listenfd(SV_PORT);

    printf("%s Server opened on port %d.\n", SV_PFX, SV_PORT);

    /* Création des esclaves du pool de processus.
    * Un esclave s'occupe d'un unique client. */
    for (i=0; i<SV_NPROC; i++) {
        if (getpid() == ppid) {
            // printf("%s Creation of server slave n°%d...\n", SV_PFX, i+1);
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

            printf("%s Server connected to %s (%s).\n", SV_PFX, client_hostname, client_ip_string);

            Rio_readinitb(&rio, connfd);

            // while (1) {
            while (Rio_readnb(&rio, inlen, 5) != 0) {
                printf("taille string : %s\n",inlen);
                printf("taille int : %d\n", atoi(inlen));
                // Rio_readlineb(&rio, inlen, MAXLINE);
                Rio_readnb(&rio, buf, (size_t)atoi(inlen));
                Rio_writen(1, buf,(size_t)atoi(inlen));
                // printf("%s Server received %u bytes.\n", SV_PFX, (unsigned int)n);
                // cmdsize = parsecmd(args, buf);
                // ml = strlen(args[0]);
                // printf("%s Command '%s' sent by client %s (%s).\n", SV_PFX, args[0], client_hostname, client_ip_string);
                // printf("> %d\n", ml);
                // Rio_writen(connfd, &ml, sizeof(int));
                // Rio_writen(connfd, args[0], strlen(args[0]));
                // freecmd(args, cmdsize);
            }

            // get(connfd);

            Close(connfd);
            printf("%s Client %s (%s) disconnected from the server.\n", SV_PFX, client_hostname, client_ip_string);
        }
    }

    exit(0);
    return 0;
}

