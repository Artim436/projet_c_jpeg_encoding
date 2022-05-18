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

extern struct image_mcu_rgb_sub *creation_mcu_rgb_sub(uint32_t width, uint32_t height, uint8_t h1, uint8_t v1);

extern struct image_mcu_rgb_sub *decoupe_mcu_rgb_sub(struct main_mcu_rgb_sub *p_main);

extern void affiche_img_mcu_rgb_sub(struct image_mcu_rgb_sub *p_gmu, uint8_t h1, uint8_t v1);

extern void clean_image_mcu_sub(struct image_mcu_rgb_sub *image, uint8_t h1, uint8_t v1);


#endif
