#ifndef DOWNSAMPLING_H
#define DOWNSAMPLING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define N 8

void downsampling(uint8_t** mcu, int h1, int v1, int hi, int vi, uint8_t*** mcu_sortie);

#endif
