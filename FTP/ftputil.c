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

error_t newerr() {
    error_t e;
    e.type = ERR_NULL;
    e.reason = "Unspecified reason";
    return e;
}

void updterr(error_t *err, int type, char *reason) {
    err->type = type;
    err->reason = reason;
}

void printerr(char *pfx, error_t *err) {
    char *errtype;
    switch (err->type) {
        case ERR_NULL:
            errtype = "Error";
            break;
        case ERR_CMD:
            errtype = "Command error";
            break;
        case ERR_FILE:
            errtype = "File error";
            break;
        case ERR_SV:
            errtype = "Server error";
            break;
        case ERR_CL:
            errtype = "Client error";
            break;
        default:
            errtype = "Error";
            break;
    }
    fprintf(stderr, "%s \033[1;91m%s :\033[0m %s\n", pfx, errtype, err->reason);
}

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

void ftp_send(int fd, char *usrbuf, int len) {
    char outlen[INT_LEN];
    sprintf(outlen, "%05d", len);
    Rio_writen(fd, outlen, INT_LEN);
    Rio_writen(fd, usrbuf, atoi(outlen));
}

void ftp_get(rio_t *rp, char *usrbuf) {
    char inlen[INT_LEN];
    Rio_readnb(rp, inlen, INT_LEN);
    Rio_readnb(rp, usrbuf, (size_t) atoi(inlen));
}

void ftp_error(int fd, char *pfx, int type, char *reason) {
    error_t e = newerr();
    updterr(&e, type, reason);
    printerr(pfx, &e);
    if (fd) ftp_send(fd, "error", 5);
}