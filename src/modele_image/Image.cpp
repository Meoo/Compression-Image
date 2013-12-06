#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "Image.h"

using namespace std;

Image::Image(int width, int height) {
	this->image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	this->width = width;
	this->height = height;
	this->nbChannels = 3;

	CvScalar pixel;
	pixels = new Pixel*[width];

	// Matrice de pixels vide
	for (int i = 0; i < width; i++) {
		pixels[i] = new Pixel[height];
		for (int j = 0; j < height; j++) {
			setPixel(Pixel(0, 0, 0), i, j);
		}
	}
}

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

			CvScalar pix = cvGet2D(img, j, i);
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
	
	CvScalar value;
	for (int i = 0; i < width; i++) {

		for (int j = 0; j < height; j++) {
			
			value.val[2] = pixels[i][j].getR();
			value.val[1] = pixels[i][j].getV();
			value.val[0] = pixels[i][j].getB();
			cvSet2D(image, j, i, value);
		}
	}
	return this->image;
}

Image::~Image() {}
