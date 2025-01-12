#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define N 8
#include "../include/zigzag.h"


int16_t* zigzag( int16_t** image_2D) {
   
    int16_t* result=(int16_t*)malloc(N*N*sizeof(int16_t));


    result[0]=image_2D[0][0];
    result[1]=image_2D[0][1];
    result[2]=image_2D[1][0];
    result[3]=image_2D[2][0];
    result[4]=image_2D[1][1];
    result[5]=image_2D[0][2];
    result[6]=image_2D[0][3];
    result[7]=image_2D[1][2];
    result[8]=image_2D[2][1];
    result[9]=image_2D[3][0];
    result[10]=image_2D[4][0];
    result[11]=image_2D[3][1];
    result[12]=image_2D[2][2];
    result[13]=image_2D[1][3];
    result[14]=image_2D[0][4];
    result[15]=image_2D[0][5];
    result[16]=image_2D[1][4];
    result[17]=image_2D[2][3];
    result[18]=image_2D[3][2];
    result[19]=image_2D[4][1];
    result[20]=image_2D[5][0];
    result[21]=image_2D[6][0];
    result[22]=image_2D[5][1];
    result[23]=image_2D[4][2];
    result[24]=image_2D[3][3];
    result[25]=image_2D[2][4];
    result[26]=image_2D[1][5];
    result[27]=image_2D[0][6];
    result[28]=image_2D[0][7];
    result[29]=image_2D[1][6];
    result[30]=image_2D[2][5];
    result[31]=image_2D[3][4];
    result[32]=image_2D[4][3];
    result[33]=image_2D[5][2];
    result[34]=image_2D[6][1];
    result[35]=image_2D[7][0];
    result[36]=image_2D[7][1];
    result[37]=image_2D[6][2];
    result[38]=image_2D[5][3];
    result[39]=image_2D[4][4];
    result[40]=image_2D[3][5];
    result[41]=image_2D[2][6];
    result[42]=image_2D[1][7];
    result[43]=image_2D[2][7];
    result[44]=image_2D[3][6];
    result[45]=image_2D[4][5];
    result[46]=image_2D[5][4];
    result[47]=image_2D[6][3];
    result[48]=image_2D[7][2];
    result[49]=image_2D[7][3];
    result[50]=image_2D[6][4];
    result[51]=image_2D[5][5];
    result[52]=image_2D[4][6];
    result[53]=image_2D[3][7];
    result[54]=image_2D[4][7];
    result[55]=image_2D[5][6];
    result[56]=image_2D[6][5];
    result[57]=image_2D[7][4];
    result[58]=image_2D[7][5];
    result[59]=image_2D[6][6];
    result[60]=image_2D[5][7];
    result[61]=image_2D[6][7];
    result[62]=image_2D[7][6];
    result[63]=image_2D[7][7];

   
    return result;
}



