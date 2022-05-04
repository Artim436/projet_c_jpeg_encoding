#ifndef _MCU_H_
#define _MCU_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lecture_pgm.h>
#include <structure.h>

extern void decoupe_mcu_8x8(struct main_mcu *p_main);

extern void affiche_img_mcu(struct main_mcu *p_main);

#endif
