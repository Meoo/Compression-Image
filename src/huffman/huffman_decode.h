/**
 * huffman_decode.h
 */

#ifndef __HUFFMAN_DECODE_H__
#define __HUFFMAN_DECODE_H__

class HuffmanDecode
{
    // <tableau de bits, nombre de bits à utiliser>
    typedef std::pair<unsigned long long, unsigned> enco_t;
    typedef std::map<int, enco_t> symap_t;

    /**
     * Table des symboles.
     */
    symap_t _symboles;

public:
    HuffmanDecode();
    
    void lire_table_symboles();

    int decoder();

};

#endif
