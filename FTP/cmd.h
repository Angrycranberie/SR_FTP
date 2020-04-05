#ifndef __CMD_H__
#define __CMD_H__

#include "ftp.h"

// CONSTANTES //

#define CMD_MAXSIZE 16  // Nombre maximum d'arguments dans une commande.

/* Types de commande */
#define CMD_T_NONE  -1  // Aucune commande.
#define CMD_T_BYE   0   // bye - Quitter le client.
#define CMD_T_GET   1   // get - Récupérer un fichier du serveur.

/* Raisons d'erreurs liées aux commandes */
#define CMD_ERR_UNK "Unknown command."      // Commande inconnue.
#define CMD_ERR_NEA "Not enough arguments." // Pas assez d'arguments passés en entrée.


// STRUCTURES //

/* Structure d'une commande */
typedef struct command {
    int type;                   // Type de commande.
    char * argv[CMD_MAXSIZE];   // Arguments de la commande.
    size_t argc;                // Nombre d'arguments effectifs dans la commande.
} command_t;


// FONCTIONS //

/** Fonctions de gestion des commandes **/

/* Fonction d'instanciation d'une commande. */
command_t newcmd(void);

/* Fonction de conversion d'une chaîne vers une commande.
* Le délimiteur par défaut est l'espace, i.e. les noms de fichiers avec
* espaces ne sont pas pris en compte. */
void str2cmd(char str[], command_t * c);

/* Fonction de libération de la mémoire attribuée aux arguments d'une commande. */
void freecmd(command_t * c);


/** Commandes côté serveur **/

/* Commande 'get' gérée par le serveur. */
void get_sv(rio_t *rio, char *filename);


/** Commandes côté client **/

/* Commande 'get' gérée par le client.
* Renvoie la taille du fichier reçu. */
double get_cl(rio_t *rp, char *buf);

/* Fonction permettant d'afficher le temps mis par la commande 'get'
* pour récupérer un fichier. */
void print_gettime(double bytes, clock_t a, clock_t b);

#endif // !__CMD_H__