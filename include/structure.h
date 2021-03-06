
#ifndef _STRUCTURE_H
#define _STRUCTURE_H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <jpeg_writer.h>
#include <bitstream.h>



// Structure de donnée main

struct main_mcu {
    uint8_t** data; // On stock les données de l'image
    int16_t **bloc; //Liste contenants les pointeurs vers les blocs
    uint32_t n_mcu;//taille de la liste bloc
    uint32_t width; // Largeur de l'image d'entrée
    uint32_t height; // Hauteur de l'image d'entrée
    uint32_t max_value; // Valeur maximal des nuances RGB
    char type_pgm[3]; //Type de l'image d'entrée : permet de définir nb de components
    const char *ppm_filename; // nom du fichier d'entrée
    const char *jpeg_filename; // Nom du fichier de sortie
    struct huff_table **htable;
    struct bitstream *blitzstream;
};

struct bloc_8x8_dtc {
    float matrix_bloc[8][8]; //mettre peut etre des floats pour les operations dct
};


struct image_mcu{
    /*Structure des mcu*/
    uint8_t **l_mcu;
    uint32_t nmcu;//Pour la largeur comme pour la hauteur on renverra la taille par pixel et non par MCU
    uint8_t dev_width;//En soit pas utile pour la structure mais nécessaire pour la construction, il s'agit du reste modulo8 des dimensions de image pgm
    uint8_t dev_height;
};



struct image_YCbCr{
    /*Structure des YCbCr, ressemble beaucoup aux structures différentes mais nécessaires pour la manipulation.*/
    float **l_ycbcr;
    uint32_t nmcu;//Equivalent aux nombre de MCU
};

struct rgb{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

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

#endif