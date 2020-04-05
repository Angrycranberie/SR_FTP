#ifndef __FTP_H__
#define __FTP_H__

#include "csapp.h"

// STRUCTURES //

/* Erreur */
typedef struct error {
    int type;
    char *reason;
} error_t;


// CONSTANTES //

/* Constantes générales */
#define MAX_NAME_LEN 256        // Longueur maximum du nom de domaine d'un client.
#define INT_LEN 5               // Nombre de chiffres pouvant représenter un entier sur 32 bits (max. 65536).
#define FILE_CHUNKSIZE 1024     // Taille d'un paquet à envoyer/recevoir pour le transfert d'un fichier.

/* Constantes liées au serveur */
#define SV_PFX "\033[1;94mftpserver>\033[0m"    // Préfixe des logs générés par le serveur.
#define SV_PORT 2118                            // Port utilisé par le serveur que les clients devront renseigner pour se connecter.
#define SV_NPROC 10                             // Nombre de clients pouvant se connecter simultanément au serveur.

/* Constantes liées au client */
#define CL_PFX "\033[1;32mftp>\033[0m"  // Préfixe de la console du client.
#define CL_PORT 2118                    // Port du serveur sur lequel se connectera le client.

/* Types d'erreurs */
#define ERR_NULL    0   // Erreur non spécifiée ou globale.
#define ERR_CMD     1   // Erreur de commande.


// FONCTIONS //

/* Fonctions générales */

error_t newerr(void);

void updterr(error_t *err, int type, char *reason);

void printerr(char *pfx, error_t *err);

/* Supprime les retour à la ligne (\n) d'une chaîne */
void strip(char *s);

void ftp_send(int fd, char *usrbuf, int len);

void ftp_get(rio_t *rp, char *usrbuf);

void ftp_error(int fd, char *pfx, int type, char *reason);


/* Fonctions liées au serveur */

/* Fonction de propagation du signal d'arrêt du serveur.
* Celle-ci permet d'éviter que les esclaves ne deviennent des zombies. */
void shutchildren(int sig);

#endif // !__FTP_H__