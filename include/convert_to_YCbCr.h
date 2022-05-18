#ifndef _convert_to_YCbCr_H_
#define _convert_to_YCbCr_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <MCU.h>
#include <structure.h>

extern struct image_YCbCr *convert_YCbCr(struct image_mcu *p_mcu);

extern struct image_YCbCr *creation_YCbCr(struct image_mcu *p_mcu);

extern void afficher_YCbCr(struct image_YCbCr *p_ycbcr);

extern void clean_image_YCbCr_sub(struct image_YCbCr_sub *ycbcr);

extern void afficher_YCbCr_sub(struct image_YCbCr_sub *p_ycbcr);

extern struct image_YCbCr_sub *convert_YCbCr_RGB_sub(struct image_mcu_rgb_sub *p_mcu , struct main_mcu_rgb_sub* p_main);

extern struct image_YCbCr_sub *creation_YCbCr_rgb_sub(struct main_mcu_rgb_sub* p_main);

#endif
