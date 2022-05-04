#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <lecture_pgm.h>
# include <MCU.h>
#include <structure.h>

/*Amélioration : modifier j en uint8_t*/


void decoupe_mcu_8x8(struct main_mcu *p_main){
/*Cette fonction prend en entrée une image pgm "écrite" en décimal modifie le p_main en découpant les MCU.

    La sortie sera une structure image_mcu définie ci-dessus

    La partie qui nous intéresse: **l_mcu sera une liste de pointeur de taille égale aux nombre de MCUs.
    Chaque pointeur dirigera vers une liste de taille 64(=8x8) donc chaque case comprend une des valeurs du pixel

    l_mcu[i] correspondra au i+1 ème MCU (lu de gauche à droite de haut en bas.
    l_mcu[i][j] correspondra au j+1 ème pixel (avec  0<= j<= 63) lu de gauche à droite et de haut en bas du MCU i*/

    //Puis définit la de liste de mcu
    
    
    //Calcule du nombre de MCU
    uint32_t nmcu = p_main->width / 8;
    uint32_t dev_height = p_main->height % 8;
    uint32_t dev_width = p_main->width % 8;
    if (dev_height != 0){
        nmcu ++;
    }
    uint32_t tmp = height / 8;
    if (dev_width != 0){
        tmp ++ ;
    }
    p_main -> n_mcu = nmcu*tmp;


    //Allocation de la mémoire pour la liste de mcu
    p_main->bloc = calloc(p_main->n_mcu, sizeof(char *));
    for (uint32_t i=0; i<p_mcu->nmcu; i++) {
            p_main->bloc[i] = calloc(1, sizeof(struct bloc_8x8));
    }

    //On boucle directement sur la matrice ppm (de gauche à droite et de haut en bas)
    for(uint32_t pos_x = 0; pos_x < p_main->height; pos_x++){
        for(uint32_t pos_y = 0; pos_y < p_main->width; pos_y++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = pos_y / 8;
            i = i + (p_main->width / 8 ) * (pos_x / 8);

            //Puis on calcule sa position dans le MCU
            uint8_t pos_init_x = pos_x % 8 - 1;
            uint8_t pos_init_y = pos_y % 8 - 1;

            //Puis on le rajoute à la matrice correspondante
            p_main->bloc[i]->[pos_init_x][pos_init_y] = p_main->data[pos_x][pos_y];
        }
        //Traite le cas du débordement sur y
        uint8_t last_pix = p_main->data[pos_x][p_main->width - 1];
        for(uint8_t debord = 1; debord <= dev_width; debord ++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = (p_main->width - 1) / 8;
            i = i + (p_main->width / 8 ) * (pos_x / 8);

            //Puis on calcule sa position dans le MCU
            uint8_t pos_init_x = pos_x % 8 - 1;
            uint8_t pos_init_y = (p_main->width-1)%8;
            //Puis on le rajoute à la matrice correspondante
            p_main->bloc[i]->[pos_init_x][pos_init_y];
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

void affiche_img_mcu(struct image_mcu *p_gmu){
    /*Affiche les éléments de chaques MCU*/
    for(uint32_t j = 0; j<p_gmu->nmcu; j++){
        printf("----- MCU numéro %u RGB----- \n", j+1);
        for(uint8_t i = 0; i<64; i++){
            printf("%u ", p_gmu->l_mcu[j][i]);
            if(i % 8 == 7){
                printf("\n");
            }
        }
    }
}