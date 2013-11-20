/**
 * huffman_encode.cpp
 */

#include "huffman_encode.h"

#include <iostream>
#include <bitset>

HuffmanEncode::HuffmanEncode()
{
}

void HuffmanEncode::ecrire_table_symboles()
{
    for (symap_t::iterator it = _symboles.begin(); it != _symboles.end(); ++it)
    {
        std::cout << it->first << " : " << it->second.second << " : " << std::bitset<8>(it->second.first) << std::endl;
    }
}

void HuffmanEncode::encoder(int symbole)
{
}

void HuffmanEncode::ajouter_symbole(int symbole, unsigned encodage, unsigned num_bits)
{
    _symboles.insert(std::pair<int, enco_t>(symbole, enco_t(encodage, num_bits)));
}
