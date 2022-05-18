#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void controle_arg(int argc, char **argv){
    if(argc < 2 || argc > 7){
        printError("Erreur : le nombre d'arguments n'est pas correct\n");
        return EXIT_FAILURE;
    }
    uint8_t arg_i = 1;
    uint8_t tab_arg[4] = {0,0,0,0};//Sert à savoir quels arguments sont déjà passé
    uint8_t reconnaissance = 1;
    while(arg_i < argc){
        //Étape de reconnaissance :
        if(reconnaissance == 1){
            is_sample(arg_i, tab_arg);
        }


    }
}

is_sample(char* arg, uint8_t* tab_arg){
    uint8_t pos = 0;
    while(){

    }
}