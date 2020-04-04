#ifndef _FTP_H_
#define _FTP_H_

/* Constantes générales */
#define MAX_NAME_LEN 256                        // Longueur maximum du nom de domaine d'un client.
#define MAX_ARG_LEN 256                         // Longueur maximum d'un argument de commande.
#define MAX_CMD_SIZE 16                         // Nombre maximum d'arguments dans une commande.

/* Constantes liées au serveur */
#define SV_PFX "\033[1;94mftpserver>\033[0m"    // Préfixe des logs générés par le serveur.
#define SV_PORT 2118                            // Port utilisé par le serveur que les clients devront renseigner pour se connecter.
#define SV_NPROC 10                             // Nombre de clients pouvant se connecter simultanément au serveur.

/* Constantes liées au client */
#define CL_PFX "\033[1;32mftp>\033[0m"          // Préfixe de la console du client.
#define CL_PORT 2118                            // Port du serveur sur lequel se connectera le client.

/* Fonction de propagation du signal d'arrêt du serveur.
* Celle-ci permet d'éviter que les esclaves ne deviennent des zombies. */
void shutchildren(int sig);

#endif // !_FTP_H_