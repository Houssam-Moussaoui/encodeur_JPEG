#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>  



void debut_ecriture(const char *nom_de_fichier);
void ecrire_octet( uint32_t code, int nb_bits );
void fin_ecriture();


#endif 
