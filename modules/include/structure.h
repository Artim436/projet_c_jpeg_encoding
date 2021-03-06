#ifndef _STRUCTURE_H
#define _STRUCTURE_H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <jpeg_writer_maison.h>
#include <bitstream_maison.h>



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
    uint8_t sampling_factor; // Facteur d'échantillonage
    struct huff_table_M *AC_Y;
    struct huff_table_M *AC_Cb;
    struct huff_table_M *AC_Cr;
    struct huff_table_M *DC_Y;
    struct huff_table_M *DC_Cb;
    struct huff_table_M *DC_Cr;
    uint8_t *qtable;
    struct bitstream_M *blitzstream;
};

struct bloc_8x8_dtc {
    float matrix_bloc[8][8]; //mettre peut etre des floats pour les operations dct
};


struct image_mcu{
    /*Structure des mcu*/
    char type_pgm[3];
    uint8_t **l_mcu;
    uint32_t nmcu;//Pour la largeur comme pour la hauteur on renverra la taille par pixel et non par MCU
    uint32_t max_value;
    uint8_t dev_width;//En soit pas utile pour la structure mais nécessaire pour la construction, il s'agit du reste modulo8 des dimensions de image pgm
    uint8_t dev_height;
};



struct YCbCr{
    /*Donnée contenant les valeurs de YCbCr d'un pixel*/
    uint32_t Y;
    uint32_t Cb;
    uint32_t Cr;
};


struct image_YCbCr{
    /*Structure des YCbCr, ressemble beaucoup aux structures différentes mais nécessaires pour la manipulation.*/
    char type_pgm[3];
    struct YCbCr ***l_ycbcr;
    uint32_t nmcu;//Equivalent aux nombre de MCU
};


struct main_mcu_rgb {
    struct rgb* **data; // On stock les données de l'image dans une nouvelle structure rgb
    int16_t ***bloc; //Liste contenants les pointeurs vers les blocs
    uint32_t n_mcu;//taille de la liste bloc
    uint32_t width; // Largeur de l'image d'entrée
    uint32_t height; // Hauteur de l'image d'entrée
    uint32_t max_value; // Valeur maximal des nuances RGB
    char type_pgm[3]; //Type de l'image d'entrée : permet de définir nb de components
    const char *ppm_filename; // nom du fichier d'entrée
    const char *jpeg_filename; // Nom du fichier de sortie
    uint8_t sampling_factor; // Facteur d'échantillonage
    uint8_t *qtable;
    struct bitstream_M *blitzstream;
    struct huff_table_M *AC_Y;
    struct huff_table_M *AC_Cb;
    struct huff_table_M *AC_Cr;
    struct huff_table_M *DC_Y;
    struct huff_table_M *DC_Cb;
    struct huff_table_M *DC_Cr;
};

struct rgb{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

struct image_mcu_rgb{
    /*Structure des mcu*/
    char type_pgm[3];
    struct rgb ***l_mcu;
    uint32_t nmcu;//Pour la largeur comme pour la hauteur on renverra la taille par pixel et non par MCU
    uint32_t max_value;
    uint8_t dev_width;//En soit pas utile pour la structure mais nécessaire pour la construction, il s'agit du reste modulo8 des dimensions de image pgm
    uint8_t dev_height;
};
/*
    Type opaque représentant le flux d'octets à écrire dans le fichier JPEG de
    sortie (appelé bitstream dans le sujet).
*/

struct bitstream_M {
    uint8_t buffer;
    uint8_t nb_bits;
    uint8_t flush_octet;
    FILE *file;
};
#endif
