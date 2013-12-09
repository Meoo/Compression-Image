/**
 * huffman_encode.h
 */

#ifndef __HUFFMAN_ENCODE_H__
#define __HUFFMAN_ENCODE_H__

#include <map>

#include "../outils_bits/outil_bits_out.h"
#include "huffman_config.h"

/**
 * Encodeur de Huffman.
 *
 * Il faut utiliser la classe HuffmanBuilder pour construire un encodage,
 * cette classe permet alors d'encoder des symboles dans un flux.
 */
template<class T>
class HuffmanEncode
{
    // <tableau de bits, nombre de bits � utiliser>
    typedef std::pair<unsigned, unsigned> enco_t;
    typedef std::map<int, enco_t> symap_t;

    /**
     * Table des symboles.
     */
    symap_t _symboles;

    /**
     * Flux de données.
     */
    FluxBitsOut<T> & _bit_stream;

public:
    /**
     * Construire un encodeur à partir d'un flux de bits.
     *
     * @param bit_stream Flux de bits.
     */
    HuffmanEncode(FluxBitsOut<T> & bit_stream)
        : _bit_stream(bit_stream)
    {
    }

    /**
     * Ecrire la table des symboles dans le flux.
     */
    void ecrire_table_symboles()
    {
        // Nombre de symboles
        _bit_stream.ecrire_entier(HUFFMAN_NOMBRE_SYMBOLES_BITS, _symboles.size());

        for (symap_t::iterator it = _symboles.begin(); it != _symboles.end(); ++it)
        {
            // Taille de l'encodage du symbole
            _bit_stream.ecrire_octet(it->second.second);

            // Encodage du symbole
            _bit_stream.ecrire_entier(it->second.second, it->second.first);

            // Symbole
            _bit_stream.ecrire_entier(HUFFMAN_SYMBOLE_BITS, it->first);
        }
    }

    /**
     * Ecrire l'encodage pour un symbole donné dans le flux.
     *
     * @param symbole
     */
    void encoder(int symbole)
    {
        enco_t e (_symboles.at(symbole));
        _bit_stream.ecrire_entier(e.second, e.first);
    }

protected:
    friend class HuffmanBuilder;

    /**
     * Utilisé en interne par HuffmanBuilder pour rajouter des symboles.
     *
     * @param symbole
     * @param encodage
     * @param num_bits
     */
    void ajouter_symbole(int symbole, unsigned encodage, unsigned num_bits)
    {
        _symboles.insert(std::pair<int, enco_t>(symbole, enco_t(encodage, num_bits)));
    }

};

#endif
