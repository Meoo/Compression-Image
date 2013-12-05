/**
 * huffman_builder.h
 */

#ifndef __HUFFMAN_BUILDER_H__
#define __HUFFMAN_BUILDER_H__

#include <map>
#include <vector>
#include <algorithm>

#include "huffman_encode.h"

class Noeud;

class HuffmanBuilder
{
    typedef std::map<int, unsigned> symap_t;

    /**
     * Liste de symboles.
     */
    symap_t _symboles;
    
    // Variables pour la r�cursion
    unsigned _enco;
    unsigned _nb_bits;
    void * _huff;

public:
    /**
     * Constructeur.
     */
    HuffmanBuilder() {}
    
    /**
     * Ajouter ou incr�menter le nombre d'occurences d'un symbole.
     */
    void ajouter_symbole(int symbole, unsigned occurences);
    
    /**
     * Utiliser la liste des symboles actuels pour construire un
     * encodeur Huffman.
     */
    template<class T>
    void construire_encodeur(HuffmanEncode<T> & encodeur);
    
private:
    class Noeud;

    /**
     * R�cursion sur les noeuds.
     */
    template<class T>
    void traiter_noeud(Noeud * noeud);


    // Noeud pour la construction de Huffman
    class Noeud
    {
    public:
        // Nombre d'occurences
        unsigned occurences;

        // Vrai si le noeud est un symbole
        bool is_symbole;

        union
        {
            int symbole;

            // Sous noeuds si pas un symbole
            struct
            {
                Noeud * gauche;
                Noeud * droite;

            } noeud;
        };

        // Destructeur
        ~Noeud()
        {
            if (!is_symbole)
            {
                delete noeud.gauche;
                delete noeud.droite;
            }
        }

        // Comparateur pour trier les noeuds
        static bool noeud_comp(Noeud * i,Noeud * j)
        {
            return i->occurences > j->occurences;
        }

    }; // Noeud

}; // Huffman

// --------

inline
void HuffmanBuilder::ajouter_symbole(int symbole, unsigned occurences)
{
    symap_t::iterator it = _symboles.find(symbole);

    if (it != _symboles.end())
        it->second += occurences;
    else
        _symboles.insert(std::pair<int, unsigned>(symbole, occurences));
}

template<class T>
void HuffmanBuilder::construire_encodeur(HuffmanEncode<T> & encodeur)
{
    std::vector<Noeud *> vec;

    // Construire le vecteur de noeuds
    for (symap_t::iterator it = _symboles.begin(); it != _symboles.end(); ++it)
    {
        Noeud * n = new Noeud();
        n->occurences = it->second;
        n->is_symbole = true;
        n->symbole = it->first;
        vec.push_back(n);
    }

    // Construire l'arbre
    while (vec.size() != 1)
    {
        // Trier
        std::sort(vec.begin(), vec.end(), Noeud::noeud_comp);

        // Prendre les 2 plus petits
        Noeud * a = vec.back(); vec.pop_back();
        Noeud * b = vec.back(); vec.pop_back();

        // En faire un nouveau noeud
        Noeud * n = new Noeud();
        n->occurences = a->occurences + b->occurences;
        n->is_symbole = false;
        n->noeud.gauche = a;
        n->noeud.droite = b;
        vec.push_back(n);
    }

    // L'arbre est pr�t
    Noeud * arbre = vec.at(0);

    // Ajouter les symboles � l'encodeur
    _nb_bits = 0;
    _enco = 0;
    _huff = reinterpret_cast<void*>(&encodeur);
    traiter_noeud<T>(arbre);

    // Lib�rer la m�moire
    delete arbre;
}

template<class T>
void HuffmanBuilder::traiter_noeud(Noeud * noeud)
{
    // Si c'est un symbole
    if (noeud->is_symbole)
    {
        reinterpret_cast<HuffmanEncode<T>*>(_huff)
            ->ajouter_symbole(noeud->symbole, _enco >> (32 - _nb_bits), _nb_bits);
    }

    // Si c'est un noeud
    else
    {
        ++_nb_bits;

        // Bit � 1
        _enco |= 1ull << (32 - _nb_bits);
        traiter_noeud<T>(noeud->noeud.droite);

        // Bit � 0
        _enco &= ~(1ull << (32 - _nb_bits));
        traiter_noeud<T>(noeud->noeud.gauche);

        --_nb_bits;
    }
}

#endif
