/**
 * huffman_builder.cpp
 */

#include "huffman_builder.h"

#include <vector>
#include <algorithm>

HuffmanBuilder::HuffmanBuilder()
{
}

void HuffmanBuilder::ajouter_symbole(int symbole, unsigned occurences)
{
    symap_t::iterator it = _symboles.find(symbole);
    
    if (it != _symboles.end())
        it->second += occurences;
    else
        _symboles.insert(std::pair<int, unsigned>(symbole, occurences));
}

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
};

// Pour trier les noeuds
bool noeud_comp(Noeud * i,Noeud * j)
{
    return i->occurences > j->occurences;
}

void HuffmanBuilder::construire_encodeur(HuffmanEncode & encodeur)
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
        std::sort(vec.begin(), vec.end(), noeud_comp);
        
        Noeud * a = vec.back(); vec.pop_back();
        Noeud * b = vec.back(); vec.pop_back();
        
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
    _huff = &encodeur;
    traiter_noeud(arbre);
    
    // Lib�rer la m�moire
    delete arbre;
}

void HuffmanBuilder::traiter_noeud(Noeud * noeud)
{
    // Si c'est un symbole
    if (noeud->is_symbole)
    {
        _huff->ajouter_symbole(noeud->symbole, _enco >> (32 - _nb_bits), _nb_bits);
    }
    
    // Si c'est un noeud
    else
    {
        ++_nb_bits;
        
        // Bit � 1
        _enco |= 1ull << (32 - _nb_bits);
        traiter_noeud(noeud->noeud.droite);

        // Bit � 0
        _enco &= ~(1ull << (32 - _nb_bits));
        traiter_noeud(noeud->noeud.gauche);
        
        --_nb_bits;
    }
}
