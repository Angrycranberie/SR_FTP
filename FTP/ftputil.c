/*
 * ftputil.c
 * Fonctions utilitaires aux serveur et client FTP
 *
 * Mathias DEPLANQUE
 * Alexis YVON
 * Groupe 2
 *
 */

#include "cmd.h"

/* Supprime les retour à la ligne (\n) d'une chaîne */
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

void ftp_send(int fd, char *usrbuf) {
    char outlen[INT_LEN];
    sprintf(outlen, "%05d", (int)strlen(usrbuf));
    Rio_writen(fd, outlen, INT_LEN);
    Rio_writen(fd, usrbuf, atoi(outlen));
}

void ftp_get(rio_t *rp, char *usrbuf) {
    char inlen[INT_LEN];
    Rio_readnb(rp, inlen, INT_LEN);
    Rio_readnb(rp, usrbuf, (size_t) atoi(inlen));
}