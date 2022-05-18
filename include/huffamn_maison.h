#ifndef HUFFMAN_MAISON_H
#define HUFFMAN_MAISON_H



/* Type opaque représentant un arbre de Huffman. */


typedef struct node_M {
    uint8_t value;
    struct node_M *left;
    struct node_M *right;
} node_M;


struct huff_table_M;
    uint8_t *nb_symb_per_lengths;
    uint8_t *symbols;
    uint8_t nb_symbols;

/*
 Construit un arbre de Huffman à partir d'une table
 de symboles comme présenté en section 
 "Compression d'un bloc fréquentiel -> Le codage de Huffman" du sujet.
 nb_symb_per_lengths est un tableau contenant le nombre
 de symboles pour chaque longueur de 1 à 16,
 symbols est le tableau des symboles ordonnés,
 et nb_symbols représente la taille du tableau symbols.
*/

struct huff_table_M *huffman_table_build_M(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols);
    struct huff_table_M *huff = calloc(1, sizeof(struct huff_table_M));
    huff->nb_symb_per_lengths = nb_symb_per_lengths;
    huff->symbols = symbols;
    huff->nb_symbols = nb_symbols;
    return huff;

/*
 Retourne le chemin dans l'arbre ht permettant d'atteindre
 la feuille de valeur value. nb_bits est un paramètre de sortie
 permettant de stocker la longueur du chemin retourné.
*/
uint32_t huffman_table_get_path_M(struct huff_table_M *ht, uint8_t value, uint8_t *nb_bits);


/*
 Retourne le tableau des symboles associé à l'arbre de
 Huffman passé en paramètre.
*/
uint8_t *huffman_table_get_symbols_M(struct huff_table_M *ht);
    return ht->symbols;


/*
 Retourne le tableau du nombre de symboles de chaque longueur
 associé à l'arbre de Huffman passé en paramètre.
*/
uint8_t *huffman_table_get_length_vector_M(struct huff_table_M *ht);

/*
 Détruit l'arbre de Huffman passé en paramètre et libère
 toute la mémoire qui lui est associée.
*/
void huffman_table_destroy_M(struct huff_table_M *ht);
    free(ht)


#endif /* HUFFMAN_MAISON_H */