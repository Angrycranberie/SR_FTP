/*
 * ftpclient.c
 * Client FTP
 *
 * Mathias DEPLANQUE
 * Alexis YVON
 * Groupe 2
 *
 */

#include "cmd.h"

/* Programme principal du client FTP. */
int main(int argc, char **argv)
{
    int clientfd;
    char *host;
    char buf[MAXLINE], cmdbuf[MAXLINE];
    rio_t rio;
    command_t cmd = newcmd();

    if (argc != 2) {
        fprintf(stderr, "\033[1;93mUsage:\033[0m %s <host>\n", argv[0]);
        exit(0);
    }
    host = argv[1];

    clientfd = Open_clientfd(host, CL_PORT); // Connexion au serveur.

    printf("%s Client \033[1;32mconnected\033[0m to server \033[1;37m[%s:%d]\033[0m.\n", CL_PFX, host, CL_PORT);

    Rio_readinitb(&rio, clientfd);

    printf("> ");
    Fgets(buf, MAXLINE, stdin);
    while (!strcmp(buf, "\n")) {
        printf("> ");
        Fgets(buf, MAXLINE, stdin);
    }
    strip(buf); // Enlèvement d'éventuels retours à la ligne gênants.
    strcpy(cmdbuf, buf); // Copie de l'entrée pour ne pas perdre son contenu lors de la transformation en commande.
    str2cmd(cmdbuf, &cmd); // Transformation de l'entrée récupérée en commande.
    switch (cmd.type) {
        case CMD_T_NONE:
            ftp_error(clientfd, CL_PFX, ERR_CMD, CMD_ERR_UNK);
            break;
        case CMD_T_BYE:
            ftp_send(clientfd, buf, (int)strlen(buf));
            break;
        case CMD_T_GET:
            if (cmd.argc < 2) {
                ftp_error(clientfd, CL_PFX, ERR_CMD, CMD_ERR_NEA);
            } else {
                double bytes = -1.0;
                clock_t clkA, clkB;
                clkB = clock();
                printf("%s Requesting file \033[1;37m'%s'\033[0m from server...\n", CL_PFX, cmd.argv[1]);
                ftp_send(clientfd, buf, (int)strlen(buf));
                bytes = get_cl(&rio, buf);
                clkA = clock();
                if (bytes > 0.000000) print_gettime(bytes, clkB, clkA);
            }
            break;
        default:
            ftp_send(clientfd, buf, (int)strlen(buf));
            break;
    }
    freecmd(&cmd);

    Close(clientfd);
    printf("%s Client \033[1;91mdisconnected\033[0m from server \033[1;37m[%s:%d]\033[0m.\n", CL_PFX, host, CL_PORT);
    exit(0);
}
