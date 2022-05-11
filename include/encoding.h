#ifndef _ENCODING_H
#define _ENCODING_H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <jpeg_writer.h>
#include <bitstream.h>
#include <huffman.h>
#include <structure.h>

extern void creation_table(struct main_mcu *mcu);

extern void encodage_Y(struct main_mcu *p_main);

extern uint8_t magnitude_table(int16_t value);

extern uint8_t encoding_rle_AC(int16_t *F, uint8_t *i);

extern void rle(int16_t *F, uint8_t *R, uint8_t* taille);

extern uint32_t index(int16_t value);

extern void affichage_encodage(struct main_mcu *p_main);

extern uint8_t encoding_rle_2(int16_t* F, uint8_t * cpt_bloc_0,uint8_t *pos);

extern void encodage_Y_rgb_2(struct main_mcu_rgb *p_main);

extern void affiche_encodage_rgb(struct main_mcu_rgb *p_main);

#endif