#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <write_jpeg_file.h> 
#include <bitstream_maison.h>
#include <structure.h>
#include <qtables.h>
#include <encoding.h>
#include <MCU.h>
#include <jpeg_writer_maison.h> 
#include <bitstream_maison.h>

void write_jpeg_Y(struct main_mcu *p_main){
    struct jpeg_M *p_jpeg = jpeg_create_M();
    //On rentre les paramètre dans la structure entete set_XXX
    // Caractèristique de l'image à traiter
    jpeg_set_ppm_filename_M(p_jpeg, p_main->ppm_filename);
    jpeg_set_image_height_M(p_jpeg, p_main->height);
    jpeg_set_image_width_M(p_jpeg, p_main->width);
    jpeg_set_nb_components_M(p_jpeg, 1);
    
    // Paramètre de l'encodage
    jpeg_set_jpeg_filename_M(p_jpeg, p_main->jpeg_filename);
    jpeg_set_sampling_factor_M(p_jpeg, Y, H, 1);
    jpeg_set_sampling_factor_M(p_jpeg, Cb, H, 1);
    jpeg_set_sampling_factor_M(p_jpeg, Cr, H, 1);
    jpeg_set_sampling_factor_M(p_jpeg, Y, V, 1);
    jpeg_set_sampling_factor_M(p_jpeg, Cb, V, 1);
    jpeg_set_sampling_factor_M(p_jpeg, Cr, V, 1);

    //Par convention, on fait l'encodage dès maintenant car on crée les tables de huffman dans cette fonction
    creation_table(p_main);

    //Table de Huffman
    jpeg_set_huffman_table_M(p_jpeg, DC, Y, p_main->DC_Y);
    jpeg_set_huffman_table_M(p_jpeg, AC, Y, p_main->AC_Y);
    /*jpeg_set_huffman_table(p_jpeg, DC, Cb, p_main->htable[2]);
    jpeg_set_huffman_table(p_jpeg, AC, Cb, p_main->htable[3]);
    jpeg_set_huffman_table(p_jpeg, DC, Cr, p_main->htable[2]);
    jpeg_set_huffman_table(p_jpeg, AC, Cr, p_main->htable[3]);*/
    
    //Table de quantification
    jpeg_set_quantization_table_M(p_jpeg, Y, quantification_table_Y);
    /*jpeg_set_quantization_table(p_jpeg, Cb, quantification_table_CbCr);
    jpeg_set_quantization_table(p_jpeg, Cb, quantification_table_CbCr);
    */

    //On écrit l'entete dans le fichier

    jpeg_write_header_M(p_jpeg);
    //printf("Header done\n");
    //On récupère le bitstream positioné à la fin du header
    p_main->blitzstream = bitstream_create_M(p_jpeg->jpeg_filename);
    encodage_Y(p_main);
    //affichage_encodage(p_main);
    //On écrit dans le bitstream sur toutes les valeurs necéssaires
    bitstream_flush_M(p_main->blitzstream);

    //On écrit le footer
    //jpeg_write_footer_M(p_jpeg);
    jpeg_destroy_M(p_jpeg);
}


void write_jpeg_Y_RGB(struct main_mcu_rgb *p_main_rgb){
    struct jpeg_M *p_jpeg = jpeg_create_M();
    //On rentre les paramètre dans la structure entete set_XXX
    // Caractèristique de l'image à traiter
    jpeg_set_ppm_filename_M(p_jpeg, p_main_rgb->ppm_filename);
    jpeg_set_image_height_M(p_jpeg, p_main_rgb->height);
    jpeg_set_image_width_M(p_jpeg, p_main_rgb->width);
    jpeg_set_nb_components_M(p_jpeg, 3);
    
    // Paramètre de l'encodage
    jpeg_set_jpeg_filename_M(p_jpeg, p_main_rgb->jpeg_filename);
    jpeg_set_sampling_factor_M(p_jpeg, Y, H, 1);
    jpeg_set_sampling_factor_M(p_jpeg, Cb, H, 1);
    jpeg_set_sampling_factor_M(p_jpeg, Cr, H, 1);
    jpeg_set_sampling_factor_M(p_jpeg, Y, V, 1);
    jpeg_set_sampling_factor_M(p_jpeg, Cb, V, 1);
    jpeg_set_sampling_factor_M(p_jpeg, Cr, V, 1);

    //Par convention, on fait l'encodage dès maintenant car on crée les tables de huffman dans cette fonction
    creation_table_rgb(p_main_rgb);

    //Table de Huffman
    jpeg_set_huffman_table_M(p_jpeg, DC, Y, p_main_rgb->DC_Y);
    jpeg_set_huffman_table_M(p_jpeg, AC, Y, p_main_rgb->AC_Y);
    jpeg_set_huffman_table_M(p_jpeg, DC, Cb, p_main_rgb->DC_Cb);
    jpeg_set_huffman_table_M(p_jpeg, AC, Cb, p_main_rgb->AC_Cb);
    jpeg_set_huffman_table_M(p_jpeg, DC, Cr, p_main_rgb->DC_Cb);
    jpeg_set_huffman_table_M(p_jpeg, AC, Cr, p_main_rgb->AC_Cb);
    printf("Huffman done\n");
    
    //Table de quantification
    jpeg_set_quantization_table_M(p_jpeg, Y, quantification_table_Y);
    jpeg_set_quantization_table_M(p_jpeg, Cb, quantification_table_CbCr);
    jpeg_set_quantization_table_M(p_jpeg, Cr, quantification_table_CbCr);
    printf("Quantification done\n");

    //On écrit l'entete dans le fichier

    jpeg_write_header_M(p_jpeg);
    printf("Header done\n");
    //On récupère le bitstream positioné à la fin du header
    p_main_rgb->blitzstream = bitstream_create_M(p_jpeg->jpeg_filename);
    printf("Bitstream done\n");
    encodage_Y_rgb_2(p_main_rgb);
    printf("Encodage done\n");
    //affiche_encodage_rgb(p_main_rgb);
    //On écrit dans le bitstream sur toutes les valeurs necéssaires
    bitstream_flush_M(p_main_rgb->blitzstream);

    //On écrit le footer
    //jpeg_write_footer_M(p_jpeg);
    jpeg_destroy_M(p_jpeg);
}
