#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define N 8


void downsampling(uint8_t** mcu, int h1, int v1, int hi, int vi, uint8_t*** mcu_sortie) {

    *mcu_sortie = (uint8_t**)malloc(sizeof(uint8_t*) * N * vi);

    for (int i = 0; i < N*vi; i++) {                                        
        (*mcu_sortie)[i] = (uint8_t*)malloc(sizeof(uint8_t) * N * hi);
        for (int j = 0; j < N*hi; j++) {
            int moyenne = 0;
            for (int x = 0; x < v1 / vi; x++) {
                for (int y = 0; y < h1 / hi; y++) {
                    moyenne += mcu[x + v1 / vi * i][y + h1 / hi * j];
                }
            }
            (*mcu_sortie)[i][j] = (moyenne*hi*vi)/(v1*h1);
        }
    }
}
