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
#include <nettoyage.h>



int main (int argc, char **argv) {
    if (argc == 2) {
        //On commence par créer les structures:
        struct main_mcu *mcu = malloc(sizeof(struct main_mcu)); //On alloue la mémoire pour la structure main_mcu
        mcu->jpeg_filename = "mon_test.jpg";
        mcu->ppm_filename = argv[1]; //We store the name of the file in the structure main_mcu
        struct main_mcu_rgb* mcu_rgb = malloc(sizeof(struct main_mcu_rgb));
        mcu_rgb->jpeg_filename = "mon_test.jpg";
        mcu_rgb->ppm_filename = argv[1]; //We store the name of the file in the structure main_mcu

        process_file(mcu->ppm_filename, mcu, mcu_rgb); //We call the function process_file to read the file
        if(mcu->type_pgm[1] == '5'){
            //Cas où le fichier est en noir et blanc
            affiche_details_image(mcu, mcu->ppm_filename); // On affiche les données de l'image
            //printf("hauteur :  %u    largeur : %u\n",mcu->height, mcu->width );
            struct image_mcu *img_mcu = decoupe_mcu_8x8(mcu); //On découpe l'image en blocs de 8x8
            //affiche_img_mcu(img_mcu);
            //printf("%u\n", img_mcu->nmcu);
            struct image_YCbCr *p_ycbcr = convert_YCbCr(img_mcu);
            //afficher_YCbCr(p_ycbcr);
            printf("\n on est l38\n");
            fonction(mcu, p_ycbcr);
            clean_image_mcu(img_mcu);
            clean_image_YCbCr(p_ycbcr);
            printf("\n on est l42\n");
            printf("nmcu = %u\n", mcu->n_mcu);
            //affiche_bloc(mcu);
            printf("On rentre dans write_jpeg : \n");
            write_jpeg_Y(mcu);
            clean_main_mcu(mcu);
        }
        else{
            //Cas où le fichier est en noir et blanc
            //affiche_details_image_rgb(mcu_rgb, mcu_rgb->ppm_filename); // On affiche les données de l'image
            struct image_mcu_rgb *img_mcu_rgb = decoupe_mcu_8x8_rgb(mcu_rgb); //On découpe l'image en blocs de 8x8
            //affiche_img_mcu_rgb(img_mcu_rgb);
            //printf("%u\n", img_mcu_rgb->nmcu);
             struct image_YCbCr *p_ycbcr = convert_YCbCr_RGB(img_mcu_rgb);
            //afficher_YCbCr_rgb(p_ycbcr);
            fonction_RGB(mcu_rgb, p_ycbcr);
            //printf("On est sortie de fonction_RGB\n");
            // //clean_image_mcu(img_mcu);
            // clean_image_YCbCr(p_ycbcr);
            // printf(" on est l61\n");
            //affiche_bloc_rgb(mcu_rgb);
            // printf("On rentre dans write_jpeg : \n");
            write_jpeg_Y_RGB(mcu_rgb);
        }
    } else {
        printf("Il faut passer en paramètre le nom d'un fichier image valide \n");
    }
}