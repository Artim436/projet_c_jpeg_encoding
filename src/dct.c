#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dct.h>
#include <qtables.h>
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

float coef_dct(bloc_8x8 *S, int i, int j, int n){ // coefficient of DCT matrix (ne pas oublier de faire-128 au coef dans le dct du cours)
    float pi = 3.14159265358979323;
    float phi = 0.0;
    for (int x=0; x<n; x++){
        for (int y=0; y<n; y++){
            S->matrix_bloc[x][y] -= 128; 
            phi = phi + S->matrix_bloc[x][y]*cos((((2.0*x)+1)*(float)i*pi)/(2.0*(float)n))*cos((((2.0*y)+1)*(float)j*pi)/(2.0*(float)n));
        }
    }
    phi*=0.25;
    phi*=C_function(i);
    phi*=C_function(j);
    return phi;
}



int dct(bloc_8x8 *S){ // DCT matrix
    int n=8;
    struct bloc_8x8 *I;
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            I->matrix_bloc[i][j] = coef_dct(S, i, j, n);  // I is a middle matrix use to changeall coef with dct at the same time (first change doesn't have to influence others)
        }
    }
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            S->matrix_bloc[i][j] = I->matrix_bloc[i][j];
        }
    }
    return 0;
}

// int coef_quantization(matrix D, matrix Q, int i, int j){ // quantization matrix
//     D[i][j] = D[i][j]+floor(Q[i][j]/(float) 2.0); //F[i][j] = floor(D[i][j]/Q[i][j])
//     D[i][j] = D[i][j]/Q[i][j];
//     return floor(D[i][j]);
// }

// int quantization(matrix D, matrix Q){ // quantization matrix
//     int n=8;
//     matrix I;
//     for (int i=0; i<n; i++){
//         for (int j=0; j<n; j++){
//             I[i][j] = coef_quantization(D, Q, i, j);
//         }
//     }
//     for (int i=0; i<n; i++){
//         for (int j=0; j<n; j++){
//             D[i][j] = I[i][j];
//         }
//     }
//     return 0;
// }


// int coef_quantization(bloc_64 *D, int i){ // quantization vector
//     D[i] = D[i]/quantification_table_Y[i]; //D = D/Y
//     return (int)D[i];  //better than floor for numbers between -1 and 0 (we want 0 and not 1)
// }

// int quantization(bloc_64 *D){ // quantization vector
//     for (int i =0; i<64; i++){
//         D[i] = coef_quantization(D, i);
//     }
//     return 0;
//     }


// int zigzag(bloc_8x8 *D, bloc_64 *F){ // zigzag matrix
//     int cpt = 0;
//     int len = 8;
//     for (int k = 0; k<2*len-1; k++){
//         if (k%2 == 0){
//             for (int j = max(0, k-len+1); j<=min(k,len-1); j++){
//                 F[cpt] = D[k-j][j];
//                 cpt++;
//             }
//         }
//         else{
//             for (int j = max(0, k-len+1); j<=min(k,len-1); j++){
//                 F[cpt] = D[j][k-j];
//                 cpt++;
//             }
//         }
//     }
//     return 0;
// }

// int main(){
//     struct bloc_8x8 *tab;
//     float test[8][8]  = {{139,144,149,153,155,155,155,155},
//                   {144,151,153,156,159,156,156,156},
//                   {150,155,160,163,158,156,156,156},
//                   {159,161,162,160,160,159,159,159},
//                   {161,161,161,161,160,157,157,157},
//                   {162,162,161,163,162,157,157,157},
//                   {162,162,161,161,163,158,158,158},
//                   {159,160,161,162,162,155,155,155}
//                   };
//     for (int i=0; i<8; i++){CU.c:84
//         for (int j=0; j<8; j++){
//             tab->matrix_bloc[i][j] = test[i][j];
//         }
//     }
//     if (argc == 2) {
//         struct image_pgm *pgm = malloc(sizeof(struct image_pgm));
//         char *file_name = argv[1];
//         process_file(pgm, file_name);
//         affiche_details_image(pgm, file_name);
//         struct image_mcu *img_mcu = decoupe_mcu_8x8(pgm);
//         affiche_img_mcu(img_mcu);
//         struct image_YCbCr *p_ycbcr = convert_YCbCr(img_mcu);
//         afficher_YCbCr(p_ycbcr);
//     } else {
//         printf("Il faut passer en paramètre le nom d'un fichier image valide \n");
//     }
// },
                  
    // matrix Q = {
    //     {16,11,10,16,24,40,51,61},
    //               {12,12,14,19,26,58,60,55},
    //               {14,13,16,24,40,57,69,56},
    //               {14,17,22,29,51,87,80,62},
    //               {18,22,37,56,68,109,103,77},
    //               {24,35,55,64,81,104,113,92},
    //               {49,64,78,87,103,121,120,101},
    //               {72,92,95,98,112,100,103,99}
    //               };
    // matrix Qtest = {
    //               {16,11,10,16,24,40,51,61},
    //               {12,12,14,19,26,58,60,55},
    //               {14,13,16,24,40,57,69,56},
    //               {14,17,22,29,51,87,80,62},
    //               {18,22,37,56,68,109,103,77},
    //               {24,35,55,64,81,104,113,92},
    //               {49,64,78,87,103,121,120,101},
    //               {72,92,95,98,112,100,103,99}
    //};  
//     printf("------------------matrix--------------\n");
//     for (int i=0; i<8; i++){
//         for (int j=0; j<8; j++){
//             printf("%i ", (int)tab->matrix_bloc[i][j]);
//         }
//         printf("\n");
//     }           
//     dct(tab);
//     printf("-----------dct matrix-----------\n");
//     for (int i=0; i<8; i++){
//         for (int j=0; j<8; j++){
//             printf("%i ", (int)tab->matrix_bloc[i][j]);
//         }
//         printf("\n");
//     }
// }
//     printf("\n");
//     zigzag(tab, F);
//     printf("-----------zigzag dct matrix-----------\n");
//     for (int i=0; i<64; i++){
//         printf("%i ", (int)F[i]);
//     }
//     printf("\n");
//     quantization(F);
//     printf("-----------quantization table-----------\n");
//     for (int i=0; i<64; i++){
//         printf("%i ", (int)quantification_table_Y[i]);
//     }
//     printf("\n");
//     printf("-----------zigzag quantization matrix-----------\n");
//     for (int i=0; i<64; i++){
//         printf("%i ", (int)F[i]);
//     }
//     return 0;
// }

