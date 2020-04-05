#ifndef __FTP_H__
#define __FTP_H__

#include "csapp.h"

// CONSTANTES //

/* Constantes générales */
#define MAX_NAME_LEN 256        // Longueur maximum du nom de domaine d'un client.
#define INT_LEN 5               // Nombre de chiffres pouvant représenter un entier sur 32 bits (max. 65536).
#define FILE_CHUNKSIZE 1024     // Taille d'un paquet à envoyer/recevoir pour le transfert d'un fichier.

/* Constantes liées au serveur */
#define SV_PFX "\033[1;94mftpserver>\033[0m"    // Préfixe des logs générés par le serveur.
#define SV_PORT 2120                            // Port utilisé par le serveur que les clients devront renseigner pour se connecter.
#define SV_NPROC 10                             // Nombre de clients pouvant se connecter simultanément au serveur.

/* Constantes liées au client */
#define CL_PFX "\033[1;32mftp>\033[0m"  // Préfixe de la console du client.
#define CL_PORT 2120                    // Port du serveur sur lequel se connectera le client.

/* Types d'erreurs */
#define ERR_NULL    0   // Erreur non spécifiée ou globale.
#define ERR_CMD     1   // Erreur de commande.
#define ERR_FILE    2   // Erreur de fichier.
#define ERR_SV      3   // Erreur du serveur.
#define ERR_CL      4   // Erreur du client.

/* Raisons d'erreurs générales */
#define FTP_ERR_BRCL   "Bad response from client."             // Erreur si le client est interrompu lors d'un transaction.
#define FTP_ERR_NFCOF  "File not found or can't be opened."    // Échec d'ouverture du fichier ou fichier introuvable.


// STRUCTURES //

/* Erreur */
typedef struct error {
    int type;
    char *reason;
} error_t;


// FONCTIONS //

/** Fonctions générales **/

/* Fonction permettant d'instancier une erreur.
* La raison de cette dernière n'est pas spécifiée. */
error_t newerr(void);

/* Fonction permettant de mettre à jour le type et la raison d'une erreur. */
void updterr(error_t *err, int type, char *reason);

/* Fonction permettant d'afficher une erreur avec un préfixe spécifié (client, serveur, autre). */
void printerr(char *pfx, error_t *err);

/* Supprime les retours à la ligne (\n) d'une chaîne. */
void strip(char *s);

/* Fonctions d'envoi et de réception de données.
* Ces fonctions suivent la procédure suivante :
* - On envoie/reçoit la taille des données,
* - On envoie/reçoit les données en fonction de la taille indiquée. */
void ftp_send(int fd, char *usrbuf, int len);   // Envoi
void ftp_get(rio_t *rp, char *usrbuf);          // Réception

/* Fonction permettant de gérer et d'afficher une erreur rencontrée.
* Si le descripteur fd est nul, l'erreur n'est pas répercutée sur  */
void ftp_error(int fd, char *pfx, int type, char *reason);

/** Fonctions liées au serveur **/

/* Fonction de propagation du signal d'arrêt du serveur.
* Celle-ci permet d'éviter que les sous-processus ne deviennent des zombies. */
void shutchildren(int sig);

#endif // !__FTP_H__