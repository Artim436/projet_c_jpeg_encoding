#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lecture_pgm.h>
#include <MCU.h>



int main (int argc, char **argv) {
    if (argc == 2) {
        printf("On est ici");
        struct image_pgm *pgm = malloc(sizeof(struct image_pgm));
        char *file_name = argv[1];
        process_file(pgm, file_name);
        affiche_details_image(pgm, file_name);
        printf("On rentre dans mcu\n");
        struct image_mcu *img_mcu = decoupe_mcu_8x8(pgm);
        printf("On affiche image_mcu\n");
        affiche_img_mcu(img_mcu);
        //struct image_YCbCr *p_ycbcr = convert_YCbCr(img_mcu);
        printf("On est sortie de la création\n");
        //afficher_YCbCr(p_ycbcr);
    } else {
        printf("Il faut passer en paramètre le nom d'un fichier image valide \n");
    }
}