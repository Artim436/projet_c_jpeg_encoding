#ifndef _LECTURE_PGM_H_
#define _LECTURE_PGM_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <structure.h>

struct image_pgm;

extern void process_file(struct image_pgm *pgm, const char *file_name);

extern void affiche_details_image(struct image_pgm *pgm, const char * file_name);


#endif
