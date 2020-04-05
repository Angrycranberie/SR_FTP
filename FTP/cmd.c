/*
 * cmd.c
 * Fonctions concernant les commandes d'un FTP
 *
 * Mathias DEPLANQUE
 * Alexis YVON
 * Groupe 2
 *
 */

#include "cmd.h"

command_t newcmd(void) {
    command_t c;
    c.type = -1;
    c.argc = 0;
    return c;
}

void str2cmd(char str[], command_t * c) {
    char *d = " ";
    char *ptr = strtok(str, d);
    while (ptr != NULL) {
        c->argv[c->argc] = malloc(strlen(ptr) + 1);
        strcpy(c->argv[c->argc], ptr);
        c->argc++;
        ptr = strtok(NULL, d);
    }
    if (!strcmp(c->argv[0], "get")) c->type = CMD_T_GET;
    else c->type = CMD_T_NONE;
}

void freecmd(command_t * c) {
    int i;
    for (i=0; i < c->argc; i++) Free(c->argv[i]);
}

void get_sv(int cfd, char *filename) {
    int fd, n = 0;
    char buf[MAXLINE];
    rio_t rio;

    char * fn = filename;
    fd = open(fn, O_RDONLY, S_IRUSR);
    Rio_readinitb(&rio,fd);

    if (fd) {
        printf("%s Sending file '%s' to client...\n", SV_PFX, fn);
        while ((n = Rio_readnb(&rio, buf, 1024)) != 0) {
            // printf("%s\n", buf);
            ftp_send(cfd, buf,n);
        }
        Close(fd);
    }else{
        ftp_send(cfd,buf,0);
        printf("Problème lors de l'ouverture du fichier\n");
    }
}

void get_cl(rio_t *rp, char *buf) {
    char inlen[INT_LEN];
    while (Rio_readnb(rp, inlen, INT_LEN) != 0){
        Rio_readnb(rp, buf, atoi(inlen));
        Rio_writen(STDOUT_FILENO, buf, atoi(inlen));
    }
}