#ifndef __CMD_H__
#define __CMD_H__

#include "ftp.h"

#define CMD_MAXSIZE 16  // Nombre maximum d'arguments dans une commande.

/* Types de commande */
#define CMD_T_NONE  -1  // Aucune commande.
#define CMD_T_BYE   0   // bye - Quitter le client.
#define CMD_T_GET   1   // get - Récupérer un fichier du serveur.

/* Structure d'une commande */
typedef struct command {
    int type;
    char * argv[CMD_MAXSIZE];
    size_t argc;
} command_t;


/* Fonctions de gestion des commandes */

command_t newcmd(void);

void str2cmd(char str[], command_t * c);

void freecmd(command_t * c);


/* Commandes côté serveur */

void get_sv(int cfd, char *filename);


/* Commandes côté client */

void get_cl(rio_t *rp, char *buf);

#endif // !__CMD_H__