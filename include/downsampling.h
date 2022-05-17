#ifndef _ENCODING_H
#define _ENCODING_H
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

struct main_mcu_rgb_sub {
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

void verif_sample_factor(uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3);


# endif