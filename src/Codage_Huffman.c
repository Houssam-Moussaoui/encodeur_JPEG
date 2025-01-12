#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../include/Codage_Huffman.h"
#include "../include/htables.h"


void DC(int16_t* bloc,int16_t valeur){

    bloc[0]=bloc[0]-valeur;
    
}

uint8_t magnitude(int16_t freq){
     if (freq<0) {
        freq=-freq;
    }
    uint8_t mag=0;
    while(freq>1){
        freq/=2;
        mag++;
    }
    if(freq==1){
        mag++;
    }
    return mag;
}



int indice(int16_t elm){
    int indice;
    uint8_t mag=magnitude(elm);
    int16_t puissance_2_mag = (int16_t)pow(2, mag); 
    int16_t puissance_2_mag_1 = (int16_t)pow(2, mag-1); 
    if((-puissance_2_mag + 1 <= elm) && (elm <= -puissance_2_mag_1)){
        indice=elm-(-puissance_2_mag+1)+1;
    }else if((puissance_2_mag_1  <= elm) && (elm <= puissance_2_mag -1)){
        indice =puissance_2_mag_1+elm-(puissance_2_mag_1)+1;
    }else{
        //indice=-1;
        return EXIT_FAILURE;
    }
        
    return indice-1;
}





uint32_t* Huffsize_DC_Y;

uint8_t* generate_Huffman_size_table(uint8_t bits[16] ){
    uint8_t* Huffsize = malloc(256*sizeof(uint8_t));

    int k = 0;
    int i = 0;
    int j = 1;
    while(i < 16){
        while(j <= bits[i]){
            Huffsize[k]=(uint8_t)(i+1);
            k++;
            j++;
        }
        i++;
        j = 1;
    }
    Huffsize[k] = 0;
 
    return Huffsize;
}



uint16_t* generate_code_table(uint8_t* Huffsize) {
    uint16_t* Huffcode = malloc(256*sizeof(uint16_t));
    int k = 0;
    uint16_t code = 0;  
    uint8_t si = Huffsize[0];

    while (Huffsize[k] != 0) { 
        if (Huffsize[k] == si) {
            do {
                Huffcode[k] = code;  
                code++;              
                k++;                 
            } while (Huffsize[k] == si && Huffsize[k] != 0);  
        }

        if (Huffsize[k] == 0) {
            break;  
        }

        
        do {
            code <<= 1;  
            si++;        
        } while (Huffsize[k] != si);  
    }
    return Huffcode;
}





uint32_t code_DC_Y(int16_t element,uint8_t* Huffval,int* nb_bits,uint8_t* Huffsize,uint16_t* Huffcode){
    uint16_t code_mag;
    int idx=-1;
    uint8_t mag=magnitude(element);
    
    for(int i=0;i<12;i++){
        if (mag==Huffval[i]){
            idx=i;
            code_mag= Huffcode[idx];
            *nb_bits=Huffsize[idx]+mag;
            break;
        }
    }
    

    
    int indice_elem=indice(element);
    uint32_t res= ((uint32_t)(code_mag << mag)+(uint32_t)indice_elem );
    return res;

}



uint32_t code_DC_CbCr(int16_t element,uint8_t* Huffval,int* nb_bits,uint8_t* Huffsize,uint16_t* Huffcode){
   
    uint16_t code_mag;
    int idx=-1;
    uint8_t mag=magnitude(element);
    
    for(int i=0;i<12;i++){
        if (mag==Huffval[i]){
            idx=i;
            code_mag= Huffcode[idx];
            *nb_bits=Huffsize[idx]+mag;
            break;
        }
    }
    

    
    int indice_elem=indice(element);
    uint32_t res= ((uint32_t)(code_mag << mag)+(uint32_t)indice_elem );
    return res;

}



















uint32_t code_AC_Y(int16_t element,uint8_t* Huffval,int taille,int nb_0,int* nb_bits,uint8_t*Huffsize,uint16_t* Huffcode){
    
    uint32_t code_8_bits;
    int idx=-1;
    uint8_t mag=magnitude(element);
    if (nb_0 <0){
        uint32_t element_a_coder=0x00;
        for(int i=0;i<taille;i++){
            if (element_a_coder==Huffval[i]){
                idx=i;
                code_8_bits= Huffcode[idx];
               
                *nb_bits=Huffsize[idx];
                break;
            }
        }
        
        return (uint32_t)code_8_bits;
    }else{
        if(nb_0<16){
            uint32_t elmene_a_coder=  (nb_0<<4)+mag;
            
            for(int i=0;i<taille;i++){
                if (elmene_a_coder==Huffval[i]){
                    idx=i;
                    code_8_bits= Huffcode[idx];

                    *nb_bits=Huffsize[idx]+mag;

                    break;
                }
            }
        
            int indice_elem=indice(element);

            uint32_t result = (uint32_t)(code_8_bits<<mag)+(uint32_t)indice_elem;
            return result;

        }else{
            uint32_t elmene_a_coder=240;
            for(int i=0;i<taille;i++){
                if (elmene_a_coder==Huffval[i]){
                    idx=i;
                    code_8_bits= Huffcode[idx];
                    *nb_bits=Huffsize[idx];
                    break;
                }
            }
            return (uint32_t)code_8_bits;
        }
    }
    
    

}

uint32_t code_AC_CbCr(int16_t element,uint8_t* Huffval,int taille,int nb_0,int* nb_bits,uint8_t* Huffsize,uint16_t* Huffcode){
    
    uint32_t code_8_bits;
    int idx=-1;
    uint8_t mag=magnitude(element);
    if (nb_0 <0){
        uint32_t element_a_coder=0x00;
        for(int i=0;i<taille;i++){
            if (element_a_coder==Huffval[i]){
                idx=i;
                code_8_bits= Huffcode[idx];
                
                *nb_bits=Huffsize[idx];
                break;
            }
        }
        
        return (uint32_t)code_8_bits;
    }else{
        if(nb_0<16){
            uint32_t elmene_a_coder=  (nb_0<<4)+mag;
            
            for(int i=0;i<taille;i++){
                if (elmene_a_coder==Huffval[i]){
                    idx=i;
                    code_8_bits= Huffcode[idx];

                    *nb_bits=Huffsize[idx]+mag;

                    break;
                }
            }
        
            
            int indice_elem=indice(element);
           

            uint32_t result = (uint32_t)(code_8_bits<<mag)+(uint32_t)indice_elem;
            return result;

        }else{
            uint32_t elmene_a_coder=240;
            for(int i=0;i<taille;i++){
                if (elmene_a_coder==Huffval[i]){
                    idx=i;
                    code_8_bits= Huffcode[idx];
                    
                    *nb_bits=Huffsize[idx];
                    break;
                }
            }
            
            return (uint32_t)code_8_bits;
        }
    }
    
    

}





bool tous_nuls_apres_k(int k, int16_t* liste) {
    for (int i = k; i < 64; i++) {
        if (liste[i] != 0) {
            return false;
        }
    }
    return true;
}




