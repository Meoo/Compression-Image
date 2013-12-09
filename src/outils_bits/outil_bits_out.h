/**
 * outils_bits_out.h
 */

#ifndef __OUTILS_BITS_OUT_H__
#define __OUTILS_BITS_OUT_H__

#include <stdexcept>

typedef bool bit_t;
typedef unsigned char byte_t;

/**
 * Classe pour écrire des bits dans un flux.
 */
template<class T>
class FluxBitsOut
{
private:
    T & _flux;

    // Octet en cours
    unsigned char _octet;

    // Bits déjà lus dans l'octet à "position"
    unsigned _position_bit;

public:
    /**
     * Construire un flux de bits.
     *
     * @param flux Flux à utiliser.
     */
    FluxBitsOut(T & flux)
        : _flux(flux), _octet(0), _position_bit(0)
    {
    }

    /**
     * Ecrire un bit.
     *
     * @param bit Bit à écrire.
     */
    void ecrire_bit(bit_t bit)
    {
        _octet |= ((bit & 1) << (7 - _position_bit));

        if (++_position_bit == 8)
        {
            suivant();
            _position_bit = 0;
        }
    }

    /**
     * Ecrire un octet.
     *
     * @param octet Octet à écrire.
     */
    void ecrire_octet(byte_t byte)
    {
        if (_position_bit == 0)
        {
            // Ecrire un octet entier
            _octet = byte;
            suivant();
        }
        else
        {
            // Ecrire un octet sur deux octets
            _octet |= byte >> _position_bit;
            suivant();
            _octet |= byte << (8 - _position_bit);
        }
    }

    /**
     * Ecrire un entier avec un nombre variable de bits (jusqu'à 64).
     *
     * @param nombre_bits Nombre de bits à écrire (max 64).
     * @param entier Entier à écrire.
     */
    void ecrire_entier(unsigned nombre_bits, unsigned long long entier)
    {
        if (nombre_bits > 64 || nombre_bits == 0)
            throw std::range_error("Nombre invalide de bits demandé");
        
        // Lire les petits bits qui trainent avant
        while (nombre_bits % 8 > 0)
            ecrire_bit((entier >> (--nombre_bits)) & 1);

        // Lire les blocs d'octets
        while (nombre_bits > 0)
            ecrire_octet(entier >> (nombre_bits -= 8));
    }

    /**
     * Remplir les derniers bits de 0.
     */
    void finaliser()
    {
        if (_position_bit != 0)
        {
            suivant();
            _position_bit = 0;
        }
    }

private:
    /**
     * Ecrire un octet et passer au suivant.
     */
    void suivant()
    {
        _flux.put((const char) _octet);
        _octet = 0;
    }

};

#endif

