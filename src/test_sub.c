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
        struct main_mcu_rgb_sub* mcu_rgb = malloc(sizeof(struct main_mcu_rgb_sub));
        mcu_rgb->jpeg_filename = "mon_test.jpg";
        mcu_rgb->ppm_filename = argv[1]; //We store the name of the file in the structure main_mcu
        mcu_rgb->sampling_factor[0] = 2;
        mcu_rgb->sampling_factor[1] = 2;
        mcu_rgb->sampling_factor[2] = 2;
        mcu_rgb->sampling_factor[3] = 1;
        mcu_rgb->sampling_factor[4] = 2;
        mcu_rgb->sampling_factor[5] = 1;


        process_file_sub(mcu_rgb->ppm_filename, mcu_rgb); //We call the function process_file to read the file
        //Cas où le fichier est en noir et blanc
        //affiche_details_image_rgb_sub(mcu_rgb); // On affiche les données de l'image
        // printf("hauteur :  %u    largeur : %u\n",mcu_rgb->height, mcu_rgb->width );
        struct image_mcu_rgb_sub *img_mcu = decoupe_mcu_rgb_sub(mcu_rgb); //On découpe l'image en blocs de 8x8

        //affiche_img_mcu_rgb_sub(img_mcu, 2,2);
        //printf("%u\n", img_mcu->nmcu);
        struct image_YCbCr_sub *p_ycbcr = convert_YCbCr_RGB_sub(img_mcu, mcu_rgb);

        afficher_YCbCr_sub(p_ycbcr);

        fonction_rgb_sub(mcu_rgb, p_ycbcr);
        //clean_image_mcu(img_mcu);
        //clean_image_YCbCr(p_ycbcr);
       
        //affiche_bloc_rgb_sub(mcu_rgb);

        write_jpeg_rgb_sub(mcu_rgb);
        //clean_main_mcu(mcu);
}