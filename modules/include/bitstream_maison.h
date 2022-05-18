
#ifndef _BITSTREAM_MAISON_H_
#define _BITSTREAM_MAISON_H_

#include <stdint.h>
#include <stdbool.h>
#include <structure.h>

/* Retourne un nouveau bitstream prêt à écrire dans le fichier filename. */
extern struct bitstream_M *bitstream_create_M(const char *filename);

/*
    Ecrit nb_bits bits dans le bitstream. La valeur portée par cet ensemble de
    bits est value. Le paramètre is_marker permet d'indiquer qu'on est en train
    d'écrire un marqueur de section dans l'entête JPEG ou non (voir section
    "Encodage dans le flux JPEG -> Byte stuffing" du sujet).
*/
extern void bitstream_write_bits_M(struct bitstream_M *stream,
                                 uint32_t value,
                                 uint8_t nb_bits,
                                 bool is_marker);

/*
    Force l'exécution des écritures en attente sur le bitstream, s'il en
    existe.
*/
extern void bitstream_flush_M(struct bitstream_M *stream);

/*
    Détruit le bitstream passé en paramètre, en libérant la mémoire qui lui est
    associée.
*/
extern void bitstream_destroy_M(struct bitstream_M *stream);

#endif /* _BITSTREAM_H_ */
