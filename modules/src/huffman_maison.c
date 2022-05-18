#include <huffman_maison.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dct.h>
#include <structure.h>
#include <MCU.h>
#include <convert_to_YCbCr.h>
#include <lecture_pgm.h>
#include <jpeg_writer.h>
#include <test.h>
#include <qtables.h>
#include <encoding.h>
#include <write_jpeg_file.h>
#include <nettoyage.h>
#include <time.h>
#include <htables.h>
#include <huffman.h>


/* Type opaque représentant un arbre de Huffman. */


/* structure d'un noeud */

/**
 * @brief structure d'un noeud
 * @param left
 * @param right
 * @param used
 */
struct node_M {
    bool used;
    struct node_M *left;
    struct node_M *right;
};


/*fonction de création d'un noeud*/
/**
 * @brief fonction de création d'un noeud
 * @return noeud
 */

struct node_M *create_node_M(void) {
    struct node_M *node = calloc(1, sizeof(node_M));
    node->used = false;
    node->left = NULL;
    node->right = NULL;
    return node;
}


/* fonction recursive intermedaire pour la création de la liste encoded à partir de la structure d'un arbre de Huffman */

/**
 * @brief 
 * 
 * @param encoded_table 
 * @param huff 
 * @param current_node 
 * @param index 
 * @param depth 
 * @param encoded_value 
 * @return true 
 * @return false 
 */

bool fct_rec(uint16_t *encoded_table, huff_table_M *huff, node_M *current_node, uint32_t *index, uint8_t depth, uint32_t encoded_value){
    if (depth==0){
        if (current_node->used==0){
            current_node->used=true;
            encoded_table[*index] = encoded_value;
            (*index)++;
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if (current_node->used){
            return false;
        }
        if (current_node->left==NULL){
            current_node->left = create_node_M();
        }
        if (fct_rec(encoded_table, huff, current_node->left, index, depth-1, encoded_value*2)){
            return true;
        }
        if (current_node->right==NULL){
            current_node->right = create_node_M();
        }
        if (fct_rec(encoded_table, huff, current_node->right, index, depth-1, (encoded_value*2)+1)){
            return true;
        }
    }
    return false;
}


/* fonction qui crée la liste encoded à partir de la structure d'un arbre de Huffman */

/**
 * @brief 
 * 
 * @param huff 
 * @return uint16_t* 
 */

uint16_t *generate_huffman_table(huff_table_M *huff){
    uint32_t index = 0;
    node_M *current_node = create_node_M();
    uint16_t *encoded_table = calloc(huff->nb_symbols, sizeof(uint16_t));
    uint32_t s=0;
    for (int i=0; i<16; i++){
        for (int j=0; j<huff->nb_symb_per_lengths[i]; j++){
            fct_rec(encoded_table, huff, current_node, &index, i+1, 0);
        }
    }
    return encoded_table;
}


/*
 Construit un arbre de Huffman à partir d'une table
 de symboles comme présenté en section 
 "Compression d'un bloc fréquentiel -> Le codage de Huffman" du sujet.
 nb_symb_per_lengths est un tableau contenant le nombre
 de symboles pour chaque longueur de 1 à 16,
 symbols est le tableau des symboles ordonnés,
 nb_symbols représente la taille du tableau symbols.
 et encoded_table est le tableau des codes huffman ordonnés dans le même ordre que le tableau symbols.
*/

/**
 * @brief 
 * 
 * @param nb_symb_per_lengths 
 * @param symbols 
 * @param nb_symbols 
 * @return struct huff_table_M* 
 */

struct huff_table_M *huffman_table_build_M(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols){
    huff_table_M *huff = calloc(1, sizeof(huff_table_M));
    huff->nb_symb_per_lengths = nb_symb_per_lengths;
    huff->symbols = symbols;
    huff->nb_symbols = nb_symbols;
    huff->encoded_table = calloc(nb_symbols, sizeof(uint32_t));
    huff->encoded_table = generate_huffman_table(huff);
    return huff;
}


/*
 Retourne le chemin dans l'arbre huff permettant d'atteindre
 la feuille de valeur value. nb_bits est un paramètre de sortie
 permettant de stocker la longueur du chemin retourné.
*/

/**
 * @brief 
 * 
 * @param huff 
 * @param value 
 * @param nb_bits 
 * @return uint32_t 
 */
uint32_t huffman_table_get_path_M(struct huff_table_M *huff, uint8_t value, uint8_t *nb_bits){
    uint8_t i;
    uint16_t j;
    uint32_t s=0;
    for (i =0; i<16; i++){
        for (j=0; j<huff->nb_symb_per_lengths[i]; j++){
            if (huff->symbols[s+j]==value){
                *nb_bits = i+1;
                return huff->encoded_table[s+j];
            }
        }
        s += huff->nb_symb_per_lengths[i];
    }
    return 0;
}


/*
 Retourne le tableau des symboles associé à l'arbre de
 Huffman passé en paramètre.
*/

/**
 * @brief 
 * 
 * @param huff 
 * @return uint8_t* 
 */
uint8_t *huffman_table_get_symbols_M(struct huff_table_M *huff){
    return huff->symbols;
}

/*
 Retourne le tableau du nombre de symboles de chaque longueur
 associé à l'arbre de Huffman passé en paramètre.
*/

/**
 * @brief 
 * 
 * @param huff 
 * @return uint8_t* 
 */
uint8_t *huffman_table_get_length_vector_M(struct huff_table_M *huff){
    return huff->nb_symb_per_lengths;
}

/*
 Détruit l'arbre de Huffman passé en paramètre et libère
 toute la mémoire qui lui est associée.
*/
/**
 * @brief 
 * 
 * @param huff 
 */
void huffman_table_destroy_M(struct huff_table_M *huff){
    free(huff);
}




// void main(){
//     uint8_t *nb_bits = calloc(1,sizeof(uint8_t));
//     uint16_t *encoded = calloc(16, sizeof(uint16_t));
//     uint32_t path = huffman_table_get_path_M(huffman_table_build_M(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]), 106, nb_bits);
//     uint32_t path2 = huffman_table_get_path(huffman_table_build(htables_nb_symb_per_lengths[AC][Cr], htables_symbols[AC][Cr], htables_nb_symbols[AC][Cr]), 106, nb_bits);
//     huff_table_M *huff = huffman_table_build_M(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]);
//     for (int i=0; i<htables_nb_symbols[AC][Cb]; i++){
//         encoded[i] = huff->encoded_table[i];
//         printf(" %x ", encoded[i]);
//     }
//     printf("\n");
//     printf("%i\n", path);
//     printf("\n");
//     printf("%i\n", path2);
//     printf("\n");
//     // printf("%i\n", *nb_bits);
//     // for (int i=0; i<htables_nb_symbols[AC][Cb]; i++){
//     //     printf(" %i ", htables_symbols[AC][Cb][i]);
//     // }
//     // printf("\n");
//     // for (int i=0; i<16; i++){
//     //     printf(" %i ", htables_nb_symb_per_lengths[AC][Cb][i]);
//     // }
//     // printf("\n");
// }