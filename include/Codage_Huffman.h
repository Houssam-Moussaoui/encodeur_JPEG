#ifndef  DC_AC_H
#define DC_AC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

void DC(int16_t* bloc, int16_t valeur);

uint8_t magnitude(int16_t freq);

uint8_t* generate_Huffman_size_table(uint8_t bits[16] );

uint16_t* generate_code_table(uint8_t* Huffsize);

uint32_t code_DC_Y(int16_t elem,uint8_t* Huffval,int* nb_bits,uint8_t* Huffsize,uint16_t* Huffcode);

uint32_t code_DC_CbCr(int16_t elem,uint8_t* Huffval,int* nb_bits,uint8_t* Huffsize,uint16_t* Huffcode);


int indice(int16_t elm);

uint32_t code_AC_Y(int16_t element,uint8_t* Huffval,int taille,int nb_0,int* nb_bits,uint8_t* Huffsize,uint16_t* Huffcode);

uint32_t code_AC_CbCr(int16_t element,uint8_t* Huffval,int taille,int nb_0,int* nb_bits,uint8_t* Huffsize,uint16_t* Huffcode);




bool tous_nuls_apres_k(int k, int16_t* liste);




#endif

