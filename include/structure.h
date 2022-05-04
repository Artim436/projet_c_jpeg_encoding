
#ifndef _STRUCTURE_H
#define _STRUCTURE_H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <jpeg_writer.h>
#include <bitstream.h>


//Structure des données récupérées du fichier binaire
// struct image_pgm ;
// struct image_pgm {
//     uint8_t** data; 
//     uint32_t width;
//     uint32_t height;
//     uint32_t max_value;
//     char type_pgm[3];
// };
// Structure de donnée main
struct main_mcu ;
struct main_mcu {
    uint8_t *bloc; //Liste contenants les pointeurs vers les blocs
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
    struct huff_table *htable;
    uint8_t *qtable;
};
//Structure d'un bloc mcu 8x8
struct bloc_8x8;
struct bloc_8x8 {
    float matrix_bloc[8][8]; //mettre peut etre des floats pour les operations dct
};
//Structure d'un vecteur bloc 64
struct bloc_64;
struct bloc_64 {
    float vector[64];
};

#endif