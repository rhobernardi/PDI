
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
using namespace std;



typedef unsigned char uchar;

typedef struct IMGstructure {
    char type[3];
    int width;
    int height;
    int maxVal;
    uchar **pixel;
} Image;




/**
 * Aloca memória necessária para uma Image, recebendo suas informações
 * 
 * @param img       			Imagem de entrada
 * @param type      			Tipo (P1,P2,P3,P4,P5,P6)
 * @param width     			Largura
 * @param height    			Altura
 * @param maxVal    			Valor maximo do tom de cinza
 */
void allocData(Image *img, char* type, int width, int height, int maxVal);

/**
 * Libera memória usada por uma Image.
 * 
 * @param img   				Imagem de entrada
 */
void freeData(Image *img);

/**
 * Cria uma estrutura Image, incluindo a leitura do arquivo e a chamada da função para alocar a imagem na memória.
 * 
 * @param imgIn     			Imagem de entrada
 * @param imgOut    			Imagem de saida
 * @param in        			Nome do arquivo da imagem de entrada
 */
void readImage(Image *imgIn, Image *imgOut, char *in, int bord);

/**
 * Cria um arquivo com o resultado guardado na matriz de imagem de saída.
 * 
 * @param img   				Imagem de entrada
 * @param out   				Nome do arquivo da imagem de saida
 */
void saveImage(Image *img, const char *out);

/**
 * Copia a imagem de entrada para a imagem de saida.
 * 
 * @param imgIn     			Imagem de entrada
 * @param imgOut    			Imagem de saida
 */
void copyImage(Image *imgIn, Image *imgOut);

/**
 * Mudar cor do pixel
 * 
 * @param pixel     			Pixel de entrada
 * @param tone      			Tom de cinza para o qual quer mudar
 */
void colorPixel(uchar *pixel, uchar tone);

/**
 * Inverte as tonalidades de cor da imagem original.
 * 
 * @param imgIn     			Imagem de entrada
 * @param imgOut    			Imagem de saida
 */
void processInversion(Image *imgIn, Image *imgOut);

/**
 * Apaga a imagem toda
 * 
 * @param img 					Imagem para manipulacao
 */
void eraseAll (Image *img);

/**
 * Transpoe a matriz de imagem de entrada
 * 
 * @param img 					Imagem de entrada
 */
void transposeImage (Image *img);

/**
 * Converte numero no formato de string para inteiro
 * 
 * @param  str 					String de entrada
 * @return     					Retorna inteiro da string
 */
int stringToInteger(string str);

void mediaFilter(Image *img, unsigned int mask);
void medianaFilter(Image *img, unsigned int mask);
void gaussFilter(Image *img, unsigned int mask);
void saveImageBord (Image *img, const char *out, int bord); 
void copyImageBord (Image *imgIn, Image *imgOut, int bord);

#endif