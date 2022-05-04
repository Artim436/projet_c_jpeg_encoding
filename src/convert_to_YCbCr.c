#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <lecture_pgm.h>
#include <MCU.h>
#include <convert_to_YCbCr.h>
#include <math.h>
#include <structure.h>

/*Amélioration : -chercher le nb de bits nécessaires pour YCbCr
-Faire des free()*/


struct image_YCbCr *convert_YCbCr(struct image_mcu *p_mcu){
    /*Prend en entrée le découpage MCU et transforme les RGB en YCbCr.
    La valeur retournée sera un pointeur vers l'image convertie en YCbCr et toujours découpée en MCU.
    Cette fonction traite dans un premier temps le cas où l'image est en noir et blanc (P5), on pourra l'améliorer à base de if.*/
    
    struct image_YCbCr *p_ycbcr = creation_YCbCr(p_mcu);

    for(uint32_t i = 0; i < p_ycbcr->nmcu; i++){
        for(uint8_t j = 0; j<64; j++){
            struct YCbCr *new_ycbcr= calloc(1, sizeof(struct YCbCr));
            uint8_t coeff = p_mcu->l_mcu[i][j];
            new_ycbcr->Y = (0.299 * coeff + 0.587 * coeff + 0.114 * coeff);
            new_ycbcr->Cb = (-0.1687 * coeff + -0.3313 * coeff + 0.5 * coeff+ 128);
            new_ycbcr->Cr = (0.5 * coeff + -0.4187 * coeff + -0.0813 * coeff+ 128);
            p_ycbcr->l_ycbcr[i][j] = new_ycbcr;
        }
    }
    return p_ycbcr;
}

struct image_YCbCr *creation_YCbCr(struct image_mcu *p_mcu){
    /*Création d'un YCbCr avec initialisation des valeurs*/

    struct image_YCbCr *p_ycbcr = malloc( sizeof(struct image_YCbCr));
    
    p_ycbcr->nmcu = p_mcu->nmcu;
    *(p_ycbcr -> type_pgm) = *p_mcu->type_pgm;
    
    p_ycbcr->l_ycbcr = calloc(p_ycbcr->nmcu, sizeof(char *));

    for (uint32_t i=0; i<p_mcu->nmcu; i++) {
            p_ycbcr->l_ycbcr[i] = calloc(64, sizeof(char *));
    }
    return p_ycbcr;
}


void afficher_YCbCr(struct image_YCbCr *p_ycbcr){
    //Parcours les éléments de image_YCbCr et affiche le Y
    for(uint32_t j = 0; j<p_ycbcr->nmcu; j++){
        printf("----- MCU numéro %u YCbCr----- \n", j+1);
        printf("Affichage de Y: \n");
        for(uint8_t i = 0; i<64; i++){
            printf("%x ", p_ycbcr->l_ycbcr[j][i]->Y);
            if(i % 8 == 7){
                printf("\n");
            }
        }
        // printf("Affichage de Cb : \n");
        // for(uint8_t i = 0; i<64; i++){
        //     printf("%u ", p_ycbcr->l_ycbcr[j][i]->Cb);
        //     if(i % 8 == 7){
        //         printf("\n");
        //     }
        // }
    }
}