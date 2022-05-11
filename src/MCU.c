#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <lecture_pgm.h>
#include <MCU.h>
#include <structure.h>
#include <math.h>

struct image_mcu *decoupe_mcu_8x8(struct main_mcu *p_main){
    /*Cette fonction prend en entrée une image pgm "écrite" en décimal et la renvoie avec un découpage en MCU.

    La sortie sera une structure image_mcu définie ci-dessus

    La partie qui nous intéresse: **l_mcu sera une liste de pointeur de taille égale aux nombre de MCUs.
    Chaque pointeur dirigera vers une liste de taille 64(=8x8) donc chaque case comprend une des valeurs du pixel

    l_mcu[i] correspondra au i+1 ème MCU (lu de gauche à droite de haut en bas.
    l_mcu[i][j] correspondra au j+1 ème pixel (avec  0<= j<= 63) lu de gauche à droite et de haut en bas du MCU i*/
    struct image_mcu *p_mcu = creation_mcu_8x8(p_main->type_pgm, p_main->width, p_main->height, p_main->max_value);//Commence par créer une table de MCU vierge.
    p_main->n_mcu = p_mcu->nmcu;//On oublie pas de mettre à jour la structure principale

    printf("débordement droite : %u \n débordement bas : %u\n",p_mcu->dev_width, p_mcu->dev_height);
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
            p_mcu->l_mcu[i][j] = p_main->data[pos_x][pos_y];
        }
        //Traite le cas du débordement sur y
        uint8_t last_pix = p_main->data[pos_x][p_main->width - 1];
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
            
            uint32_t j = (pos_x % 8) * 8 + debord;

            //Puis on le rajoute à la matrice correspondante
            
            p_mcu->l_mcu[i][j] = last_pix;
        }
    }
    for (uint8_t debord_x = 1; debord_x <= (8-p_mcu->dev_height)%8; debord_x ++){
        for(uint32_t pos_y = 0; pos_y < p_main->width; pos_y++){
            uint8_t last_pix = p_main->data[p_main->height-1][pos_y];

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
            p_mcu->l_mcu[i][j] = last_pix;
        }
        //Traite le cas du débordement sur y
        uint8_t last_pix = p_main->data[p_main->height - 1][p_main->width - 1];
        for(uint8_t debord_y = 1; debord_y <= (8-p_mcu->dev_width)%8; debord_y ++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = p_main->n_mcu-1;

            //Puis on calcule sa position dans le MCU

            uint32_t j = (((p_mcu->dev_height)+debord_x-1)%8) * 8 + (p_mcu->dev_width+debord_y -1)%8;

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
    for(uint32_t j = 1108; j<1115; j++){
        printf("----- MCU numéro %u ----- \n", j);
        for(uint8_t i = 0; i<64; i++){
            printf("%x ", p_gmu->l_mcu[j][i]);
            if(i % 8 == 7){
                printf("\n");
            }
        }
    }
}



void print_mat(float** mat, uint8_t len){
    for(uint8_t i =0; i<len;i++){
        for(uint8_t j =0; j<len;j++){
            printf("%x ", (int16_t) mat[i][j]);
        }
        printf("\n");
    }   
}

struct image_mcu_rgb *decoupe_mcu_8x8_rgb(struct main_mcu_rgb *p_main){
    /*Cette fonction prend en entrée une image pgm "écrite" en décimal et la renvoie avec un découpage en MCU.

    La sortie sera une structure image_mcu définie ci-dessus

    La partie qui nous intéresse: **l_mcu sera une liste de pointeur de taille égale aux nombre de MCUs.
    Chaque pointeur dirigera vers une liste de taille 64(=8x8) donc chaque case comprend une des valeurs du pixel

    l_mcu[i] correspondra au i+1 ème MCU (lu de gauche à droite de haut en bas.
    l_mcu[i][j] correspondra au j+1 ème pixel (avec  0<= j<= 63) lu de gauche à droite et de haut en bas du MCU i*/
    struct image_mcu_rgb *p_mcu = creation_mcu_8x8_rgb(p_main->type_pgm, p_main->width, p_main->height, p_main->max_value);//Commence par créer une table de MCU vierge.
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
            
//             uint32_t j = (pos_x % 8) * 8 + debord;

//             //Puis on le rajoute à la matrice correspondante
            
            p_mcu->l_mcu[i][j] = last_pix;
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
            p_mcu->l_mcu[i][j] = last_pix;
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
            p_mcu->l_mcu[i][j] = last_pix;
        }
    }
    return p_mcu;
}


struct image_mcu_rgb *creation_mcu_8x8_rgb(char type_pgm[3], uint32_t width, uint32_t height, uint32_t max_value){
    /*Création d'un mcu avec toutes les valeures*/
    struct image_mcu_rgb *p_mcu = malloc(sizeof(struct image_mcu_rgb));
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
            p_mcu->l_mcu[i] = calloc(64, sizeof(struct rgb*));
    }
    return p_mcu;
}

void affiche_img_mcu_rgb(struct image_mcu_rgb *p_gmu){
    /*Affiche les éléments de chaques MCU*/
    for(uint32_t j = 0; j<20; j++){
        printf("----- MCU numéro %u ----- \n", j);
        for(uint8_t i = 0; i<64; i++){
            printf("%x%x%x ", p_gmu->l_mcu[j][i]->R, p_gmu->l_mcu[j][i]->G, p_gmu->l_mcu[j][i]->B);
            if(i % 8 == 7){
                printf("\n");
            }
        }
    }
}

float **convert_YCbCr_mat(struct YCbCr **p_YCbCr){
    float **matrice = malloc(8*sizeof(float*));
    for(uint8_t j = 0;j<8; j++){
        matrice[j] = malloc(8*sizeof(float));
        for(uint8_t k=0; k<8; k++){
            matrice[j][k] = (float) p_YCbCr[j*8+k]->Y;
        }
    }
    return matrice;
}

float ***convert_YCbCr_mat_rgb(struct YCbCr **p_YCbCr){
    float ***matrice = malloc(8*sizeof(float**));
    for(uint8_t j = 0;j<8; j++){
        matrice[j] = malloc(8*sizeof(float*));
        for(uint8_t k=0; k<8; k++){
            float* new_YCbCr = calloc(3, sizeof(float));
            new_YCbCr[0] = (float) p_YCbCr[j*8+k]->Y;
            new_YCbCr[1] = (float) p_YCbCr[j*8+k]->Cb;
            new_YCbCr[2] = (float) p_YCbCr[j*8+k]->Cr;
            matrice[j][k] = new_YCbCr;
        }
    }
    return matrice;
}