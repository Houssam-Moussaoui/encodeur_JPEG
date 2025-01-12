#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#define TAILLE_BLOC 8
#include "../include/zigzag.h"
#include "../include/quantification.h"
#include "../include/dct.h"
#include "../include/qtables.h"
#include "../include/htables.h"
#include "../include/decomposer_en_mcu.h"
#include "../include/Codage_Huffman.h"
#include "../include/bitstream.h"
#include "../include/entete.h"
#include "../include/RGB_to_YCbCr.h"
#include "../include/downsampling.h"
#include "../include/gestion_entree.h"





int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("entrée invalide\n");
        print_help(argv[0]);
        return 1;
    }

    char *parametre = NULL;
    int h1, v1, h2, v2, h3, v3;
    char* nom_fichier_complet = NULL;
    char* chemin_sortie = NULL;
    char sortie[256] = {0};
    int presence_outfile = 0;
    


    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help();
            return 1;
        }
        else if (strncmp(argv[i], "--sample=", 9) == 0) {
            parametre = argv[i] + 9;
            
            sscanf(parametre, "%dx%d,%dx%d,%dx%d", &h1,&v1,&h2,&v2,&h3,&v3);

            if (h1<1 || h1>4 || v1<1 || v1>4 || h2<1 || h2>4 || v2<1 || v2>4 || h3<1 || h3>4 || v3<1 || v3>4) 
            {
                printf("La valeur de chaque facteur h ou v doit être comprise entre 1 et 4.\n");
                return 1;
            } 
            
            if ( (h1*v1 + h2*v2 + h3*v3) > 10) 
            {
                printf("Erreur : La somme des produits hi x vi doit être inférieure ou égale à 10.\n");
                return 1;
            }

            if (h1 % h2 != 0 || v1 % v2 != 0 || h1 % h3 != 0 || v1 % v3 != 0) 
            {
                printf("Erreur : Les facteurs d'échantillonnage des chrominances doivent diviser parfaitement ceux de la luminance.\n");
                return 1;
            }
        }
        else if (strncmp(argv[i], "--outfile=",10) == 0) {
            presence_outfile = 1;
            chemin_sortie = argv[i] + 10;
            extraire_outfile(chemin_sortie,sortie);
            printf("Voici le path de sortie:'%s'\n",sortie);
        }
        
        else if (est_ppm_ou_pgm(argv[i])) {
            nom_fichier_complet = argv[i];
        }
    }

    if (nom_fichier_complet == NULL) {
        printf("vous n'avez entrer aucun fichier PPM ou PGM dans les arguments!\n");
        print_help(argv[0]);
        return 1;
    }else if(presence_outfile==0){
        extraire_nom_sans_extension(nom_fichier_complet, sortie);
        strcat(sortie,".jpg");

    }else if(presence_outfile==1){
        extraire_outfile(chemin_sortie,sortie);
    }


    if (parametre == NULL) {
                h1 = 1;
                v1 = 1;
                h2 = 1;
                v2 = 1;
                h3 = 1;
                v3 = 1;
            }
    
    FILE *file = fopen(nom_fichier_complet, "rb");
    if (file == NULL) {
        printf("fichier vide!\n");
        return 1;
    }

    char format[3];
    int largeur, hauteur, valeur_max;
    fscanf(file,"%s",format);
    fscanf(file,"%d %d",&largeur,&hauteur);
    fscanf(file, "%d" ,&valeur_max);
    
    //htables
    uint8_t* Huffsize_DC_Y=generate_Huffman_size_table(htables_nb_symb_per_lengths[0][0]);
    uint16_t* Huffcode_DC_Y=generate_code_table(Huffsize_DC_Y);

    uint8_t* Huffsize_AC_Y=generate_Huffman_size_table(htables_nb_symb_per_lengths[1][0]);
    uint16_t* Huffcode_AC_Y=generate_code_table(Huffsize_AC_Y);


    uint8_t* Huffsize_DC_CbCr=generate_Huffman_size_table(htables_nb_symb_per_lengths[0][1]);
    uint16_t* Huffcode_DC_CbCr=generate_code_table(Huffsize_DC_CbCr);


    uint8_t* Huffsize_AC_CbCr=generate_Huffman_size_table(htables_nb_symb_per_lengths[1][1]);
    uint16_t* Huffcode_AC_CbCr=generate_code_table(Huffsize_AC_CbCr);



    if (strcmp(format,"P5") == 0) { 

        while (fgetc(file)!='\n');

        printf("Image en format Noir et Blanc.\n");
        
        uint8_t** image_2D = malloc(hauteur * sizeof(uint8_t*));
        for (int i = 0; i < hauteur; i++) {
            image_2D[i] = malloc(largeur * sizeof(uint8_t));
            fread(image_2D[i], sizeof(uint8_t), largeur, file);
        }

        fclose(file);

        uint8_t*** blocs;
        int nb_blocs;
        decomposerenMCU(image_2D, hauteur, largeur, &blocs, &nb_blocs,1,1);

        int16_t*** blocs_dct= malloc(nb_blocs* sizeof(int16_t**));
        
        int16_t** blocs_zigzag= (int16_t**)malloc(sizeof(int16_t*) * (nb_blocs)); // Tableau de blocs

        for (int i = 0; i < hauteur; i++) {
            free(image_2D[i]);
        }
        free(image_2D);


        int16_t liste_1ere_elem_de_chaque_bloc[1];
        liste_1ere_elem_de_chaque_bloc[0]=0;

        int nb_bits;

        Entete(sortie, hauteur, largeur); //Ecriture de l'entete

        debut_ecriture(sortie);




        for (int i = 0; i < nb_blocs; i++) {

            blocs_dct[i]=malloc(TAILLE_BLOC*sizeof(int16_t*));
            for(int j=0;j<TAILLE_BLOC;j++){
                blocs_dct[i][j]=malloc(TAILLE_BLOC*sizeof(int16_t));
            }
            
            // Calcul de la DCT 2D
            DCT_2D(blocs[i], blocs_dct[i]);


                for (int j = 0; j < TAILLE_BLOC; j++) {
                    free(blocs[i][j]);
                }
                free(blocs[i]);
            
            // Zigzag
            blocs_zigzag[i] = zigzag(blocs_dct[i]);

            for (int j = 0; j < TAILLE_BLOC; j++) {
                    free(blocs_dct[i][j]);
                }

            free(blocs_dct[i]);


            //Quantification
            quantification(blocs_zigzag[i],quantification_table_Y);


            //DC1-DC2...
            DC(blocs_zigzag[i],liste_1ere_elem_de_chaque_bloc[0]);
            liste_1ere_elem_de_chaque_bloc[0]=blocs_zigzag[i][0]+liste_1ere_elem_de_chaque_bloc[0];


            
            int16_t elem_DC =blocs_zigzag[i][0];


            uint32_t code_elem_DC=code_DC_Y(elem_DC,htables_symbols[0][0],&nb_bits, Huffsize_DC_Y, Huffcode_DC_Y);
            int min_bits_pour_ecire = nb_bits;
            ecrire_octet(code_elem_DC,min_bits_pour_ecire);

            uint32_t code_elem_AC;
            int nb_0=0;
            for(int k=1;k<(TAILLE_BLOC*TAILLE_BLOC);k++){
                int16_t elem_AC=blocs_zigzag[i][k];
                if (blocs_zigzag[i][k]!=0){
                    code_elem_AC=code_AC_Y(elem_AC,htables_symbols[1][0],162,nb_0, &nb_bits, Huffsize_AC_Y, Huffcode_AC_Y);
                    min_bits_pour_ecire = nb_bits;
                    ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                    nb_0=0;
                }else if(tous_nuls_apres_k(k,blocs_zigzag[i])){
                    code_elem_AC=code_AC_Y(elem_AC,htables_symbols[1][0],162,-1,&nb_bits, Huffsize_AC_Y, Huffcode_AC_Y);
                    min_bits_pour_ecire = nb_bits;
                    ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                    break;
                }else{
                    if(nb_0==15){
                        code_elem_AC=code_AC_Y(elem_AC,htables_symbols[1][0],162,nb_0,&nb_bits, Huffsize_AC_Y, Huffcode_AC_Y);
                        min_bits_pour_ecire = nb_bits;
                        ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                        nb_0=0;
                    }else{
                        nb_0++;
                    }
                }
              
            }
            
            free(blocs_zigzag[i]);
            
        }

        fin_ecriture();



        
        free(blocs);
        free(Huffsize_AC_CbCr);
        free(Huffsize_AC_Y);
        free(Huffsize_DC_CbCr);
        free(Huffsize_DC_Y);
        free(Huffcode_DC_CbCr);
        free(Huffcode_DC_Y);
        free(Huffcode_AC_CbCr);
        free(Huffcode_AC_Y);
        free(blocs_zigzag);
        free(blocs_dct);

    }else{

        while (fgetc(file)!='\n');
        uint8_t** R=malloc(hauteur*sizeof(uint8_t*));
        uint8_t** G=malloc(hauteur*sizeof(uint8_t*));
        uint8_t** B=malloc(hauteur*sizeof(uint8_t*));
        for (int i=0;i<hauteur;i++) {
            R[i]=malloc(largeur*sizeof(uint8_t));
            G[i]=malloc(largeur*sizeof(uint8_t));
            B[i]=malloc(largeur*sizeof(uint8_t));
            for (int j=0; j<largeur;j++) {
                fread(&R[i][j],sizeof(uint8_t),1,file);  // lire les trois composantes.
                fread(&G[i][j],sizeof(uint8_t),1,file);  
                fread(&B[i][j],sizeof(uint8_t),1,file);  
            }
        }

        fclose(file);



        //RGB to YCbCr
        uint8_t** Y;
        uint8_t** Cb;
        uint8_t** Cr;

        RGB_to_YCbCr(R,G,B,&Y,&Cb,&Cr,hauteur,largeur);


        for (int i = 0; i < hauteur; i++) {
            free(R[i]);
            free(G[i]);
            free(B[i]);
        }
        free(R);
        free(G);
        free(B);



        int presence_downsampling=1;
        if (h1==1 && h2==1 && h3==1 && v1==1 && v2==1 && v3==1){
            presence_downsampling=0; //0 pour : no downsampling
            printf("Pas de downsampling.\n");
        }
        
            uint8_t*** blocs_Y; 
            uint8_t*** blocs_Cb; 
            uint8_t*** blocs_Cr; 
            int nb_mcu;
        


        decomposerenMCU(Y,hauteur,largeur,&(blocs_Y),&(nb_mcu),h1,v1);
        decomposerenMCU(Cb,hauteur,largeur,&(blocs_Cb),&(nb_mcu),h1,v1);
        decomposerenMCU(Cr,hauteur,largeur,&(blocs_Cr),&(nb_mcu),h1,v1);
        
        for (int i = 0; i < hauteur; i++) {
            free(Y[i]);
            free(Cb[i]);
            free(Cr[i]);
        }
        free(Y);
        free(Cb);
        free(Cr);


        
        uint8_t*** blocs_Cb_downsampling = malloc(nb_mcu * sizeof(uint8_t **));
        uint8_t*** blocs_Cr_downsampling = malloc(nb_mcu * sizeof(uint8_t **));

   
        int nb_bits;

        Entete2(sortie, hauteur, largeur,h1,v1,h2,v2,h3,v3); //Ecriture de  l'entete


        debut_ecriture(sortie);


        int16_t liste_1ere_elem_de_chaque_bloc_Y[1];
        liste_1ere_elem_de_chaque_bloc_Y[0]=0;


        int16_t liste_1ere_elem_de_chaque_bloc_Cb[1];
        liste_1ere_elem_de_chaque_bloc_Cb[0]=0;


        int16_t liste_1ere_elem_de_chaque_bloc_Cr[1];
        liste_1ere_elem_de_chaque_bloc_Cr[0]=0;


      
        for (int i=0;i<nb_mcu;i++){


            uint8_t*** blocs_Y_8;
            int nb_blocs_Y_8;
            decomposerenMCU(blocs_Y[i], v1*8, h1*8, &blocs_Y_8, &nb_blocs_Y_8,1,1);


            for(int k=0;k<8*v1;k++){
                free(blocs_Y[i][k]);
            }
            free(blocs_Y[i]);



            //Downsampling

            uint8_t*** blocs_Cb_8;
            int nb_blocs_Cb_8;

            uint8_t*** blocs_Cr_8;
            int nb_blocs_Cr_8;


            if(presence_downsampling!=0){
                
                downsampling(blocs_Cb[i],h1,v1,h2,v2,&(blocs_Cb_downsampling[i]));
                downsampling(blocs_Cr[i],h1,v1,h3,v3,&(blocs_Cr_downsampling[i]));

                for(int k=0;k<8*v1;k++){
                    free(blocs_Cb[i][k]);
                    free(blocs_Cr[i][k]);
                }
                free(blocs_Cb[i]);
                free(blocs_Cr[i]);

               
                decomposerenMCU(blocs_Cb_downsampling[i], 8*v2, 8*h2, &blocs_Cb_8, &nb_blocs_Cb_8,1,1);

                for(int k=0;k<8*v2;k++){
                    free(blocs_Cb_downsampling[i][k]);
                }
                free(blocs_Cb_downsampling[i]);
            

                
                decomposerenMCU(blocs_Cr_downsampling[i], 8*v3, 8*h3, &blocs_Cr_8, &nb_blocs_Cr_8,1,1);
                for(int k=0;k<8*v3;k++){
                    free(blocs_Cr_downsampling[i][k]);
                }
                free(blocs_Cr_downsampling[i]);
            


            }else{
                
                decomposerenMCU(blocs_Cb[i], v2*8, h2*8, &blocs_Cb_8, &nb_blocs_Cb_8,1,1);
                decomposerenMCU(blocs_Cr[i], v3*8, h3*8, &blocs_Cr_8, &nb_blocs_Cr_8,1,1);
                for(int k=0;k<8*v1;k++){
                    free(blocs_Cb[i][k]);
                    free(blocs_Cr[i][k]);
                }
                free(blocs_Cb[i]);
                free(blocs_Cr[i]);
            }
          


            //DCT
            int16_t*** blocs_dct_Y= malloc(nb_blocs_Y_8* sizeof(int16_t**));
            int16_t*** blocs_dct_Cb= malloc(nb_blocs_Cb_8* sizeof(int16_t**));
            int16_t*** blocs_dct_Cr= malloc(nb_blocs_Cr_8* sizeof(int16_t**));



            //zigzag
            int16_t** blocs_zigzag_Y= (int16_t**)malloc(sizeof(int16_t*) * (nb_blocs_Y_8)); // Tableau de blocs
            int16_t** blocs_zigzag_Cb= (int16_t**)malloc(sizeof(int16_t*) * (nb_blocs_Cb_8)); // Tableau de blocs
            int16_t** blocs_zigzag_Cr= (int16_t**)malloc(sizeof(int16_t*) * (nb_blocs_Cr_8)); // Tableau de blocs

           
            
            for (int j = 0; j < nb_blocs_Y_8; j++) {
      

                blocs_dct_Y[j]=malloc(TAILLE_BLOC*sizeof(int16_t*));
                for(int k=0;k<TAILLE_BLOC;k++){
                    blocs_dct_Y[j][k]=malloc(TAILLE_BLOC*sizeof(int16_t));
                }


                DCT_2D(blocs_Y_8[j], blocs_dct_Y[j]);

                for (int k = 0; k < TAILLE_BLOC; k++) {
                    free(blocs_Y_8[j][k]);
                }
                free(blocs_Y_8[j]);

       

                //zigzag 
                blocs_zigzag_Y[j] = zigzag(blocs_dct_Y[j]);

                for (int k = 0; k < TAILLE_BLOC; k++) {
                    free(blocs_dct_Y[j][k]);
                }
                free(blocs_dct_Y[j]);

       

                //quant
                quantification(blocs_zigzag_Y[j],quantification_table_Y);

                //avant DC 
                DC(blocs_zigzag_Y[j],liste_1ere_elem_de_chaque_bloc_Y[0]);
                liste_1ere_elem_de_chaque_bloc_Y[0]=blocs_zigzag_Y[j][0]+liste_1ere_elem_de_chaque_bloc_Y[0];
                


                //encodage 
                int16_t elem_DC =blocs_zigzag_Y[j][0];

                uint32_t code_elem_DC=code_DC_Y(elem_DC,htables_symbols[0][0],&nb_bits,Huffsize_DC_Y,Huffcode_DC_Y);
                int min_bits_pour_ecire = nb_bits;
                ecrire_octet(code_elem_DC,min_bits_pour_ecire);

                uint32_t code_elem_AC;
                int nb_0=0;
                for(int k=1;k<(TAILLE_BLOC*TAILLE_BLOC);k++){
                    int16_t elem_AC=blocs_zigzag_Y[j][k];
                    if (blocs_zigzag_Y[j][k]!=0){
                        code_elem_AC=code_AC_Y(elem_AC,htables_symbols[1][0],162,nb_0, &nb_bits,Huffsize_AC_Y, Huffcode_AC_Y);
                        min_bits_pour_ecire = nb_bits;
                        ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                        nb_0=0;
                    }else if(tous_nuls_apres_k(k,blocs_zigzag_Y[j])){
                        code_elem_AC=code_AC_Y(elem_AC,htables_symbols[1][0],162,-1,&nb_bits,Huffsize_AC_Y, Huffcode_AC_Y);
                        min_bits_pour_ecire = nb_bits;
                        ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                        break;
                    }else{
                        if(nb_0==15){
                            code_elem_AC=code_AC_Y(elem_AC,htables_symbols[1][0],162,nb_0,&nb_bits,Huffsize_AC_Y, Huffcode_AC_Y);
                            min_bits_pour_ecire = nb_bits;
                            ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                            nb_0=0;
                        }else{
                            nb_0++;
                        }
                    }
                
                }
                free(blocs_zigzag_Y[j]);
            }

           
            for (int j = 0; j < nb_blocs_Cb_8; j++) {

                // Calcul de la DCT 2D

                blocs_dct_Cb[j]=malloc(TAILLE_BLOC*sizeof(int16_t*));
                for(int k=0;k<TAILLE_BLOC;k++){
                    blocs_dct_Cb[j][k]=malloc(TAILLE_BLOC*sizeof(int16_t));
                }


                DCT_2D(blocs_Cb_8[j], blocs_dct_Cb[j]);

                for (int k = 0; k < TAILLE_BLOC; k++) {
                    free(blocs_Cb_8[j][k]);
                }
                free(blocs_Cb_8[j]);


          
                //zigzag
                blocs_zigzag_Cb[j] = zigzag(blocs_dct_Cb[j]);


                for (int k = 0; k < TAILLE_BLOC; k++) {
                    free(blocs_dct_Cb[j][k]);
                }
                free(blocs_dct_Cb[j]);


             
                //quantif
                quantification(blocs_zigzag_Cb[j],quantification_table_CbCr);

                //avant DC 
   
                DC(blocs_zigzag_Cb[j],liste_1ere_elem_de_chaque_bloc_Cb[0]);
                liste_1ere_elem_de_chaque_bloc_Cb[0]=blocs_zigzag_Cb[j][0]+liste_1ere_elem_de_chaque_bloc_Cb[0];



                //encodage 
                int16_t elem_DC =blocs_zigzag_Cb[j][0];
 

                uint32_t code_elem_DC=code_DC_CbCr(elem_DC,htables_symbols[0][1],&nb_bits,Huffsize_DC_CbCr,Huffcode_DC_CbCr);
                int min_bits_pour_ecire = nb_bits;
                ecrire_octet(code_elem_DC,min_bits_pour_ecire);

                uint32_t code_elem_AC;
                int nb_0=0;
                for(int k=1;k<(TAILLE_BLOC*TAILLE_BLOC);k++){
                    int16_t elem_AC=blocs_zigzag_Cb[j][k];
                    if (blocs_zigzag_Cb[j][k]!=0){
                        code_elem_AC=code_AC_CbCr(elem_AC,htables_symbols[1][1],162,nb_0, &nb_bits,Huffsize_AC_CbCr,Huffcode_AC_CbCr);
                        min_bits_pour_ecire = nb_bits;
                        ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                        nb_0=0;
                    }else if(tous_nuls_apres_k(k,blocs_zigzag_Cb[j])){
                        code_elem_AC=code_AC_CbCr(elem_AC,htables_symbols[1][1],162,-1,&nb_bits,Huffsize_AC_CbCr,Huffcode_AC_CbCr);
                        min_bits_pour_ecire = nb_bits;
                        ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                        break;
                    }else{
                        if(nb_0==15){
                            code_elem_AC=code_AC_CbCr(elem_AC,htables_symbols[1][1],162,nb_0,&nb_bits,Huffsize_AC_CbCr,Huffcode_AC_CbCr);
                            min_bits_pour_ecire = nb_bits;
                            ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                            nb_0=0;
                        }else{
                            nb_0++;
                        }
                    }
                
                }
                free(blocs_zigzag_Cb[j]);
            }


            for (int j = 0; j < nb_blocs_Cr_8; j++) {

                // Calcul de la DCT 2D

                blocs_dct_Cr[j]=malloc(TAILLE_BLOC*sizeof(int16_t*));
                for(int k=0;k<TAILLE_BLOC;k++){
                    blocs_dct_Cr[j][k]=malloc(TAILLE_BLOC*sizeof(int16_t));
                }


                DCT_2D(blocs_Cr_8[j], blocs_dct_Cr[j]);

  
            
                for (int k = 0; k < TAILLE_BLOC; k++) {
                    free(blocs_Cr_8[j][k]);
                }
                free(blocs_Cr_8[j]);


                //zigzag
                blocs_zigzag_Cr[j] = zigzag(blocs_dct_Cr[j]);



                for (int k = 0; k < TAILLE_BLOC; k++) {
                    free(blocs_dct_Cr[j][k]);
                }
                free(blocs_dct_Cr[j]);

                //quantif
                quantification(blocs_zigzag_Cr[j],quantification_table_CbCr);


               
                DC(blocs_zigzag_Cr[j],liste_1ere_elem_de_chaque_bloc_Cr[0]);
                liste_1ere_elem_de_chaque_bloc_Cr[0]=blocs_zigzag_Cr[j][0]+liste_1ere_elem_de_chaque_bloc_Cr[0];

                //encodage 
                int16_t elem_DC =blocs_zigzag_Cr[j][0];


                uint32_t code_elem_DC=code_DC_CbCr(elem_DC,htables_symbols[0][2],&nb_bits,Huffsize_DC_CbCr,Huffcode_DC_CbCr);
                int min_bits_pour_ecire = nb_bits;

                ecrire_octet(code_elem_DC,min_bits_pour_ecire);

                uint32_t code_elem_AC;
                int nb_0=0;
                for(int k=1;k<(TAILLE_BLOC*TAILLE_BLOC);k++){
                    int16_t elem_AC=blocs_zigzag_Cr[j][k];
                    if (blocs_zigzag_Cr[j][k]!=0){
                        code_elem_AC=code_AC_CbCr(elem_AC,htables_symbols[1][2],162,nb_0, &nb_bits,Huffsize_AC_CbCr,Huffcode_AC_CbCr);
                        min_bits_pour_ecire = nb_bits;
                        ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                        nb_0=0;
                    }else if(tous_nuls_apres_k(k,blocs_zigzag_Cr[j])){
                        code_elem_AC=code_AC_CbCr(elem_AC,htables_symbols[1][2],162,-1,&nb_bits,Huffsize_AC_CbCr,Huffcode_AC_CbCr);
                        min_bits_pour_ecire = nb_bits;

                        ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                        break;
                    }else{
                        if(nb_0==15){
                            code_elem_AC=code_AC_CbCr(elem_AC,htables_symbols[1][2],162,nb_0,&nb_bits,Huffsize_AC_CbCr,Huffcode_AC_CbCr);
                            min_bits_pour_ecire = nb_bits;
                            ecrire_octet(code_elem_AC,min_bits_pour_ecire);
                            nb_0=0;
                        }else{
                            nb_0++;
                        }
                    }
                
                }

                free(blocs_zigzag_Cr[j]);

            }

            free(blocs_Y_8);
            free(blocs_Cb_8);
            free(blocs_Cr_8);

            free(blocs_zigzag_Y);
            free(blocs_zigzag_Cb);
            free(blocs_zigzag_Cr);

            free(blocs_dct_Y);
            free(blocs_dct_Cb);
            free(blocs_dct_Cr);

        }


        fin_ecriture();



        free(blocs_Y);
        free(blocs_Cb);
        free(blocs_Cr);    


        free(blocs_Cb_downsampling);
        free(blocs_Cr_downsampling);


        free(Huffsize_AC_CbCr);
        free(Huffsize_AC_Y);
        free(Huffsize_DC_CbCr);
        free(Huffsize_DC_Y);
        free(Huffcode_DC_CbCr);
        free(Huffcode_DC_Y);
        free(Huffcode_AC_CbCr);
        free(Huffcode_AC_Y);


    
    }

    return EXIT_SUCCESS;
}

