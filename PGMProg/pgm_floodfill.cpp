
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Tarefa 3 - Flood Fill em imagem.
*
*    -Rodrigo Bernardi
*
******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//#include "invert.hpp"
//#include "average.hpp"
//#include "floodfill.hpp"

#include <stack>
#include <iostream>
using namespace std;

typedef unsigned char uchar;

typedef struct node
{
    int 
};

typedef struct IMGstructure {
    char type[3];
    int width;
    int height;
    int maxVal;
    uchar *pixel;
} Image;



/**
 * Aloca memória necessária para uma Image, recebendo suas informações
 * 
 * @param img       Imagem de entrada
 * @param type      Tipo (P1,P2,P3,P4,P5,P6)
 * @param width     Largura
 * @param height    Altura
 * @param maxVal    Valor maximo do tom de cinza
 */
void allocData(Image *img, char* type, int width, int height, int maxVal) 
{
    strcpy(img->type, type);
    img->width = width;
    img->height = height;
    img->maxVal = maxVal;

    if(img->type[0] == 'P' && img->type[1] == '2') 
    {
        img->pixel = (uchar*) malloc(width * height * sizeof(uchar));
    }
}

/**
 * Libera memória usada por uma Image.
 * 
 * @param img   Imagem de entrada
 */
void freeData(Image *img) 
{    
    if (img->type[0] == 'P' && img->type[1] == '2') 
    {
        free(img->pixel);
    }
}

/**
 * Cria uma estrutura Image, incluindo a leitura do arquivo e a chamada da função para alocar a imagem na memória.
 * 
 * @param imgIn     Imagem de entrada
 * @param imgOut    Imagem de saida
 * @param in        Nome do arquivo da imagem de entrada
 */
void readImage(Image *imgIn, Image *imgOut, char *in) 
{
    FILE *input;
    char type[3];
    int maxVal, width, height;

    input = fopen(in, "r");

    if (input == NULL)
        printf("Can't read image file.\n");
    
    else 
    {
        // le tipo
        rewind(input);
        fscanf(input, "%s", type);

        // compara tipo para ver se é pgm (escala de cinza)
        if(type[0] == 'P' && type[1] == '2') 
        {
            // pula fim da linha
            while (getc(input) != '\n');

            // pula comentario da linha
            while (getc(input) == '#') 
            {
                while (getc(input) != '\n');
            }

            // volta um caracter
            fseek(input, -1, SEEK_CUR);

            // le dimensões da imagem e a escala das cores
            fscanf(input, "%d", &width);
            fscanf(input, "%d", &height);
            fscanf(input, "%d", &maxVal);

            // aloca as matrizes width x height das imagens de entrada e saída na memória
            allocData(imgIn, type, width, height, maxVal);
            allocData(imgOut, type, width, height, maxVal);
            
            // le dados do arquivo
            for(int i = 0; i < width * height ; i++) 
            {
                // fscanf(input, "%hhu %hhu %hhu", &(imgIn->data->r[i]), &(imgIn->data->g[i]), &(imgIn->data->b[i]));
                fscanf(input, "%hhu\n", &(imgIn->pixel[i]));        
            }
        }
    }

    fclose(input);
}

/**
 * Cria um arquivo com o resultado guardado na matriz de imagem de saída.
 * 
 * @param img   Imagem de entrada
 * @param out   Nome do arquivo da imagem de saida
 */
void saveImage(Image *img, char *out) 
{
    FILE *output;

    // escreve header contendo tipo, comentário, dimensões e escala
    output = fopen(out, "w");
    fprintf(output, "%s\n", img->type);
    fprintf(output, "#imagem gerada pelo programa\n");
    fprintf(output, "%d %d\n", img->width, img->height);
    fprintf(output, "%d\n", img->maxVal);

    // escreve dados da imagem

    if(img->type[0] == 'P' && img->type[1] == '2') 
    {
        for (int i = 0; i < img->height * img->width; i++) 
        {
            fprintf(output, "%hhu\n", img->pixel[i]);
        }
    }

    fclose(output);
}

/**
 * Copia a imagem de entrada para a imagem de saida.
 * 
 * @param imgIn     Imagem de entrada
 * @param imgOut    Imagem de saida
 */
void copyImage(Image *imgIn, Image *imgOut)
{
    for(int i = 0; i < imgIn->height * imgIn->width; i++)
        imgOut->pixel[i] = imgIn->pixel[i];
}

/**
 * Mudar cor do pixel
 * 
 * @param pixel     Pixel de entrada
 * @param tone      Tom de cinza para o qual quer mudar
 */
void colorPixel(uchar *pixel, uchar tone)
{
    (*pixel) = tone;
}

/**
 * Inverte as tonalidades de cor da imagem original.
 * 
 * @param imgIn     Imagem de entrada
 * @param imgOut    Imagem de saida
 */
void processInversion(Image *imgIn, Image *imgOut) 
{
    for (int i = 0; i < imgIn->height * imgIn->width; i++) 
    {
        imgOut->pixel[i] = imgIn->pixel[i];
        colorPixel(&imgOut->pixel[i], imgIn->maxVal - imgOut->pixel[i]);
    }
}

/**
 * Calcula a media aritmetica dos pixels.
 * 
 * @param imgIn         Imagem de entrada
 * @param percent_vec   Vetor de porcentagens de cada tom
 * @param n             Tamanho do vetor de porcentagem
 * @return              Media Aritmetica
 */
double averageCalc(Image *imgIn, double *percent_vec, int n) 
{    
    double sum=0;
    
    for (int i = 0; i < imgIn->maxVal; ++i) 
    {
        if(percent_vec[i] > 0) 
        {
            sum += (i*percent_vec[i]);
        }
    }

    return sum / n;
}

/**
 * Calcula o desvio padrao a partir da media.
 * 
 * @param imgIn         Imagem de entrada
 * @param percent_vec   Vetor de porcentagens de cada tom
 * @param average       Media Aritmetica calculada
 * @param n             Tamanho do vetor de porcentagem
 * @return              Desvio Padrao
 */
double standardDeviation(Image *imgIn, double *percent_vec, double *average, int n) 
{
    double dp=0;

    for(int i=0; i <= imgIn->maxVal; ++i)
    {
        if(percent_vec[i] > 0)
            dp += pow((percent_vec[i]-(*average)), 2);
    }

    return sqrt(dp / (n-1));
}

/**
 * Analiza frequencia para cada tonalidade de cinza e retorna vetor com indice indicando valores dos tons e suas porcentagens.
 * Retorna tambem o valor medio e o desvio padrao por referencia.
 * 
 * @param imgIn         Imagem de entrada
 * @param percent_vec   Vetor de porcentagens de cada tom
 * @param average       Media Aritmetica calculada
 * @param stand_dev     Desvio Padrao calculado
 */
void analyzeFrequency(Image *imgIn, double *percent_vec, double *average, double *stand_dev) 
{    
    int total=0;

    // Contagem dos pixels da imagem
    for(int i=0; i<=imgIn->maxVal; i++)
        percent_vec[i] = 0;

    for(int i = 0; i < imgIn->height * imgIn->width; i++) 
    {
        percent_vec[(int)imgIn->pixel[i]]++;
        if(percent_vec[(int)imgIn->pixel[i]] > 0)
            total++;
    }

    // Calcula media aritmetica dos valores dos pixels
    printf("\nCalculating average...\n");
    
    *average = averageCalc(imgIn, percent_vec, total);
    printf("Done.\n");


    // Calcula desvio padrao dos valores dos pixels
    printf("\nCalculating standard deviation...\n");
    
    *stand_dev = standardDeviation(imgIn, percent_vec, average, total);
    printf("Done.\n");


    // Calcula frequencia dos valores dos pixels
    printf("\nCalculating frequency...\n");

    for(int i = 0; i<=imgIn->maxVal; i++)
        percent_vec[i] = (percent_vec[i] / total)*100;

    printf("Done.\n");
}

void processFloodFill(Image *img, int x, int y, uchar last_tone, uchar new_tone)
{
    int index; uchar this_pixel;

    if((x >= 0 || x <= img->width) && (y >= 0 || y <= img->height))
    {  
        index = (img->width*x);
        this_pixel = img->pixel[index];

        if(this_pixel == last_tone)
        {   
            img->pixel[index] = new_tone;

            processFloodFill(img, x,    y+1,   last_tone,   new_tone);
            // processFloodFill(img, x,    y-1,   last_tone,   new_tone);
            // processFloodFill(img, x+1,  y+1,   last_tone,   new_tone);
            // processFloodFill(img, x+1,  y,     last_tone,   new_tone);
            // processFloodFill(img, x+1,  y-1,   last_tone,   new_tone);
            // processFloodFill(img, x-1,  y-1,   last_tone,   new_tone);
            // processFloodFill(img, x-1,  y,     last_tone,   new_tone);
            // processFloodFill(img, x-1,  y+1,   last_tone,   new_tone);
        }

        else return;        
    }

    else return;

    return;
}

/**
 * [floodFill description]
 * 
 * @param imgIn      Imagem de entrada
 * @param imgOut     Imagem de saida
 * @param x          Coordenada x
 * @param y          Coordenada y
 * @param newtone    Novo tom que os pixels assumirao
 */
void floodFill(Image *imgIn, Image *imgOut, int x, int y, uchar new_tone)
{
    copyImage(imgIn, imgOut);

	stack<uchar> stack;
    

    int index; uchar last_tone;
    index = (imgOut->width*x);
    last_tone = imgOut->pixel[index];

    processFloodFill(imgOut, x, y, last_tone, new_tone);

    // processFloodFill(imgOut, x, y-1);
    // processFloodFill(imgOut, x+1, y+1);
    // processFloodFill(imgOut, x+1, y);
    // processFloodFill(imgOut, x+1, y-1);
    // processFloodFill(imgOut, x-1, y-1);
    // processFloodFill(imgOut, x-1, y);
    // processFloodFill(imgOut, x-1, y+1);

    //imgOut->pixel[index] = newtone;

    // for(int i = 0; i < imgIn->width * imgIn->height; i++)
    // {
    //     //if()
    //     //imgOut->pixel[p_aux]
    // }
}



/**
 * Funcao Main
 * 
 * @param  argc     -
 * @param  argv     Nome do arquivo de imagem de entrada
 * @return          0
 */
int main(int argc, char const *argv[]) 
{
    Image imgIn, imgOut;

    uchar *inPixel, *outPixel;
    int size;
    char input[20], output[20];
    double result;

    strcpy(input, argv[1]);
    strcpy(output, "img_out.pgm");

    // Le a imagem de entrada
    readImage(&imgIn, &imgOut, input);

    size = imgIn.width * imgIn.height;

    // Execucao do programa para imagens em escala de cinza (PGM)
    if (imgIn.type[0] == 'P' && imgIn.type[1] == '2')
    {
        double average = 0, stand_dev = 0;
        double *percent_vec = (double *) malloc((imgIn.maxVal+1)*sizeof(double)); // 255 valores para pixel

        unsigned int coordx, coordy, value;
        printf("\nCoordinates(x y): ");
        scanf("%d %d", &coordx, &coordy);

        if((coordx < imgIn.width || coordx >= 0) && (coordy < imgIn.height || coordy >= 0))
        {
            //cout << coordx << ' ' << coordy << endl;
            printf("\nColor value(0 <= val <= 255): ");
            scanf("%d", &value);

            if(value >= 0 && value <= imgIn.maxVal)
            {
                printf("\nProcessing...\n");
                //processInversion(&imgIn, &imgOut);
                //analyzeFrequency(&imgIn, percent_vec, &average, &stand_dev);
                floodFill(&imgIn, &imgOut, coordx, coordy, value);
                
                saveImage(&imgOut, output);
                printf("Done.\n");
            }

            else printf("== ERROR. VALUE MUST BE BETWEEN '0' AND '%d' ==\n\n", imgIn.maxVal);
        }

        else printf("== ERROR. COORDINATE 'X' MUST BE BETWEEN '0' AND '%d', COORDINATE 'Y' BETWEEN '0' AND '%d' ==\n\n", imgIn.width-1, imgIn.height-1);
    }

    else printf("== IMAGE IS NOT IN PGM FORMAT (P2 format) ==\n\n");

    freeData(&imgIn);
    freeData(&imgOut);

    return 0;
}