
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Arquivo pdi.h - Header
*
*    -Rodrigo Bernardi
*
******************************************************************/

#ifndef _PDI_HPP_
#define _PDI_HPP_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


typedef unsigned char uchar;

struct IMGstructure {
    char type[3];
    int width;
    int height;
    int maxVal;
    uchar *pixel;
};



/**
 * Aloca memória necessária para uma Image, recebendo suas informações
 * 
 * @param img       Imagem de entrada
 * @param type      Tipo (P1,P2,P3,P4,P5,P6)
 * @param width     Largura
 * @param height    Altura
 * @param maxVal    Valor maximo do tom de cinza
 */
void allocData(Image *img, char* type, int width, int height, int maxVal);

/**
 * Libera memória usada por uma Image.
 * 
 * @param img   Imagem de entrada
 */
void freeData(Image *img);

/**
 * Cria uma estrutura Image, incluindo a leitura do arquivo e a chamada da função para alocar a imagem na memória.
 * 
 * @param imgIn     Imagem de entrada
 * @param imgOut    Imagem de saida
 * @param in        Nome do arquivo da imagem de entrada
 */
void readImage(Image *imgIn, Image *imgOut, char *in);

/**
 * Cria um arquivo com o resultado guardado na matriz de imagem de saída.
 * 
 * @param img   Imagem de entrada
 * @param out   Nome do arquivo da imagem de saida
 */
void saveImage(Image *img, char *out);

/**
 * Copia a imagem de entrada para a imagem de saida.
 * 
 * @param imgIn     Imagem de entrada
 * @param imgOut    Imagem de saida
 */
void copyImage(Image *imgIn, Image *imgOut);

/**
 * Inverte as tonalidades de cor da imagem original.
 * 
 * @param imgIn     Imagem de entrada
 * @param imgOut    Imagem de saida
 */
void processInversion(Image *imgIn, Image *imgOut);

/**
 * Calcula a media aritmetica dos pixels.
 * 
 * @param imgIn         Imagem de entrada
 * @param percent_vec   Vetor de porcentagens de cada tom
 * @param n             Tamanho do vetor de porcentagem
 * @return              Media Aritmetica
 */
double averageCalc(Image *imgIn, double *percent_vec, int n);

/**
 * Calcula o desvio padrao a partir da media.
 * 
 * @param imgIn         Imagem de entrada
 * @param percent_vec   Vetor de porcentagens de cada tom
 * @param average       Media Aritmetica calculada
 * @param n             Tamanho do vetor de porcentagem
 * @return              Desvio Padrao
 */
double standardDeviation(Image *imgIn, double *percent_vec, double *average, int n);

/**
 * Analiza frequencia para cada tonalidade de cinza e retorna vetor com indice indicando valores dos tons e suas porcentagens.
 * Retorna tambem o valor medio e o desvio padrao por referencia.
 * 
 * @param imgIn         Imagem de entrada
 * @param percent_vec   Vetor de porcentagens de cada tom
 * @param average       Media Aritmetica calculada
 * @param stand_dev     Desvio Padrao calculado
 */
void analyzeFrequency(Image *imgIn, double *percent_vec, double *average, double *stand_dev);


#endif