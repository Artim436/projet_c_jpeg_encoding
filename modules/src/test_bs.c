
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


// int main (int argc, char **argv) {
//     srand(time(0));
//     struct bitstream_M* bm = bitstream_create_M("maison.txt");
//     struct bitstream* bp = bitstream_create("prof.txt");
//     for (uint32_t i=0; i<1000; i++) {
//         uint32_t value = rand()/70000;
//         uint32_t nb_bit = log2(value) + 1;
//         bitstream_write_bits(bp, value, nb_bit, true);
//         bitstream_write_bits_M(bm, value, nb_bit, true);
//     }
//     bitstream_flush_M(bm);
//     bitstream_flush(bp);
//     bitstream_destroy_M(bm);
//     bitstream_destroy(bp);
// }