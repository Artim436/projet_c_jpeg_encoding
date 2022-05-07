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


uint8_t magnitude_table_DC(int16_t value){
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

uint8_t magnitude_table_AC(int16_t value){
    if (value < 0){
        value = -value;
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
    else {
        return 10;
    }
}

uint8_t encoding_rle_AC(int16_t *F, uint8_t *i){ 
    uint8_t cpt_0 = 0;
    for(uint8_t k = *i; k < min(64, *i+15); k++){
        if(F[k] == 0){
            cpt_0++;
        }
        else{
            if(cpt_0 <= 15){ //0000000000bloc[k]
                cpt_0 = cpt_0*pow(2,4) + magnitude_table_DC(F[k]);  //cpt_0magnitude(bloc[k])
                *i += cpt_0+1;
                return cpt_0;
            }
            else{ //cpt_0 =16
                return 0xF0;
            }
        }
    }
}

void rle(int16_t *F){
    uint8_t i = 0;
    while (i < 64){
        printf(" %x ", encoding_rle_DC(F, &i));
    }
}



