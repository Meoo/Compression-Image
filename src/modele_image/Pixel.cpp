#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "modele_image/Pixel.h"


Pixel::Pixel() {
	Pixel(0, 0, 0);
}

Pixel::Pixel(uchar r, uchar v, uchar b)
{
	this->r = r;
	this->v = v;
	this->b = b;
}

uchar Pixel::getR() { return r; }
uchar Pixel::getV() { return v; }
uchar Pixel::getB() { return b; }

void Pixel::setR(uchar r) { this->r = r; }
void Pixel::setV(uchar v) { this->v = v; }
void Pixel::setB(uchar b) { this->b = b; }

Pixel::~Pixel()
{
}
