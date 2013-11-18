/**
 * outils_bits_in.h
 */

#ifndef __OUTILS_BITS_IN_H__
#define __OUTILS_BITS_IN_H__

typedef bool bit_t;
typedef unsigned char byte_t;

/**
 * Classe pour lire des bits dans un buffer.
 */
class FluxBitsIn
{
private:
    const byte_t * buffer;
    const unsigned taille;
    
    // Octets déjà lus
    unsigned position;

    // Bits déjà lus dans l'octet à "position"
    unsigned position_bit;

public:
    /**
     * Construire un flux de bits à partir d'un buffer.
     *
     * @param buffer Buffer à utiliser.
     * @param taille Taille du buffer.
     */
    FluxBitsIn(const byte_t * buffer, unsigned taille);

    /**
     * Lire un bit.
     *
     * @return Bit lu.
     */
    bit_t lire_bit();

    /**
     * Lire un octet.
     *
     * @return Octet lu.
     */
    byte_t lire_octet();

    /**
     * Lire un entier avec un nombre variable de bits (jusqu'à 64).
     *
     * @param nombre_bits Nombre de bits à lire (max 64).
     * @return Entier lu.
     */
    unsigned long long lire_entier(unsigned nombre_bits);

private:
    // Avancer position et position_bits de nombre_bits
    void avancer(unsigned nombre_bits);

    // Vérifier que nombre_bits soient encore présents
    // Lève une exception en cas d'erreur
    void verifier_dispo_or_throw(unsigned nombre_bits);

};

#endif

