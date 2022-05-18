
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <nettoyage.h>
#include <structure.h>

void clean_main_mcu(struct main_mcu *mcu) {
    // On nettoie data
    for (uint32_t i=0; i<mcu->height; i++) {
        free(mcu->data[i]);
    }
    // On nettoie bloc
    for (uint32_t i=0; i<mcu->n_mcu; i++) {
        free(mcu->bloc[i]);
    }
    // Ici on à nettoyé lecture_pgm.c
    free(mcu->data);
    free(mcu->bloc);
    free(mcu);
}

void clean_image_mcu(struct image_mcu *image) {
    for (uint32_t i=0; i<image->nmcu; i++) {
            free(image->l_mcu[i]);
    }
    free(image->l_mcu);
    free(image);
}

void clean_image_YCbCr(struct image_YCbCr *ycbcr) {
    for (uint32_t i = 0; i<ycbcr->nmcu; i++) {
        for (uint32_t j = 0; j<64; j++) {
            free(ycbcr->l_ycbcr[i][j]);
        }
        free(ycbcr->l_ycbcr[i]);
    }
    free(ycbcr->l_ycbcr);
    free(ycbcr);
}
