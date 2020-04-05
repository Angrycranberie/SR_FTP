#ifndef __FTP_H__
#define __FTP_H__

#include "csapp.h"

// CONSTANTES //

/* Constantes générales */
#define MAX_NAME_LEN 256                        // Longueur maximum du nom de domaine d'un client.
#define INT_LEN 5                               // Nombre de chiffres pouvant représenter un entier sur 32 bits (max. 65536).

/* Constantes liées au serveur */
#define SV_PFX "\033[1;94mftpserver>\033[0m"    // Préfixe des logs générés par le serveur.
#define SV_PORT 2119                            // Port utilisé par le serveur que les clients devront renseigner pour se connecter.
#define SV_NPROC 10                             // Nombre de clients pouvant se connecter simultanément au serveur.

/* Constantes liées au client */
#define CL_PFX "\033[1;32mftp>\033[0m"          // Préfixe de la console du client.
#define CL_PORT 2119                            // Port du serveur sur lequel se connectera le client.



// FONCTIONS //

/* Fonctions générales */

/* Supprime les retour à la ligne (\n) d'une chaîne */
void strip(char *s);

void ftp_send(int fd, char *usrbuf,int taille);

void ftp_get(rio_t *rp, char *usrbuf);


/* Fonctions liées au serveur */

/* Fonction de propagation du signal d'arrêt du serveur.
* Celle-ci permet d'éviter que les esclaves ne deviennent des zombies. */
void shutchildren(int sig);

#endif // !__FTP_H__