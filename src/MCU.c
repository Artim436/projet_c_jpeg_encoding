#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
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
            
            uint32_t j = (pos_x % 8) * 8 + p_mcu->dev_width + debord -1;

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
   
    p_mcu->l_mcu = calloc(p_mcu->nmcu, sizeof(char *));
    for (uint32_t i=0; i<p_mcu->nmcu; i++) {
            p_mcu->l_mcu[i] = calloc(64, sizeof(uint8_t));
    }
    return p_mcu;
}

struct image_mcu_rgb_sub *creation_mcu_rgb_sub(uint32_t width, uint32_t height, uint8_t h1, uint8_t v1){
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
            p_mcu->l_mcu[i] = calloc(64*h1*v1, sizeof(struct rgb*));
    }
    return p_mcu;
}


struct image_mcu_rgb_sub *decoupe_mcu_rgb_sub(struct main_mcu_rgb_sub *p_main){
 
    struct image_mcu_rgb_sub *p_mcu = creation_mcu_rgb_sub(p_main->width, p_main->height, p_main->sampling_factor[0], p_main->sampling_factor[1]);//Commence par créer une table de MCU vierge.
    p_main->n_mcu = p_mcu->nmcu;//On oublie pas de mettre à jour la structure principale
    uint8_t h1= p_main->sampling_factor[0];
    uint8_t v1 = p_main->sampling_factor[1];
    //Puis définit la de liste de mcu
    //On boucle directement sur la matrice ppm (de gauche à droite et de haut en bas)
    for(uint32_t pos_x = 0; pos_x < p_main->height; pos_x++){
        for(uint32_t pos_y = 0; pos_y < p_main->width; pos_y++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position

            uint32_t i = pos_y / (h1*8);
            if((float)p_main->width / (h1*8) > p_main->width/(h1*8)){
                i = i + (floor((p_main->width / (h1*8) ))+1) * (pos_x / (v1*8));
            }
            else{
                i = i + (p_main->width / (h1*8) ) * (pos_x / (v1*8));//nb_de_mcu par lignes * la ligne
            }

            //Puis on calcule sa position dans le MCU
            uint32_t j = pos_y % (h1*8) + (h1*8) * (pos_x % (v1 * 8)); 

            //Puis on le rajoute à la matrice correspondante
            struct rgb* new_rgb = malloc(sizeof (struct rgb));
            new_rgb->B = p_main->data[pos_x][pos_y]->B;
            new_rgb->R = p_main->data[pos_x][pos_y]->R;
            new_rgb->G = p_main->data[pos_x][pos_y]->G;
            p_mcu->l_mcu[i][j] = new_rgb;

        }
        //Traite le cas du débordement sur y
        struct rgb* last_pix= malloc(sizeof (struct rgb));
        last_pix->B = p_main->data[pos_x][p_main->width-1]->B;
        last_pix->R = p_main->data[pos_x][p_main->width-1]->R;
        last_pix->G = p_main->data[pos_x][p_main->width-1]->G;

        for(uint8_t debord = 1; debord <= ((h1*8)-p_mcu->dev_width)%(h1*8); debord ++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = ((p_main->width - 1)) / (h1*8);
            if((float)p_main->width / (h1*8) > p_main->width/(h1*8)){
                i = i + (floor((p_main->width / (h1*8) ))+1) * (pos_x / (h1*8));
            }
            else{
                i = i + (p_main->width / (h1*8) ) * (pos_x / (v1*8));
            }

            

            //Puis on calcule sa position dans le MCU
            
            uint32_t j = (pos_x % (v1*8)) * (h1*8) + p_mcu->dev_width + debord-1;
            

            
            //Puis on le rajoute à la matrice correspondante
            p_mcu->l_mcu[i][j] = malloc(sizeof(struct rgb));
            p_mcu->l_mcu[i][j]->B = last_pix->B;
            p_mcu->l_mcu[i][j]->G = last_pix->G;
            p_mcu->l_mcu[i][j]->R = last_pix->R;

        }
    }
    for (uint8_t debord_x = 1; debord_x <= ((v1*8)-p_mcu->dev_height)%(v1 * 8); debord_x ++){
        for(uint32_t pos_y = 0; pos_y < p_main->width; pos_y++){
            struct rgb* last_pix= malloc(sizeof (struct rgb));
            last_pix->B = p_main->data[p_main->height-1][pos_y]->B;
            last_pix->R = p_main->data[p_main->height -1][pos_y]->R;
            last_pix->G = p_main->data[p_main->height-1][pos_y]->G;
            //On calcule le MCU du dernier pixel qui est celle qui nous intéresse
            uint32_t i = pos_y / (h1*8);
            if((float)p_main->width / (h1*8) > p_main->width/(h1*8)){
                i = i + (floor((p_main->width / (h1*8) ))+1) * ((p_main->height-1) / (v1*8));
            }
            else{
                i = i + (p_main->width / (h1*8) ) * ((p_main->height-1)  / (v1*8));
            }

            //Puis on calcule sa position dans le MCU

            uint32_t j = (((p_mcu->dev_height)+debord_x-1)%(v1*8)) * (h1*8) + pos_y%(h1*8);
            

            //Puis on le rajoute à la matrice correspondante


            p_mcu->l_mcu[i][j] = malloc(sizeof(struct rgb));
            p_mcu->l_mcu[i][j]->B = last_pix->B;
            p_mcu->l_mcu[i][j]->G = last_pix->G;
            p_mcu->l_mcu[i][j]->R = last_pix->R;
        }
        //Traite le cas du débordement sur y
        struct rgb* last_pix= calloc(1, sizeof (struct rgb));
        last_pix->B = p_main->data[p_main->height-1][p_main->width - 1]->B;
        last_pix->R = p_main->data[p_main->height -1][p_main->width - 1]->R;
        last_pix->G = p_main->data[p_main->height-1][p_main->width - 1]->G;
        for(uint8_t debord_y = 1; debord_y <= ((h1*8)-p_mcu->dev_width)%(h1*8); debord_y ++){
            //On commence par calculer l'appartenance du pixel au MCU correspondant en fonction de la position
            uint32_t i = p_main->n_mcu-1;

            //Puis on calcule sa position dans le MCU

            uint32_t j = (((p_mcu->dev_height)+debord_x-1)%(v1*8)) * (h1*8) + (p_mcu->dev_width+debord_y -1)%(h1*8);

            //Puis on le rajoute à la matrice correspondante

            p_mcu->l_mcu[i][j] = malloc(sizeof(struct rgb));
            p_mcu->l_mcu[i][j]->B = last_pix->B;
            p_mcu->l_mcu[i][j]->G = last_pix->G;
            p_mcu->l_mcu[i][j]->R = last_pix->R;
        }
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

void affiche_img_mcu_rgb_sub(struct image_mcu_rgb_sub *p_gmu, uint8_t h1, uint8_t v1){
    /*Affiche les éléments de chaques MCU*/
    for(uint32_t j = 0; j<20; j++){
        printf("----- MCU numéro %u ----- \n", j);
        for(uint32_t i = 0; i<64*h1*v1; i++){
            printf("%x%x%x ", p_gmu->l_mcu[j][i]->R, p_gmu->l_mcu[j][i]->G, p_gmu->l_mcu[j][i]->B);
            if(i % 8 == 7){
                printf("   ");
            }
            if(i % (8 * (uint32_t) h1) == 8 * (uint32_t) h1 -1){
                printf("\n");
            }
            if(i % (64* (uint32_t) h1) == (uint32_t) h1*64 - 1){
                printf("\n");
            }
        }
    }
}

void clean_main_mcu_sub(struct main_mcu_rgb_sub *mcu) {
    // On nettoie data
    for (uint32_t i=0; i<mcu->height; i++) {
        for(uint32_t j=0; j<mcu->width; j++){
            free(mcu->data[i][j]);
        }
        free(mcu->data[i]);
    }

    // On nettoie bloc
    for (uint32_t i=0; i<mcu->n_mcu; i++) {
        for(uint8_t comp_i = 0; comp_i<mcu->nb_comp; comp_i++){
            free(mcu->bloc[i][comp_i]);
        }
        free(mcu->bloc[i]);
    }

    //On nettoie les htable
    for(uint8_t i = 0; i<4; i++){
        huffman_table_destroy(mcu->htable[i]);
    }
    free(mcu->htable);
    free(mcu->data);
    free(mcu->bloc);
    free(mcu);
}
