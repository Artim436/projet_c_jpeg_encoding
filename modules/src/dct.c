#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dct.h>
#include <structure.h>
#include <MCU.h>
#include <convert_to_YCbCr.h>
#include <lecture_pgm.h>
#include <jpeg_writer_maison.h>
#include <test.h>
#include <huffman_maison.h>
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

float coef_dct_v2(float** S, int i, int j, int n){ // coefficient of DCT matrix with cos storage
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
    // dct function on a matrix of size 8x8 with cos storage
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
    free(I);
}

void dct_zone(float **S){
    // fonction which optimize dct by doing it on a zone of the image, we don't use it beacause of invader.pgm
    uint8_t n=8;
    float **I = calloc(8, sizeof(float*));
    for (uint8_t i=0; i<n; i++){
        I[i] = calloc(8, sizeof(float));
    }
    for (uint8_t x=0; x<n; x++){
        for (uint8_t y=0; y<n; y++){
            S[x][y]-=128.0;
        }
    }
    for (uint8_t i=0; i<n; i++){
        for (uint8_t j=0; j<n; j++){
            if ( i+j < n){
                I[i][j] = coef_dct_v2(S, i, j, n);  // I is a middle matrix use to changeall coef with dct at the same time (first change doesn't have to influence others)
            }
            else{
                I[i][j] = 0.0;
            }
        }
    }
    for (uint8_t i=0; i<n; i++){
        for (uint8_t j=0; j<n; j++){
            S[i][j] = I[i][j];
        }
    }
    free(I);
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
    // transform a matrix in a vector with the zigzag order
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
    // fonction which do all the job for pgm images
    //On prépare la structure de bloc
    main_mcu->bloc = calloc(main_mcu->n_mcu, sizeof(float *));
    for (uint32_t i=0; i<main_mcu->n_mcu; i++) {
        main_mcu->bloc[i] = calloc(64, sizeof(float));
    }
    //On applique les dtc
    for (uint32_t k =0; k<main_mcu->n_mcu; k++){
        float** p_mat= convert_YCbCr_mat(im_ycbcr->l_ycbcr[k]);
        dct(p_mat);
        zigzag(p_mat, main_mcu->bloc[k]);
        quantization_Y(main_mcu->bloc[k]);
    }  
}


void fonction_RGB(struct main_mcu_rgb *main_mcu_rgb, struct image_YCbCr *im_ycbcr){
    // fonction which do all the job for rgb images
    //On prépare la structure de bloc
    main_mcu_rgb->bloc = calloc(main_mcu_rgb->n_mcu, sizeof(float *));
    for (uint32_t i=0; i<main_mcu_rgb->n_mcu; i++) {
        main_mcu_rgb->bloc[i] = calloc(3, sizeof(float*));
        main_mcu_rgb->bloc[i][0] = calloc(64, sizeof(float));
        main_mcu_rgb->bloc[i][1] = calloc(64, sizeof(float));
        main_mcu_rgb->bloc[i][2] = calloc(64, sizeof(float));
    }
    //On applique les dtc
    for (uint32_t k =0; k<main_mcu_rgb->n_mcu; k++){
        float*** p_mat_rgb= convert_YCbCr_mat_rgb(im_ycbcr->l_ycbcr[k]);
        dct(p_mat_rgb[0]);//Matrice des Y
        dct(p_mat_rgb[1]);//Matrice des Cb
        dct(p_mat_rgb[2]);//Matrice des Cr

        zigzag(p_mat_rgb[0], main_mcu_rgb->bloc[k][0]);
        zigzag(p_mat_rgb[1], main_mcu_rgb->bloc[k][1]);
        zigzag(p_mat_rgb[2], main_mcu_rgb->bloc[k][2]);

        quantization_Y(main_mcu_rgb->bloc[k][0]);
        quantization_C(main_mcu_rgb->bloc[k][1]);
        quantization_C(main_mcu_rgb->bloc[k][2]);

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

void affiche_bloc_rgb(struct main_mcu_rgb *main_mcu){
    for(uint32_t i = 3466; i<3467; i++){
        printf("-------mcu : %u --------\n", i);
        printf("Composante Y :  \n");
        for(uint8_t j = 0; j<64; j++){
            printf("%x ", main_mcu->bloc[i][0][j]);
            if(j%8  == 7){
                printf("\n");
            }
        }
        printf("Composante Cb :  \n");
        for(uint8_t j = 0; j<64; j++){
            printf("%x ", main_mcu->bloc[i][1][j]);
            if(j%8  == 7){
                printf("\n");
            }
        }
        printf("Composante Cr :  \n");
        for(uint8_t j = 0; j<64; j++){
            printf("%x ", main_mcu->bloc[i][2][j]);
            if(j%8  == 7){
                printf("\n");
            }
        }
    }

}

