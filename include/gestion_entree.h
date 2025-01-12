#ifndef GESTION_ENTREE_H
#define GESTION_ENTREE_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>




int est_ppm_ou_pgm(const char *nom_fichier );



void print_help();


void extraire_nom_sans_extension(char *chemin_complet, char *nom_fichier);

void extraire_outfile(char* chemin_sortie, char* nom_sortie);


#endif 