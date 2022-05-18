
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
#include <jpeg_writer_maison.h>
#include <test.h>
#include <huffman_maison.h>
#include <encoding.h>
#include <math.h>
#include <write_jpeg_file.h>
#include <jpeg_writer_maison.h>
#include <huffman_maison.h>

struct jpeg_M *jpeg_create_M(void) {
    struct jpeg_M * jpeg = calloc(1, sizeof(struct jpeg_M));
    return jpeg;
}

void jpeg_set_ppm_filename_M(struct jpeg_M *jpg,
                                  const char *ppm_filename)
{
    jpg->ppm_filename = ppm_filename;
}

void jpeg_set_jpeg_filename_M(struct jpeg_M *jpg,
                                  const char *jpeg_filename)
{
    jpg->jpeg_filename = jpeg_filename;
}

void jpeg_set_image_height_M(struct jpeg_M *jpg,
                                  uint32_t image_height)
{
   jpg->height = image_height;
}

void jpeg_set_image_width_M(struct jpeg_M *jpg,
                                  uint32_t image_width)
{
   jpg->width = image_width;
}

void jpeg_set_nb_components_M(struct jpeg_M *jpg,
                                   uint8_t nb_components) 
{
    jpg->nb_component = nb_components;
}

void jpeg_set_quantization_table_M(struct jpeg_M * jpg, enum color_component cc, uint8_t *qtable) {
    jpg->cc = cc;
    jpg->qtable = qtable;
}
void jpeg_set_sampling_factor_M(struct jpeg_M *jpg,
                                     enum color_component cc,
                                     enum direction dir,
                                     uint8_t sampling_factor)
{
    struct sampling_factor s_f;
    if (dir == H) {
        if (cc == Y) {
            s_f.H_Y = sampling_factor;
        } else if (cc==Cb){
            s_f.H_Cb = sampling_factor;
        } else if (cc==Cr) {
            s_f.H_Cr = sampling_factor;
        }
    } else if (dir == V) {
        if (cc == Y) {
            s_f.V_Y = sampling_factor;
        } else if (cc==Cb){
            s_f.V_Cb = sampling_factor;
        } else if (cc==Cr) {
            s_f.V_Cr = sampling_factor;
        }
    }
    jpg->ssp = s_f;
}


void jpeg_set_huffman_table_M(struct jpeg_M *jpg,
                                   enum sample_type acdc,
                                   enum color_component cc,
                                   struct huff_table_M *htable)
{
    if (acdc == DC) {
        if (cc == Y) {
           jpg->DC_Y = htable; 
        } else if (cc == Cb) {
           jpg->DC_Cb = htable; 
        } else if (cc == Cr) {
            jpg->DC_Cr = htable;
        }
    } else if (acdc = AC) {
        if (cc == Y) {
           jpg->AC_Y = htable; 
        } else if (cc == Cb) {
           jpg->AC_Cb = htable; 
        } else if (cc == Cr) {
            jpg->AC_Cr = htable;
        }
    }
}                


void jpeg_write_header_M(struct jpeg_M *jpg) {
    //Ouverture fichier
    FILE * file = fopen(jpg->jpeg_filename, "wb");
    //SET SOI
    jpg->SOI = 0xd8ff;
    //SET EOI
    jpg->EOI = 0xd9ff;
    //SET APPX
    jpg->APPx = calloc(18, sizeof(uint8_t*));
    jpg->APPx[0] = 0xff; //Marqueur APO
    jpg->APPx[1] = 0xe0; //Marqueur APO
    jpg->APPx[2] = 0b0000; //Longueur de la section
    jpg->APPx[3] = 0b10000; //verifier l'ordre de l'écriture, longueur de la section
    jpg->APPx[4] = 'J'; // JIFI
    jpg->APPx[5] = 'F'; // JIFI
    jpg->APPx[6] = 'I'; // JIFI
    jpg->APPx[7] = 'F'; // JIFI
    jpg->APPx[8] = '\0'; // JIFI
    jpg->APPx[9] =1; // version
    jpg->APPx[10] =1; // version
    for (uint8_t i=11; i<18; i++) { //Données spécifiques
        jpg->APPx[i] = 0;
    }
    //SET COM
    uint16_t com1[] = {0xfeff, 0x1000};
    char *com2 = "<3 le projet C";
    //SET DQT
    jpg->DQT = calloc(5, sizeof(uint8_t*));
    jpg->DQT[0] = 0xff; //Marqueur
    jpg->DQT[1] = 0xdb; // Marqueur DQT
    jpg->DQT[2] = 0x0; // taille 1
    jpg->DQT[3] = 0x43; // taille 2
    jpg->DQT[4] = 0x0 + (jpg->cc)>>4; 
    //SET SOFx
    uint8_t tab[19];
    tab[0] = 0xff; //Marqueur
    tab[1] = 0xc0; // Marqueur SOFx
    tab[2] = 0x0; // Longueur
    tab[3] = 10 + 3*jpg->nb_component -2 ; // Longueur
    tab[4] = 0x8;
    tab[5] = (jpg->height)>>8; // Hauteur
    uint16_t tmp = jpg->height <<8;
    tab[6] = tmp>>8; // Hauteur
    tab[7] = jpg->width>>8; // Largeur
    uint16_t tmp1 = jpg->width <<8;
    tab[8] = tmp1>>8; // Largeur
    tab[9] = jpg->nb_component;
    tab[10] = 0x1;
    uint8_t tmp4 = jpg->ssp.H_Y <<4;
    tab[11] = tmp4+ jpg->ssp.V_Y;
    tab[12] = 0;
    if (jpg->nb_component == 3) {
        tab[13] = 0x2;
        tmp4 = jpg->ssp.H_Cb <<4;
        tab[14] = tmp4 +jpg->ssp.V_Cb;
        tab[15] = 0x1;
        tab[16] = 0x3;
        tmp4 = jpg->ssp.H_Cr << 4;
        tab[17] = tmp4 +jpg->ssp.V_Cr ;
        tab[18] = 0x1;
    }
    //Ecriture fichier SOI, APPX DQT SOFX
    fwrite(&(jpg->SOI), sizeof(uint16_t), 1, file); //SOI
    fwrite(&(jpg->APPx[0]), sizeof(uint8_t), 18, file); //APPX
    fwrite(com1, sizeof(uint16_t), 2, file); //COM
    fwrite(com2, 14*sizeof(uint8_t), 1, file); //COM
    fwrite(&(jpg->DQT[0]), sizeof(uint8_t), 5, file); //DQT
    fwrite(&((jpg->qtable)[0]), sizeof(uint8_t), 64, file); //DQT
    if (jpg->nb_component == 1) {
        fwrite(tab, sizeof(uint8_t), 13, file); //SOFx
    } else if (jpg->nb_component == 3){
        fwrite(tab, sizeof(uint8_t), 19, file); //SOFx
    }

    //DC_Y
    uint32_t l_DC_Y = (17 + jpg->DC_Y->nb_symbols + 2)<<8;
    uint16_t m_DC_Y = 0xc4ff;
    fwrite(&m_DC_Y, sizeof(uint16_t), 1, file);
    fwrite(&l_DC_Y, sizeof(uint16_t), 1, file);
    uint8_t inf0_DC_Y = 0x0;
    fwrite(&inf0_DC_Y, sizeof(uint8_t), 1, file);
    fwrite(jpg->DC_Y->nb_symb_per_lengths, sizeof(uint8_t), 16, file);
    fwrite(jpg->DC_Y->symbols, sizeof(uint8_t), jpg->DC_Y->nb_symbols, file);

    //AC_Y
    uint32_t l_AC_Y = (17 + jpg->AC_Y->nb_symbols + 2)<<8;
    uint16_t m_AC_Y = 0xc4ff;
    fwrite(&m_AC_Y, sizeof(uint16_t), 1, file);
    fwrite(&l_AC_Y, sizeof(uint16_t), 1, file);
    uint8_t inf0_AC_Y = 0x10;
    fwrite(&inf0_AC_Y, sizeof(uint8_t), 1, file);
    fwrite(jpg->AC_Y->nb_symb_per_lengths, sizeof(uint8_t), 16, file);
    fwrite(jpg->AC_Y->symbols, sizeof(uint8_t), jpg->AC_Y->nb_symbols, file);
    // // A FAIRE CB CR

    //SOS 
    uint16_t m_SOS = 0xdaff;
    fwrite(&m_SOS, sizeof(uint16_t), 1, file);
    uint16_t l_SOS= (2*jpg->nb_component + 6)<<8;
    fwrite(&l_SOS, sizeof(uint16_t), 1, file);
    printf("zebi \n");
    fwrite(&(jpg->nb_component), sizeof(uint8_t), 1, file);
    //Y
    uint8_t i_C = 1;
    uint8_t i_H = 0;
    uint8_t Ss = 0;
    uint8_t Se = 63;
    uint8_t AhAl = 0;
    fwrite(&i_C, sizeof(uint8_t), 1, file);
    fwrite(&i_H, sizeof(uint8_t), 1, file);
    if (jpg->nb_component == 3) {
        i_C = 2;
        i_H = 11;
        fwrite(&i_C, sizeof(uint8_t), 1, file);
        fwrite(&i_H, sizeof(uint8_t), 1, file);
        i_C = 3;
        i_H = 11;
        fwrite(&i_C, sizeof(uint8_t), 1, file);
        fwrite(&i_H, sizeof(uint8_t), 1, file);
    }
    fwrite(&Ss, sizeof(uint8_t), 1, file);
    fwrite(&Se, sizeof(uint8_t), 1, file);
    fwrite(&AhAl, sizeof(uint8_t), 1, file);



    fclose(file);
}

void jpeg_write_footer_M(struct jpeg_M *jpg) {
    FILE * file =fopen(jpg->jpeg_filename, "ab");
    uint16_t footer = 0xd9ff;
    fwrite(&footer, sizeof(uint16_t), 1, file);
    fclose(file);
}

void jpeg_destroy_M(struct jpeg_M *jpg) {
    free(jpg);
}
