#ifndef _WRITE_JPEG_FILE_H
#define _WRITE_JPEG_FILE_H
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
#include <MCU.h>

extern void write_jpeg_Y(struct main_mcu *p_main);

#endif