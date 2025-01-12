#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define N 8


void decomposerenMCU (uint8_t** matrice , int hauteur, int largeur , uint8_t**** mcus ,int* nb_total_mcus, int h1 , int v1){
    // mcus est un pointeur vers un tableau de mcus (cad *mcus est un tableau de mcus )
    int nb_mcus_ligne = (largeur + h1*N - 1) / (h1*N); // Nombre de mcus par ligne
    int nb_mcus_colonne = (hauteur + v1*N - 1) / (v1*N); // Nombre de mcus par colonne
    *nb_total_mcus = nb_mcus_ligne * nb_mcus_colonne ;
    *mcus =(uint8_t***)malloc(sizeof(uint8_t**)*(*nb_total_mcus)) ;
    int mcu_index = 0 ;
    for(int i = 0 ; i < hauteur ; i += v1*N ){ 
        for(int j = 0  ; j < largeur ; j += h1*N){
            (*mcus)[mcu_index] = (uint8_t **)malloc(sizeof(uint8_t*)*v1*N );
            for (int x = 0; x < v1*N ; x++){
                (*mcus)[mcu_index][x] = (uint8_t*)malloc(sizeof(uint8_t)*h1*N);
                for (int y = 0 ; y < h1*N ; y++){
                
                int matrice_x_mcus;
                if (i + x < hauteur) {
                    matrice_x_mcus = i + x;
                } else {
                    matrice_x_mcus = hauteur - 1;
                }

                int matrice_y_mcus;
                if (j + y < largeur) {
                    matrice_y_mcus = j + y;
                } else {
                    matrice_y_mcus = largeur - 1;
                }

                (*mcus)[mcu_index][x][y] =  matrice[matrice_x_mcus][matrice_y_mcus] ; 
                }   
            }
            mcu_index ++ ;
        }
    }
}
   







