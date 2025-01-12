#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>  
#include "../include/bitstream.h"


FILE* file;
uint8_t octet;
int nb_bits_dans_octet;



void debut_ecriture(const char *nom_de_fichier) {
    file=fopen(nom_de_fichier, "ab");
    octet = 0;
    nb_bits_dans_octet=0;
    
    
}

void ecrire_octet( uint32_t code, int nb_bits ) {
    for (int i=0;i<nb_bits;i++) {
        octet = (octet << 1) | ((code >> (nb_bits - 1 - i)) & 1); 

        nb_bits_dans_octet++;
        if (nb_bits_dans_octet == 8) {
            fwrite(&octet,1,1,file);
            if (octet == 0xFF) {
                uint8_t value = 0;
                fwrite(&value,1,1,file);
            }

            octet= 0;
            nb_bits_dans_octet = 0;
        }
    }
}


void fin_ecriture() {
    if (nb_bits_dans_octet > 0) {       
        fwrite(&octet,1,1,file);
        octet = 0;
        nb_bits_dans_octet = 0;
    }

    uint32_t bin =0xd9ff ;
    fwrite(&bin,1,2,file);

    fclose(file);
}




