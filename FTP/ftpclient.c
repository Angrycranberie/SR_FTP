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
    char buf[MAXLINE];
    rio_t rio;
    command_t cmd = newcmd();

    if (argc != 2) {
        fprintf(stderr, "\033[1;93mUsage:\033[0m %s <host>\n", argv[0]);
        exit(0);
    }
    host = argv[1];

    clientfd = Open_clientfd(host, CL_PORT);

    printf("%s Client \033[1;32mconnected\033[0m to server [%s:%d].\n", CL_PFX, host, CL_PORT);
    printf("%s ", CL_PFX);

    Rio_readinitb(&rio, clientfd);

    Fgets(buf, MAXLINE, stdin);
    strip(buf); // Enlèvement des retours à la ligne gênants.
    ftp_send(clientfd, buf);
    str2cmd(buf, &cmd);
    switch (cmd.type) {
        case CMD_T_GET:
            get_cl(&rio, buf);
            break;
        default:
            break;
    }
    freecmd(&cmd);

    Close(clientfd);
    printf("\n%s Client \033[1;91mdisconnected\033[0m from server [%s:%d].\n", CL_PFX, host, CL_PORT);
    exit(0);
}
