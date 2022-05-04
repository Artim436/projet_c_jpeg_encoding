// #ifndef _convert_to_YCbCr_H_
// #define _convert_to_YCbCr_H_
// #include <stdlib.h>
// #include <stdint.h>
// #include <stdio.h>
// #include <stdbool.h>
// #include <lecture_pgm.h>
// #include <MCU.h>
// #include <math.h>

// struct YCbCr;
// struct YCbCr{
//     /*Donnée contenant les valeurs de YCbCr d'un pixel*/
//     uint32_t Y;
//     uint32_t Cb;
//     uint32_t Cr;
// };

// struct image_YCbCr;
// struct image_YCbCr{
//     /*Structure des YCbCr, ressemble beaucoup aux structures différentes mais nécessaires pour la manipulation.*/
//     char type_pgm[3];
//     struct YCbCr ***l_ycbcr;
//     uint32_t nmcu;//Equivalent aux nombre de MCU
// };

// extern struct image_YCbCr *convert_YCbCr(struct image_mcu *p_mcu);

// extern struct image_YCbCr *creation_YCbCr(struct image_mcu *p_mcu);

// extern void afficher_YCbCr(struct image_YCbCr *p_ycbcr);

// #endif
