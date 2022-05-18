
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <lecture_pgm.h>
#include <MCU.h>
#include <convert_to_YCbCr.h>
#include <math.h>
#include <structure.h>
#include <bitstream_maison.h>
#include <math.h>


struct write_uint8 {
    uint8_t w8;
};



struct bitstream_M *bitstream_create_M(const char*filename) {
    struct bitstream_M * bs = malloc(sizeof(struct bitstream_M));
    FILE *file = fopen(filename, "ab");
    bs->file = file;
    bs->buffer = 0;
    bs->nb_bits = 0;
    bs->flush_octet = 0;
    return bs;
}


void bitstream_write_bits_M(struct bitstream_M *stream,
                                 uint32_t value,
                                 uint8_t nb_bits,
                                 bool is_marker) 
{
    uint8_t byte_stuffing = 0x00;
    uint8_t big_nb = nb_bits + stream->nb_bits;
    uint32_t buffer32 = (uint32_t) stream->buffer;
    uint64_t big_buffer = (buffer32 << nb_bits) + value;
    uint64_t left_big_buffer = big_buffer<<(64-big_nb);
    while (big_nb >= 8) {
        uint64_t tmp_bb = left_big_buffer;
        uint8_t write_octet = tmp_bb>>56;
        fwrite(&write_octet, sizeof(uint8_t), 1, stream->file);
        if (!is_marker && write_octet == 0xFF) {
            fwrite(&byte_stuffing, sizeof(uint8_t), 1, stream->file);
        }
        left_big_buffer= left_big_buffer << 8;
        big_nb -=8;
    }
    stream->buffer = left_big_buffer>>(64-big_nb);
    stream->flush_octet = left_big_buffer>>(56);
    stream->nb_bits = big_nb;
}

void bitstream_flush_M(struct bitstream_M *stream) {
    if (stream->nb_bits !=0) {
        fwrite(&(stream->flush_octet), sizeof(uint8_t), 1, stream->file);
        stream->buffer = 0x00;
        stream->nb_bits = 0;
        stream->flush_octet = 0x00;

    }
}

void bitstream_destroy_M(struct bitstream_M *stream) {
    fclose(stream->file);
    free(stream);
}