#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <bitset>
#include <cstdlib>

#include "../modele_image/Image.h"
#include "../huffman.h"
#include "../division_resolution/division.hpp"

using namespace std;


///////////////////////////////////////////
///// Décodage différentiel en carré
///////////////////////////////////////////

void diff_decode_quad(int taille_blocs, Image& differences, Image& moyennes, Image& imagedecodee) {
    imagedecodee = Image(differences.getWidth(), differences.getHeight());
    int k = 0;
	int i = 0;


	// Calcul de la matrice image
	while (k < differences.getHeight()) {
		imagedecodee.setPixel(differences.getPixel(i, k).add(moyennes.getPixel(i / taille_blocs, k / taille_blocs)), i, k);
		i++;
		if (i == differences.getWidth()) {
			k++;
			i = 0;
		}
	}
}


///////////////////////////////////////////
///// Codage différentiel en carrés
///////////////////////////////////////////

void diff_encode_quad(int taille_blocs, Image img, Image& moyennes, Image& differences) {
	int delta = 0;
	int moyR = 0, moyG = 0, moyB = 0;
	int sumR = 0, sumG = 0, sumB = 0;
	int j = 0;
	int i = 0;
	int bordi = 0;
	int bordj = 0;
	int widthmoy = 0;
	int heightmoy = 0;
	differences = Image(img.getWidth(), img.getHeight());
	int m = 0;
	int k = 0;

	// Calcul de la dimension de la matrice des moyennes
	int wmodulo = img.getWidth() % taille_blocs;
	if (wmodulo == 0) widthmoy = img.getWidth() / taille_blocs;
	else widthmoy = img.getWidth() / taille_blocs + 1;

	int hmodulo = img.getHeight() % taille_blocs;
	if (hmodulo == 0) heightmoy = img.getHeight() / taille_blocs;
	else heightmoy = img.getHeight() / taille_blocs + 1;

	moyennes = Image(widthmoy, heightmoy);

	// Calcul de la matrice des moyennes
    for(j = 0; j < img.getHeight(); j = j+taille_blocs) {
        for(i = 0; i < img.getWidth() ; i = i+taille_blocs) {
            for(int x = 0; x < taille_blocs; x++) {
                for(int y = 0; y < taille_blocs; y++) {
                    if((i+y) < img.getWidth() && (j+x) < img.getHeight()) {
                        sumR += img.getPixel(i+y,j+x).getR();
                        sumG += img.getPixel(i+y,j+x).getV();
                        sumB += img.getPixel(i+y,j+x).getB();
                    }
                }
            }

            if((i+taille_blocs-1) >= img.getWidth())
                bordi = (i+taille_blocs-1)-img.getWidth();
            if((j+taille_blocs-1) >= img.getHeight())
                bordj = (j+taille_blocs-1)-img.getHeight();

            moyR = sumR / ((taille_blocs-bordi)*(taille_blocs-bordj));
            moyG = sumG / ((taille_blocs-bordi)*(taille_blocs-bordj));
            moyB = sumB / ((taille_blocs-bordi)*(taille_blocs-bordj));

            moyennes.setPixel(Pixel(moyR, moyG, moyB), m, k);

            sumR = 0;
            sumG = 0;
            sumB = 0;
            m++;
            bordi = 0;
            bordj = 0;
        }
        m = 0;
        k++;
    }

	j = 0;
	i = 0;

	// Calcul de la matrice différentielle
	while (j < img.getHeight()) {
		differences.setPixel(img.getPixel(i, j).sub(moyennes.getPixel(i / taille_blocs, j / taille_blocs)), i, j);
		i++;
		if (i == img.getWidth()) {
			j++;
			i = 0;
		}
	}
}

void encoder(const char * fic, const char * sortie, int taille_blocs, int div_resolution)
{
	IplImage *img = cvLoadImage(fic);

	Image image = Image(img);
    Image compress = Image(1, 1);
    
    while (div_resolution-- > 0)
    {
        divResolution(image, compress);
        image = compress;
    }
    
    Image differences = Image(image.getWidth(), image.getHeight());
    Image moyennes = Image(image.getWidth()/taille_blocs, image.getHeight()/taille_blocs);

	diff_encode_quad(taille_blocs, image, moyennes, differences);

    // Flux de sortie
    std::ofstream fichier(sortie, std::ios_base::out | std::ios_base::binary);

    // Flux de bits
    FluxBitsOut<std::ofstream> fichier_bits(fichier);

    fichier_bits.ecrire_entier(32, image.getWidth());
    fichier_bits.ecrire_entier(32, image.getHeight());
    fichier_bits.ecrire_octet(taille_blocs);

    // Moyennes
    for (unsigned x = 0; x < moyennes.getWidth(); ++x)
    {
        for (unsigned y = 0; y < moyennes.getHeight(); ++y)
        {
            Pixel p = moyennes.getPixel(x, y);
            fichier_bits.ecrire_octet(p.getR());
            fichier_bits.ecrire_octet(p.getV());
            fichier_bits.ecrire_octet(p.getB());
        }
    }

    // HUFFMAN !
    
    // Differences
    // Construction de Huffman
    HuffmanBuilder builder;
    for (unsigned x = 0; x < differences.getWidth(); ++x)
    {
        for (unsigned y = 0; y < differences.getHeight(); ++y)
        {
            Pixel p = differences.getPixel(x, y);
            builder.ajouter_symbole(p.getR(), 1);
            builder.ajouter_symbole(p.getV(), 1);
            builder.ajouter_symbole(p.getB(), 1);
        }
    }

    // Encodeur Huffman
    HuffmanEncode<std::ofstream> fichier_huff(fichier_bits);
    builder.construire_encodeur(fichier_huff);
    fichier_huff.ecrire_table_symboles();

    for (unsigned x = 0; x < differences.getWidth(); ++x)
    {
        for (unsigned y = 0; y < differences.getHeight(); ++y)
        {
            Pixel p = differences.getPixel(x, y);
            fichier_huff.encoder(p.getR());
            fichier_huff.encoder(p.getV());
            fichier_huff.encoder(p.getB());
        }
    }

    // Ecrire les derniers bits du buffer dans le flux
    fichier_bits.finaliser();

    // Finaliser l'ecriture dans le fichier
    fichier.close();
}

void decoder(const char * fic)
{
    // Flux d'entrée
    std::ifstream fichier(fic, std::ios_base::in | std::ios_base::binary);

    // Flux de bits
    FluxBitsIn<std::ifstream> fichier_bits(fichier);
    
    unsigned width = fichier_bits.lire_entier(32);
    unsigned height = fichier_bits.lire_entier(32);
    int taille_blocs = fichier_bits.lire_octet();

    Image differences = Image(width, height);
    Image moyennes = Image((width+taille_blocs-1)/taille_blocs, (height+taille_blocs-1)/taille_blocs);
    Image imagedecodee = Image(width, height);

    // Moyennes
    for (unsigned x = 0; x < moyennes.getWidth(); ++x)
    {
        for (unsigned y = 0; y < moyennes.getHeight(); ++y)
        {
            Pixel p;
            p.setR(fichier_bits.lire_octet());
            p.setV(fichier_bits.lire_octet());
            p.setB(fichier_bits.lire_octet());
            moyennes.setPixel(p, x, y);
        }
    }

    
    // Décodeur Huffman
    HuffmanDecode<std::ifstream> fichier_huff(fichier_bits);
    fichier_huff.lire_table_symboles();

    // Différences
    for (unsigned x = 0; x < differences.getWidth(); ++x)
    {
        for (unsigned y = 0; y < differences.getHeight(); ++y)
        {
            Pixel p;
            p.setR(fichier_huff.decoder());
            p.setV(fichier_huff.decoder());
            p.setB(fichier_huff.decoder());
            differences.setPixel(p, x, y);
        }
    }

    diff_decode_quad(taille_blocs, differences, moyennes, imagedecodee);

                            cout << "Matrice des moyennes" << endl;
                            cout << moyennes.getWidth() << endl;
                            IplImage *imgMoy = moyennes.getIplImage();
                            cvNamedWindow("Moyennes", 1);
                            cvShowImage("Moyennes", imgMoy);
                            cout << endl;

                            cout << "Matrice des différences" << endl;
                            IplImage *imgDiff = differences.getIplImage();
                            cvNamedWindow("Difference", 1);
                            cvShowImage("Difference", imgDiff);
                            cout << endl;

                            cout << "Matrice image décodée" << endl;
                            IplImage *imgDec = imagedecodee.getIplImage();
                            cvNamedWindow("Decodee", 1);
                            cvShowImage("Decodee", imgDec);

	cvWaitKey(0);
}

void help()
{
    std::cout << "Utilisation : " << std::endl
              << "Enco : prog enco entree sortie taille_blocs(=4) div_resolution(=0)" << std::endl
              << "Deco : prog deco entree" << std::endl;
}

int main(int argc, char ** argv)
{
    if (argc < 3)
    {
        help();
        return 0;
    }

    if (argv[1][0] == 'e')
    {
        if (argc < 4)
        {
            help();
            return 0;
        }
        int taille_blocs = 4;
        int div_res = 0;
        
        if (argc >= 5)
        {
            taille_blocs = std::atoi(argv[4]);
        }
        
        if (argc >= 6)
        {
            div_res = std::atoi(argv[5]);
        }

        encoder(argv[2], argv[3], taille_blocs, div_res);
    }
    else if (argv[1][0] == 'd')
    {
        decoder(argv[2]);
    }
    else
    {
        help();
        return 0;
    }

	return 0;
}
