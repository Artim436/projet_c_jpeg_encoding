#ifndef _convert_to_YCbCr_H_
#define _convert_to_YCbCr_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lecture_pgm.h>
#include <MCU.h>
#include <math.h>
#include <structure.h>

extern struct image_YCbCr *convert_YCbCr(struct image_mcu *p_mcu);

extern struct image_YCbCr *creation_YCbCr(struct image_mcu *p_mcu);

extern void afficher_YCbCr(struct image_YCbCr *p_ycbcr);

extern struct image_YCbCr *convert_YCbCr_RGB(struct image_mcu_rgb *p_mcu);

extern void afficher_YCbCr_rgb(struct image_YCbCr *p_ycbcr);

#endif
