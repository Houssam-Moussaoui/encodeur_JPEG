#ifndef RGB_TO_YCBCR_H
#define RGB_TO_YCBCR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void RGB_to_YCbCr(uint8_t** R, uint8_t** G, uint8_t** B, uint8_t*** Y, uint8_t*** Cb, uint8_t*** Cr, int hauteur, int largeur);

#endif 
