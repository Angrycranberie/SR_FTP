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
    fd = open(fn, O_RDONLY, S_IRUSR);
    Rio_readinitb(&riof,fd);

    if (fd) {
        ftp_send(rio->rio_fd, "ok",2);
        ftp_get(rio, retourcl);
        printf("%s\n", retourcl);
        if(!strcmp(retourcl, "ok")){
            printf("%s Sending file '%s' to client...\n", SV_PFX, fn);
            while ((n = Rio_readnb(&riof, buf, 1024)) != 0) {
            //printf("%s\n", buf);
                ftp_send(rio->rio_fd, buf,n);
            }
            Close(fd);
        }else {
            printf("Erreur client\n");
        }
        
    }else{
        ftp_send(rio->rio_fd,buf,0);
        printf("Problème lors de l'ouverture du fichier\n");
    }
}

void get_cl(rio_t *rp, char *buf) {
    char inlen[INT_LEN];
    char retoursv[3] = "";
    retoursv[2] = '\0'; 
    char name[MAXLINE];
    int fd, n; 
    double nbBytes,DLDelta;
    //long clk_tck = CLOCKS_PER_SEC;
    clock_t DLTimeb, DLTimea;

    //printf("retoursv : %s\n", retoursv);
    ftp_get(rp, retoursv);
    //printf("retoursv : %s\n", retoursv);
    

    if(!strcmp(retoursv,"ok")){
        printf("Donner le nom du fichier : \n");
        Fgets(name, MAXLINE, stdin);
        strip(name);
        ftp_send(rp->rio_fd, "ok",2);
        fd = open(name, O_CREAT | O_RDWR, S_IRWXU);
        DLTimeb = clock();
        while (Rio_readnb(rp, inlen, INT_LEN) != 0){
            n = Rio_readnb(rp, buf, atoi(inlen));
            //printf("%d\n", n);
            nbBytes += n;
            Rio_writen(fd, buf, atoi(inlen));
        }
        DLTimea = clock();
        DLDelta = (double)(DLTimea - DLTimeb);
        printf("%lf bytes received in %lf seconds (%lf KB/s)\n", nbBytes, DLDelta/(double)CLOCKS_PER_SEC, (double)(nbBytes/1000.0)/(DLDelta/(double)CLOCKS_PER_SEC) );
    }else{
        printf("fichier non trouvé ou problème lors de l'ouverture\n");
    }


}