
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
#include <huffman_maison.h>
#include <encoding.h>
#include <write_jpeg_file.h>
#include <nettoyage.h>
#include <bitstream_maison.h>
#include <time.h>
#include <jpeg_writer_maison.h>
#include <qtables.h>


// int main (int argc, char **argv) {
//     struct jpeg_M *jpg = jpeg_create_M();
//     jpeg_set_ppm_filename_M(jpg, "test.pmp");
//     jpeg_set_jpeg_filename_M(jpg, "writer_test.jpg");
//     jpeg_set_quantization_table_M(jpg, Y,quantification_table_Y);
//     jpeg_set_nb_components_M(jpg, 1);
//     jpeg_set_image_height_M(jpg, 8);
//     jpeg_set_image_width_M(jpg, 8);
//     jpeg_set_sampling_factor_M(jpg, Y, V, 1);
//     jpeg_set_sampling_factor_M(jpg, Cb, V, 1);
//     jpeg_set_sampling_factor_M(jpg, Cr, V, 1);
//     jpeg_set_sampling_factor_M(jpg, Y, H, 1);
//     jpeg_set_sampling_factor_M(jpg, Cb, H, 1);
//     jpeg_set_sampling_factor_M(jpg, Cr, H, 1);
//     struct huff_table_M *AC_Y = calloc(1, sizeof(struct huff_table_M));
//     AC_Y->nb_symb_per_lengths = htables_nb_symb_per_lengths[AC][Y];
//     AC_Y->symbols = htables_symbols[AC][Y];
//     AC_Y->nb_symbols = htables_nb_symbols[DC][Y];
//     printf("nb %d \n", AC_Y->nb_symbols);
//     AC_Y->nb_symbols = htables_nb_symbols[AC][Y];
//     jpeg_set_huffman_table_M(jpg, AC, Y, AC_Y);
//     struct huff_table_M *DC_Y = calloc(1, sizeof(struct huff_table_M));
//     DC_Y->nb_symb_per_lengths = htables_nb_symb_per_lengths[DC][Y];
//     DC_Y->symbols = htables_symbols[DC][Y];
//     DC_Y->nb_symbols = htables_nb_symbols[DC][Y];
//     jpeg_set_huffman_table_M(jpg, DC, Y, DC_Y);
//     jpeg_write_header_M(jpg);
//     // struct jpeg *jpgp = jpeg_create();
//     // jpeg_set_ppm_filename(jpgp, "testp.ppm");
//     // jpeg_set_jpeg_filename(jpgp, "writer_prof.jpg");
//     // jpeg_set_quantization_table(jpgp, Y,quantification_table_Y);
//     // jpeg_write_header(jpgp);
// }