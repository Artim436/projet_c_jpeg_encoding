#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <MCU.h>
#include <convert_to_YCbCr.h>
#include <math.h>
#include <structure.h>


/*Amélioration : -chercher le nb de bits nécessaires pour YCbCr
-Faire des free()*/

struct image_YCbCr *creation_YCbCr(struct image_mcu *p_mcu){
    /*Création d'un YCbCr avec initialisation des valeurs*/

    struct image_YCbCr *p_ycbcr = malloc( sizeof(struct image_YCbCr));
    
    p_ycbcr->nmcu = p_mcu->nmcu;
    
    p_ycbcr->l_ycbcr = calloc(p_ycbcr->nmcu, sizeof(char *));

    for (uint32_t i=0; i<p_mcu->nmcu; i++) {
            p_ycbcr->l_ycbcr[i] = calloc(64, sizeof(char *));
    }
    return p_ycbcr;
}

struct image_YCbCr *convert_YCbCr(struct image_mcu *p_mcu){
    /*Prend en entrée le découpage MCU et transforme les RGB en YCbCr.
    La valeur retournée sera un pointeur vers l'image convertie en YCbCr et toujours découpée en MCU.
    Cette fonction traite dans un premier temps le cas où l'image est en noir et blanc (P5), on pourra l'améliorer à base de if.*/

    struct image_YCbCr *p_ycbcr = creation_YCbCr(p_mcu);

    for(uint32_t i = 0; i < p_ycbcr->nmcu; i++){
        for(uint8_t j = 0; j<64; j++){
            struct YCbCr *new_ycbcr= calloc(1, sizeof(struct YCbCr));
            uint8_t coeff = p_mcu->l_mcu[i][j];
            new_ycbcr->Y = round(0.299 * coeff + 0.587 * coeff + 0.114 * coeff);
            new_ycbcr->Cb = round(-0.1687 * coeff + -0.3313 * coeff + 0.5 * coeff+ 128);
            new_ycbcr->Cr = round(0.5 * coeff + -0.4187 * coeff + -0.0813 * coeff+ 128);
            p_ycbcr->l_ycbcr[i][j] = new_ycbcr;
        }
    }
    return p_ycbcr;
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
        for(uint8_t j = 0; j< p_ycbcr->nb_comp; j++){
            uint32_t *pos_x_cb = malloc(sizeof(uint32_t));
            uint32_t *pos_x_cr = malloc(sizeof(uint32_t));
            uint32_t limite_cb ; 
            uint32_t limite_cr ; 
            if( j >= p_ycbcr->sampling_factor[0] * p_ycbcr->sampling_factor[1]){
                //Cas où l'on traite des cb / cr (évite les calculs inutiles)
                uint8_t new_j = j - p_ycbcr->sampling_factor[0] * p_ycbcr->sampling_factor[1];
                uint8_t new_h = new_j - p_ycbcr->sampling_factor[2] * p_ycbcr->sampling_factor[3];
                uint8_t coeff_h = p_ycbcr->sampling_factor[0] / p_ycbcr->sampling_factor[2];
                uint32_t pos_cb = (new_j%p_ycbcr->sampling_factor[2]) * 8 + (new_j / p_ycbcr->sampling_factor[2]) * 64 * p_ycbcr->sampling_factor[0];

                uint32_t pos_cr = (new_h%p_ycbcr->sampling_factor[4]) * 8 + (new_h / p_ycbcr->sampling_factor[4]) * 64 * p_ycbcr->sampling_factor[0];
                pos_x_cb = &pos_cb;
                pos_x_cr = &pos_cr;
                
                limite_cb = (pos_cb+coeff_h * 8-1) %(p_ycbcr->sampling_factor[0] * 8);
                coeff_h = p_ycbcr->sampling_factor[0] / p_ycbcr->sampling_factor[4];
                limite_cr = (pos_cr+coeff_h * 8-1) %(p_ycbcr->sampling_factor[0] * 8);
            }


            p_ycbcr->bloc[i][j] = calloc(64, sizeof(float));
            if(j < p_ycbcr->sampling_factor[0]*p_ycbcr->sampling_factor[1]){
            //Cas pour la luminescence
                for(uint8_t k = 0; k < 64; k++){
                    uint32_t pos =  (8*p_ycbcr->sampling_factor[0]) * (k/8)  + (k%8) + (j % p_ycbcr->sampling_factor[0]) * 8 + (j / p_ycbcr->sampling_factor[0]) * 64 * p_ycbcr->sampling_factor[0];
                    // 1 : calcul la ligne , 2 : calcule la pos dans la ligne, 3 : décale ou non de 8, 4 : défini la position de départ du curseur
                    uint8_t cr = p_mcu -> l_mcu[i][pos]->R;
                    uint8_t cb = p_mcu -> l_mcu[i][pos]->B;
                    uint8_t cg = p_mcu -> l_mcu[i][pos]->G;
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
                    if(*pos_x_cb % (p_ycbcr->sampling_factor[0] *8 ) == 0 || *pos_x_cb % (p_ycbcr->sampling_factor[0] *8 ) > limite_cb ){
                        //cas où nous avons eu un changement de ligne qui nécessite un saut de ligne
                        *pos_x_cb = *pos_x_cb - (8*coeff_h) + p_ycbcr->sampling_factor[0] *8 * coeff_v;//Il faut potentiellement multiplier par 8 ici
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
                    if(*pos_x_cr % (p_ycbcr->sampling_factor[0] *8 ) == 0 || *pos_x_cr % (p_ycbcr->sampling_factor[0] *8 ) > limite_cr ){
                        //cas où nous avons eu un changement de ligne qui nécessite un saut de ligne
                        *pos_x_cr = *pos_x_cr - (8*coeff_h) + p_ycbcr->sampling_factor[0] *8 * coeff_v;//Il faut potentiellement multiplier par 8 ici
                    }
                }          
            }
        }
    }
    return p_ycbcr;
}


/*Fonction d'affichages*/
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

void afficher_YCbCr(struct image_YCbCr *p_ycbcr){
    //Parcours les éléments de image_YCbCr et affiche le Y
    for(uint32_t j = 0; j<20; j++){
        printf("----- MCU numéro %u YCbCr----- \n", j);
        printf("Affichage de Y: \n");
        for(uint8_t i = 0; i<64; i++){
            printf("%x ", p_ycbcr->l_ycbcr[j][i]->Y);
            if(i % 8 == 7){
                printf("\n");
            }
        }
    }
}


/*Fonction de nettoyages*/

void clean_image_YCbCr_sub(struct image_YCbCr_sub *ycbcr) {
    for (uint32_t i = 0; i<ycbcr->n_mcu; i++) {
        for (uint32_t j = 0; j<ycbcr->nb_comp; j++) {
            free(ycbcr->bloc[i][j]);
        }
        free(ycbcr->bloc[i]);
    }
    free(ycbcr->bloc);
    free(ycbcr);
}
