#ifndef _DCT_H_
#define _DCT_H_

#include <stdint.h>
#include <math.h>
#include <qtables.h>
#include <structure.h>

typedef struct bloc_8x8_dtc bloc_8x8_dtc;

typedef struct bloc_64_dtc bloc_64_dtc;


extern float coef_dct(float *S[8][8], int i, int j, int n);

extern float C_function(int i);

extern int dct(float *S[8][8]);

extern int quantization(float *D[64]);

extern int coef_quantization(float *D[64], int i);

extern int zigzag(float *D[8][8], float *F[64]);

extern void fonction(struct main_mcu *main_mcu, struct image_YCbCr *im_ycbcr);

extern void affiche_bloc(struct main_mcu *main_mcu);

#endif /* _DCT_H_ */