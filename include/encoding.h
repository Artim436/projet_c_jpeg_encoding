#ifndef _ENCODING_H
#define _ENCODING_H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <jpeg_writer.h>
#include <bitstream.h>
#include <huffman.h>
#include <structure.h>

extern void creation_table(struct main_mcu *mcu);

extern void encodage_Y(struct main_mcu *p_main);

#endif