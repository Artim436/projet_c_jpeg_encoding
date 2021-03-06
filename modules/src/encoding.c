#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    
#include <lecture_pgm.h>
#include <MCU.h>
#include <structure.h>
#include <convert_to_YCbCr.h>
#include <htables.h>
#include <jpeg_writer_maison.h>
#include <test.h>
#include <huffman_maison.h>
#include <encoding.h>
#include <math.h>
#include <write_jpeg_file.h>
#include <bitstream_maison.h>

#define min(a,b) (a<=b?a:b) //if a<=b return a else return b


void creation_table(struct main_mcu *mcu){
    /*Allocation mémoire pour Y : AC et DC,    et : Cb/Cr : Ac et DC*/
    /* 0 Y Dc, 1 Y Ac, 2 CbCr Dc, 3CbCr Ac*/
    mcu->AC_Y =huffman_table_build_M(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]); 
    mcu->AC_Cb =huffman_table_build_M(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]); 
    mcu->AC_Cr =huffman_table_build_M(htables_nb_symb_per_lengths[AC][Cr], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]); 
    mcu->DC_Y =huffman_table_build_M(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]); 
    mcu->DC_Cb =huffman_table_build_M(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]); 
    mcu->DC_Cr =huffman_table_build_M(htables_nb_symb_per_lengths[DC][Cr], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]); 


    // mcu->htable = calloc(4,sizeof(struct huff_table_M *));
    // mcu->htable[0] = huffman_table_build_M(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]);
    // mcu->htable[1] = huffman_table_build_M(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]);
    // mcu->htable[2] = huffman_table_build_M(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]);
    // mcu->htable[3] = huffman_table_build_M(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]);
}

void creation_table_rgb(struct main_mcu_rgb *mcu){
    /*Allocation mémoire pour Y : AC et DC,    et : Cb/Cr : Ac et DC*/
    /* 0 Y Dc, 1 Y Ac, 2 CbCr Dc, 3CbCr Ac*/
    mcu->AC_Y =huffman_table_build_M(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]); 
    mcu->AC_Cb =huffman_table_build_M(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]); 
    mcu->AC_Cr =huffman_table_build_M(htables_nb_symb_per_lengths[AC][Cr], htables_symbols[AC][Cr], htables_nb_symbols[AC][Cr]); 
    mcu->DC_Y =huffman_table_build_M(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]); 
    mcu->DC_Cb =huffman_table_build_M(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]); 
    mcu->DC_Cr =huffman_table_build_M(htables_nb_symb_per_lengths[DC][Cr], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]); 
}


void encodage_Y(struct main_mcu *p_main){
    int16_t precursor = 0;
    for(uint32_t mcu_i=0; mcu_i<p_main->n_mcu; mcu_i++){  
        uint8_t *R = calloc(64, sizeof(uint8_t));
        uint8_t compteur = 1;
        int16_t tmp = precursor;
        precursor = p_main->bloc[mcu_i][0];
        p_main->bloc[mcu_i][0] = p_main->bloc[mcu_i][0] - tmp;
        uint8_t* taille = calloc(1, sizeof(uint8_t));
        rle(p_main->bloc[mcu_i], R, taille);//On écrit dans R l'encodage RLE de toutes les valeurs
        
        //Encoding DC:
        uint8_t *nb_bits = calloc(1,sizeof(uint8_t));
        uint32_t huffman_path = huffman_table_get_path_M(p_main->DC_Y, R[0], nb_bits);

        bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
        bitstream_write_bits_M(p_main->blitzstream, index(p_main->bloc[mcu_i][0]), magnitude_table(p_main->bloc[mcu_i][0]), false);


        //Encoding AC:
        for (uint8_t i=1; i<64; i++){
            if(p_main->bloc[mcu_i][i] != 0){
                while(R[compteur] == 0xF0){
                    huffman_path = huffman_table_get_path_M(p_main->AC_Y, R[compteur], nb_bits);
                    bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
                    compteur ++;
                }
                huffman_path = huffman_table_get_path_M(p_main->AC_Y, R[compteur], nb_bits);
                bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
                bitstream_write_bits_M(p_main->blitzstream, index(p_main->bloc[mcu_i][i]), magnitude_table(p_main->bloc[mcu_i][i]), false);
                compteur ++;
            }
        }
        if(compteur == *taille-1){
            huffman_path = huffman_table_get_path_M(p_main->AC_Y, R[compteur], nb_bits);
            bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
        }
        else if(compteur < *taille){
            printf("Erreur dans le compte\n");
        }
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
 

uint8_t encoding_rle_ac_2(int16_t* F, uint8_t* cpt_bloc_0, uint8_t *pos){
    uint8_t cpt_0 = 0;
    while(F[*pos] == 0 && *pos<64){
        cpt_0 ++;
        *pos = *pos + 1;
        if(cpt_0 == 16){
            cpt_0=0;
            *cpt_bloc_0 = *cpt_bloc_0 + 1;

        }
    }
    if(*pos == 64 && F[*pos-1] == 0 ){
        return 0x00;
    }
    while(*cpt_bloc_0 != 0){
        *cpt_bloc_0 = *cpt_bloc_0 - 1;
        while(cpt_0 != 0){
            *pos = *pos - 1;
            cpt_0 --;
        }
        return 0xF0;
    }    
    cpt_0 = cpt_0*pow(2,4) + magnitude_table(F[*pos]);  //cpt_0magnitude(bloc[k])
    *pos = *pos + 1;
    return cpt_0;

}

void rle(int16_t *F, uint8_t *R, uint8_t* taille_R){
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
    if(k == 2 && R[0] == 0){
        R[1] = 0x00;
        //k --;
    }
    *taille_R = k;
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
    for(uint32_t k = 0; k <20; k++){
        printf("-----MCU : %u ------\n", k);
        uint8_t *R = calloc(65, sizeof(uint8_t));
        uint8_t *nb_bits = calloc(1,sizeof(uint8_t));
        uint8_t compteur = 0;
        uint8_t* taille = calloc(1, sizeof(uint8_t));
        rle(p_main->bloc[k], R, taille);
        for (int i=0; i<64; i++){
            if(p_main->bloc[k][i] != 0){
                while(R[compteur] == 0xF0){
                    printf("Ecriture de 0xF0\n");
                    compteur ++;
                }
                printf(" value: ");
                printf(" %d ", p_main->bloc[k][i]);
                printf(" magnitude: ");
                printf(" %d ", magnitude_table(p_main->bloc[k][i]));
                printf(" index: ");
                printf(" %d ", index(p_main->bloc[k][i]));
                printf(" rle: ");
                printf(" %d ", R[compteur]);
                uint32_t huffman = huffman_table_get_path_M(p_main->AC_Y, R[compteur], nb_bits);
                printf("huffman path : %d   nb_bits : %u\n", huffman, *nb_bits);      
                printf("\n");
                compteur ++;
            }
        }
        if(compteur == *taille-1){
            printf("EndOfBlock : %d\n", R[compteur]);
        }
        else if(compteur < *taille){
            printf("erreur dans le compte\n");
        }
    }
}

void encodage_Y_rgb_2(struct main_mcu_rgb *p_main){
    int16_t precursor_Y = 0;
    int16_t precursor_Cb = 0;
    int16_t precursor_Cr = 0;
    for(uint32_t mcu_i=0; mcu_i<p_main->n_mcu; mcu_i++){  
        //Calcul des codes rle de toutes les composantes ainsi que desprécurseurs
        uint8_t *RY = calloc(64, sizeof(uint8_t));
        uint8_t *RCb = calloc(64, sizeof(uint8_t));
        uint8_t *RCr = calloc(64, sizeof(uint8_t));

        uint8_t compteur_Y = 1;
        uint8_t compteur_Cb = 1;
        uint8_t compteur_Cr = 1;

        int16_t tmp = precursor_Y;
        precursor_Y = p_main->bloc[mcu_i][0][0];
        p_main->bloc[mcu_i][0][0] = p_main->bloc[mcu_i][0][0] - tmp;

        tmp = precursor_Cb;
        precursor_Cb = p_main->bloc[mcu_i][1][0];
        p_main->bloc[mcu_i][1][0] = p_main->bloc[mcu_i][1][0] - tmp;

        tmp = precursor_Cr;
        precursor_Cr = p_main->bloc[mcu_i][2][0];
        p_main->bloc[mcu_i][2][0] = p_main->bloc[mcu_i][2][0] - tmp;

        uint8_t* taille_Cb = calloc(1, sizeof(uint8_t));
        uint8_t* taille_Y = calloc(1, sizeof(uint8_t));
        uint8_t* taille_Cr = calloc(1, sizeof(uint8_t));

        rle(p_main->bloc[mcu_i][0], RY, taille_Y);//On écrit dans RY l'encodage RLE des valeurs de Y
        rle(p_main->bloc[mcu_i][1], RCb, taille_Cb);//On écrit dans RCb l'encodage RLE des valeurs de Cb
        rle(p_main->bloc[mcu_i][2], RCr, taille_Cr);//On écrit dans RCr l'encodage RLE des valeurs de Cr
        

        //On commence par Y:
            //Encoding DC:
        uint8_t *nb_bits = calloc(1,sizeof(uint8_t));
        uint32_t huffman_path = huffman_table_get_path_M(p_main->DC_Y, RY[0], nb_bits);

        bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
        bitstream_write_bits_M(p_main->blitzstream, index(p_main->bloc[mcu_i][0][0]), magnitude_table(p_main->bloc[mcu_i][0][0]), false);


            //Encoding AC:
        for (uint8_t i=1; i<64; i++){
            if(p_main->bloc[mcu_i][0][i] != 0){
                while(RY[compteur_Y] == 0xF0){
                    huffman_path = huffman_table_get_path_M(p_main->AC_Y, RY[compteur_Y], nb_bits);

                    bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
                    compteur_Y ++;
                }
                huffman_path = huffman_table_get_path_M(p_main->AC_Y, RY[compteur_Y], nb_bits);
                bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
                bitstream_write_bits_M(p_main->blitzstream, index(p_main->bloc[mcu_i][0][i]), magnitude_table(p_main->bloc[mcu_i][0][i]), false);
                compteur_Y ++;
            }
        }
        if(compteur_Y == *taille_Y-1){
            huffman_path = huffman_table_get_path_M(p_main->AC_Y, RY[compteur_Y], nb_bits);
            bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
        }
        else if(compteur_Y < *taille_Y){
            printf("Erreur dans le compte de Y\n");
        }
        
        
        //Encodage de Cb
            //Encoding DC:
        huffman_path = huffman_table_get_path_M(p_main->DC_Cb, RCb[0], nb_bits);

        bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
        bitstream_write_bits_M(p_main->blitzstream, index(p_main->bloc[mcu_i][1][0]), magnitude_table(p_main->bloc[mcu_i][1][0]), false);


            //Encoding AC:
        for (uint8_t i=1; i<64; i++){
            if(p_main->bloc[mcu_i][1][i] != 0){
                while(RCb[compteur_Cb] == 0xF0){
                    huffman_path = huffman_table_get_path_M(p_main->AC_Cb, RCb[compteur_Cb], nb_bits);
                    bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
                    compteur_Cb ++;
                }
                huffman_path = huffman_table_get_path_M(p_main->AC_Cb, RCb[compteur_Cb], nb_bits);
                bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
                bitstream_write_bits_M(p_main->blitzstream, index(p_main->bloc[mcu_i][1][i]), magnitude_table(p_main->bloc[mcu_i][1][i]), false);
                compteur_Cb ++;
            }
        }
        if(compteur_Cb == *taille_Cb-1){
            huffman_path = huffman_table_get_path_M(p_main->AC_Cb, 0x00, nb_bits);
            bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
        }
        else if(compteur_Cb < *taille_Cb){
            printf("Erreur dans le compte de Cb\n");
        }
        
        //Encodage de Cr
            //Encoding DC:
        huffman_path = huffman_table_get_path_M(p_main->DC_Cb, RCr[0], nb_bits);

        bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
        bitstream_write_bits_M(p_main->blitzstream, index(p_main->bloc[mcu_i][2][0]), magnitude_table(p_main->bloc[mcu_i][2][0]), false);


            //Encoding AC:
        for (uint8_t i=1; i<64; i++){
            if(p_main->bloc[mcu_i][2][i] != 0){
                while(RCr[compteur_Cr] == 0xF0){
                    huffman_path = huffman_table_get_path_M(p_main->AC_Cb, RCr[compteur_Cr], nb_bits);
                    bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
                    compteur_Cr ++;
                }
                huffman_path = huffman_table_get_path_M(p_main->AC_Cb, RCr[compteur_Cr], nb_bits);
                bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
                bitstream_write_bits_M(p_main->blitzstream, index(p_main->bloc[mcu_i][2][i]), magnitude_table(p_main->bloc[mcu_i][2][i]), false);
                compteur_Cr ++;
            }
        }
        if(compteur_Cr == *taille_Cr-1){
            huffman_path = huffman_table_get_path_M(p_main->AC_Cb, 0x00, nb_bits);
            bitstream_write_bits_M(p_main->blitzstream, huffman_path, *nb_bits, false);
        }
        else if(compteur_Cr < *taille_Cr){
            printf("Erreur dans le compte de Cr\n");
        }
    }
}


void affiche_encodage_rgb(struct main_mcu_rgb *p_main){
    printf("On est dans affichage\n");
    for(uint32_t mcu_i=3466; mcu_i<3467; mcu_i++){ 
        printf("-----MCU : %u ------\n", mcu_i);

        //Calcul des codes rle de toutes les composantes ainsi que desprécurseurs
        uint8_t *RY = calloc(64, sizeof(uint8_t));
        uint8_t *RCb = calloc(64, sizeof(uint8_t));
        uint8_t *RCr = calloc(64, sizeof(uint8_t));
        uint8_t compteur_Y = 1;
        uint8_t compteur_Cb = 1;
        uint8_t compteur_Cr = 1;
        
        uint8_t* taille_Cb = calloc(1, sizeof(uint8_t));
        uint8_t* taille_Y = calloc(1, sizeof(uint8_t));
        uint8_t* taille_Cr = calloc(1, sizeof(uint8_t));
        uint8_t* nb_bits = calloc(1, sizeof(uint8_t));
        rle(p_main->bloc[mcu_i][0], RY, taille_Y);//On écrit dans RY l'encodage RLE des valeurs de Y
        rle(p_main->bloc[mcu_i][1], RCb, taille_Cb);//On écrit dans RCb l'encodage RLE des valeurs de Cb
        rle(p_main->bloc[mcu_i][2], RCr, taille_Cr);//On écrit dans RCr l'encodage RLE des valeurs de Cr

        printf("Composante Y : \n\n");
        printf(" DC : \n");
        printf(" value: ");
        printf(" %d ", p_main->bloc[mcu_i][0][0]);
        printf(" magnitude: ");
        printf(" %d ", magnitude_table(p_main->bloc[mcu_i][0][0]));
        printf(" index: ");
        printf(" %d ", index(p_main->bloc[mcu_i][0][0]));
        printf(" rle: ");
        printf(" %d ", RY[compteur_Y]);
        uint32_t huffman = huffman_table_get_path_M(p_main->DC_Y, RY[0], nb_bits);
        printf("huffman path : %d   nb_bits : %u\n", huffman, *nb_bits);      
        printf("\n");
        printf(" AC : \n");
        for (int i=1; i<64; i++){
            if(p_main->bloc[mcu_i][0][i] != 0){
                while(RY[compteur_Y] == 0xF0){
                    printf("Ecriture de 0xF0\n");
                    compteur_Y ++;
                }
                printf(" value: ");
                printf(" %d ", p_main->bloc[mcu_i][0][i]);
                printf(" magnitude: ");
                printf(" %d ", magnitude_table(p_main->bloc[mcu_i][0][i]));
                printf(" index: ");
                printf(" %d ", index(p_main->bloc[mcu_i][0][i]));
                printf(" rle: ");
                printf(" %d ", RY[compteur_Y]);
                uint32_t huffman = huffman_table_get_path_M(p_main->AC_Y, RY[compteur_Y], nb_bits);
                printf("huffman path : %d   nb_bits : %u\n", huffman, *nb_bits);      
                printf("\n");
                compteur_Y ++;
            }
        }
        if(compteur_Y == *taille_Y-1){
            printf("EndOfBlock : %d\n", RY[compteur_Y]);
        }
        else if(compteur_Y < *taille_Y){
            printf("erreur dans le compte\n");
        }
        printf("\n\n");
        printf("Composante Cb : \n\n");
        printf(" DC : \n");
        printf(" value: ");
        printf(" %d ", p_main->bloc[mcu_i][1][0]);
        printf(" magnitude: ");
        printf(" %d ", magnitude_table(p_main->bloc[mcu_i][1][0]));
        printf(" index: ");
        printf(" %d ", index(p_main->bloc[mcu_i][1][0]));
        printf(" rle: ");
        printf(" %d ", RCb[compteur_Cb]);
        huffman = huffman_table_get_path_M(p_main->DC_Cb, RCb[0], nb_bits);
        printf("huffman path : %d   nb_bits : %u\n", huffman, *nb_bits);      
        printf("\n");
        printf(" AC : \n");
        for (int i=1; i<64; i++){
            if(p_main->bloc[mcu_i][1][i] != 0){
                while(RCb[compteur_Cb] == 0xF0){
                    printf("Ecriture de 0xF0\n");
                    compteur_Cb ++;
                }
                printf(" value: ");
                printf(" %d ", p_main->bloc[mcu_i][1][i]);
                printf(" magnitude: ");
                printf(" %d ", magnitude_table(p_main->bloc[mcu_i][1][i]));
                printf(" index: ");
                printf(" %d ", index(p_main->bloc[mcu_i][1][i]));
                printf(" rle: ");
                printf(" %d ", RCb[compteur_Cb]);
                huffman = huffman_table_get_path_M(p_main->AC_Cb, RCb[compteur_Cb], nb_bits);
                printf("huffman path : %d   nb_bits : %u\n", huffman, *nb_bits);      
                printf("\n");
                compteur_Cb ++;
            }
        }
        if(compteur_Cb == *taille_Cb-1){
            printf("EndOfBlock : %d\n", RCb[compteur_Cb]);
        }
        else if(compteur_Cb < *taille_Cb){
            printf("erreur dans le compte\n");
        }
        printf("\n\n");

        printf("Composante Cr : \n\n");
        printf(" DC : \n");
        printf(" value: ");
        printf(" %d ", p_main->bloc[mcu_i][2][0]);
        printf(" magnitude: ");
        printf(" %d ", magnitude_table(p_main->bloc[mcu_i][2][0]));
        printf(" index: ");
        printf(" %d ", index(p_main->bloc[mcu_i][2][0]));
        printf(" rle: ");
        printf(" %d ", RCr[compteur_Cr]);
        huffman = huffman_table_get_path_M(p_main->DC_Cb, RCr[0], nb_bits);
        printf("huffman path : %d   nb_bits : %u\n", huffman, *nb_bits);      
        printf("\n");
        printf(" AC : \n");
        for (int i=1; i<64; i++){
            if(p_main->bloc[mcu_i][2][i] != 0){
                while(RCr[compteur_Cr] == 0xF0){
                    printf("Ecriture de 0xF0\n");
                    compteur_Cr ++;
                }
                printf(" value: ");
                printf(" %d ", p_main->bloc[mcu_i][2][i]);
                printf(" magnitude: ");
                printf(" %d ", magnitude_table(p_main->bloc[mcu_i][2][i]));
                printf(" index: ");
                printf(" %d ", index(p_main->bloc[mcu_i][2][i]));
                printf(" rle: ");
                printf(" %d ", RCr[compteur_Cr]);
                uint32_t huffman = huffman_table_get_path_M(p_main->AC_Cb, RCr[compteur_Cr], nb_bits);
                printf("huffman path : %d   nb_bits : %u\n", huffman, *nb_bits);      
                printf("\n");
                compteur_Cr ++;
            }
        }
        if(compteur_Cr == *taille_Cr-1){
            printf("EndOfBlock : %d\n", RCr[compteur_Cr]);
        }
        else if(compteur_Cr < *taille_Cr){
            printf("erreur dans le compte\n");
        }
        printf("\n\n\n\n");
    }
}