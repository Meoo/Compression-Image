/**
 * huffman_builder.h
 */

#ifndef __HUFFMAN_BUILDER_H__
#define __HUFFMAN_BUILDER_H__

#include <map>

#include "huffman_encode.h"

class Noeud;

class HuffmanBuilder
{
    typedef std::map<int, unsigned> symap_t;

    /**
     * Liste de symboles.
     */
    symap_t _symboles;
    
    // Variables pour la récursion
    unsigned _enco;
    unsigned _nb_bits;
    HuffmanEncode * _huff;

public:
    /**
     * Constructeur.
     */
    HuffmanBuilder();
    
    /**
     * Ajouter ou incrémenter le nombre d'occurences d'un symbole.
     */
    void ajouter_symbole(int symbole, unsigned occurences);
    
    /**
     * Utiliser la liste des symboles actuels pour construire un
     * encodeur Huffman.
     */
    void construire_encodeur(HuffmanEncode & encodeur);
    
private:
    /**
     * Récursion sur les noeuds.
     */
    void traiter_noeud(Noeud * noeud);

};

#endif
