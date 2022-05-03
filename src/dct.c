#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <qtables.h>
#include <dct.h>

typedef float matrix[8][8];

float C_function(int i){  // middle function for DCT
    if (i == 0){
        return 1/(float)sqrt(2.0);
    }
    else{
        return 1.0;
    }
}

float coef_dct(matrix S, int i, int j, int n){ // coefficient of DCT matrix
    float pi = 3.14159265358979323;
    float phi = 0.0;
    for (int x=0; x<n; x++){
        for (int y=0; y<n; y++){
            phi = phi + S[x][y]*cos((((2.0*x)+1)*(float)i*pi)/(2.0*(float)n))*cos((((2.0*y)+1)*(float)j*pi)/(2.0*(float)n));
        }
    }
    phi*=0.25;
    phi*=C_function(i);
    phi*=C_function(j);
    return phi;
}



float dct(matrix S){ // DCT matrix
    int n=8;
    matrix D;
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            D[i][j] = coef_dct(S, i, j, n);
        }
    }
    return 0;
}

float quantization(matrix D, matrix Q, int i, int j){ // quantization matrix
    D[i][j] = D[i][j]+floor(Q[i][j]/(float) 2.0); //F[i][j] = floor(D[i][j]/Q[i][j])
    D[i][j] = D[i][j]/Q[i][j];
    return floor(D[i][j]);
}



int main(){
    matrix S;
    matrix L;
    matrix tab = {
        {139,144,149,153,155,155,155,155},
                  {144,151,153,156,159,156,156,156},
                  {150,155,160,163,158,156,156,156},
                  {159,161,162,160,160,159,159,159},
                  {159,160,161,162,162,155,155,155},
                  {161,161,161,161,160,157,157,157},
                  {162,162,161,163,162,157,157,157},
                  {162,162,161,161,163,158,158,158}
                  };
    matrix Q = {
        {16,11,10,16,24,40,51,61},
                  {12,12,14,19,26,58,60,55},
                  {14,13,16,24,40,57,69,56},
                  {14,17,22,29,51,87,80,62},
                  {18,22,37,56,68,109,103,77},
                  {24,35,55,64,81,104,113,92},
                  {49,64,78,87,103,121,120,101},
                  {72,92,95,98,112,100,103,99}
                  };
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            S[i][j] = coef_dct(tab, i, j, 8);
            printf("%i ", (int)floor(S[i][j]));
        }
        printf("\n");
    }
    printf("\n");
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            L[i][j] = quantization(S, Q, i, j);
            printf("%i ", (int)floor(L[i][j]));
        }
        printf("\n");
    }
    return 0;
}
