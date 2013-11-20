/**
 * huffman_encode.h
 */

#ifndef __HUFFMAN_ENCODE_H__
#define __HUFFMAN_ENCODE_H__

#include <map>

class HuffmanEncode
{
    // <tableau de bits, nombre de bits à utiliser>
    typedef std::pair<unsigned, unsigned> enco_t;
    typedef std::map<int, enco_t> symap_t;

    /**
     * Table des symboles.
     */
    symap_t _symboles;

public:
    HuffmanEncode();
    
    void ecrire_table_symboles();

    void encoder(int symbole);
    
protected:
    friend class HuffmanBuilder;
    
    void ajouter_symbole(int symbole, unsigned encodage, unsigned num_bits);

};

#endif
