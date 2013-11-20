#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "modele_image/Image.h"

using namespace std;

Image::Image(IplImage* img) {
	
	this->image = img;
	width = img->width;
	height = img->height;
	nbChannels = img->nChannels;
	
	// Création de la matrice de pixels depuis l'image
	CvScalar pixel;
	pixels = new Pixel*[width];

	for (int i = 0; i < width; i++) {

		pixels[i] = new Pixel[height];

		for (int j = 0; j < height; j++) {

			CvScalar pix = cvGet2D(img, i, j);
			pixels[i][j] = Pixel(pix.val[2], pix.val[1], pix.val[0]);
		}
	}
}

int Image::getWidth() { return width; }
int Image::getHeight() { return height; }
int Image::getNbChannels() { return nbChannels; }
Pixel Image::getPixel(int x, int y) { return pixels[x][y]; }
int Image::getSize() { return height * width * nbChannels; }

void Image::setPixel(Pixel pixel, int x, int y) { pixels[x][y] = pixel; }

IplImage* Image::getIplImage() {
	
	for (int i = 0; i < width; i++) {

		for (int j = 0; j < height; j++) {
			
			CvScalar value;
			value.val[2] = (double)pixels[i][j].getR();
			value.val[1] = (double)pixels[i][j].getV();
			value.val[0] = (double)pixels[i][j].getB();
			cvSet2D(image, i, j, value);
		}
	}
	return this->image;
}

Image::~Image() {}
