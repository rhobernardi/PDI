
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Arquivo pdi.hpp - Header com escopo de funcoes
*
*    -Rodrigo das Neves Bernardi - 8066395
*
******************************************************************/

#ifndef _PDI_HPP_
#define _PDI_HPP_


#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;



typedef unsigned char uchar;

typedef struct IMGstructure {
    char type[3];
    int width;
    int height;
    int maxVal;
    uchar **pixel;
} Image;



void allocData(Image *img, char* type, int width, int height, int maxVal);

void freeData(Image *img);

void readImage(Image *imgIn, Image *imgOut, char *in, int bord);

void saveImage(Image *img, const char *out);

void saveImageBord (Image *img, const char *out, int bord);

void copyImage(Image *imgIn, Image *imgOut);

void copyImageBord (Image *imgIn, Image *imgOut, int bord);

void colorPixel(uchar *pixel, uchar tone);

void processInversion(Image *imgIn, Image *imgOut);

void transposeImage (Image *img);

int stringToInteger(string str);

void mediaFilter(Image *img, unsigned int mask, int bord);

void medianaFilter(Image *img, unsigned int mask, int bord);

void gaussFilter(Image *img, unsigned int mask, int bord);


#endif