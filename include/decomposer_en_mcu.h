#ifndef DECOMPOSER_EN_MCU_H
#define DECOMPOSER_EN_MCU_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define TAILLE_BLOC 8


void decomposerenMCU (uint8_t** matrice , int hauteur, int largeur , uint8_t**** mcus ,int* nb_total_mcus, int h1 , int v1);

#endif 
