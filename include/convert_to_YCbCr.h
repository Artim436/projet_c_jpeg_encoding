#ifndef _convert_to_YCbCr_H_
#define _convert_to_YCbCr_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lecture_pgm.h>
#include <MCU.h>

struct YCbCr;
struct YCbCr{
    /*Donnée contenant les valeurs de YCbCr d'un pixel*/
    float Y;
    float Cb;
    float Cr;
};

struct image_YCbCr;
struct image_YCbCr{
    /*Structure des YCbCr, ressemble beaucoup aux structures différentes mais nécessaires pour la manipulation.*/
    char type_pgm[3];
    struct YCbCr ***l_ycbcr;
    uint32_t nmcu;//Equivalent aux nombre de MCU
};

extern struct image_YCbCr *convert_YCbCr(struct image_mcu *p_mcu);

extern struct image_YCbCr *creation_YCbCr(struct image_mcu *p_mcu);

#endif
