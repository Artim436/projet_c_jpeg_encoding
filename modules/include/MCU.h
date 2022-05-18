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

extern void print_mat(float** mat, uint8_t  len);

extern void affiche_img_mcu_rgb(struct image_mcu_rgb *p_gmu);

extern struct image_mcu_rgb *creation_mcu_8x8_rgb(char type_pgm[3], uint32_t width, uint32_t height, uint32_t max_value);

extern struct image_mcu_rgb *decoupe_mcu_8x8_rgb(struct main_mcu_rgb *p_main);

extern float **convert_YCbCr_mat(struct YCbCr **p_YCbCr);

extern float ***convert_YCbCr_mat_rgb(struct YCbCr **p_YCbCr);

#endif
