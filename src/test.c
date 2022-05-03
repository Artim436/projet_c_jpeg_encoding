#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lecture_pgm.h>
#include <MCU.h>


int main (int argc, char **argv) {
    if (argc == 2) {
        struct image_pgm *pgm = malloc(sizeof(struct image_pgm));
        char *file_name = argv[1];
        process_file(pgm, file_name);
        affiche_details_image(pgm, file_name);
        struct image_mcu *img_mcu = decoupe_mcu_8x8(pgm);
        affiche_img_mcu(img_mcu);
    } else {
        printf("Il faut passer en paramètre le nom d'un fichier image valide \n");
    }
}