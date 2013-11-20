#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

class Pixel
{
public:
	Pixel();
	Pixel(uchar r, uchar v, uchar b);

	uchar getR();
	uchar getV();
	uchar getB();

	void setR(uchar r);
	void setV(uchar v);
	void setB(uchar b);

	~Pixel();

private:
	uchar r, v, b;
	int x, y;
};
