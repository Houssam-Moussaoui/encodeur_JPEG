#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>







int est_ppm_ou_pgm(const char *nom_fichier ) {
    size_t longueur = strlen(nom_fichier);
    return (longueur >= 4 && strcmp(nom_fichier + longueur - 4, ".ppm") == 0) ||
           (longueur >= 4 && strcmp(nom_fichier + longueur - 4, ".pgm") == 0);
}



void print_help() {
    printf("\n");
    printf("    \033[36;01mUsage:\033[00m ./ppm2jpeg --sample=h1xv1,h2xv2,h3xv3 --outfile=../chemin/complique/mouton.jpg ./images/image.ppm (ou .pgm)\n");
    printf("    \033[36;01mOptions:\033[00m\n");
    printf("        \033[35;01m--help\033[00m                      ->  \033[35;01mAfficher la liste des options acceptées\033[00m  \n");
    printf("        \033[35;01m--outfile=sortie.jpg\033[00m        ->  \033[35;01mRedéfinir le nom du fichier de sortie\033[00m  \n");
    printf("        \033[35;01m--sample=h1xv1,h2xv2,h3xv3\033[00m  ->  \033[35;01mDéfinir les facteurs d'échantillonnage h*v des trois composantes de couleur\033[00m  \n\n");
}


void extraire_nom_sans_extension(char *chemin_complet, char *nom_fichier) {
    const char *dernier_slash = NULL;
    const char *debut_nom_fichier = chemin_complet;
    const char *dernier_point = NULL;
    size_t longueur_nom = 0;

    dernier_slash = strrchr(chemin_complet, '/');
    if (dernier_slash != NULL) {
        debut_nom_fichier = dernier_slash + 1;
    }

    dernier_point = strrchr(debut_nom_fichier, '.');
    longueur_nom = dernier_point - debut_nom_fichier; 
    
    strncpy(nom_fichier, debut_nom_fichier, longueur_nom);
    nom_fichier[longueur_nom] = '\0'; 
}

void extraire_outfile(char* chemin_sortie, char* nom_sortie) {
    size_t longueur_nom = 0;
    const char *dernier_charactere = NULL;
    const char *debut_nom_sortie = chemin_sortie;
    dernier_charactere = strchr(chemin_sortie , ' ');
    if (dernier_charactere != NULL) {
        longueur_nom = dernier_charactere - debut_nom_sortie;
        strncpy(nom_sortie , debut_nom_sortie , longueur_nom);
        nom_sortie[longueur_nom] = '\0';

    } else {

        strcpy(nom_sortie, chemin_sortie);
    }

}