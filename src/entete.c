#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include  "../include/qtables.h"
#include  "../include/htables.h"
#include "../include/entete.h"




void Entete(const char *file_sortie,int hauteur,int largeur ){

    FILE *file = fopen(file_sortie, "wb");
    //marqueur de début
    int32_t bin=0xd8ff;
    fwrite(&bin,1,2,file);

    //Appx
    bin=0xe0ff; //debut de jfif
    fwrite(&bin, 1, 2, file);
    bin = 0x1000 ;
    fwrite(&bin, 1, 2, file); //taille
    fwrite("\x4a\x46\x49\x46\x00", 5, 1, file); //JFIF
    bin = 0x0101; //JFIF 1.1
    fwrite(&bin, 1, 2, file);
    fwrite("\x00\x00\x00\x00\x00\x00\x00", 7, 1, file);

    //DQT
    // i_Q = 0 pour Y et 1 pour CbCr
    bin = 0xdbff ;
    fwrite(&bin, 1,2,file);
    bin = 0x4300 ;
    fwrite(&bin, 1,2,file);
    bin = 0 ;
    fwrite(&bin, 1,1,file);
    fwrite(quantification_table_Y,1,64,file);

    //SOFx
    //iC = 1 pour Y et 2 pour Cb et 3 pour Cr
    bin = 0xc0ff ;
    fwrite(&bin,1,2,file);
    bin = 0x0b00 ;
    fwrite(&bin,1,2,file);
    bin = 8;
    fwrite(&bin, 1,1,file);


    uint16_t valeur =((uint16_t)hauteur<<8) + ((uint16_t)hauteur>>8) ; 
    uint16_t *point_bin = &valeur;
    fwrite(point_bin,1,2,file);



    *point_bin=((uint16_t)largeur<<8) + ((uint16_t)largeur>>8);
    fwrite(point_bin,1,2,file);

    bin = 1 ;
    fwrite(&bin,1,1,file);
    bin = 1;
    fwrite(&bin,1,1,file);
    bin = 17 ; //Downsampling
    fwrite(&bin,1,1,file);
    bin = 0 ;
    fwrite(&bin,1,1,file);


    //DHT_Y_DC
    //iH 0 pour Y 1 pour Cb et 2 pour Cr
    bin = 0xc4ff ;
    fwrite(&bin,1,2,file);
    bin = 0x1f00 ;
    fwrite(&bin,1,2,file);

    bin = 0 ;
    fwrite(&bin,1,1,file);
    fwrite(htables_nb_symb_per_lengths[0][0],1,16,file);
    fwrite(htables_symbols[0][0],1,12,file);

    //DHT_Y_AC
    bin = 0xc4ff ;
    fwrite(&bin,1,2,file);
    bin = 0xb500;
    fwrite(&bin,1,2,file);

    bin = 16 ;
    fwrite(&bin,1,1,file);
    fwrite(htables_nb_symb_per_lengths[1][0],1,16,file);
    fwrite(htables_symbols[1][0],1,162,file);


    //SOS
    bin = 0xdaff ;
    fwrite(&bin,1,2,file);
    bin = 0x0800 ;
    fwrite(&bin,1,2,file);
    bin = 1 ;
    fwrite(&bin,1,1,file);
    bin = 1 ;
    fwrite(&bin,1,1,file);
    bin = 0 ;
    fwrite(&bin,1,1,file);
    bin = 0 ;
    fwrite(&bin,1,1,file);
    bin = 63 ;
    fwrite(&bin,1,1,file);
    bin = 0 ;
    fwrite(&bin,1,1,file);
    bin = 0xd9ff ;

    fclose(file);

}


    



void Entete2(const char *file_sortie,int hauteur,int largeur,int h1,int v1 ,int h2 ,int v2,int h3,int v3 ){

    FILE *file = fopen(file_sortie, "wb");
    //marqueur de début
    int32_t bin=0xd8ff;
    fwrite(&bin,1,2,file);

    //Appx
    bin=0xe0ff; //debut de jfif
    fwrite(&bin, 1, 2, file);
    bin = 0x1000 ;
    fwrite(&bin, 1, 2, file); //taille
    fwrite("\x4a\x46\x49\x46\x00", 5, 1, file); //JFIF
    bin = 0x0101; //JFIF 1.1
    fwrite(&bin, 1, 2, file);
    fwrite("\x00\x00\x00\x00\x00\x00\x00", 7, 1, file);

    //DQT


    // i_Q = 0 pour Y et 1 pour CbCr
    //entete pour Y 
    bin = 0xdbff ;
    fwrite(&bin, 1,2,file);
    bin = 0x4300 ;
    fwrite(&bin, 1,2,file);
    bin = 0 ;
    fwrite(&bin, 1,1,file);
    fwrite(quantification_table_Y,1,64,file);

    // entete pour Cb et Cr 
    bin = 0xdbff ;
    fwrite(&bin , 1, 2,file);
    bin = 0x4300 ;
    fwrite(&bin , 1, 2,file);
    bin = 1 ;
    fwrite(&bin, 1,1,file);
    fwrite(quantification_table_CbCr,1,64,file);

    //SOFx
    //iC = 1 pour Y et 2 pour Cb et 3 pour Cr
    //SOF pour Y 
    bin = 0xc0ff ;
    fwrite(&bin,1,2,file);
    bin = 0x1100 ;
    fwrite(&bin,1,2,file);
    bin = 8;
    fwrite(&bin, 1,1,file);
    

    uint16_t valeur =((uint16_t)hauteur<<8) + ((uint16_t)hauteur>>8) ; // on ne fait qu'échanger les deux octets de poids fort et de poids faible
    uint16_t *point_bin = &valeur;
    fwrite(point_bin,1,2,file);



    *point_bin=((uint16_t)largeur<<8) + ((uint16_t)largeur>>8);
    fwrite(point_bin,1,2,file);

    bin = 3 ;
    fwrite(&bin,1,1,file);
    // composante Y 
    bin = 1;
    fwrite(&bin,1,1,file);
    
    *point_bin=(h1<<4)+v1;
    fwrite(point_bin,1,1,file);
    bin = 0 ;
    fwrite(&bin,1,1,file);
    // composante Cb 
    bin = 2 ;
    fwrite(&bin , 1, 1,file);
    
    *point_bin = (h2 << 4) + v2;
    fwrite(point_bin , 1 , 1,file);
    bin = 1 ;
    fwrite(&bin , 1, 1, file );

    //composante Cr
    bin = 3 ;
    fwrite(&bin , 1, 1,file);
    *point_bin = (h3 << 4) +v3;
    fwrite(point_bin , 1 , 1,file);
    bin = 1 ;
    fwrite(&bin , 1, 1, file );



    //DHT


    //DHT_Y_DC
    //iH 0 pour Y 1 pour Cb et 2 pour Cr
    bin = 0xc4ff ;
    fwrite(&bin,1,2,file);
    bin = 0x1f00 ;
    fwrite(&bin,1,2,file);
    bin = 0 ;
    fwrite(&bin,1,1,file);
    fwrite(htables_nb_symb_per_lengths[0][0],1,16,file);
    fwrite(htables_symbols[0][0],1,12,file);

    //DHT_Y_AC
    bin = 0xc4ff ;
    fwrite(&bin,1,2,file);
    bin = 0xb500;
    fwrite(&bin,1,2,file);
    bin = 16 ;
    fwrite(&bin,1,1,file);
    fwrite(htables_nb_symb_per_lengths[1][0],1,16,file);
    fwrite(htables_symbols[1][0],1,162,file);

    //DHT_Cb_DC
    bin = 0xc4ff ;
    fwrite(&bin,1,2,file);
    bin = 0x1f00;
    fwrite(&bin,1,2,file);
    bin = 1 ;
    fwrite(&bin,1,1,file);
    fwrite(htables_nb_symb_per_lengths[0][1],1,16,file);
    fwrite(htables_symbols[0][1],1,12,file);

    //DHT_Cb_AC
    bin = 0xc4ff ;
    fwrite(&bin,1,2,file) ;
    bin = 0xb500;
    fwrite(&bin,1,2,file);
    bin = 17 ;
    fwrite(&bin,1,1,file);
    fwrite(htables_nb_symb_per_lengths[1][1],1,16,file);
    fwrite(htables_symbols[1][1],1,162,file);


    




    //SOS
    bin = 0xdaff ;
    fwrite(&bin,1,2,file);
    bin = 0x0c00 ;
    fwrite(&bin,1,2,file);
    bin = 3 ;
    fwrite(&bin,1,1,file);
    // pour Y 
    bin = 1 ;
    fwrite(&bin,1,1,file);
    bin = 0 ;
    fwrite(&bin,1,1,file);
    // pour Cb 
    bin = 2 ; 
    fwrite(&bin,1,1,file);
    bin = 17 ;
    fwrite(&bin,1,1,file);
    // pour Cr 
    bin = 3 ; 
    fwrite(&bin,1,1,file);
    bin = 17 ;
    fwrite(&bin,1,1,file);


    bin = 0 ;
    fwrite(&bin,1,1,file);
    bin = 63 ;
    fwrite(&bin,1,1,file);
    bin = 0 ;
    fwrite(&bin,1,1,file);
    bin = 0xd9ff ;

    fclose(file);

}


    



