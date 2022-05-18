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
#include <controle_arguments.h>



int main (int argc, char **argv) {
    /*La fonction main est celle qui contient le code de l'exécutable généré.
    Elle ne print rien mais est juste là pour encoder une image ppm P5 ou P6 en jpeg
    Des fonctions d'affichage sont présentes dans la fonction mais sont commentées, les décommenter peut aider à débugger
    
    Nous encodons les fichiers jpeg en suivant le mode baseline.
    Le cheminement est simple, nous créons une structure principale dans laquelle nous ajoutons au fur et à mesure du code
    tous les éléments nécessaires pour l'écriture du jpeg.*/
    if (argc >= 2) {
        /*Analyse des arguments */
        /*En sortie de cette fonction, nous avons un tableau de taille 3 dans lequel est stocké les positions de chaque argument
        (sample, outfile et fichier_ppm) dans le tableau argv*/
        uint8_t *pos_des_arg = controle_arg(argc, argv);

        //Création des structures principales
        struct main_mcu *mcu = malloc(sizeof(struct main_mcu)); //On alloue la mémoire pour la structure main_mcu
        mcu->ppm_filename = argv[pos_des_arg[2]]; //On récupère le nom du fichier .ppm
        struct main_mcu_rgb_sub* mcu_rgb = malloc(sizeof(struct main_mcu_rgb_sub));
        mcu_rgb->ppm_filename = argv[pos_des_arg[2]]; //On récupère le nom du fichier .ppm

        process_file(mcu->ppm_filename, mcu, mcu_rgb); //Lecture du fichier et récupération des données
        if(mcu->type_pgm[1] == '5'){
            /*Nous sommes rentrés dans le cas où nous avons un fichier P5 et donc des composantes en noir et blanc.*/

            //affiche_details_image(mcu, mcu->ppm_filename); // On affiche les données de l'image

            /*Analyse des arguments passés en lignes de commandes*/
            analyse_arg(pos_des_arg, mcu, argv);
            
            /*Découpage des données en MCU*/
            struct image_mcu *img_mcu = decoupe_mcu_8x8(mcu); //On découpe l'image en blocs de 8x8
            //affiche_img_mcu(img_mcu);
            
            /*Conversion des Données en Y*/
            struct image_YCbCr *p_ycbcr = convert_YCbCr(img_mcu);
            //afficher_YCbCr(p_ycbcr);
            
            
            /*Étapes de dct, zig-zag et quantification*/
            fonction(mcu, p_ycbcr);
            clean_image_mcu(img_mcu);
            clean_image_YCbCr(p_ycbcr);
            //affiche_bloc(mcu);

            write_jpeg_Y(mcu);
            clean_main_mcu(mcu);
        }
        else{
            analyse_arg_sub(pos_des_arg, mcu_rgb, argv);

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