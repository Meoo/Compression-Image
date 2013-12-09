/*
 * File:   codage.cpp
 * Author: Gildas
 *
 * Created on 20 novembre 2013, 14:59
 */

#include <iostream>
#include <math.h>

#include "../modele_image/Image.h"

using namespace std;

/* Function to calculate the average of 4 neighbors pixels
 *
 */
int moyenne(Image raw, int x, int y, int color){
    int average;
    if(color == 0)
        average = round((raw.getPixel(x,y).getR() + raw.getPixel(x+1,y).getR() +raw.getPixel(x,y+1).getR() + raw.getPixel(x+1,y+1).getR())/4.0);
    else if(color == 1)
        average = round((raw.getPixel(x,y).getV() + raw.getPixel(x+1,y).getV() +raw.getPixel(x,y+1).getV() + raw.getPixel(x+1,y+1).getV())/4.0);
    else
        average = round((raw.getPixel(x,y).getB() + raw.getPixel(x+1,y).getB() +raw.getPixel(x,y+1).getB() + raw.getPixel(x+1,y+1).getB())/4.0);
    return average;
}


/* Function to code
 *
 */
void divResolution(Image& raw, Image& compress) {
    compress = Image(raw.getWidth()/2, raw.getHeight()/2);

    for (int i=0; i<=raw.getWidth()-1; i=i+2)
    {
        for (int j=0; j<=raw.getHeight()-1; j=j+2)
        {
            if( i != 0 && j != 0)
                compress.setPixel(Pixel(moyenne(raw, i, j, 0), moyenne(raw, i, j, 1), moyenne(raw, i, j, 2)),i/2, j/2);
            else if( i == 0 )
                compress.setPixel(Pixel(moyenne(raw, 0, j, 0), moyenne(raw, 0, j, 1), moyenne(raw, 0, j, 2)), 0, j/2);
	    else
            compress.setPixel(Pixel(moyenne(raw, i, 0, 0), moyenne(raw, i, 0, 1), moyenne(raw, i, 0, 2)), i/2, 0);
        }
    }
}



/* Function to decode
 *
 */
void decodeDivResolution(Image& compress, Image& raw) {
    raw = Image(compress.getWidth()*2, compress.getHeight()*2);

    for (int i=0; i<=compress.getWidth()-1; i++)
    {
        for (int j=0; j<=compress.getHeight()-1; j++)
        {
            raw.setPixel(Pixel(compress.getPixel(i,j).getR(), compress.getPixel(i,j).getV(), compress.getPixel(i,j).getB()), 2*i, 2*j);
            raw.setPixel(Pixel(compress.getPixel(i,j).getR(), compress.getPixel(i,j).getV(), compress.getPixel(i,j).getB()), 2*i+1, 2*j);
            raw.setPixel(Pixel(compress.getPixel(i,j).getR(), compress.getPixel(i,j).getV(), compress.getPixel(i,j).getB()), 2*i, 2*j+1);
            raw.setPixel(Pixel(compress.getPixel(i,j).getR(), compress.getPixel(i,j).getV(), compress.getPixel(i,j).getB()), 2*i+1, 2*j+1);
        }
    }
}
