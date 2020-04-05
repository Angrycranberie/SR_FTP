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

    clientfd = Open_clientfd(host, CL_PORT);

    printf("%s Client \033[1;32mconnected\033[0m to server [%s:%d].\n", CL_PFX, host, CL_PORT);

    Rio_readinitb(&rio, clientfd);

    printf("%s ", CL_PFX);
    Fgets(buf, MAXLINE, stdin);
    while (!strcmp(buf, "\n")) {
        printf("%s ", CL_PFX);
        Fgets(buf, MAXLINE, stdin);
    }
    strip(buf); // Enlèvement d'éventuels retours à la ligne gênants.
    strcpy(cmdbuf, buf);
    str2cmd(cmdbuf, &cmd);
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
                ftp_send(clientfd, buf, (int)strlen(buf));
                get_cl(&rio, buf);
            }
            break;
        default:
            break;
    }
    freecmd(&cmd);

    Close(clientfd);
    printf("%s Client \033[1;91mdisconnected\033[0m from server [%s:%d].\n", CL_PFX, host, CL_PORT);
    exit(0);
}
