
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
    uint8_t sampling_factor; // Facteur d'échantillonage
    enum color_component cc; 
    enum direction dir;
    enum sample_type acdc;
    struct huff_table **htable;
    uint8_t *qtable;
    struct bitstream* blitztream;
};
//Structure d'un bloc mcu 8x8
struct bloc_8x8 {
    uint8_t matrix_bloc[8][8]; //mettre peut etre des floats pour les operations dct
};

struct bloc_8x8_dtc {
    float matrix_bloc[8][8]; //mettre peut etre des floats pour les operations dct
};
//Structure d'un vecteur bloc 64
struct bloc_64 {
    uint8_t vector[64];
};

struct bloc_64_dtc {
    float vector[64];
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

#endif