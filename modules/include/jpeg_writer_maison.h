#ifndef JPEG_WRITER_MAISON_H
#define JPEG_WRITER_MAISON_H

#include <stdint.h>
#include <huffman_maison.h>

/********************/
/* Types de données */
/********************/

/* Type énuméré représentant les composantes de couleur YCbCr. */
enum color_component
{
    Y,
    Cb,
    Cr,
    NB_COLOR_COMPONENTS
};

/*
    Type énuméré représentant les types de composantes fréquentielles (DC ou AC).
*/
enum sample_type
{
    DC,
    AC,
    NB_SAMPLE_TYPES
};

/*
    Type énuméré représentant la direction des facteurs d'échantillonnage (H
    pour horizontal, V pour vertical).
*/
enum direction
{
    H,
    V,
    NB_DIRECTIONS
};
struct sampling_factor {
    uint8_t H_Y;
    uint8_t H_Cb;
    uint8_t H_Cr;
    uint8_t V_Y;
    uint8_t V_Cb;
    uint8_t V_Cr;

};
/*
    Type opaque contenant l'intégralité des informations 
    nécessaires à l'écriture de l'en-tête JPEG.
*/
struct jpeg_M {
    uint16_t SOI;
    uint16_t EOI;
    uint8_t *APPx;
    uint8_t *COM;
    uint8_t *DQT;
    uint8_t *SOFx;
    uint8_t *DHT;
    uint8_t *SOS;
    uint16_t height;
    uint16_t width;
    uint8_t nb_component;
    uint8_t *qtable;
    enum color_component cc;
    char* ppm_filename;
    char* jpeg_filename;
    struct sampling_factor ssp;
    struct huff_table_M *DC_Y;
    struct huff_table_M *DC_Cb;
    struct huff_table_M *DC_Cr;
    struct huff_table_M *AC_Y;
    struct huff_table_M *AC_Cr;
    struct huff_table_M *AC_Cb;
};

/* Type opaque représentant un arbre de Huffman. */
struct huff_table;


/***********************************************/
/* Ouverture, fermeture et fonctions générales */
/***********************************************/

/* Alloue et retourne une nouvelle structure jpeg. */
extern struct jpeg_M *jpeg_create_M(void); //GOOD

/*
    Détruit une structure jpeg. 
    Toute la mémoire qui lui est associée est libérée.
*/
extern void jpeg_destroy_M(struct jpeg_M *jpg);
extern void jpeg_destroy_M(struct jpeg_M *jpg);

/*
    Ecrit tout l'en-tête JPEG dans le fichier de sortie à partir des
    informations contenues dans la structure jpeg passée en paramètre. 
    En sortie, le bitstream est positionné juste après l'écriture de 
    l'en-tête SOS, à l'emplacement du premier octet de données brutes à écrire.
*/
extern void jpeg_write_header_M(struct jpeg_M *jpg);

/* Ecrit le footer JPEG (marqueur EOI) dans le fichier de sortie. */
extern void jpeg_write_footer_M(struct jpeg_M *jpg);

/*
    Retourne le bitstream associé au fichier de sortie enregistré 
    dans la structure jpeg.
*/
extern struct bitstream *jpeg_get_bitstream_M(struct jpeg_M *jpg);


/****************************************************/
/* Gestion des paramètres de l'encodeur via le jpeg */
/****************************************************/

/* Ecrit le nom de fichier PPM ppm_filename dans la structure jpeg. */
extern void jpeg_set_ppm_filename_M(struct jpeg_M *jpg, //GOOD
                                  const char *ppm_filename);

/* Retourne le nom de fichier PPM lu dans la structure jpeg. */
extern char *jpeg_get_ppm_filename_M(struct jpeg_M *jpg);

/* Ecrit le nom du fichier de sortie jpeg_filename dans la structure jpeg. */
extern void jpeg_set_jpeg_filename_M(struct jpeg_M *jpg, //GOOD
                                   const char *jpeg_filename);

/* Retourne le nom du fichier de sortie lu depuis la structure jpeg. */
extern char *jpeg_get_jpeg_filename_M(struct jpeg_M *jpg);


/*
    Ecrit la hauteur de l'image traitée, en nombre de pixels,
    dans la structure jpeg.
*/
extern void jpeg_set_image_height_M(struct jpeg_M *jpg,
                                  uint32_t image_height);

/*
    Retourne la hauteur de l'image traitée, en nombre de pixels,
    lue dans la structure jpeg.
*/
extern uint32_t jpeg_get_image_height_M(struct jpeg_M *jpg);

/*
    Ecrit la largeur de l'image traitée, en nombre de pixels,
    dans la structure jpeg.
*/
extern void jpeg_set_image_width_M(struct jpeg_M *jpg,
                                 uint32_t image_width);

/*
    Retourne la largeur de l'image traitée, en nombre de pixels,
    lue dans la structure jpeg.
*/
extern uint32_t jpeg_get_image_width_M(struct jpeg_M *jpg);


/*
    Ecrit le nombre de composantes de couleur de l'image traitée
    dans la structure jpeg.
*/
extern void jpeg_set_nb_components_M(struct jpeg_M *jpg,
                                   uint8_t nb_components);

/*
    Retourne le nombre de composantes de couleur de l'image traitée 
    lu dans la structure jpeg.
*/
extern uint8_t jpeg_get_nb_components_M(struct jpeg_M *jpg);


/*
    Ecrit dans la structure jpeg le facteur d'échantillonnage sampling_factor
    à utiliser pour la composante de couleur cc et la direction dir.
*/
extern void jpeg_set_sampling_factor_M(struct jpeg_M *jpg,
                                     enum color_component cc,
                                     enum direction dir,
                                     uint8_t sampling_factor);

/*
    Retourne le facteur d'échantillonnage utilisé pour la composante 
    de couleur cc et la direction dir, lu dans la structure jpeg.
*/
extern uint8_t jpeg_get_sampling_factor_M(struct jpeg_M *jpg,
                                        enum color_component cc,
                                        enum direction dir);


/*
    Ecrit dans la structure jpeg la table de Huffman huff_table à utiliser
    pour encoder les données de la composante fréquentielle acdc, pour la
    composante de couleur cc.
*/
extern void jpeg_set_huffman_table_M(struct jpeg_M *jpg,
                                   enum sample_type acdc,
                                   enum color_component cc,
                                   struct huff_table_M *htable);

/*
    Retourne un pointeur vers la table de Huffman utilisée pour encoder
    les données de la composante fréquentielle acdc pour la composante 
    de couleur cc, lue dans la structure jpeg.
*/
extern struct huff_table *jpeg_get_huffman_table_M(struct jpeg_M *jpg,
                                                 enum sample_type acdc,
                                                 enum color_component cc);


/*
    Ecrit dans la structure jpeg la table de quantification à utiliser
    pour compresser les coefficients de la composante de couleur cc.
*/
extern void jpeg_set_quantization_table_M(struct jpeg_M *jpg,
                                        enum color_component cc,
                                        uint8_t *qtable);

/*
    Retourne un pointeur vers la table de quantification associée à la 
    composante de couleur cc, lue dans a structure jpeg.
*/
extern uint8_t *jpeg_get_quantization_table_M(struct jpeg_M *jpg,
                                            enum color_component cc);

#endif /* JPEG_WRITER_MAISON_H */
