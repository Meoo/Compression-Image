/**
 * outils_bits_in.h
 */

#ifndef __OUTILS_BITS_IN_H__
#define __OUTILS_BITS_IN_H__

#include <stdexcept>

typedef bool bit_t;
typedef unsigned char byte_t;

/**
 * Classe pour lire des bits dans un flux.
 */
template<class T>
class FluxBitsIn
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
    FluxBitsIn(T & flux)
        : _flux(flux), _position_bit(0)
    {
        suivant();
    }

    /**
     * Lire un bit.
     *
     * @return Bit lu.
     */
    bit_t lire_bit()
    {
        bit_t bit = _octet >> (7 - _position_bit) & 1;

        if (++_position_bit == 8)
        {
            suivant();
            _position_bit = 0;
        }

        return bit;
    }

    /**
     * Lire un octet.
     *
     * @return Octet lu.
     */
    byte_t lire_octet()
    {
        byte_t byte = 0;

        if (_position_bit == 0)
        {
            // Lire un octet entier
            byte = _octet;
            
            suivant();
        }
        else
        {
            // Lire un octet sur deux octets
            byte = _octet << _position_bit;
            suivant();
            byte |= (_octet >> (8 - _position_bit));
        }

        return byte;
    }

    /**
     * Lire un entier avec un nombre variable de bits (jusqu'à 64).
     *
     * @param nombre_bits Nombre de bits à lire (max 64).
     * @return Entier lu.
     */
    unsigned long long lire_entier(unsigned nombre_bits)
    {
        if (nombre_bits > 64 || nombre_bits == 0)
            throw std::range_error("Nombre invalide de bits demandé");

        unsigned long long res = 0;
        
        // Lire les petits bits qui trainent avant
        while (nombre_bits % 8 > 0)
            res |= ((unsigned long long) lire_bit()) << (--nombre_bits);
        
        // Lire les blocs d'octets
        while (nombre_bits > 0)
            res |= ((unsigned long long) lire_octet()) << (nombre_bits -= 8);
        
        return res;
    }
    
private:
    /**
     * Lire l'octet suivant.
     */
    void suivant()
    {
        _flux.get((char&) _octet);
    }

};

#endif

