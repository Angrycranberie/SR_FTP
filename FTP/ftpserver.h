#ifndef _FTPSERVER_H_
#define _FTPSERVER_H_

#define MAX_NAME_LEN 256                        // Longueur maximum du nom de domaine d'un client.
#define NPROC 10                                // Nombre d'esclaves i.e. nombre de clients pouvant se connecter simultanément au serveur.
#define PORT 2119                               // Port utilisé par le serveur que les clients devront renseigner pour se connecter.
#define PREFIX "\033[1;94mftpserver>\033[0m"    // Préfixe des logs générés par le serveur.

/* Fonction de propagation du signal d'arrêt du serveur.
* Celle-ci permet d'éviter que les esclaves ne deviennent des zombies. */
void shutchildren(int sig);

void get(int connfd);

#endif // !_FTPSERVER_H_