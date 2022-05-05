#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dct.h>
#include <structure.h>

#define max(a,b) (a>=b?a:b) //if a>=b return a else return b
#define min(a,b) (a<=b?a:b) //if a<=b return a else return b


float C_function(int i){  // middle function for DCT
    if (i == 0){
        return 1/(float)sqrt(2.0);
    }
    else{
        return 1.0;
    }
}

float coef_dct(float *S[8][8], int i, int j, int n){ // coefficient of DCT matrix (ne pas oublier de faire-128 au coef dans le dct du cours)
    float pi = 3.14159265358979323;
    float phi = 0.0;
    for (int x=0; x<n; x++){
        for (int y=0; y<n; y++){ 
            float s = (*S)[x][y];  //(*S).matrix_bloc[x][y]
            phi += s*cos((((2.0*x)+1)*(float)i*pi)/(2.0*(float)n))*cos((((2.0*y)+1)*(float)j*pi)/(2.0*(float)n));
        }
    }
    return phi*0.25*C_function(i)*C_function(j);
}



int dct(float *S[8][8]){ // DCT matrixs
    int n=8;
    bloc_8x8_dtc *I = malloc(sizeof(bloc_8x8_dtc));
    for (int x=0; x<n; x++){
        for (int y=0; y<n; y++){
            (*S)[x][y]-=128.0; // (*S)
        }
    }
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            I->matrix_bloc[i][j] = coef_dct(S, i, j, n);  // I is a middle matrix use to changeall coef with dct at the same time (first change doesn't have to influence others)
        }
    }
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            (*S)[i][j] = I->matrix_bloc[i][j];
        }
    }
    return 0;
}


int coef_quantization(float *D[64], int i){ // quantization vector
    (*D)[i] = (*D)[i]/quantification_table_Y[i]; //D = D/Y
    return (int)(*D)[i];  //better than floor for numbers between -1 and 0 (we want 0 and not 1)
}

int quantization(float *D[64]){ // quantization vector
    for (int i =0; i<64; i++){
        (*D)[i] = coef_quantization(D, i);
    }
    return 0;
    }


int zigzag(float *D[8][8], float *F[64]){ // zigzag matrix
    int cpt = 0;
    int len = 8;
    for (int k = 0; k<2*len-1; k++){
        if (k%2 == 0){
            for (int j = max(0, k-len+1); j<=min(k,len-1); j++){
                (*F)[cpt] = (*D)[k-j][j];
                cpt++;
            }
        }
        else{
            for (int j = max(0, k-len+1); j<=min(k,len-1); j++){
                (*F)[cpt] = (*D)[j][k-j];
                cpt++;
            }
        }
    }
    return 0;
}

void fonction(struct main_mcu *main_mcu, struct image_YCbCr *im_ycbcr){
    
    //On prépare la structure de bloc
    main_mcu->bloc = calloc(main_mcu->n_mcu, sizeof(char *));
    for (uint32_t i=0; i<main_mcu->n_mcu; i++) {
            main_mcu->bloc[i] = calloc(64, sizeof(float));
    }
    printf("on a fini les callocs\n");
    //On applique les dtc
    for (uint32_t k =0; k<main_mcu->n_mcu; k++){
        float (*p_mat)[8][8] = convert_vect_ycbcr_to_mat(im_ycbcr->l_ycbcr[k]);
        print_mat(*p_mat);
        dct(p_mat);
        zigzag(p_mat, main_mcu->bloc[k]);
        quantization(main_mcu->bloc[k]);
    }   
}
 
void affiche_bloc(struct main_mcu *main_mcu){
    for(uint8_t i = 0; i<main_mcu->n_mcu; i++){
        for(uint8_t j = 0; j<64; j++){
            printf("%f ", main_mcu->bloc[i][j]);
        }
        printf("\n");
    }
}

// int main(){
//     bloc_8x8_dtc *tab = malloc(sizeof(struct bloc_8x8_dtc));
//     float test[8][8]  = {{139,144,149,153,155,155,155,155},
//                   {144,151,153,156,159,156,156,156},
//                   {150,155,160,163,158,156,156,156},
//                   {159,161,162,160,160,159,159,159},
//                   {161,161,161,161,160,157,157,157},
//                   {162,162,161,163,162,157,157,157},
//                   {162,162,161,161,163,158,158,158},
//                   {159,160,161,162,162,155,155,155}
//                   };
//     for (int i=0; i<8; i++){
//         for (int j=0; j<8; j++){
//             tab[i][j] = test[i][j];
//         }
//     }             
//     printf("------------------matrix--------------\n");
//     for (int i=0; i<8; i++){
//         for (int j=0; j<8; j++){
//             printf("%i ", (short)tab[i][j]);
//         }
//         printf("\n");
//     }           
//     dct(tab);
//     printf("-----------dct matrix-----------\n");
//     for (int i=0; i<8; i++){
//         for (int j=0; j<8; j++){
//             printf("%i ", (short)tab[i][j]);
//         }
//         printf("\n");
//     }
//     printf("\n");
//     bloc_64_dtc *F = malloc(sizeof(struct bloc_64_dtc));
//     zigzag(tab, F);
//     printf("-----------zigzag dct matrix-----------\n");
//     for (int i=0; i<64; i++){
//         printf("%i ", (int)F[i]);
//     }
//     printf("\n");
//     quantization(F);
//     printf("-----------quantization table-----------\n");
//     for (int i=0; i<64; i++){
//         printf("%i ", (short)quantification_table_Y[i]);
//     }
//     printf("\n");
//     printf("-----------zigzag quantization matrix-----------\n");
//     for (int i=0; i<64; i++){
//         printf("%i ", (short)F[i]);
//     }
//     printf("\n");
//     return 0;
// }
