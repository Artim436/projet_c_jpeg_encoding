#ifndef _DCT_H_
#define _DCT_H_

#include <stdint.h>
#include <math.h>
#include <qtables.h>

typedef struct bloc_8x8_dtc bloc_8x8_dtc;

typedef struct bloc_64_dtc bloc_64_dtc;


extern float coef_dct(bloc_8x8_dtc *S, int i, int j, int n);

extern float C_function(int i);

extern int dct(bloc_8x8_dtc *S);

extern int quantization(bloc_64_dtc *D);

extern int coef_quantization(bloc_64_dtc *D, int i);

extern int zigzag(bloc_8x8_dtc *D, bloc_64_dtc *F);


#endif /* _DCT_H_ */