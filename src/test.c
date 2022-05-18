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
#include <downsampling.h>



int main (int argc, char **argv) {
    if (argc == 2) {
        //On commence par créer les structures:
        struct main_mcu *mcu = malloc(sizeof(struct main_mcu)); //On alloue la mémoire pour la structure main_mcu
        mcu->jpeg_filename = "mon_test.jpg";
        mcu->ppm_filename = argv[1]; //We store the name of the file in the structure main_mcu
        struct main_mcu_rgb_sub* mcu_rgb = malloc(sizeof(struct main_mcu_rgb_sub));
        mcu_rgb->jpeg_filename = "mon_test.jpg";
        mcu_rgb->ppm_filename = argv[1]; //We store the name of the file in the structure main_mcu

        process_file(mcu->ppm_filename, mcu, mcu_rgb); //We call the function process_file to read the file
        if(mcu->type_pgm[1] == '5'){
            //Cas où le fichier est en noir et blanc
            //affiche_details_image(mcu, mcu->ppm_filename); // On affiche les données de l'image
            //printf("hauteur :  %u    largeur : %u\n",mcu->height, mcu->width );
            struct image_mcu *img_mcu = decoupe_mcu_8x8(mcu); //On découpe l'image en blocs de 8x8
            //affiche_img_mcu(img_mcu);
            //printf("%u\n", img_mcu->nmcu);
            struct image_YCbCr *p_ycbcr = convert_YCbCr(img_mcu);
            //afficher_YCbCr(p_ycbcr);
            fonction(mcu, p_ycbcr);
            //clean_image_mcu(img_mcu);
            //clean_image_YCbCr(p_ycbcr);
            //affiche_bloc(mcu);
            write_jpeg_Y(mcu);
            //clean_main_mcu(mcu);
        }
        else{
            mcu_rgb->sampling_factor[0] = 2;
            mcu_rgb->sampling_factor[1] = 2;
            mcu_rgb->sampling_factor[2] = 1;
            mcu_rgb->sampling_factor[3] = 1;
            mcu_rgb->sampling_factor[4] = 1;
            mcu_rgb->sampling_factor[5] = 1;


            //Cas où le fichier est en noir et blanc
            //affiche_details_image_rgb_sub(mcu_rgb); // On affiche les données de l'image
            // printf("hauteur :  %u    largeur : %u\n",mcu_rgb->height, mcu_rgb->width );
            struct image_mcu_rgb_sub *img_mcu = decoupe_mcu_rgb_sub(mcu_rgb); //On découpe l'image en blocs de 8x8
            

            //affiche_img_mcu_rgb_sub(img_mcu, mcu_rgb->sampling_factor[0], mcu_rgb->sampling_factor[1]);
            struct image_YCbCr_sub *p_ycbcr = convert_YCbCr_RGB_sub(img_mcu, mcu_rgb);

            //afficher_YCbCr_sub(p_ycbcr);

            fonction_rgb_sub(mcu_rgb, p_ycbcr);


            clean_image_mcu_sub(img_mcu, mcu_rgb->sampling_factor[0], mcu_rgb->sampling_factor[1]);

            clean_image_YCbCr_sub(p_ycbcr);


            //affiche_bloc_rgb_sub(mcu_rgb);

            write_jpeg_rgb_sub(mcu_rgb);
            clean_main_mcu_sub(mcu_rgb);
        }
    } else {
        printf("Il faut passer en paramètre le nom d'un fichier image valide \n");
    }
}