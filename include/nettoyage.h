
#ifndef _NETTOYAGE_H_
#define _NETTOYAGE_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <structure.h>

void clean_main_mcu(struct main_mcu *mcu);

void clean_image_mcu(struct image_mcu *mcu);

void clean_image_YCbCr(struct image_YCbCr *ycbcr);
#endif