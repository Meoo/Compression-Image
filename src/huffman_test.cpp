/**
 * huffman.h
 */

#include <iostream>
#include <fstream>
#include <bitset>

#include <huffman.h>

void encoder()
{
    // Construction de Huffman

    HuffmanBuilder builder;

    // Paramètres : symbole, nombre d'occurences
    // Plusieurs appels avec le même nombre de symbole incrémente le nombre d'occurences
    builder.ajouter_symbole(1, 10);
    builder.ajouter_symbole(2, 7);
    builder.ajouter_symbole(3, 5);
    builder.ajouter_symbole(4, 3);
    builder.ajouter_symbole(5, 1);
    builder.ajouter_symbole(6, 1);


    // Construction de l'encodeur

    // Flux de sortie
    std::ofstream fichier("huffman_test.bin");

    // Flux de bits
    FluxBitsOut<std::ofstream> fichier_bits(fichier);

    // Encodeur
    HuffmanEncode<std::ofstream> fichier_huff(fichier_bits);
    builder.construire_encodeur(fichier_huff);


    // Encodage de la table
    fichier_huff.ecrire_table_symboles();

    // Encodage des symboles
    fichier_huff.encoder(1);
    fichier_huff.encoder(2);
    fichier_huff.encoder(3);

    fichier_huff.encoder(1);
    fichier_huff.encoder(1);
    fichier_huff.encoder(1);
    fichier_huff.encoder(1);

    fichier_huff.encoder(2);
    fichier_huff.encoder(2);

    fichier_huff.encoder(3);
    fichier_huff.encoder(3);

    fichier_huff.encoder(2);
    fichier_huff.encoder(2);

    fichier_huff.encoder(4);
    fichier_huff.encoder(4);

    fichier_huff.encoder(5);

    fichier_huff.encoder(6);


    // Fermer le fichier

    // Ecrire les derniers bits du buffer dans le flux
    fichier_bits.finaliser();

    // Finaliser l'ecriture dans le fichier
    fichier.close();

}

void decoder()
{
    // Construire le décodeur

    // Flux d'entrée
    std::ifstream fichier("huffman_test.bin");

    // Flux de bits
    FluxBitsIn<std::ifstream> fichier_bits(fichier);

    // Décodeur
    HuffmanDecode<std::ifstream> fichier_huff(fichier_bits);


    // Lire la table des symboles
    fichier_huff.lire_table_symboles();


    // Lire les symboles jusqu'au 6 (pour l'exemple)
    int symbole = 0;
    do
    {
        symbole = fichier_huff.decoder();
        std::cout << symbole << std::endl;
    }
    while (symbole != 6);


    // Fermer le fichier proprement
    fichier.close();

}

int main()
{
    encoder();

    decoder();

    return 0;
}
