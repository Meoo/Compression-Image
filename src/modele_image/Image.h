#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "Pixel.h"

class Image
{
public:

	Image(int width, int height);
	Image(IplImage* img);

	int getWidth(void);
	int getHeight(void);
	int getNbChannels(void);
	Pixel getPixel(int x, int y);
	IplImage* getIplImage(void);
	int getSize();

	void setPixel(Pixel pixel, int x, int y);

	~Image();

private:
	int width, height, nbChannels;
	IplImage* image;
	Pixel **pixels;
};

