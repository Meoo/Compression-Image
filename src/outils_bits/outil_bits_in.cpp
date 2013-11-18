/**
 * outil_bits_in.cpp
 */

#include <stdexcept>

#include "outil_bits_in.h"

FluxBitsIn::FluxBitsIn(const byte_t * buffer, unsigned taille)
    : buffer(buffer), taille(taille), position(0), position_bit(0)
{
}

bit_t FluxBitsIn::lire_bit()
{
    verifier_dispo_or_throw(1);

	// Aller chercher un unique bit
    bit_t bit = buffer[position] >> (7 - position_bit) & 1;

    avancer(1);

    return bit;
}

byte_t FluxBitsIn::lire_octet()
{
    verifier_dispo_or_throw(8);
    
    byte_t byte = 0;

    if (position_bit == 0)
    {
		// Lire un octet entier
        byte = buffer[position];
    }
    else
    {
		// Lire un octet sur deux octets
        byte = buffer[position] << position_bit |
               buffer[position + 1] >> (8 - position_bit);
    }

    avancer(8);

    return byte;
}

unsigned long long FluxBitsIn::lire_entier(unsigned nombre_bits)
{
	if (nombre_bits > 64 || nombre_bits == 0)
        throw std::range_error("Nombre invalide de bits demandé");
		
    verifier_dispo_or_throw(nombre_bits);
	
    unsigned long long res = 0;
	
	// Lire les petits bits qui trainent avant
	while (nombre_bits % 8 > 0)
		res |= ((unsigned long long) lire_bit()) << (--nombre_bits);
	
	// Lire les blocs d'octets
	while (nombre_bits > 0)
		res |= ((unsigned long long) lire_octet()) << (nombre_bits -= 8);
	
	return res;
}

void FluxBitsIn::avancer(unsigned nombre_bits)
{
    // Avancer de x octets pleins
    position += nombre_bits / 8;

    // Bits restants
    position_bit += nombre_bits % 8;

    // Vérifier le dépassement
    if (position_bit >= 8)
    {
        ++position;
        position_bit -= 8;
    }
}

void FluxBitsIn::verifier_dispo_or_throw(unsigned nombre_bits)
{
    if (position + ((nombre_bits - 1) / 8) >= taille
      || (position == (taille - 1)
         && position_bit + ((nombre_bits - 1) % 8) >= 8))
    {
        throw std::range_error("Pas assez de bits dans le FluxBitsIn");
    }
}

