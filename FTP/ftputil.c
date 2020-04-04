/*
 * Commande 'get' - côté serveur
 */

#include "csapp.h"
#include "ftp.h"

/* Supprime les retour à la ligne (\n) d'une chaîne */
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

