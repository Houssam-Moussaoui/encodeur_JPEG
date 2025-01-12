#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


void quantification(int16_t* matrice, uint8_t quantification_table[64]) {
    for (int i = 0; i < 64; i++) {
        matrice[i]=(matrice[i]/quantification_table[i]);
        }       
}
