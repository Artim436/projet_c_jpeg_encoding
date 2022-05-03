#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lecture_pgm.h>
#include <MCU.h>


void affiche_img_mcu(struct image_mcu *p_gmu){
    for(uint8_t i = 0; i< p_gmu->nmcu; i++){
        for (uint8_t j_i = 0; j_i < 8; j_i++){
            for (uint8_t j_j = 0; j_j < 8; j_j++){
                printf("%u", p_gmu->l_mcu[i][j_j + 8 * j_i]);
            }
            printf("\n");
        }
    }
}
int main (int argc, char **argv) {
    if (argc == 2) {
        struct image_pgm *pgm = malloc(sizeof(struct image_pgm));
        char *file_name = argv[1];
        process_file(pgm, file_name);
        affiche_details_image(pgm, file_name);
        printf("On est ici\n");
        struct image_mcu *img_mcu = decoupe_mcu_8x8(pgm);
        printf("On sort de image_mcu\n");
        affiche_img_mcu(img_mcu);
    } else {
        printf("Il faut passer en paramètre le nom d'un fichier image valide \n");
    }
}