#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "Pixel.h"


Pixel::Pixel() {
	Pixel(0, 0, 0);
}

Pixel::Pixel(int r, int v, int b)
{
	this->r = r;
	this->v = v;
	this->b = b;
}

int Pixel::getR() { return r; }
int Pixel::getV() { return v; }
int Pixel::getB() { return b; }

void Pixel::setR(int r) { this->r = r; }
void Pixel::setV(int v) { this->v = v; }
void Pixel::setB(int b) { this->b = b; }

Pixel Pixel::sub(Pixel p) {
	return Pixel(this->r - p.getR(), this->v - p.getV(), this->b - p.getB());
}

Pixel Pixel::add(Pixel p) {
	return Pixel(this->r + p.getR(), this->v + p.getV(), this->b + p.getB());
}

Pixel Pixel::div(Pixel p) {
	return Pixel(this->r / p.getR(), this->v / p.getV(), this->b / p.getB());
}

Pixel Pixel::mul(Pixel p) {
	return Pixel(this->r * p.getR(), this->v * p.getV(), this->b * p.getB());
}

Pixel::~Pixel()
{
}
