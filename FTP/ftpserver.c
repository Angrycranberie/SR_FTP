/*
 * ftpserver.c
 * Serveur FTP
 *
 * Mathias DEPLANQUE
 * Alexis YVON
 * Groupe 2
 *
 */

#include "cmd.h"

/* Création du pool de sous-processus. */
pid_t pool[SV_NPROC];

void shutchildren(int sig) {
    int i;
    for(i=0; i<SV_NPROC; i++) Kill(pool[i], SIGKILL);
    printf("\n%s Server shutting down gracefully...\n", SV_PFX);
    exit(0);
}

/* Programme principal du serveur FTP. */
int main(int argc, char **argv) {
    int listenfd, connfd, i;
    socklen_t clientlen;
    struct sockaddr_in clientaddr; // Port du client.
    char client_ip_string[INET_ADDRSTRLEN]; // Adresse IP du client.
    char client_hostname[MAX_NAME_LEN]; // Nom du client.
    char buf[MAXLINE]; // Buffer où récupérer les données envoyées par le client.
    rio_t rio;
    command_t cmd = newcmd();

    /* Sauvegarde du PID du serveur principal. */
    pid_t ppid = getpid();

    /* Récupération du signal d'arrêt du serveur pour le transmettre aux sous-processus. */
    Signal(SIGINT, shutchildren);

    clientlen = (socklen_t)sizeof(clientaddr);
    listenfd = Open_listenfd(SV_PORT);

    printf("%s Server opened on port \033[1;37m%d\033[0m.\n", SV_PFX, SV_PORT);

    /* Création des sous-processus du pool.
    * Un sous-processus s'occupe d'un unique client. */
    for (i=0; i<SV_NPROC; i++) {
        if (getpid() == ppid) {
            pool[i] = Fork();
        }
    }

    /* Boucle de travail des sous-processus du serveur.
    * Tout le traitement des commandes envoyées par le client s'effectue ici. */
    while (1) {
        if (getpid() != ppid) {
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); // Ouverture du la connexion entre le serveur et le client.

            /* Récupération du nom du client */
            Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAX_NAME_LEN, 0, 0, 0);

            /* Détermination de la représentation textuelle de l'adresse IP du client */
            Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string, INET_ADDRSTRLEN);

            printf("%s Server \033[1;32mconnected\033[0m to \033[1;37m%s\033[0m (\033[1;37m%s\033[0m).\n", SV_PFX, client_hostname, client_ip_string);

            Rio_readinitb(&rio, connfd);

            ftp_get(&rio, buf); // Récupération de la commande du client.
            str2cmd(buf, &cmd); // Transformation de l'entrée récupérée en commande.
            switch (cmd.type) { // Choix de la procédure à suivre suivant la commande entrée.
                case CMD_T_NONE:
                case CMD_T_BYE:
                    break;
                case CMD_T_GET:
                    printf("%s File \033[1;37m'%s'\033[0m requested by client \033[1;37m%s\033[0m (\033[1;37m%s\033[0m)...\n", SV_PFX, cmd.argv[1], client_hostname, client_ip_string);
                    get_sv(&rio, cmd.argv[1]); // Traitement de la commande 'get' côté serveur.
                    break;
                default:
                    break;
            }
            freecmd(&cmd);

            Close(connfd);
            printf("%s Client \033[1;37m%s\033[0m (\033[1;37m%s\033[0m) \033[1;91mdisconnected\033[0m from server.\n", SV_PFX, client_hostname, client_ip_string);
        }
    }

    exit(0);
    return 0;
}

