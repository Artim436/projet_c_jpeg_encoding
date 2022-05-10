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

void encodage_Y(struct main_mcu *p_main){
    int16_t precursor = 0;
    for(uint32_t mcu_i=0; mcu_i<p_main->n_mcu; mcu_i++){
        
        uint8_t *R = calloc(64, sizeof(uint8_t));
        uint8_t compteur = 1;
        int16_t tmp = precursor;
        precursor = p_main->bloc[mcu_i][0];
        p_main->bloc[mcu_i][0] = p_main->bloc[mcu_i][0] - tmp;
        rle(p_main->bloc[mcu_i], R);//On écrit dans R l'encodage RLE de toutes les valeurs
        //Encoding DC:
       
        uint8_t *nb_bits = calloc(1,sizeof(uint8_t));
        uint32_t huffman_path = huffman_table_get_path(p_main->htable[0], R[0], nb_bits);

        bitstream_write_bits(p_main->blitzstream, huffman_path, *nb_bits, false);
        bitstream_write_bits(p_main->blitzstream, index(p_main->bloc[mcu_i][0]), magnitude_table(p_main->bloc[mcu_i][0]), false);


        //Encoding AC:
        for (uint8_t i=1; i<64; i++){
            if(p_main->bloc[mcu_i][i] != 0){
                huffman_path = huffman_table_get_path(p_main->htable[1], R[compteur], nb_bits);
                bitstream_write_bits(p_main->blitzstream, huffman_path, *nb_bits, false);
                bitstream_write_bits(p_main->blitzstream, index(p_main->bloc[mcu_i][i]), magnitude_table(p_main->bloc[mcu_i][i]), false);
                compteur ++;
            }
        
        }
        huffman_path = huffman_table_get_path(p_main->htable[1], R[compteur], nb_bits);
        bitstream_write_bits(p_main->blitzstream, huffman_path, *nb_bits, false);
    }
}



uint8_t magnitude_table(int16_t value){
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
                cpt_0 = cpt_0*pow(2,4) + magnitude_table(F[k]);  //cpt_0magnitude(bloc[k])
                return cpt_0;
            }
            else{ //cpt_0 =16
                *i += 16; //
                return 0xF0;
            }
        }        
    }
    return 0;//sert juste à enlever le warning
}

uint8_t encoding_rle_ac_2(int16_t* F, uint8_t * cpt_bloc_0,uint8_t *pos){
    uint8_t cpt_0 = 0;
    while(F[*pos] == 0 && *pos<64){
        cpt_0 ++;
        *pos = *pos + 1;
        if(cpt_0 == 16){
            cpt_0=0;
            *cpt_bloc_0 = *cpt_bloc_0 + 1;

        }
    }
    if(*pos == 64){
        return 0x00;
    }
    while(*cpt_bloc_0 != 0){
        *cpt_bloc_0 = *cpt_bloc_0 - 1;
        return 0xF0;
    }
    
    cpt_0 = cpt_0*pow(2,4) + magnitude_table(F[*pos]);  //cpt_0magnitude(bloc[k])
    *pos = *pos + 1;
    return cpt_0;

}

void rle(int16_t *F, uint8_t *R){
    uint8_t index = 0;
    uint8_t k = 0;
    uint8_t cpt_bloc_0 = 0;
    if(F[0]==0){
        R[k] = 0;
        index ++;
        k ++;
    }
    while (index < 64){
        R[k] = encoding_rle_ac_2(F, &cpt_bloc_0, &index);
        k++;
    }
    if (R[k] != 0x00){
        R[k] = 0x00;
    }
}


uint32_t index(int16_t value){
    uint8_t m = magnitude_table(value);
    if(value < 0){
        return pow(2,m)-1 + value;
    }
    else{
        return pow(2,m)-1-pow(2,m-1)+ - pow(2,m-1)+ value +1;
    }
}

void affichage_encodage(struct main_mcu *p_main){
    for(uint32_t k = 0; k <p_main->n_mcu; k++){
        uint8_t *R = calloc(65, sizeof(uint8_t));
        uint8_t *nb_bits = calloc(1,sizeof(uint8_t));
        uint8_t compteur = 0;
        rle(p_main->bloc[k], R);
        for (int i=0; i<64; i++){
            if(p_main->bloc[k][i] != 0){
                printf(" value: ");
                printf(" %d ", p_main->bloc[k][i]);
                printf(" magnitude: ");
                printf(" %d ", magnitude_table(p_main->bloc[k][i]));
                printf(" index: ");
                printf(" %d ", index(p_main->bloc[k][i]));
                printf(" rle: ");
                printf(" %d ", R[compteur]);
                uint32_t huffman = huffman_table_get_path(p_main->htable[1], R[compteur], nb_bits);
                printf("huffman path : %d   nb_bits : %u\n", huffman, *nb_bits);      
                printf("\n");
                compteur ++;
            }
        }
        printf("EndOfBlock : %d\n", R[compteur]);
    }
}