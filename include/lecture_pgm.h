#ifndef _LECTURE_PGM_H_
#define _LECTURE_PGM_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <structure.h>

extern void process_file(const char *file_name, struct main_mcu *mcu);

extern void affiche_details_image(struct main_mcu *mcu, const char * file_name);


#endif
