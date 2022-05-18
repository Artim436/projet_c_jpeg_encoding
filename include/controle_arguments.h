#ifndef _CONTROLE_ARGUMENTS_H
#define _CONTROLE_ARGUMENTS_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    


uint8_t* controle_arg(int argc, char **argv);

void analyse_arg(uint8_t* pos_des_arguments, struct main_mcu* p_main, char** argv);

void analyse_arg_sub(uint8_t* pos_des_arguments, struct main_mcu_rgb_sub* p_main, char** argv);


# endif