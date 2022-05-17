#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    
#include <structure.h>
#include <htables.h>
#include <huffman.h>
#include <math.h>
#include <dct.h>
#include <encoding.h>
#include <downsampling.h>

void process_file_sub(const char *file_name, struct main_mcu_rgb_sub* mcu_rgb) {
    //Ouvre le fichier en mode lecture binaire
    FILE *file = fopen(file_name, "rb");
    //Lecture du type de fichier
    fscanf(file, "%s", mcu_rgb->type_pgm);


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
    //Ferme le fichier
    fclose(file);
}

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


    
    for(uint8_t i=0; i<6; i++){
        p_ycbcr->sampling_factor[i] = p_main->sampling_factor[i];
    }

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
        uint32_t pos_x = 0;
        uint32_t pos_cr = 0;
        uint32_t* pos_x_cb = &pos_x;
        uint32_t* pos_x_cr = &pos_cr;
        for(uint8_t j = 0; j< p_ycbcr->nb_comp; j++){
            p_ycbcr->bloc[i][j] = calloc(64, sizeof(float));
            if(j < p_ycbcr->sampling_factor[0]*p_ycbcr->sampling_factor[1]){
            //Cas pour la luminescence
                for(uint8_t k = 0; k < 64; k++){ 
                    uint8_t cr = p_mcu -> l_mcu[i][j * 64 + k]->R;
                    uint8_t cb = p_mcu -> l_mcu[i][j * 64 + k]->B;
                    uint8_t cg = p_mcu -> l_mcu[i][j * 64 + k]->G;
                    p_ycbcr->bloc[i][j][k] = round(0.299 * cr + 0.587 * cg + 0.114 * cb);
                }
            }
            
            else if(j < p_ycbcr->sampling_factor[0] * p_ycbcr->sampling_factor[1] + p_ycbcr->sampling_factor[2] * p_ycbcr->sampling_factor[3]){
            // Cas pour Cb:
                uint8_t coeff_h = p_ycbcr->sampling_factor[0] / p_ycbcr->sampling_factor[2];
                uint8_t coeff_v = p_ycbcr->sampling_factor[1] / p_ycbcr->sampling_factor[3];
                for(uint8_t k = 0; k < 64; k++){
                        //On récupère les pixels qui nous intéressent:
                        uint32_t cr = 0;
                        uint32_t cb = 0;
                        uint32_t cg = 0;
                        for(uint32_t x = 0; x < coeff_v ; x ++){
                            for(uint32_t y = *pos_x_cb; y < *pos_x_cb + coeff_h ; y ++){
                            //Déroulement sur x
                                cr += p_mcu -> l_mcu[i][x * p_ycbcr->sampling_factor[0] *8 + y]->R;
                                cb += p_mcu -> l_mcu[i][x * p_ycbcr->sampling_factor[0] *8 + y]->B;
                                cg += p_mcu -> l_mcu[i][x * p_ycbcr->sampling_factor[0] *8 + y]->G;
                            }
                        }
                        //On les affecte à l'emplacement correspondant (sans oublier la division):

                        p_ycbcr->bloc[i][j][k] =  round(((-0.1687 * cr - 0.3313 * cg + 0.5 * cb)/(coeff_h * coeff_v))+128);

                        
                    
                    //On met à jour la position de notre curseur dans le mcu
                    *pos_x_cb = *pos_x_cb + coeff_h;//Incrémente la position du pointeur
                    if(coeff_v != 1 && *pos_x_cb % (p_ycbcr->sampling_factor[0] *8 ) == 0){
                        //cas où nous avons eu un changement de ligne qui nécessite un saut de ligne
                        *pos_x_cb = *pos_x_cb + p_ycbcr->sampling_factor[0] *8 * (coeff_v-1);//Il faut potentiellement multiplier par 8 ici
                    }
                }         
            }
            else{
                // Cas pour Cr :
                uint8_t coeff_h = p_ycbcr->sampling_factor[0] / p_ycbcr->sampling_factor[4];
                uint8_t coeff_v = p_ycbcr->sampling_factor[1] / p_ycbcr->sampling_factor[5];
                
                for(uint8_t k = 0; k < 64; k++){
                    //On récupère les pixels qui nous intéressent:
                    uint32_t cr = 0;
                    uint32_t cb = 0;
                    uint32_t cg = 0;
                    for(uint32_t x = 0; x < coeff_v ; x ++){
                        for(uint32_t y = *pos_x_cr; y < *pos_x_cr + coeff_h ; y ++){
                        //Déroulement sur x
                            cr += p_mcu -> l_mcu[i][x * p_ycbcr->sampling_factor[0] *8 + y]->R;
                            cb += p_mcu -> l_mcu[i][x * p_ycbcr->sampling_factor[0] *8 + y]->B;
                            cg += p_mcu -> l_mcu[i][x * p_ycbcr->sampling_factor[0] *8 + y]->G;
                        }
                    }
                    //On les affecte à l'emplacement correspondant (sans oublier la division):
                    p_ycbcr->bloc[i][j][k] = round(((0.5 * cr + -0.4187 * cg + -0.0813 * cb)/(coeff_h * coeff_v))+128);

                    //On met à jour la position de notre curseur dans le mcu
                    *pos_x_cr = *pos_x_cr + coeff_h;//Incrémente la position du pointeur
                    if(coeff_v != 1 && *pos_x_cr % (p_ycbcr->sampling_factor[0] *8 ) == 0){
                        //cas où nous avons eu un changement de ligne qui nécessite un saut de ligne
                        *pos_x_cr = *pos_x_cr + p_ycbcr->sampling_factor[0] *8 * (coeff_v-1);
                    }
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
        p_main->bloc[mcu_i] = calloc(p_main->nb_comp, sizeof(int16_t *));
        for(uint8_t comp_i = 0; comp_i<p_main->nb_comp; comp_i ++){
            p_main->bloc[mcu_i][comp_i] = calloc(64, sizeof(int16_t));
        }
    }
    for(uint32_t mcu_i = 0; mcu_i < p_main->n_mcu; mcu_i ++){
        for(uint8_t comp_i = 0; comp_i<p_main->nb_comp; comp_i ++){
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


void encodage_rgb_sub(struct main_mcu_rgb_sub *p_main){
    for(uint32_t mcu_i=0; mcu_i<p_main->n_mcu; mcu_i++){
        int16_t precursor = 0;
        for(uint8_t comp_i = 0; comp_i< p_main->nb_comp; comp_i ++){
            //Calcul des codes rle de toutes les composantes ainsi que desprécurseurs
            uint8_t *R = calloc(64, sizeof(uint8_t));

            uint8_t compteur = 1;


            int16_t tmp = precursor;
            precursor = p_main->bloc[mcu_i][comp_i][0];
            p_main->bloc[mcu_i][comp_i][0] = p_main->bloc[mcu_i][comp_i][0] - tmp;


            uint8_t* taille = calloc(1, sizeof(uint8_t));

            rle(p_main->bloc[mcu_i][comp_i], R, taille);//On écrit dans R l'encodage RLE des valeurs de de la composante en cours (comp_i)

            

            //Encoding DC:
            uint8_t *nb_bits = calloc(1,sizeof(uint8_t));
            
            uint8_t ht_i = 2;//Cas où l'on traite une composante Cb/Cr
            if(comp_i < p_main->sampling_factor[0]* p_main->sampling_factor[1]){
                //Cas où on traite une luminescence
                ht_i = 0;
            }   

            uint32_t huffman_path = huffman_table_get_path(p_main->htable[ht_i], R[0], nb_bits);

            bitstream_write_bits(p_main->blitzstream, huffman_path, *nb_bits, false);
            bitstream_write_bits(p_main->blitzstream, index(p_main->bloc[mcu_i][comp_i][0]), magnitude_table(p_main->bloc[mcu_i][comp_i][0]), false);


                //Encoding AC:
            for (uint8_t i=1; i<64; i++){
                if(p_main->bloc[mcu_i][comp_i][i] != 0){
                    while(R[compteur] == 0xF0){
                        huffman_path = huffman_table_get_path(p_main->htable[ht_i + 1], R[compteur], nb_bits);
                        bitstream_write_bits(p_main->blitzstream, huffman_path, *nb_bits, false);
                        compteur ++;
                    }
                    huffman_path = huffman_table_get_path(p_main->htable[ht_i + 1], R[compteur], nb_bits);
                    bitstream_write_bits(p_main->blitzstream, huffman_path, *nb_bits, false);
                    bitstream_write_bits(p_main->blitzstream, index(p_main->bloc[mcu_i][comp_i][i]), magnitude_table(p_main->bloc[mcu_i][comp_i][i]), false);
                    compteur ++;
                }
            }
            if(compteur == *taille-1){
                huffman_path = huffman_table_get_path(p_main->htable[ht_i + 1], R[compteur], nb_bits);
                bitstream_write_bits(p_main->blitzstream, huffman_path, *nb_bits, false);
            }
            else if(compteur < *taille){
                printf("Erreur dans le compte de Y\n");
            }
        }
    }
}

void creation_table_sub(struct main_mcu_rgb_sub *mcu){
    /*Allocation mémoire pour Y : AC et DC,    et : Cb/Cr : Ac et DC*/
    /* 0 Y Dc, 1 Y Ac, 2 CbCr Dc, 3CbCr Ac*/
    mcu->htable = calloc(4,sizeof(struct huff_table *));
    mcu->htable[0] = huffman_table_build(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]);
    mcu->htable[1] = huffman_table_build(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]);
    mcu->htable[2] = huffman_table_build(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]);
    mcu->htable[3] = huffman_table_build(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]);
}

void write_jpeg_rgb_sub(struct main_mcu_rgb_sub *p_main){
    struct jpeg *p_jpeg = jpeg_create();
    //On rentre les paramètre dans la structure entete set_XXX
    // Caractèristique de l'image à traiter
    jpeg_set_ppm_filename(p_jpeg, p_main->ppm_filename);
    jpeg_set_image_height(p_jpeg, p_main->height);
    jpeg_set_image_width(p_jpeg, p_main->width);
    jpeg_set_nb_components(p_jpeg, p_main->nb_comp);
    
    // Paramètre de l'encodage
    jpeg_set_jpeg_filename(p_jpeg, p_main->jpeg_filename);
    jpeg_set_sampling_factor(p_jpeg, Y, H, p_main->sampling_factor[0]);
    jpeg_set_sampling_factor(p_jpeg, Cb, H, p_main->sampling_factor[2]);
    jpeg_set_sampling_factor(p_jpeg, Cr, H, p_main->sampling_factor[4]);
    jpeg_set_sampling_factor(p_jpeg, Y, V, p_main->sampling_factor[1]);
    jpeg_set_sampling_factor(p_jpeg, Cb, V, p_main->sampling_factor[3]);
    jpeg_set_sampling_factor(p_jpeg, Cr, V, p_main->sampling_factor[5]);

    //Par convention, on fait l'encodage dès maintenant car on crée les tables de huffman dans cette fonction
    creation_table_sub(p_main);

    //Table de Huffman
    jpeg_set_huffman_table(p_jpeg, DC, Y, p_main->htable[0]);
    jpeg_set_huffman_table(p_jpeg, AC, Y, p_main->htable[1]);
    jpeg_set_huffman_table(p_jpeg, DC, Cb, p_main->htable[2]);
    jpeg_set_huffman_table(p_jpeg, AC, Cb, p_main->htable[3]);
    jpeg_set_huffman_table(p_jpeg, DC, Cr, p_main->htable[2]);
    jpeg_set_huffman_table(p_jpeg, AC, Cr, p_main->htable[3]);
    
    //Table de quantification
    jpeg_set_quantization_table(p_jpeg, Y, quantification_table_Y);
    jpeg_set_quantization_table(p_jpeg, Cb, quantification_table_CbCr);
    jpeg_set_quantization_table(p_jpeg, Cr, quantification_table_CbCr);

    //On écrit l'entete dans le fichier

    jpeg_write_header(p_jpeg);
    printf("Header done\n");
    //On récupère le bitstream positioné à la fin du header
    p_main->blitzstream = jpeg_get_bitstream(p_jpeg);

    encodage_rgb_sub(p_main);
    //affiche_encodage_rgb(p_main_rgb);
    //On écrit dans le bitstream sur toutes les valeurs necéssaires
    bitstream_flush(p_main->blitzstream);

    //On écrit le footer
    jpeg_write_footer(p_jpeg);
    jpeg_destroy(p_jpeg);
}

//Fonctions d'affichage:
void affiche_details_image_rgb_sub(struct main_mcu_rgb_sub *mcu) {
    printf("Largeur : %d pixels \n", mcu->width);
    printf("Hauteur : %d pixels \n", mcu->height);
    printf("Max_valeur : %d pixels \n", mcu->max_value);
    for (uint32_t i=0; i<mcu->height; i++) {
        for (uint32_t j =0; j<mcu->width; j++) {
            printf("%x%x%x ", mcu->data[i][j]->R,mcu->data[i][j]->G ,mcu->data[i][j]->B );
        }
        printf("\n");
    }
}

void affiche_img_mcu_rgb_sub(struct image_mcu_rgb_sub *p_gmu, uint8_t h1, uint8_t v1){
    /*Affiche les éléments de chaques MCU*/
    for(uint32_t j = 0; j<20; j++){
        printf("----- MCU numéro %u ----- \n", j);
        for(uint8_t i = 0; i<64*h1*v1; i++){
            printf("%x%x%x ", p_gmu->l_mcu[j][i]->R, p_gmu->l_mcu[j][i]->G, p_gmu->l_mcu[j][i]->B);
            if(i % 8 == 7){
                printf("\n");
            }
        }
    }
}

void afficher_YCbCr_sub(struct image_YCbCr_sub *p_ycbcr){
    //Parcours les éléments de image_YCbCr et affiche le Y
    for(uint32_t j = 0; j<20; j++){
        printf("----- MCU numéro %u YCbCr----- \n", j);
        for(uint8_t comp_i = 0; comp_i< p_ycbcr->nb_comp; comp_i ++){
            printf("comp_i = %u : \n", comp_i);
            for(uint8_t i = 0; i<64; i++){
                printf("%x ", ( int32_t) p_ycbcr->bloc[j][comp_i][i]);
                if(i % 8 == 7){
                    printf("\n");
                }
            }
        }
    }
}

void affiche_bloc_rgb_sub(struct main_mcu_rgb_sub *main_mcu){
    for(uint32_t i = 0; i<20; i++){
        printf("-------mcu : %u --------\n", i);
        for(uint8_t comp_i=0; comp_i<main_mcu->nb_comp; comp_i ++){
            printf("Comp %u\n", comp_i);
            for(uint8_t j = 0; j<64; j++){
                printf("%x ", main_mcu->bloc[i][comp_i][j]);
                if(j%8  == 7){
                    printf("\n");
                }
            }
        }
    }
}