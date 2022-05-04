#ifndef _DCT_H_
#define _DCT_H_

#include <stdint.h>
#include <math.h>
#include <qtables.h>

typedef float matrix[8][8];

typedef float vector[64];


extern float coef_dct(matrix S, int i, int j, int n);

extern float C_function(int i);

extern int dct(matrix S);

extern int quantization(vector D);

extern int coef_quantization(vector D, int i);

extern int zigzag(matrix D, vector F);


#endif /* _DCT_H_ */