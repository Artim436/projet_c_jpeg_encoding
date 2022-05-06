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
void huffman_encoding(struct main_mcu *p_main){
    //Parcours tous les blocs de l'image
    for (uint8_t mcu_i=0; mcu_i<p_main->n_mcu; mcu_i++){
        //Prend le premier élément (DC)
        int16_t dc = p_main->bloc[mcu_i][0];
        
    }
}