#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    
#include <structure.h>
#include <htables.h>
#include <huffman.h>
#include <math.h>
#include <dct.h>
#include <costable.h>
#include <qtables.h>


struct image_mcu_rgb_sub{
    /*Structure des mcu*/
    struct rgb ***l_mcu;
    uint32_t nmcu;//Pour la largeur comme pour la hauteur on renverra la taille par pixel et non par MCU
    uint8_t dev_width;
    uint8_t dev_height;
};

void verif_sample_factor(uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3){
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
    if(h1 % h2 != 0 || h1 % h3 != 0 ||v1 % v2 != 0 ||v1 % v3 != 0 ){
        printf("Vos facteurs de crominances ne divisent pas parfaitement ceux de la luminescence. \n");
        //renvoyer une erreur ici
    }
}

struct image_YCbCr_sub{
    float ***bloc;
    uint32_t n_mcu;
    uint8_t sampling_factor[6];
    uint8_t nb_comp;
};

struct main_mcu_rgb_sub {
    struct rgb* **data; // On stock les données de l'image dans une nouvelle structure rgb
    int16_t ***bloc; //Liste contenants les pointeurs vers les blocs
    uint32_t n_mcu;//taille de la liste bloc
    uint32_t width; // Largeur de l'image d'entrée
    uint32_t height; // Hauteur de l'image d'entrée
    uint32_t max_value; // Valeur maximal des nuances RGB
    char type_pgm[3]; //Type de l'image d'entrée : permet de définir nb de components
    const char *ppm_filename; // nom du fichier d'entrée
    const char *jpeg_filename; // Nom du fichier de sortie
    uint8_t sampling_factor[6]; // Facteur d'échantillonage
    uint8_t nb_comp;
    struct huff_table **htable;
    struct bitstream *blitzstream;
};


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
            p_mcu->l_mcu[i] = calloc(8*h1*v1, sizeof(struct rgb*));
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
            uint32_t j = pos_y % (h1*8) + (v1*8) * (pos_x % (v1*8)); 

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

        for(uint8_t debord = 1; debord <= ((h1*8)-p_mcu->dev_width); debord ++){
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
    for (uint8_t debord_x = 1; debord_x <= ((v1*8)-p_mcu->dev_height); debord_x ++){
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
        for(uint8_t debord_y = 1; debord_y <= (h1*8)-p_mcu->dev_width; debord_y ++){
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


struct image_YCbCr_sub *creation_YCbCr_rgb_sub(struct main_mcu_rgb_sub* p_main){
    /*Création d'un YCbCr avec initialisation des valeurs*/

    struct image_YCbCr_sub *p_ycbcr = malloc( sizeof(struct image_YCbCr_sub));
    
    p_ycbcr->n_mcu = p_main->n_mcu;
    
    p_ycbcr->bloc = calloc(p_ycbcr->n_mcu, sizeof(float **));

    uint8_t nb_comp = p_main->sampling_factor[0] *p_main->sampling_factor[1]+ 
    p_main->sampling_factor[2] *p_main->sampling_factor[3]+ 
    p_main->sampling_factor[4] *p_main->sampling_factor[5];

    p_ycbcr->nb_comp = nb_comp;
    p_main->nb_comp = nb_comp;

    for (uint32_t i=0; i<p_main->n_mcu; i++) {
            p_ycbcr->bloc[i] = calloc(nb_comp, sizeof(float *));
    }
    return p_ycbcr;
}


struct image_YCbCr_sub *convert_YCbCr_RGB_sub(struct image_mcu_rgb_sub *p_mcu , struct main_mcu_rgb_sub* p_main){
    /*Prend en entrée le découpage MCU et transforme les RGB en YCbCr.
    La valeur retournée sera un pointeur vers l'image convertie en YCbCr et toujours découpée en MCU.
    Cette fonction traite dans un premier temps le cas où l'image est en noir et blanc (P5), on pourra l'améliorer à base de if.*/
    struct image_YCbCr_sub *p_ycbcr = creation_YCbCr_rgb_sub(p_main);

    for(uint32_t i = 0; i < p_ycbcr->n_mcu; i++){
        for(uint32_t j = 0; j< p_ycbcr->nb_comp; j++){
            
            p_ycbcr->bloc[i][j] = calloc(64, sizeof(float));
            if(j < p_ycbcr->sampling_factor[0]*p_ycbcr->sampling_factor[1]){
            //Cas pour la luminescence
                for(uint8_t k = 0; k < 64; k++){
                    uint8_t pos = j * 64 + k;
                    uint8_t cr = p_mcu -> l_mcu[i][pos]->R;
                    uint8_t cb = p_mcu -> l_mcu[i][pos]->G;
                    uint8_t cg = p_mcu -> l_mcu[i][pos]->B;
                    p_ycbcr->bloc[i][j][k] = round(0.299 * cr + 0.587 * cg + 0.114 * cb);
                }
            }
            else if(j < p_ycbcr->sampling_factor[0]*p_ycbcr->sampling_factor[1] + p_ycbcr->sampling_factor[2] * p_ycbcr->sampling_factor[3]){
                // Cas pour Cb
                for(uint8_t k = 0; k < 64; k++){
                    uint8_t pos = j * 64 + k;
                    uint8_t cr = p_mcu -> l_mcu[i][pos]->R;
                    uint8_t cb = p_mcu -> l_mcu[i][pos]->G;
                    uint8_t cg = p_mcu -> l_mcu[i][pos]->B;
                    p_ycbcr->bloc[i][j][k] = round(-0.1687 * cr + -0.3313 * cg + 0.5 * cb + 128);
                }
            }
            else{
               for(uint8_t k = 0; k < 64; k++){
                    uint8_t pos = j * 64 + k;
                    uint8_t cr = p_mcu -> l_mcu[i][pos]->R;
                    uint8_t cb = p_mcu -> l_mcu[i][pos]->G;
                    uint8_t cg = p_mcu -> l_mcu[i][pos]->B;
                    p_ycbcr->bloc[i][j][k] = round(0.5 * cr + -0.4187 * cg + -0.0813 * cb + 128);
                } 
            }
        }
    }
    return p_ycbcr;
}

float **convert_mat_sub(float *p_YCbCr){
    float **matrice = malloc(8*sizeof(float*));
    for(uint8_t k=0; k<8; k++){
        matrice[k] = malloc(8*sizeof(float));
        for(uint8_t i=0; i<8; i++){
            matrice[k][i] = p_YCbCr[k*8 +i];
            matrice[k][i] = p_YCbCr[k*8 +i];
            matrice[k][i] = p_YCbCr[k*8 +i];
        }
    }
    return matrice;
}

void fonction_rgb_sub(struct main_mcu_rgb_sub *p_main, struct image_YCbCr_sub *p_ycbcr){
    p_main->bloc = calloc(p_main->n_mcu, sizeof(int16_t **));
    for(uint32_t mcu_i = 0; mcu_i< p_main->n_mcu; mcu_i++){
        for(uint8_t comp_i = 0; comp_i<p_main->n_mcu; comp_i ++){
            p_main->bloc[mcu_i][comp_i] = calloc(64, sizeof(float));
        }
    }
    for(uint32_t mcu_i = 0; mcu_i < p_main->n_mcu; mcu_i ++){
        for(uint8_t comp_i = 0; comp_i<p_main->n_mcu; comp_i ++){
            float **mat = convert_mat_sub(p_ycbcr->bloc[mcu_i][comp_i]);
            dct(mat);
            zigzag(mat, p_main->bloc[mcu_i][comp_i]);
            if(comp_i < p_main->sampling_factor[0]* p_main->sampling_factor[1]){
                quantization_Y(p_main->bloc[mcu_i][comp_i]);
            }
            else{
                quantization_C(p_main->bloc[mcu_i][comp_i]);
            }
        }
    }
}
