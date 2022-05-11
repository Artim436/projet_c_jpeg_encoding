#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <lecture_pgm.h>
#include <structure.h>
//#CLEANED
void process_file(const char *file_name, struct  main_mcu *mcu, struct main_mcu_rgb* mcu_rgb) {
    //Ouvre le fichier en mode lecture binaire
    FILE *file = fopen(file_name, "rb");
    //Lecture du type de fichier
    fscanf(file, "%s", mcu->type_pgm);
    if(mcu->type_pgm[1] == '5'){
        //ici  : free mcu_rgb
        printf("%s \n", mcu->type_pgm);
        //Lecture des dimensions de l'image
        fscanf(file, "%d %d", &(mcu->width), &(mcu->height));
        //Lecture de la valeur maximal du gris
        fscanf(file, "%d", &(mcu->max_value));
        //Allocation de la mémoire pour la data
        mcu->data = calloc(mcu->height, sizeof(char*)); //CLEANED
        //Stockage des pixel
        fgetc(file);
        for (uint32_t i=0; i<mcu->height; i++) {
            mcu->data[i] = calloc(mcu->width, sizeof(char)); //CLEANED
            //Ecrit les données sur la mémoire allouée
            fread(mcu->data[i], sizeof(uint8_t), mcu->width, file);
        }
    }
    else if(mcu->type_pgm[1] == '6'){
        // ici : free mcu 
        *(mcu_rgb->type_pgm) = *(mcu->type_pgm);
        //Lecture des dimensions de l'image
        fscanf(file, "%d %d", &(mcu_rgb->width), &(mcu_rgb->height));
        //Lecture de la valeur maximal du gris
        fscanf(file, "%d", &(mcu_rgb->max_value));
        //Allocation de la mémoire pour la data
        mcu_rgb->data = calloc(mcu_rgb->height, sizeof(struct rgb* *)); //CLEANED
        //Stockage des pixel
        fgetc(file);
        for (uint32_t i=0; i<mcu_rgb->height; i++) {
            mcu_rgb->data[i] = calloc(mcu_rgb->width, sizeof(struct rgb*));
            for(uint32_t j=0; j<mcu_rgb->width; j ++){
                mcu_rgb->data[i][j] = calloc(1, sizeof(struct rgb));
                uint8_t *compo_rgb = calloc(3, sizeof(uint8_t));
                fread(compo_rgb, sizeof(uint8_t), 3, file);
                mcu_rgb->data[i][j]->R =compo_rgb[0];
                mcu_rgb->data[i][j]->G =compo_rgb[1];
                mcu_rgb->data[i][j]->B =compo_rgb[2];
            }
        }
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

void affiche_details_image_rgb(struct main_mcu_rgb *mcu, const char * file_name) {
    FILE *file = fopen(file_name,  "rb");
    printf("Largeur : %d pixels \n", mcu->width);
    printf("Hauteur : %d pixels \n", mcu->height);
    printf("Max_valeur : %d pixels \n", mcu->max_value);
    // for (uint32_t i=0; i<mcu->height; i++) {
    //     for (uint32_t j =0; j<mcu->width; j++) {
    //         printf("%x%x%x ", mcu->data[i][j]->R,mcu->data[i][j]->G ,mcu->data[i][j]->B );
    //     }
    //     printf("\n");
    // }
    fclose(file);
}