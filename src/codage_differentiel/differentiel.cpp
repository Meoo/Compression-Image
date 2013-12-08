#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <bitset>

#include "../modele_image/Image.h"
#include "../huffman.h"

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

int main()
{
	IplImage *img = cvLoadImage("C:\\Users\\Timothy\\Desktop\\src\\lena_color_512.png");

    int taille_blocs = 3;
	Image image = Image(img);
    Image differences = Image(image.getWidth(), image.getHeight());
    Image moyennes = Image(image.getWidth()/taille_blocs, image.getHeight()/taille_blocs);
    Image imagedecodee = Image(image.getWidth(), image.getHeight());
	diff_encode_quad(taille_blocs, image, moyennes, differences);

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


    // HUFFMAN !

    diff_decode_quad(taille_blocs, differences, moyennes, imagedecodee);

    cout << "Matrice image décodée" << endl;
	IplImage *imgDec = imagedecodee.getIplImage();
	cvNamedWindow("Decodee", 1);
	cvShowImage("Decodee", imgDec);

	IplImage *che = image.getIplImage();

	cvNamedWindow("Originale", 1);
	cvShowImage("Originale", che);

	cvWaitKey(0);
	cvDestroyWindow("Originale");
	cvReleaseImage(&img);

	return 0;
}
