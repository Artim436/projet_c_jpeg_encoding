#ifndef _DCT_H_
#define _DCT_H_

#include <stdint.h>
#include <math.h>
#include <qtables.h>
#include <structure.h>

typedef struct bloc_8x8_dtc bloc_8x8_dtc;

typedef struct bloc_64_dtc bloc_64_dtc;


extern float coef_dct(float** S, int i, int j, int n);

extern float coef_dct_v2(float** S, int i, int j, int n);

extern float C_function(int i);

extern void dct(float** S);

extern void quantization_Y(int16_t* D);

extern void quantization_C(int16_t* D);

extern int coef_quantization_Y(int16_t* D, int i);

extern int coef_quantization_C(int16_t* D, int i);

extern void zigzag(float** D, int16_t* F);

extern void fonction(struct main_mcu *main_mcu, struct image_YCbCr *im_ycbcr);

extern void fonction_RGB(struct main_mcu_rgb *main_mcu_rgb, struct image_YCbCr *im_ycbcr);

extern void affiche_bloc(struct main_mcu *main_mcu);

#endif /* _DCT_H_ */