#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lecture_pgm.h>
#include <MCU.h>
#include <structure.h>
#include <convert_to_YCbCr.h>
#include <dct.h>
#include <htables.h>
#include <huffman.h>
#include <encoding.h>
#include <write_jpeg_file.h>



int main (int argc, char **argv) {
    if (argc == 2) {
        struct main_mcu *mcu = malloc(sizeof(struct main_mcu)); //On alloue la mémoire pour la structure main_mcu
        mcu->jpeg_filename = "mon_test.jpeg";
        mcu->ppm_filename = argv[1]; //We store the name of the file in the structure main_mcu
        process_file(mcu->ppm_filename, mcu); //We call the function process_file to read the file
        affiche_details_image(mcu, mcu->ppm_filename); // On affiche les données de l'image
        struct image_mcu *img_mcu = decoupe_mcu_8x8(mcu); //On découpe l'image en blocs de 8x8
        affiche_img_mcu(img_mcu);
        struct image_YCbCr *p_ycbcr = convert_YCbCr(img_mcu);
        afficher_YCbCr(p_ycbcr);
        printf("\n on est l23\n");
        fonction(mcu, p_ycbcr);
        printf("\n on est l25\n");
        affiche_bloc(mcu);
        printf("On rentre dans write_jpeg : \n");
        write_jpeg_Y(mcu);
    } else {
        printf("Il faut passer en paramètre le nom d'un fichier image valide \n");
    }
}