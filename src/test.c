#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lecture_pgm.h>
#include <MCU.h>
#include <convert_to_YCbCr.h>
#include <structure.h>



int main (int argc, char **argv) {
    if (argc == 2) {
        struct image_pgm *pgm = malloc(sizeof(struct image_pgm)); //Allocation de la mémoire pour la structure image_pgm
        struct main_mcu *mcu = malloc(sizeof(struct main_mcu)); //On alloue la mémoire pour la structure main_mcu
        mcu->ppm_filename = argv[1]; //We store the name of the file in the structure main_mcu
        process_file(pgm, mcu->ppm_filename); //We call the function process_file to read the file
        affiche_details_image(pgm, file_name); // On affiche les données de l'image
        decoupe_mcu_8x8(mcu); //On découpe l'image en blocs de 8x8
        affiche_img_mcu(img_mcu);
        struct image_YCbCr *p_ycbcr = convert_YCbCr(img_mcu);
        afficher_YCbCr(p_ycbcr);
    } else {
        printf("Il faut passer en paramètre le nom d'un fichier image valide \n");
    }
}