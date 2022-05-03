#ifndef _MCU_H_
#define _MCU_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lecture_pgm.h>

struct image_mcu;
struct image_mcu{
    /*Structure des mcu*/
    char type_pgm[3];
    uint8_t **l_mcu;
    uint32_t nmcu;//Pour la largeur comme pour la hauteur on renverra la taille par pixel et non par MCU
    uint32_t max_value;
    uint8_t dev_width;//En soit pas utile pour la structure mais nécessaire pour la construction, il s'agit du reste modulo8 des dimensions de image pgm
    uint8_t dev_height;
};

extern struct image_mcu *decoupe_mcu_8x8(struct image_pgm *image_pgm);

extern struct image_mcu *creation_mcu_8x8(char type_pgm[3], uint32_t width, uint32_t height, uint32_t max_value);

extern void affiche_img_mcu(struct image_mcu *p_gmu);

#endif
