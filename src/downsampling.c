#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    
#include <lecture_pgm.h>
#include <MCU.h>
#include <structure.h>
#include <convert_to_YCbCr.h>
#include <dct.h>
#include <htables.h>
#include <jpeg_writer.h>
#include <test.h>
#include <huffman.h>
#include <encoding.h>
#include <math.h>
#include <write_jpeg_file.h>

struct image_mcu_rgb_sub{
    /*Structure des mcu*/
    struct rgb ***l_mcu;
    uint32_t nmcu;//Pour la largeur comme pour la hauteur on renverra la taille par pixel et non par MCU
    uint8_t h1;
    uint8_t v1;
    uint8_t h2;
    uint8_t v2;
    uint8_t h3;
    uint8_t v3;
    uint8_t dev_width;
    uint8_t dev_height;
};

void verif_sample_factor(h1, v1, h2, v2, h3, v3){
    if(h1 > 4|| h2 > 4||h3 > 4||v1 > 4||v2 > 4||v3 > 4){
        printf("Vos facteurs déchantillonages doivent être compris entre 1 et 4.\n");
        //renvoyer une erreur ici
    }
    if(h1 <1|| h2<1||h3 <1||v1 <1||v2 <1||v3 <1){
        printf("Vos facteurs déchantillonages doivent être compris entre 1 et 4.\n");
        //renvoyer une erreur ici
    }
    uint8_t somme_prod = h1*v1+h2*v2+h3*v2;
    if(somme_prod > 10){
        printf("La somme des produits de vos facteurs dépassent 10, essayez de réduire vos facteurs\n");
        //renvoyer une erreur ici
    }
    if(h1 % h2 != 0 || h1 % h3 != 0 ||v1 % v2 != 0 ||v1 % v3 != 0 ||){
        printf("Vos facteurs de crominances ne divisent pas parfaitement ceux de la luminescence. \n");
        //renvoyer une erreur ici
    }
}

void subsampling(struct main_mcu_rgb*  p_main, uint8_t fact_Y, uint8_t fact_Cb, uint8_t fact_Cr){
    
}

struct image_mcu_rgb *decoupe_mcu_8x8_rgb(struct main_mcu_rgb *p_main , h1, v1){

    struct image_mcu_rgb *p_mcu = creation_mcu_8x8_rgb_sub(p_main->type_pgm, p_main->width, h1, v1);
    p_main->n_mcu = p_mcu->nmcu;//On oublie pas de mettre à jour la structure principale

    //Puis définit la de liste de mcu
    //On boucle directement sur la matrice ppm (de gauche à droite et de haut en bas)
    for(uint32_t pos_x = 0; pos_x < p_main->height; pos_x++){
        for(uint32_t pos_y = 0; pos_y < p_main->width; pos_y++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = pos_y / 8;
            if((float)p_main->width / 8 > p_main->width/8){
                i = i + (floor((p_main->width / 8 ))+1) * (pos_x / 8);
            }
            else{
                i = i + (p_main->width / 8 ) * (pos_x / 8);
            }
            //Puis on calcule sa position dans le MCU
            uint32_t j = pos_y % 8 + 8 * (pos_x % 8); 

            //Puis on le rajoute à la matrice correspondante
            struct rgb* new_rgb = calloc(1, sizeof (struct rgb));
            new_rgb->B = p_main->data[pos_x][pos_y]->B;
            new_rgb->R = p_main->data[pos_x][pos_y]->R;
            new_rgb->G = p_main->data[pos_x][pos_y]->G;
            p_mcu->l_mcu[i][j] = new_rgb;

            if(i == 54){
                printf("j = %u\n",j);
                printf("%x%x%x \n", p_mcu->l_mcu[i][j]->R,p_mcu->l_mcu[i][j]->G,p_mcu->l_mcu[i][j]->B );
            }
        }
        //Traite le cas du débordement sur y
        struct rgb* last_pix= calloc(1, sizeof (struct rgb));
        last_pix->B = p_main->data[pos_x][p_main->width-1]->B;
        last_pix->R = p_main->data[pos_x][p_main->width-1]->R;
        last_pix->G = p_main->data[pos_x][p_main->width-1]->G;

        for(uint8_t debord = 1; debord <= (8-p_mcu->dev_width)%8; debord ++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = ((p_main->width - 1)) / 8;
            if((float)p_main->width / 8 > p_main->width/8){
                i = i + (floor((p_main->width / 8 ))+1) * (pos_x / 8);
            }
            else{
                i = i + (p_main->width / 8 ) * (pos_x / 8);
            }

            

            //Puis on calcule sa position dans le MCU
            
            uint32_t j = (pos_x % 8) * 8 + p_mcu->dev_width + debord-1;
            
            
            //Puis on le rajoute à la matrice correspondante
            p_mcu->l_mcu[i][j] = calloc(1, sizeof(struct rgb));
            p_mcu->l_mcu[i][j]->B = last_pix->B;
            p_mcu->l_mcu[i][j]->G = last_pix->G;
            p_mcu->l_mcu[i][j]->R = last_pix->R;

            if(i == 54){
                printf("j = %u\n",j);
                printf("%x%x%x \n", p_mcu->l_mcu[i][j]->R,p_mcu->l_mcu[i][j]->G,p_mcu->l_mcu[i][j]->B );
            }
        }
    }
    for (uint8_t debord_x = 1; debord_x <= (8-p_mcu->dev_height)%8; debord_x ++){
        for(uint32_t pos_y = 0; pos_y < p_main->width; pos_y++){
            struct rgb* last_pix= calloc(1, sizeof (struct rgb));
            last_pix->B = p_main->data[p_main->height-1][pos_y]->B;
            last_pix->R = p_main->data[p_main->height -1][pos_y]->R;
            last_pix->G = p_main->data[p_main->height-1][pos_y]->G;
            //On calcule le MCU du dernier pixel qui est celle qui nous intéresse
            uint32_t i = pos_y / 8;
            if((float)p_main->width / 8 > p_main->width/8){
                i = i + (floor((p_main->width / 8 ))+1) * ((p_main->height-1) / 8);
            }
            else{
                i = i + (p_main->width / 8 ) * ((p_main->height-1)  / 8);
            }

            //Puis on calcule sa position dans le MCU

            uint32_t j = (((p_mcu->dev_height)+debord_x-1)%8) * 8 + pos_y%8;

            //Puis on le rajoute à la matrice correspondante


            p_mcu->l_mcu[i][j] = calloc(1, sizeof(struct rgb));
            p_mcu->l_mcu[i][j]->B = last_pix->B;
            p_mcu->l_mcu[i][j]->G = last_pix->G;
            p_mcu->l_mcu[i][j]->R = last_pix->R;
        }
        //Traite le cas du débordement sur y
        struct rgb* last_pix= calloc(1, sizeof (struct rgb));
        last_pix->B = p_main->data[p_main->height-1][p_main->width - 1]->B;
        last_pix->R = p_main->data[p_main->height -1][p_main->width - 1]->R;
        last_pix->G = p_main->data[p_main->height-1][p_main->width - 1]->G;
        for(uint8_t debord_y = 1; debord_y <= (8-p_mcu->dev_width)%8; debord_y ++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = p_main->n_mcu-1;

            //Puis on calcule sa position dans le MCU

            uint32_t j = (((p_mcu->dev_height)+debord_x-1)%8) * 8 + (p_mcu->dev_width+debord_y -1)%8;

            //Puis on le rajoute à la matrice correspondante

            p_mcu->l_mcu[i][j] = calloc(1, sizeof(struct rgb));
            p_mcu->l_mcu[i][j]->B = last_pix->B;
            p_mcu->l_mcu[i][j]->G = last_pix->G;
            p_mcu->l_mcu[i][j]->R = last_pix->R;
        }
    }
    return p_mcu;
}


struct image_mcu_rgb_sub *creation_mcu_rgb(uint32_t width, uint32_t height, uint8_t h1, uint8_t v1){
    /*Création d'un mcu avec toutes les valeures*/
    struct image_mcu_rgb_sub *p_mcu = malloc(sizeof(struct image_mcu_rgb_sub));
    
    /*Calcul du nombre de mcu de taille 8x8 qu'il y aura */
    uint32_t nmcu = width / (8*h1);
    p_mcu ->dev_height = height % (8*v1);
    p_mcu ->dev_width = width % (8*h1);
    if (p_mcu->dev_width != 0){
        nmcu ++;
    }
    uint32_t tmp = height / (8*v1);
    if (p_mcu -> dev_height != 0){
        tmp ++ ;
    }
    p_mcu -> nmcu = nmcu*tmp;

    p_mcu->l_mcu = calloc(p_mcu->nmcu, sizeof(struct rgb* *));
    for (uint32_t i=0; i<p_mcu->nmcu; i++) {
            p_mcu->l_mcu[i] = calloc(8*h1*v1, sizeof(struct rgb*));
    }
    return p_mcu;
}

void affiche_img_mcu_rgb(struct image_mcu_rgb *p_gmu){
    /*Affiche les éléments de chaques MCU*/
    for(uint32_t j = 45; j<55; j++){
        printf("----- MCU numéro %u ----- \n", j);
        for(uint8_t i = 0; i<64; i++){
            printf("%x%x%x ", p_gmu->l_mcu[j][i]->R, p_gmu->l_mcu[j][i]->G, p_gmu->l_mcu[j][i]->B);
            if(i % 8 == 7){
                printf("\n");
            }
        }
    }
}

float ***convert_YCbCr_mat_rgb(struct YCbCr **p_YCbCr){
    float ***matrice = malloc(3*sizeof(float**));
    matrice[0] = malloc(8*sizeof(float *));
    matrice[1] = malloc(8*sizeof(float *));
    matrice[2] = malloc(8*sizeof(float *));
    for(uint8_t k=0; k<8; k++){
        matrice[0][k] = malloc(8*sizeof(float));
        matrice[1][k] = malloc(8*sizeof(float));
        matrice[2][k] = malloc(8*sizeof(float));
        for(uint8_t i=0; i<8; i++){
            matrice[0][k][i] = (float)p_YCbCr[k*8 +i]->Y;
            matrice[1][k][i] = (float)p_YCbCr[k*8 +i]->Cb;
            matrice[2][k][i] = (float)p_YCbCr[k*8 +i]->Cr;
        }
    }

    return matrice;
}