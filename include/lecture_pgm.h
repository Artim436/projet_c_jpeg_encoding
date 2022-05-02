#ifndef _LECTURE_PGM_H_
#define _LECTURE_PGM_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

struct image_pgm ;
struct image_pgm {
    uint8_t** data;
    uint32_t width;
    uint32_t height;
    uint32_t max_value;
    char type_pgm[3];
};

extern void process_file(struct image_pgm *pgm, const char *file_name);
extern void affiche_details_image(struct image_pgm *pgm, const char * file_name);


#endif
