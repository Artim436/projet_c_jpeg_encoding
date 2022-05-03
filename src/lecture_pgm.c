#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <lecture_pgm.h>

void process_file(struct image_pgm *pgm, const char *file_name) {
    //Ouvre le fichier en mode lecture binaire
    FILE *file = fopen(file_name, "rb");
    //Lecture du type de fichier
    fscanf(file, "%s", pgm->type_pgm);
    printf("%s \n", pgm->type_pgm);
    //Lecture des dimensions de l'image
    fscanf(file, "%d %d", &(pgm->width), &(pgm->height));
    //Lecture de la valeur maximal du gris
    fscanf(file, "%d", &(pgm->max_value));
    //Allocation de la mémoire pour la data
    pgm->data = calloc(pgm->height, sizeof(char*));
    //Stockage des pixels
    if (pgm->type_pgm[1]== '5') {
        fgetc(file);
        for (uint32_t i=0; i<pgm->height; i++) {
            pgm->data[i] = calloc(pgm->width, sizeof(char));
            //Ecrit les données sur la mémoire allouée
            fread(pgm->data[i], sizeof(uint8_t), pgm->width, file);
        }
    }
    //Ferme le fichier
    fclose(file);
}

void affiche_details_image(struct image_pgm *pgm, const char * file_name) {
    FILE *file = fopen(file_name,  "rb");
    printf("Largeur : %d pixels \n", pgm->width);
    printf("Hauteur : %d pixels \n", pgm->height);
    printf("Max_valeur : %d pixels \n", pgm->max_value);
    for (uint32_t i=0; i<pgm->height; i++) {
        for (uint32_t j =0; j<pgm->width; j++) {
            printf("%d ", pgm->data[i][j]);
        }
        printf("\n");
    }
    fclose(file);
}
// int main (int argc, char **argv) {
//     if (argc == 2) {
//         struct image_pgm *pgm = malloc(sizeof(struct image_pgm));
//         char *file_name = argv[1];
//         process_file(pgm, file_name);
//         affiche_details_image(pgm, file_name);
//     } else {
//         printf("Il faut passer en paramètre le nom d'un fichier image valide \n");
//     }
// }