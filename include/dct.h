#ifndef _DCT_H_
#define _DCT_H_

#include <stdint.h>
#include <math.h>
#include <qtables.h>
#include <structure.h>

typedef struct bloc_8x8_dtc bloc_8x8_dtc;

typedef struct bloc_64_dtc bloc_64_dtc;


extern float coef_dct(float** S, int i, int j, int n);

extern float C_function(int i);

extern int dct(float** S);

extern int quantization(float* D);

extern int coef_quantization(float* D, int i);

extern int zigzag(float** D, float* F);

extern void fonction(struct main_mcu *main_mcu, struct image_YCbCr *im_ycbcr);

extern void affiche_bloc(struct main_mcu *main_mcu);

#endif /* _DCT_H_ */