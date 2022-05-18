#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    
#include <structure.h>
#include <downsampling.h>

void verif_sample_factor(uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3){
    if(h1 > 4|| h2 > 4||h3 > 4||v1 > 4||v2 > 4||v3 > 4){
        printf("Vos facteurs déchantillonages doivent être compris entre 1 et 4.\n");
        //renvoyer une erreur ici
    }
    if(h1 <1|| h2<1||h3 <1||v1 <1||v2 <1||v3 <1){
        printf("Vos facteurs déchantillonages doivent être compris entre 1 et 4.\n");
        //renvoyer une erreur ici
    }
    uint8_t somme_prod = h1*v1+h2*v2+h3*v2;
    if(somme_prod > 10){
        printf("La somme des produits de vos facteurs dépassent 10, essayez de réduire vos facteurs\n");
        //renvoyer une erreur ici
    }
    if(h1 % h2 != 0 || h1 % h3 != 0 ||v1 % v2 != 0 ||v1 % v3 != 0 ){
        printf("Vos facteurs de crominances ne divisent pas parfaitement ceux de la luminescence. \n");
        //renvoyer une erreur ici
    }
}
