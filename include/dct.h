#ifndef _DCT_H_
#define _DCT_H_

#include <stdint.h>
#include <math.h>
#include <qtables.h>

typedef struct bloc_8x8 bloc_8x8;

typedef struct bloc_64 bloc_64;


extern float coef_dct(bloc_8x8 *S, int i, int j, int n);

extern float C_function(int i);

extern int dct(bloc_8x8 *S);

// extern int quantization(vector D);

// extern int coef_quantization(vector D, int i);

// extern int zigzag(matrix D, vector F);


#endif /* _DCT_H_ */