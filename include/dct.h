#ifndef _DCT_H_
#define _DCT_H_

#include <stdint.h>
#include <math.h>

typedef float matrix[8][8];


extern float coef_dct(matrix S, int i, int j, int n);

extern float C_function(int i);

extern int dct(matrix S);

extern int quantization(matrix D, matrix Q);

extern int coef_quantization(matrix D, matrix Q, int i, int j);


#endif /* _DCT_H_ */