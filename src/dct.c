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
#include <jpeg_writer.h>
#include <test.h>
#include <huffman.h>

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
    float pi = 3.14159265358979323;
    float phi = 0.0;
    for (int x=0; x<n; x++){
        for (int y=0; y<n; y++){ 
            float s = S[x][y]; 
            phi += s*cos((((2.0*x)+1)*(float)i*pi)/(2.0*(float)n))*cos((((2.0*y)+1)*(float)j*pi)/(2.0*(float)n));
        }
    }
    return phi*0.25*C_function(i)*C_function(j);
}



void dct(float** S){ // DCT matrixs
    int n=8;
    bloc_8x8_dtc *I = malloc(sizeof(bloc_8x8_dtc));
    for (int x=0; x<n; x++){
        for (int y=0; y<n; y++){
            S[x][y]-=128.0; // (*S)
        }
    }
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            I->matrix_bloc[i][j] = coef_dct(S, i, j, n);  // I is a middle matrix use to changeall coef with dct at the same time (first change doesn't have to influence others)
        }
    }
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            S[i][j] = I->matrix_bloc[i][j];
        }
    }
}


int coef_quantization(int16_t *D, int i){ // quantization vector
    D[i] = (float) D[i]/quantification_table_Y[i]; //D = D/Y
    return (int16_t) D[i];  //better than floor for numbers between -1 and 0 (we want 0 and not 1)
}

void quantization(int16_t *D){ // quantization vector
    for (int i =0; i<64; i++){
        D[i] = coef_quantization(D, i);
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
    //On applique les dtc
    for (uint32_t k =0; k<main_mcu->n_mcu; k++){
        float** p_mat= convert_YCbCr_mat(im_ycbcr->l_ycbcr[k]);
        print_mat(p_mat, 8);
        printf("\n");
        dct(p_mat);
        printf("après dct\n");
        print_mat(p_mat, 8);
        printf("on est ici\n");
        zigzag(p_mat, main_mcu->bloc[k]);
        quantization(main_mcu->bloc[k]);
        uint8_t *R = calloc(64, sizeof(uint8_t));
        rle(main_mcu->bloc[k], R);
        for (int i=0; i<64; i++){
            printf(" value: ");
            printf(" %d ", main_mcu->bloc[k][i]);
            printf(" magnitude: ");
            printf(" %d ", magnitude_table_AC(main_mcu->bloc[k][i]));
            printf(" index: ");
            printf(" %d ", index(main_mcu->bloc[k][i]));
            printf(" rle: ");
            printf(" %d ", R[i]);
            printf("\n");
        }
    }   
}
 
void affiche_bloc(struct main_mcu *main_mcu){
    for(uint8_t i = 0; i<main_mcu->n_mcu; i++){
        uint8_t k = 0;
        for(uint8_t j = 0; j<64; j++){
            printf("%x ", main_mcu->bloc[i][j]);
            if(k != 0){
                printf("\n");
                k = (k+1)%8;
            }
        }
    }
    printf("\n");
}

// int main(){
//     float **p_mat;
//     p_mat = calloc(8, sizeof(float* ));
//     float test[8][8]  = {{0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00},
//                         {0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00},
//                         {0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00},
//                         {0xff,0xff,0x00,0xff,0xff,0x00,0xff,0xff},
//                         {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
//                         {0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00},
//                         {0x00,0xff,0x00,0xff,0xff,0x00,0xff,0x00},
//                         {0xff,0x00,0xff,0x00,0x00,0xff,0x00,0xff}
//                   };
//     for (int i=0; i<8; i++){
//         p_mat[i] = calloc(8, sizeof(float));
//         for (int j=0; j<8; j++){
//             p_mat[i][j] = test[i][j];
//         }
//     }             
//     printf("------------------matrix--------------\n");
//     for (int i=0; i<8; i++){
//         for (int j=0; j<8; j++){
//             printf("%i ", (short)test[i][j]);
//         }
//         printf("\n");
//     }           
//     dct(p_mat);
//     printf("-----------dct matrix-----------\n");
//     for (int i=0; i<8; i++){
//         for (int j=0; j<8; j++){
//             printf("%x ", (int16_t)p_mat[i][j]);
//         }
//         printf("\n");
//     }
//     printf("\n");
//     int16_t *F = calloc(64, sizeof(int16_t));
//     zigzag(p_mat, F);
//     printf("-----------zigzag dct matrix-----------\n");
//     for (int i=0; i<64; i++){
//         printf("%x ", (int)F[i]);
//     }
//     printf("\n");
//     quantization(F);
//     printf("-----------quantization table-----------\n");
//     for (int i=0; i<64; i++){
//         printf("%x ", (short)quantification_table_Y[i]);
//     }
//     printf("\n");
//     printf("-----------zigzag quantization matrix-----------\n");
//     for (int i=0; i<64; i++){
//         printf("%x ", (short)F[i]);
//     }
//     printf("\n");
//     return 0;
// }
