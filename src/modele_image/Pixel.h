#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

class Pixel
{
public:
	Pixel();
	Pixel(int r, int v, int b);

	int getR();
	int getV();
	int getB();

	void setR(int r);
	void setV(int v);
	void setB(int b);

	Pixel sub(Pixel p);
	Pixel add(Pixel p);
	Pixel div(Pixel p);
	Pixel mul(Pixel p);

	~Pixel();

private:
	int r, v, b;
};

