#ifndef _MCU_H_
#define _MCU_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <structure.h>

extern struct image_mcu *decoupe_mcu_8x8(struct main_mcu *p_main);

extern struct image_mcu *creation_mcu_8x8(char type_pgm[3], uint32_t width, uint32_t height, uint32_t max_value);

extern void affiche_img_mcu(struct image_mcu *p_gmu);

extern void convert_vect_to_mat(uint8_t vect[64]);

extern void print_vect(uint8_t *vect, uint8_t len_vect);

extern void print_mat(uint8_t mat[8][8]);

#endif
