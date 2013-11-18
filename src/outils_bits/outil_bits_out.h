/**
 * outils_bits_out.h
 */

#ifndef __OUTILS_BITS_OUT_H__
#define __OUTILS_BITS_OUT_H__

typedef bool bit_t;
typedef unsigned char byte_t;

/**
 * Classe pour écrire des bits dans un buffer.
 */
class FluxBitsOut
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
    FluxBitsOut(const byte_t * buffer, unsigned taille);

    /**
     * Ecrire un bit.
     *
     * @param bit Bit lu.
     */
    void ecrire_bit(bit_t bit);

    /**
     * Ecrire un octet.
     *
     * @param octet Octet lu.
     */
    void ecrire_octet(byte_t octet);

    /**
     * Ecrire un entier avec un nombre variable de bits (jusqu'à 64).
     *
     * @param nombre_bits Nombre de bits à écrire (max 64).
     * @param entier Entier à écrire.
     */
    void ecrire_entier(unsigned nombre_bits, unsigned long long entier);

private:
    // Avancer position et position_bits de nombre_bits
    void avancer(unsigned nombre_bits);

    // Vérifier que nombre_bits soient encore présents
    // Lève une exception en cas d'erreur
    void verifier_dispo_or_throw(unsigned nombre_bits);

};

#endif

