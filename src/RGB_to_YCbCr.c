#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../include/RGB_to_YCbCr.h"

void RGB_to_YCbCr(uint8_t** R, uint8_t** G, uint8_t** B, uint8_t*** Y, uint8_t*** Cb, uint8_t*** Cr, int hauteur, int largeur) {

    *Y = (uint8_t**)malloc(hauteur * sizeof(uint8_t*));
    *Cb = (uint8_t**)malloc(hauteur * sizeof(uint8_t*));
    *Cr = (uint8_t**)malloc(hauteur * sizeof(uint8_t*));

    for (int i = 0; i < hauteur; i++) {
        (*Y)[i] = (uint8_t*)malloc(largeur * sizeof(uint8_t));
        (*Cb)[i] = (uint8_t*)malloc(largeur * sizeof(uint8_t));
        (*Cr)[i] = (uint8_t*)malloc(largeur * sizeof(uint8_t));

        for (int j = 0; j < largeur; j++) {
            double r = (double)R[i][j];
            double g = (double)G[i][j];
            double b = (double)B[i][j];

            double y=0.299*r+0.587*g+0.114*b;
            double cb=-0.1687*r-0.3313*g+0.5000*b+128.00;
            double cr=0.5000*r-0.4187*g-0.0813*b+128.00;

            y =fmin(fmax(y,0.0),255.0);
            cb =fmin(fmax(cb,0.0),255.0);
            cr =fmin(fmax(cr,0.0),255.0);

            (*Y)[i][j] = (uint8_t)y;
            (*Cb)[i][j] = (uint8_t)cb;
            (*Cr)[i][j] = (uint8_t)cr;
        }
    }
}
