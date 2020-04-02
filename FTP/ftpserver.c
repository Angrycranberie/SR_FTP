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

void get(int connfd);

/* Programme principal du serveur FTP. */
int main(int argc, char **argv) {
    int listenfd, connfd, i;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN]; // 
    char client_hostname[MAX_NAME_LEN];

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

            // TODO : Traitement de la commande envoyée par le client
            get(connfd);

            Close(connfd);
            printf("%s Client %s (%s) disconnected from the server.\n", PREFIX, client_hostname, client_ip_string);
        }
    }

    exit(0);
    return 0;
}

