
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <jpeg_writer.h>
#include <write_jpeg_file.h> 
#include <bitstream.h>
#include <strucure.h>

void write(const char *ppm_filename,
           uint32_t image_height,
           uint32_t image_width,
           uint8_t nb_components,
           const char *jpeg_filename,
           enum color_component cc,
           enum direction dir,
           uint8_t sampling_factor,
           enum sample_type acdc,
           struct huff_table *htable,
           uint8_t *qtable) {
    // On crée la structure JPEG
    struct jpeg *jpg = jpeg_create();

    //On rentre les paramètre dans la structure entete set_XXX
    // Caractèristique de l'image à traiter
    jpeg_set_ppm_filename(jpg, ppm_filename);
    jpeg_set_image_height(jpg, image_height);
    jpeg_set_image_width(jpg, image_width);
    jpeg_set_nb_components(jpg, nb_components);
    // Paramètre de l'encodage
    jpeg_set_jpeg_filename(jpg, jpeg_filename);
    jpeg_set_sampling_factor(jpg, cc, dir, sampling_factor);
    //Table de Huffman
    jpeg_set_huffman_table(jpg, acdc, cc, htable);
    //Table de quantification
    jpeg_set_quantization_table(jpg, cc, qtable);

    //On écrit l'entete dans le fichier
    jpeg_write_header(jpg);

    //On récupère le bitstream positioné à la fin du header
    struct bitstream * stream = jpeg_get_bitstream(jpg);
    //On écrit dans le bitstream sur toutes les valeurs necéssaires
    /////bitstream_write_bits(stream, value, nb_bits, is_marker);
    //On écrit dans le fichier meme si on à pas assez de bit
    bitstream_flush(stream);

    //On écrit le footer
    jpeg_write_footer(jpg);

    //On nettoie le mémoire du bitstream
    bitstream_destroy(stream);

    //On nettoie la mémoire jpg
    jpeg_destroy(jpg);
}


