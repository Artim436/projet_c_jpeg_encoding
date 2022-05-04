#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <lecture_pgm.h>
#include <MCU.h>
#include <convert_to_YCbCr.h>
#include <math.h>
#include <huffman.h>
#include <htables.h>
#include <write_jpeg_file.h>

/*Reçoit en entrée une */
struct huff_table *creation_table_8x8(enum sample_type acdc, enum color_component cc){

    uint8_t nb_symb_per_length = htables_nb_symb_per_lengths[acdc][cc][16];
    uint8_t *list_of_symb = htables_symbols[acdc][cc];
    uint8_t list_of_len = htables_nb_symbols[acdc][cc];

    struct huff_table *p_thuff = huffman_table_build(nb_symb_per_length, list_of_symb, list_of_len);
    return p_thuff;
}