#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    
#include <lecture_pgm.h>
#include <MCU.h>
#include <structure.h>
#include <convert_to_YCbCr.h>
#include <dct.h>
#include <htables.h>
#include <jpeg_writer.h>
#include <test.h>
#include <huffman.h>
#include <encoding.h>
#include <math.h>

#define min(a,b) (a<=b?a:b) //if a<=b return a else return b


void creation_table(struct main_mcu *mcu){
    /*Allocation mémoire pour Y : AC et DC,    et : Cb/Cr : Ac et DC*/
    /* 0 Y Dc, 1 Y Ac, 2 CbCr Dc, 3CbCr Ac*/
    mcu->htable = calloc(4,sizeof(struct huff_table *));
    mcu->htable[0] = huffman_table_build(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]);
    mcu->htable[1] = huffman_table_build(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]);
    mcu->htable[2] = huffman_table_build(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]);
    mcu->htable[3] = huffman_table_build(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]);
}

/*
void encodage_Y(struct main_mcu *p_main){
    creation_table(p_main);
    //p_main->blitztream = bitstream_create(p_main->jpeg_filename);
    for(uint32_t mcu_i=0; mcu_i<p_main->n_mcu; mcu_i++){
        //Encoding DC:
        //Encoding AC:
        uint8_t cpt_bloc_0 = 0;
        uint8_t cpt_0 = 0;

        for(uint8_t i = 1;i<64; i++){

            if(p_main->bloc[mcu_i][i] == 0 && cpt_0 < 16){
                cpt_0++;
            }
            else{
                if(cpt_0 == 16){
                    cpt_0 = 0;
                    cpt_bloc_0++;
                }
                else{
                    while(cpt_bloc_0 != 0){
                        //bitstream_write_bits(p_main->blitztream, 0xF0, 16, false);
                        printf("%x ", 0xF0);
                        cpt_bloc_0 --;
                    }
                    uint8_t *magnitude = calloc(1,sizeof(uint8_t));
                    uint32_t valeur = huffman_table_get_path(p_main->htable[1], (uint8_t) p_main->bloc[mcu_i][i], magnitude);
                    printf("%u ", valeur);
                    cpt_0 = cpt_0*pow(2,4) + *magnitude;  //cpt_0magnitude(bloc[k])
                    //bitstream_write_bits(p_main->blitztream, valeur, *magnitude, false);
                    cpt_0 = 0;
                }
            }
        }
        //bitstream_write_bits(p_main->blitztream, 0x00, 16, false);
    }
}
*/


uint8_t magnitude_table_AC(int16_t value){
    if (value < 0){
        value = -value;
    }
    if (value == 0){
        return 0;
    }
    else if (value <=1){
        return 1;
    }
    else if (value <=3){
        return 2;
    }
    else if (value <=7){
        return 3;
    }
    else if (value <=15){
        return 4;
    }
    else if (value <=31){
        return 5;
    }
    else if (value <=63){
        return 6;
    }
    else if (value <=127){
        return 7;
    }
    else if (value <=255){
        return 8;
    }
    else if (value <=511){
        return 9;
    }
    else if (value <=1023){
        return 10;
    }
    else {
        return 11;
    }
}
 

uint8_t encoding_rle_AC(int16_t *F, uint8_t *i){ 
    uint8_t cpt_0 = 0;
    for(uint8_t k = *i; k < min(64, *i+16); k++){
        if(F[k] == 0){
            cpt_0++;
            if (k == 63){
                *i = 64;
                return 0x00;
            }
        }
        else{
            if(cpt_0 <= 15){ //0000000000bloc[k]
                *i += cpt_0+1;
                cpt_0 = cpt_0*pow(2,4) + magnitude_table_DC(F[k]);  //cpt_0magnitude(bloc[k])
                return cpt_0;
            }
            else{ //cpt_0 =16
                *i += 16; //
                return 0xF0;
            }
        }
    }
}


void rle(int16_t *F, uint8_t *R){
    uint8_t index = 1;
    uint8_t k = 0;
    // cpt = 0;
    while (index < 64){
        // R = malloc(cpt*sizeof(uint8_t));
        R[k] = encoding_rle_AC(F, &index);
        //cpt++;
        k++;
    }
}


