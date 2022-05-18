#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dct.h>
#include <structure.h>
#include <convert_to_YCbCr.h>
#include <qtables.h>
#include <costable.h>

#define max(a,b) (a>=b?a:b) //if a>=b return a else return b
#define min(a,b) (a<=b?a:b) //if a<=b return a else return b

/*Note : enlever la déclaration de I en bloc et plutot mettre en float** .*/

float C_function(int i){  // middle function for DCT
    if (i == 0){
        return 1/(float)sqrt(2.0);
    }
    else{
        return 1.0;
    }
}

float coef_dct(float** S, int i, int j, int n){ // coefficient of DCT matrix (ne pas oublier de faire-128 au coef dans le dct du cours)
    float pi = 3.14159265358979323846264338;//159265358979323846264338
    float phi = 0.0;
    for (uint8_t x=0; x<n; x++){
        for (uint8_t y=0; y<n; y++){ 
            float s = S[x][y]; 
            phi += s*cos((((2.0*x)+1)*(float)i*pi)/(2.0*(float)n))*cos((((2.0*y)+1)*(float)j*pi)/(2.0*(float)n));
        }
    }
    return phi*0.25*C_function(i)*C_function(j);
}

float coef_dct_v2(float** S, int i, int j, int n){ // coefficient of DCT matrix (ne pas oublier de faire-128 au coef dans le dct du cours)
    float phi = 0.0;
    for (uint8_t x=0; x<n; x++){
        for (uint8_t y=0; y<n; y++){ 
            float s = S[x][y]; 
            phi += s*costables[8*x+i][8*y+j];
        }
    }
    return phi*0.25*C_function(i)*C_function(j);
}


void dct(float** S){ // DCT matrixs
    uint8_t n=8;
    bloc_8x8_dtc *I = malloc(sizeof(bloc_8x8_dtc));
    for (uint8_t x=0; x<n; x++){
        for (uint8_t y=0; y<n; y++){
            S[x][y]-=128.0;
        }
    }
    for (uint8_t i=0; i<n; i++){
        for (uint8_t j=0; j<n; j++){
            I->matrix_bloc[i][j] = coef_dct_v2(S, i, j, n);  // I is a middle matrix use to changeall coef with dct at the same time (first change doesn't have to influence others)
        }
    }
    for (uint8_t i=0; i<n; i++){
        for (uint8_t j=0; j<n; j++){
            S[i][j] = I->matrix_bloc[i][j];
        }
    }
}


int coef_quantization_Y(int16_t *D, int i){ // quantization vector
    D[i] = (float) D[i]/quantification_table_Y[i]; //D = D/Y
    return (int) D[i];  //better than floor for numbers between -1 and 0 (we want 0 and not 1)
}

void quantization_Y(int16_t *D){ // quantization vector
    for (int i =0; i<64; i++){
        D[i] = coef_quantization_Y(D, i);
    }
}

int coef_quantization_C(int16_t *D, int i){ // quantization vector
    D[i] = (float) D[i]/quantification_table_CbCr[i]; //D = D/C
    return (int) D[i];  //better than floor for numbers between -1 and 0 (we want 0 and not 1)
}

void quantization_C(int16_t *D){ // quantization vector
    for (int i =0; i<64; i++){
        D[i] = coef_quantization_C(D, i);
    }
}

void zigzag(float** D, int16_t* F){ // zigzag matrix
    int cpt = 0;
    int len = 8;
    for (int k = 0; k<2*len-1; k++){
        if (k%2 == 0){
            for (int j = max(0, k-len+1); j<=min(k,len-1); j++){
                F[cpt] = (int16_t) D[k-j][j];
                cpt++;
            }
        }
        else{
            for (int j = max(0, k-len+1); j<=min(k,len-1); j++){
                F[cpt] = (int16_t) D[j][k-j];
                cpt++;
            }
        }
    }
}


void fonction(struct main_mcu *main_mcu, struct image_YCbCr *im_ycbcr){
    //On prépare la structure de bloc
    main_mcu->bloc = calloc(main_mcu->n_mcu, sizeof(float *));
    for (uint32_t i=0; i<main_mcu->n_mcu; i++) {
        main_mcu->bloc[i] = calloc(64, sizeof(float));
    }
    //On applique les dtc, zigzag et quantification
    for (uint32_t k =0; k<main_mcu->n_mcu; k++){
        float** p_mat= convert_YCbCr_mat(im_ycbcr->l_ycbcr[k]);
        dct(p_mat);
        zigzag(p_mat, main_mcu->bloc[k]);
        quantization_Y(main_mcu->bloc[k]);
    }  
}


void fonction_rgb_sub(struct main_mcu_rgb_sub *p_main, struct image_YCbCr_sub *p_ycbcr){
    /*Fonction qui applique les dtc, zigzag et quantification et range les résultats dans la structure main*/
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

void affiche_bloc(struct main_mcu *main_mcu){
    for(uint32_t i = 0; i<20; i++){
        printf("-------mcu : %u --------\n", i);
        uint8_t k = 0;
        for(uint8_t j = 0; j<64; j++){
            printf("%x ", main_mcu->bloc[i][j]);
            k = (k+1)%8;
            if(k  == 0){
                printf("\n");
            }
        }
        printf("\n");
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



float **convert_mat_sub(float *p_YCbCr){
    /*Prend en entrée un vecteur de taille 64 et renvoie une matrice de taille 8x8*/
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

float **convert_YCbCr_mat(float *p_YCbCr){
    /*Prend en entrée un vecteur de taille 64 et renvoie une matrice de taille 8x8*/
    float **matrice = malloc(8*sizeof(float*));
    for(uint8_t j = 0;j<8; j++){
        matrice[j] = malloc(8*sizeof(float));
        for(uint8_t k=0; k<8; k++){
            matrice[j][k] = (float) p_YCbCr[j*8+k];
        }
    }
    return matrice;
}
