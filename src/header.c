#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <jpeg_writer.h>
#include <header.h>

// On crée le header, il ne faut pas oublier ensuite d'écrire dans le flux et faire un closer
/*
struct jpeg write_header_file(
    const char* ppm_file,
    const char* jpeg_file,
    uint32_t image_height,
    uint32_t image_width,
    uint8_t nb_components,
    enum color_component cc,
    enum direction dir, 
    uint8_t sampling_factor,
    struct huff_table *htable,
    uint8_t *qtable) {
    struct jpeg *img_jpeg = jpeg_create();
    jpeg_set_jpeg_filename(struct jpeg *img_jpeg, const char *jpeg_file);
    jpeg_set_ppm_filename(struct jpeg *img_jpeg, const char *ppm_file);
    jpeg_set_image_height(struct jpeg *img_jpeg, uint32_t image_height);
    jpeg_set_image_width(struct jpeg *img_jpeg, uint32_t image_width);
    jpeg_set_nb_components(struct jpeg *img_jpeg, uint8_t nb_components);
    jpeg_set_sampling_factor(struct jpeg *img_jpeg, enum color_component cc, enum direction dir, uint8_t sampling factor);
    jpeg_set_huffman_table(struct jpeg *img_jpeg, enum sample_type adcd, enum color_component cc, struct huff_table *htable);
    jpeg_set_quantization_table(struct jpeg *img_jpeg, enum color_component cc, uint8_t *qtable)
}*/