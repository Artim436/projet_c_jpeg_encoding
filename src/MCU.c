#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <lecture_pgm.h>
# include <MCU.h>

/*Amélioration : modifier j en uint8_t*/

struct image_mcu *decoupe_mcu_8x8(struct image_pgm *image_pgm){
    /*Cette fonction prend en entrée une image pgm "écrite" en décimal et la renvoie avec un découpage en MCU.

    La sortie sera une structure image_mcu définie ci-dessus

    La partie qui nous intéresse: **l_mcu sera une liste de pointeur de taille égale aux nombre de MCUs.
    Chaque pointeur dirigera vers une liste de taille 64(=8x8) donc chaque case comprend une des valeurs du pixel

    l_mcu[i] correspondra au i+1 ème MCU (lu de gauche à droite de haut en bas.
    l_mcu[i][j] correspondra au j+1 ème pixel (avec  0<= j<= 63) lu de gauche à droite et de haut en bas du MCU i*/
    struct image_mcu *p_mcu = creation_mcu_8x8(image_pgm->type_pgm, image_pgm->width, image_pgm->height, image_pgm->max_value);//Commence par créer une table de MCU vierge.
    
    //Puis définit la de liste de mcu
    //On boucle directement sur la matrice ppm (de gauche à droite et de haut en bas)
    for(uint32_t pos_x = 0; pos_x < image_pgm->height; pos_x++){
        for(uint32_t pos_y = 0; pos_y < image_pgm->width; pos_y++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = pos_y / 8;
            i = i + (image_pgm->width / 8 ) * (pos_x / 8);

            //Puis on calcule sa position dans le MCU
            uint32_t j = pos_y % 8 + 8 * (pos_x % 8); 

            //Puis on le rajoute à la matrice correspondante
            p_mcu->l_mcu[i][j] = image_pgm->data[pos_x][pos_y];
        }
        //Traite le cas du débordement sur y
        uint8_t last_pix = image_pgm->data[pos_x][image_pgm->width - 1];
        for(uint8_t debord = 1; debord <= p_mcu->dev_width; debord ++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = (image_pgm->width - 1) / 8;
            i = i + (image_pgm->width / 8 ) * (pos_x / 8);

            //Puis on calcule sa position dans le MCU
            uint32_t j = (image_pgm->width - 1 + debord) % 8;
            j += 8 * pos_x % 8; 

            //Puis on le rajoute à la matrice correspondante
            p_mcu->l_mcu[i][j] = last_pix;
        }
    }
    for (uint8_t debord_x = 1; debord_x <= p_mcu->dev_height; debord_x ++){
        for(uint32_t pos_y = 0; pos_y < image_pgm->width; pos_y++){
            uint8_t last_pix = image_pgm->data[image_pgm->height-1][pos_y];

            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = pos_y / 8;
            i = i + (image_pgm->width / 8 ) * ((image_pgm->height-1) / 8);

            //Puis on calcule sa position dans le MCU
            uint32_t j = pos_y % 8;
            j += 8 * (image_pgm->height - 1 + debord_x) % 8; 

            //Puis on le rajoute à la matrice correspondante
            p_mcu->l_mcu[i][j] = last_pix;
        }
        //Traite le cas du débordement sur y
        uint8_t last_pix = image_pgm->data[image_pgm->height - 1][image_pgm->width - 1];
        for(uint8_t debord_y = 1; debord_y <= p_mcu->dev_width; debord_y ++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = (image_pgm->width - 1) / 8;
            i = i + (image_pgm->width / 8 ) * ((image_pgm->height-1)/ 8);

            //Puis on calcule sa position dans le MCU
            uint32_t j = (image_pgm->width - 1 + debord_y) % 8;
            j += 8 * (image_pgm->height - 1 + debord_x) % 8; 

            //Puis on le rajoute à la matrice correspondante
            p_mcu->l_mcu[i][j] = last_pix;
        }
    }
    return p_mcu;
}

struct image_mcu *creation_mcu_8x8(char type_pgm[3], uint32_t width, uint32_t height, uint32_t max_value){
    /*Création d'un mcu avec toutes les valeures*/
    struct image_mcu *p_mcu = malloc(sizeof(struct image_mcu));
    *(p_mcu -> type_pgm) = *type_pgm;
    
    /*Calcul du nombre de mcu de taille 8x8 qu'il y aura */
    uint32_t nmcu = width / 8;
    p_mcu ->dev_height = height % 8;
    p_mcu ->dev_width = width % 8;
    if (p_mcu->dev_width != 0){
        nmcu ++;
    }
    uint32_t tmp = height / 8;
    if (p_mcu -> dev_height != 0){
        tmp ++ ;
    }
    p_mcu -> nmcu = nmcu*tmp;

    p_mcu -> max_value = max_value;
    
    p_mcu->l_mcu = calloc(p_mcu->nmcu, sizeof(char *));
    for (uint32_t i=0; i<p_mcu->nmcu; i++) {
            p_mcu->l_mcu[i] = calloc(64, sizeof(uint8_t));
    }
    return p_mcu;
}

void affiche_img_mcu(struct image_mcu *p_gmu){
    /*Affiche les éléments de chaques MCU*/
    for(uint32_t j = 0; j<p_gmu->nmcu; j++){
        printf("----- MCU numéro %u ----- \n", j+1);
        for(uint8_t i = 0; i<64; i++){
            printf("%u ", p_gmu->l_mcu[j][i]);
            if(i % 8 == 7){
                printf("\n");
            }
        }
    }
}