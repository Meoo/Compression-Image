#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <stdio.h>

#include "Image.h"

using namespace std;



///////////////////////////////////////////
///// Décodage différentiel en ligne
///////////////////////////////////////////

void diff_decode_line(int taille_blocs, Image difference, Image moyennes, int originalWidth, int originalHeight) {
    Image imagedecodee = Image(originalWidth, originalHeight);
    int k = 0;
	int i = 0;
	// Calcul de la matrice image
	while (k < originalHeight) {
		imagedecodee.setPixel(difference.getPixel(i, k).add(moyennes.getPixel(i / taille_blocs, k)), i, k);
		i++;
		if (i == originalWidth) {
			k++;
			i = 0;
		}
	}

	cout << "Matrice image décodée" << endl;
	IplImage *imgDec = imagedecodee.getIplImage();
	cvNamedWindow("Decodee", 1);
	cvShowImage("Decodee", imgDec);
}

///////////////////////////////////////////
///// Codage différentiel en ligne
///////////////////////////////////////////

void diff_encode_line(int taille_blocs, Image img) {
	int delta = 0;
	int moyR = 0, moyG = 0, moyB = 0;
	int sumR = 0, sumG = 0, sumB = 0;
	int k = 0;
	int i = 0;
	int widthmoy = 0;
	Image difference = Image(img.getWidth(), img.getHeight());
	int m = 0;

	// Calcul de la dimension de la matrice des moyennes
	int wmodulo = img.getWidth() % taille_blocs;
	if (wmodulo == 0) widthmoy = img.getWidth() / taille_blocs;
	else widthmoy = img.getWidth() / taille_blocs + 1;

	Image moyennes = Image(widthmoy, img.getHeight());

	// Calcul de la matrice des moyennes
	while (k < img.getHeight()) {

		sumR += img.getPixel(i,k).getR();
		sumG += img.getPixel(i,k).getV();
		sumB += img.getPixel(i,k).getB();
		i++;

		if (i%taille_blocs == 0 && i != 0) {
			moyR = sumR / taille_blocs;
			moyG = sumG / taille_blocs;
			moyB = sumB / taille_blocs;
			//cout << "m = " << m << " k = " << k << " nBlock = " << taille_blocs  << " i=" << i <<  endl;
			moyennes.setPixel(Pixel(moyR, moyG, moyB), m, k);
			m++;

			sumR = 0;
			sumG = 0;
			sumB = 0;
		}
		if (i == img.getWidth()) {

			//cout << " i=" << i << endl;
			if (i%taille_blocs != 0) {
				moyR = sumR / (i%taille_blocs);
				moyG = sumG / (i%taille_blocs);
				moyB = sumB / (i%taille_blocs);
				moyennes.setPixel(Pixel(moyR, moyG, moyB), m, k);


				sumR = 0;
				sumG = 0;
				sumB = 0;
			}
			m = 0;
			k++;
			i = 0;
		}
	}
	k = 0;
	i = 0;

	// Calcul de la matrice différentielle
	while (k < img.getHeight()) {
		difference.setPixel(img.getPixel(i, k).sub(moyennes.getPixel(i / taille_blocs, k)), i, k);
		i++;
		if (i == img.getWidth()) {
			k++;
			i = 0;
		}
	}

	cout << "Matrice des moyennes" << endl;
	cout << moyennes.getWidth() << endl;
	IplImage *imgMoy = moyennes.getIplImage();
	cvNamedWindow("Moyennes", 1);
	cvShowImage("Moyennes", imgMoy);
	cout << endl;

	cout << "Matrice des différences" << endl;
	IplImage *imgDiff = difference.getIplImage();
	cvNamedWindow("Difference", 1);
	cvShowImage("Difference", imgDiff);
	cout << endl;

	diff_decode_line(taille_blocs, difference, moyennes, img.getWidth(), img.getHeight());
}

int main()
{
	IplImage *img = cvLoadImage("C:\\Users\\Timothy\\Desktop\\TMP\\lena_color_512.png");

	Image image = Image(img);

	diff_encode_line(7, image);

	IplImage *che = image.getIplImage();

	cvNamedWindow("Originale", 1);
	cvShowImage("Originale", che);

	cvWaitKey(0);
	cvDestroyWindow("Originale");
	cvReleaseImage(&img);

	return 0;
}
