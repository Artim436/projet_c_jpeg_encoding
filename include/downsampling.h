#ifndef _DOWNSAMPLING_H
#define _DOWNSAMPLING_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    
#include <structure.h>
#include <htables.h>
#include <huffman.h>
#include <math.h>
#include <dct.h>
#include <encoding.h>

struct image_mcu_rgb_sub{
    /*Structure des mcu*/
    struct rgb ***l_mcu;
    uint32_t nmcu;//Pour la largeur comme pour la hauteur on renverra la taille par pixel et non par MCU
    uint8_t dev_width;
    uint8_t dev_height;
};

struct image_YCbCr_sub{
    float ***bloc;
    uint32_t n_mcu;
    uint8_t sampling_factor[6];
    uint8_t nb_comp;
};

struct main_mcu_rgb_sub{
    struct rgb* **data; // On stock les données de l'image dans une nouvelle structure rgb
    int16_t ***bloc; //Liste contenants les pointeurs vers les blocs
    uint32_t n_mcu;//taille de la liste bloc
    uint32_t width; // Largeur de l'image d'entrée
    uint32_t height; // Hauteur de l'image d'entrée
    uint32_t max_value; // Valeur maximal des nuances RGB
    char type_pgm[3]; //Type de l'image d'entrée : permet de définir nb de components
    const char *ppm_filename; // nom du fichier d'entrée
    const char *jpeg_filename; // Nom du fichier de sortie
    uint8_t sampling_factor[6]; // Facteur d'échantillonage
    uint8_t nb_comp;
    struct huff_table **htable;
    struct bitstream *blitzstream;
};

extern void process_file_sub(const char *file_name, struct main_mcu_rgb_sub* mcu_rgb);

extern void verif_sample_factor(uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3);

extern struct image_mcu_rgb_sub *creation_mcu_rgb_sub(uint32_t width, uint32_t height, uint8_t h1, uint8_t v1);

extern struct image_mcu_rgb_sub *decoupe_mcu_rgb_sub(struct main_mcu_rgb_sub *p_main);

extern struct image_YCbCr_sub *convert_YCbCr_RGB_sub(struct image_mcu_rgb_sub *p_mcu , struct main_mcu_rgb_sub* p_main);

extern float **convert_mat_sub(float *p_YCbCr);

extern void fonction_rgb_sub(struct main_mcu_rgb_sub *p_main, struct image_YCbCr_sub *p_ycbcr);

extern void encodage_rgb_sub(struct main_mcu_rgb_sub *p_main);

extern void creation_table_sub(struct main_mcu_rgb_sub *mcu);

extern void write_jpeg_rgb_sub(struct main_mcu_rgb_sub *p_main);

extern void affiche_details_image_rgb_sub(struct main_mcu_rgb_sub *mcu);

extern void affiche_bloc_rgb_sub(struct main_mcu_rgb_sub *main_mcu);

extern void affiche_img_mcu_rgb_sub(struct image_mcu_rgb_sub *p_gmu, uint8_t h1, uint8_t v1);

extern void afficher_YCbCr_sub(struct image_YCbCr_sub *p_ycbcr);

extern void clean_main_mcu_sub(struct main_mcu_rgb_sub *mcu);

extern void clean_image_mcu_sub(struct image_mcu_rgb_sub *image, uint8_t h1, uint8_t v1);

extern void clean_image_YCbCr_sub(struct image_YCbCr_sub *ycbcr);

# endif