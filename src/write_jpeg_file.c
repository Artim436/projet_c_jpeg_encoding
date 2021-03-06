#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <jpeg_writer.h>
#include <write_jpeg_file.h> 
#include <bitstream.h>
#include <structure.h>
#include <qtables.h>
#include <encoding.h>

void write_jpeg_Y(struct main_mcu *p_main){
    struct jpeg *p_jpeg = jpeg_create();
    //On rentre les paramètre dans la structure entete set_XXX
    // Caractèristique de l'image à traiter
    jpeg_set_ppm_filename(p_jpeg, p_main->ppm_filename);


    jpeg_set_image_height(p_jpeg, p_main->height);
    jpeg_set_image_width(p_jpeg, p_main->width);
    jpeg_set_nb_components(p_jpeg, 1);
    
    // Paramètre de l'encodage
    jpeg_set_jpeg_filename(p_jpeg, p_main->jpeg_filename);

    jpeg_set_sampling_factor(p_jpeg, Y, H, 1);
    jpeg_set_sampling_factor(p_jpeg, Cb, H, 1);
    jpeg_set_sampling_factor(p_jpeg, Cr, H, 1);
    jpeg_set_sampling_factor(p_jpeg, Y, V, 1);
    jpeg_set_sampling_factor(p_jpeg, Cb, V, 1);
    jpeg_set_sampling_factor(p_jpeg, Cr, V, 1);

    //Par convention, on fait l'encodage dès maintenant car on crée les tables de huffman dans cette fonction
    creation_table(p_main);

    //Table de Huffman
    jpeg_set_huffman_table(p_jpeg, DC, Y, p_main->htable[0]);
    jpeg_set_huffman_table(p_jpeg, AC, Y, p_main->htable[1]);
    
    //Table de quantification
    jpeg_set_quantization_table(p_jpeg, Y, quantification_table_Y);


    //On écrit l'entete dans le fichier

    jpeg_write_header(p_jpeg);
    //On récupère le bitstream positioné à la fin du header
    p_main->blitzstream = jpeg_get_bitstream(p_jpeg);
    encodage_Y(p_main);
    //On écrit dans le bitstream sur toutes les valeurs necéssaires
    bitstream_flush(p_main->blitzstream);

    //On écrit le footer
    jpeg_write_footer(p_jpeg);
    jpeg_destroy(p_jpeg);
}

void write_jpeg_rgb_sub(struct main_mcu_rgb_sub *p_main){
    struct jpeg *p_jpeg = jpeg_create();
    //On rentre les paramètre dans la structure entete set_XXX
    // Caractèristique de l'image à traiter
    jpeg_set_ppm_filename(p_jpeg, p_main->ppm_filename);
    jpeg_set_image_height(p_jpeg, p_main->height);
    jpeg_set_image_width(p_jpeg, p_main->width);
    jpeg_set_nb_components(p_jpeg,3);
    
    // Paramètre de l'encodage
    jpeg_set_jpeg_filename(p_jpeg, p_main->jpeg_filename);
    jpeg_set_sampling_factor(p_jpeg, Y, H, p_main->sampling_factor[0]);
    jpeg_set_sampling_factor(p_jpeg, Cb, H, p_main->sampling_factor[2]);
    jpeg_set_sampling_factor(p_jpeg, Cr, H, p_main->sampling_factor[4]);
    jpeg_set_sampling_factor(p_jpeg, Y, V, p_main->sampling_factor[1]);
    jpeg_set_sampling_factor(p_jpeg, Cb, V, p_main->sampling_factor[3]);
    jpeg_set_sampling_factor(p_jpeg, Cr, V, p_main->sampling_factor[5]);

    //Par convention, on fait l'encodage dès maintenant car on crée les tables de huffman dans cette fonction
    creation_table_sub(p_main);

    //Table de Huffman
    jpeg_set_huffman_table(p_jpeg, DC, Y, p_main->htable[0]);
    jpeg_set_huffman_table(p_jpeg, AC, Y, p_main->htable[1]);
    jpeg_set_huffman_table(p_jpeg, DC, Cb, p_main->htable[2]);
    jpeg_set_huffman_table(p_jpeg, AC, Cb, p_main->htable[3]);
    jpeg_set_huffman_table(p_jpeg, DC, Cr, p_main->htable[2]);
    jpeg_set_huffman_table(p_jpeg, AC, Cr, p_main->htable[3]);
    
    //Table de quantification
    jpeg_set_quantization_table(p_jpeg, Y, quantification_table_Y);
    jpeg_set_quantization_table(p_jpeg, Cb, quantification_table_CbCr);
    jpeg_set_quantization_table(p_jpeg, Cr, quantification_table_CbCr);

    //On écrit l'entete dans le fichier

    jpeg_write_header(p_jpeg);
    //On récupère le bitstream positioné à la fin du header
    p_main->blitzstream = jpeg_get_bitstream(p_jpeg);

    encodage_rgb_sub(p_main);
    //affiche_encodage_rgb(p_main_rgb);
    //On écrit dans le bitstream sur toutes les valeurs necéssaires
    bitstream_flush(p_main->blitzstream);

    //On écrit le footer
    jpeg_write_footer(p_jpeg);
    jpeg_destroy(p_jpeg);
}


