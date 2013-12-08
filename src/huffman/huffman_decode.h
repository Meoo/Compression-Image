/**
 * huffman_decode.h
 */

#ifndef __HUFFMAN_DECODE_H__
#define __HUFFMAN_DECODE_H__

#include <outils_bits/outil_bits_in.h>
#include "huffman_config.h"

#include <stdexcept>

/**
 * Décodeur de Huffman.
 *
 * Cette classe permet de lire des symboles depuis un flux.
 */
template<class T>
class HuffmanDecode
{
    // <tableau de bits, nombre de bits à utiliser>
    typedef std::pair<unsigned long long, unsigned> enco_t;
    typedef std::map<int, enco_t> symap_t;

    /**
     * Table des symboles.
     */
    symap_t _symboles;

    /**
     * Flux de données.
     */
    FluxBitsIn<T> & _bit_stream;

    // Noeud dans l'arbre de décodage
    class Noeud
    {
    public:
        // Vrai si le noeud est un symbole
        bool is_symbole;

        union
        {
            // Valeur du symbole
            int symbole;

            struct
            {
                // Sous noeuds dans l'arbre
                Noeud * bit_1;
                Noeud * bit_0;

            } noeud;
        };

        // Constructeur
        Noeud()
        {
            is_symbole = false;
            noeud.bit_0 = 0;
            noeud.bit_1 = 0;
        }

        // Destructeur
        ~Noeud()
        {
            if (!is_symbole)
            {
                delete noeud.bit_0;
                delete noeud.bit_1;
            }
        }
    };

    Noeud * racine;

public:
    /**
     * Créer un décodeur  à partir d'un flux de bits.
     *
     * @param bit_stream Flux de bits.
     */
    HuffmanDecode(FluxBitsIn<T> & bit_stream)
        : _bit_stream(bit_stream)
    {
        racine = new Noeud();
    }
    
    /**
     * Lire la table des symboles depuis le flux.
     */
    void lire_table_symboles()
    {
        // Nombre de symboles
        unsigned size =  _bit_stream.lire_entier(HUFFMAN_NOMBRE_SYMBOLES_BITS);

        for (unsigned i = 0; i < size; ++i)
        {
            // Taille de l'encodage du symbole (sur 6 bits)
            unsigned nb_bits = _bit_stream.lire_octet();

            // Encodage du symbole
            unsigned enco = _bit_stream.lire_entier(nb_bits);

            // Symbole
            int symbole = (int) _bit_stream.lire_entier(HUFFMAN_SYMBOLE_BITS);

            // Insertion de la branche dans l'arbre
            Noeud * n = racine;
            while (nb_bits > 0)
            {
                if ((enco >> (--nb_bits)) & 1)
                {
                    // Bit 1
                    if (n->noeud.bit_1 != 0)
                        n = n->noeud.bit_1;
                    else
                    {
                        Noeud * t = new Noeud();
                        n->noeud.bit_1 = t;
                        n = t;
                    }
                }
                else
                {
                    // Bit 0
                    if (n->noeud.bit_0 != 0)
                        n = n->noeud.bit_0;
                    else
                    {
                        Noeud * t = new Noeud();
                        n->noeud.bit_0 = t;
                        n = t;
                    }
                }
            }

            // Ecrire le symbole dans l'arbre
            n->is_symbole = true;
            n->symbole = symbole;
        }
    }

    /**
     * Décoder le symbole suivant dans le flux.
     *
     * @return Symbole décodé.
     */
    int decoder()
    {
        Noeud * n = racine;

        // Traverser l'arbre
        while (!n->is_symbole)
        {
            if (_bit_stream.lire_bit())
                n = n->noeud.bit_1;

            else
                n = n->noeud.bit_0;

            if (n == 0)
                throw std::domain_error("Séquence de bits inconnue");
        }

        return n->symbole;
    }

};

#endif
