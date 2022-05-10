#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <lecture_pgm.h>
#include <structure.h>
//#CLEANED
void process_file(const char *file_name, struct  main_mcu *mcu) {
    //Ouvre le fichier en mode lecture binaire
    FILE *file = fopen(file_name, "rb");
    //Lecture du type de fichier
    fscanf(file, "%s", mcu->type_pgm);
    printf("%s \n", mcu->type_pgm);
    //Lecture des dimensions de l'image
    fscanf(file, "%d %d", &(mcu->width), &(mcu->height));
    //Lecture de la valeur maximal du gris
    fscanf(file, "%d", &(mcu->max_value));
    //Allocation de la mémoire pour la data
    mcu->data = calloc(mcu->height, sizeof(char*)); //CLEANED
    //Stockage des pixels
    if (mcu->type_pgm[1]== '5') {
        fgetc(file);
        for (uint32_t i=0; i<mcu->height; i++) {
            mcu->data[i] = calloc(mcu->width, sizeof(char)); //CLEANED
            //Ecrit les données sur la mémoire allouée
            fread(mcu->data[i], sizeof(uint8_t), mcu->width, file);
        }
    } else if (mcu->type_pgm[1] == '6') {
        fgetc(file);
        for (uint32_t i=0; i<mcu->height; i++) {
            mcu->data[i] = calloc(mcu->width, sizeof(char)); //CLEANED
            for (uint32_t j=0; j<mcu->width; j++) {
                struct rgb *rgb_tpm = malloc(sizeof(struct rgb));
                fread(rgb_tpm->R, sizeof(uint8_t), 1, file);
                fread(rgb_tpm->G, sizeof(uint8_t), 1, file);
                fread(rgb_tpm->B, sizeof(uint8_t), 1, file);
                mcu->data[i][j] = rgb_tpm;
            }

    }
    // } else if (mcu->type_pgm[1]== '2') {
    //     for (uint32_t i=0; i<mcu->height; i++) {
    //         char *tmp[100000000];
    //         mcu->data[i] = calloc(mcu->width, sizeof(char));
    //         fgets(tmp, 10000000, file);
    //     }
    }
    //Ferme le fichier
    fclose(file);
}

void affiche_details_image(struct main_mcu *mcu, const char * file_name) {
    FILE *file = fopen(file_name,  "rb");
    printf("Largeur : %d pixels \n", mcu->width);
    printf("Hauteur : %d pixels \n", mcu->height);
    printf("Max_valeur : %d pixels \n", mcu->max_value);
    for (uint32_t i=0; i<mcu->height; i++) {
        for (uint32_t j =0; j<mcu->width; j++) {
            printf("%x ", mcu->data[i][j]);
        }
        printf("\n");
    }
    fclose(file);
}

void affiche_details_image_rgb(struct main_mcu *mcu, const char * file_name) {
    FILE *file = fopen(file_name,  "rb");
    printf("Largeur : %d pixels \n", mcu->width);
    printf("Hauteur : %d pixels \n", mcu->height);
    printf("Max_valeur : %d pixels \n", mcu->max_value);
    for (uint32_t i=0; i<mcu->height; i++) {
        for (uint32_t j =0; j<mcu->width; j++) {
            printf("%x ", mcu->data[i][j]);
        }
        printf("\n");
    }
    fclose(file);
}