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
#include <time.h>

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
    else if (!strcmp(c->argv[0], "bye")) c->type = CMD_T_BYE;
    else if (!strcmp(c->argv[0], "error")) c->type = CMD_T_NONE;
    else c->type = CMD_T_NONE;
}

void freecmd(command_t * c) {
    int i;
    for (i=0; i < c->argc; i++) Free(c->argv[i]);
}

void get_sv(rio_t *rio, char *filename) {
    int fd, n = 0;
    char buf[MAXLINE];
    char retourcl[2];
    rio_t riof;

    char * fn = filename;

    if ((fd = open(fn, O_RDONLY, S_IRUSR)) > 0) {
        printf("%s File found !\n", SV_PFX);
        ftp_send(rio->rio_fd, "ok", 2);
        ftp_get(rio, retourcl);

        if (!strcmp(retourcl, "ok")) {
            printf("%s Sending file \033[1;37m'%s'\033[0m to client...\n", SV_PFX, fn);
            Rio_readinitb(&riof, fd);
            while ((n = Rio_readnb(&riof, buf, FILE_CHUNKSIZE)) != 0) ftp_send(rio->rio_fd, buf, n);
            Close(fd);
        } else {
            ftp_error(0, SV_PFX, ERR_CL, FTP_ERR_BRCL);
        }
    } else {
        ftp_error(rio->rio_fd, SV_PFX, ERR_FILE, FTP_ERR_NFCOF);
    }
}

double get_cl(rio_t *rp, char *buf) {
    char inlen[INT_LEN]; // Taille du message entrant.
    char name[MAXLINE]; // Nom du fichier à créer chez le client.
    int fd, n;
    double bytes; // Taille reçue lors du transfert.

    /* */
    char retoursv[3] = "";
    retoursv[2] = '\0';

    ftp_get(rp, retoursv);

    if (!strcmp(retoursv,"ok")) {
        printf("%s File found !\n", CL_PFX);
        printf("%s Please give a new name to the requested file :\n> ", CL_PFX);
        Fgets(name, MAXLINE, stdin);
        while (!strcmp(name, "\n")) {
            printf("> ");
            Fgets(name, MAXLINE, stdin);
        }
        strip(name);
        ftp_send(rp->rio_fd, "ok", 2);
        fd = open(name, O_CREAT | O_RDWR, S_IRWXU);
        while (Rio_readnb(rp, inlen, INT_LEN) != 0){
            n = Rio_readnb(rp, buf, atoi(inlen));
            bytes += n;
            Rio_writen(fd, buf, atoi(inlen));
        }
        printf("%s File successfully retrieved from server !\n", CL_PFX);
        return bytes;
    } else {
        ftp_error(0, CL_PFX, ERR_FILE, FTP_ERR_NFCOF);
        return -1.0;
    }
}

void print_gettime(double bytes, clock_t a, clock_t b) {
    double d = (double)(b-a); // Différence de temps entre le début et la fin du téléchargement.
    double t = d/(double)CLOCKS_PER_SEC; // Même différence correctement tempérée par une constante du module.
    double v = (double)(bytes/1000.0)/(d/(double)CLOCKS_PER_SEC); // Vitesse de téléchargement.

    /* Choix du format approprié à afficher. */
    printf("%s ", CL_PFX);
    if (bytes > 1000.000000) printf("%.2lf KB", (double)(bytes/1000.000000));
    else printf("%.2lf B", bytes);
    printf(" received in ");
    if (t < 1.000000) printf("%.2lfms", (double)(t*1000.000000));
    else printf("%.2lfs", t);
    if (v > 1000.000000)  printf(" (%.2lf MB/s).\n", (double)(v/1000.000000));
    else printf(" (%.2lf KB/s).\n", v);
}