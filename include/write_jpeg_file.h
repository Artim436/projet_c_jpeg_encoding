#ifndef _WRITE_JPEG_FILE_H
#define _WRITE_JPEG_FILE_H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <jpeg_writer.h>
#include <bitstream.h>


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
           uint8_t *qtable);

#endif