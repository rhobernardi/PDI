
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Arquivo pdi.cpp - Funcoes implementadas
*
*    -Rodrigo das Neves Bernardi - 8066395
*
******************************************************************/

#include "pdi.hpp"



void allocData(Image *img, char* type, int width, int height, int maxVal) 
{
    strcpy(img->type, type);
    img->width = width;
    img->height = height;
    img->maxVal = maxVal;

    if(img->type[0] == 'P' && img->type[1] == '2') 
    {
        img->pixel = (uchar**) malloc(height * sizeof(uchar*));

        for (int i = 0; i < height; ++i)
        {
            img->pixel[i] = (uchar*) malloc(width * sizeof(uchar));
        }
    }
}





void freeData(Image *img) 
{    
    if (img->type[0] == 'P' && img->type[1] == '2') 
    {
        for (int i = 0; i < img->height; ++i)
        {
            free(img->pixel[i]);
        }

        free(img->pixel);
    }
}





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
            for(int i = 0; i < height ; ++i)
            {
                for (int j = 0; j < width; ++j)
                {
                    // fscanf(input, "%hhu %hhu %hhu", &(imgIn->data->r[i]), &(imgIn->data->g[i]), &(imgIn->data->b[i]));
                    fscanf(input, "%hhu\n", &(imgIn->pixel[i][j]));
                }
            }
        }
    }

    fclose(input);
}





void saveImage(Image *img, const char *out) 
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
        for (int i = 0; i < img->height; ++i)
        {
            for (int j = 0; j < img->width; ++j)
            {
                fprintf(output, "%hhu\n", img->pixel[i][j]);
            }
        }
    }

    fclose(output);
}





void copyImage(Image *imgIn, Image *imgOut)
{
    for(int i = 0; i < imgIn->height; ++i)
        for (int j = 0; j < imgIn->width; ++j)
        {
            imgOut->pixel[i][j] = imgIn->pixel[i][j];
        }
}





void colorPixel(uchar *pixel, uchar tone)
{
    (*pixel) = tone;
}





void processInversion(Image *imgIn, Image *imgOut)
{
    for (int i = 0; i < imgIn->height; ++i) 
        for (int j = 0; j < imgIn->width; ++j)
        {
            imgOut->pixel[i][j] = imgIn->pixel[i][j];
            colorPixel(&imgOut->pixel[i][j], imgIn->maxVal - imgOut->pixel[i][j]);
        }
}





double averageCalc(Image *img, double *percent_vec, int n)
{    
    double sum=0;
    
    for (int i = 0; i < img->maxVal; ++i)
    {
        if(percent_vec[i] > 0) 
        {
            sum += (i*percent_vec[i]);
        }
    }

    return sum / n;
}





double standardDeviation(Image *img, double *percent_vec, double *average, int n) 
{
    double dp=0;

    for(int i=0; i <= img->maxVal; ++i)
    {
        if(percent_vec[i] > 0)
            dp += pow((percent_vec[i]-(*average)), 2);
    }

    return sqrt(dp / (n-1));
}





void analyzeFrequency(Image *img, double *percent_vec, double *average, double *stand_dev) 
{    
    int total=0;

    // Contagem dos pixels da imagem
    for(int i=0; i<=img->maxVal; i++)
        percent_vec[i] = 0;

    for(int i = 0; i < img->height; ++i)
        for (int j = 0; j < img->width; ++j)
        {
            percent_vec[(int)img->pixel[i][j]]++;

            if(percent_vec[(int)img->pixel[i][j]] > 0)
                total++;
        }

    // Calcula media aritmetica dos valores dos pixels
    printf("\nCalculating average...\n");
    
    *average = averageCalc(img, percent_vec, total);
    printf("Done.\n");


    // Calcula desvio padrao dos valores dos pixels
    printf("\nCalculating standard deviation...\n");
    
    *stand_dev = standardDeviation(img, percent_vec, average, total);
    printf("Done.\n");


    // Calcula frequencia dos valores dos pixels
    printf("\nCalculating frequency...\n");

    for(int i = 0; i<=img->maxVal; i++)
        percent_vec[i] = (percent_vec[i] / total)*100;

    printf("Done.\n");
}





void floodFill(Image *img, unsigned int x, unsigned int y, uchar target_tone, uchar replacement_tone)
{
    unsigned int index; uchar this_pixel;

    // confere se o pixel existe, se nao retorna 
    if((x >= 0 && x < img->width) && (y >= 0 && y < img->height))
    {
        // indexa pixel
        //index = ((img->width*y)+x);
        this_pixel = img->pixel[y][x];

        // se ja estiver colorido com o tom de reposicao, retorna
        if(target_tone == replacement_tone) return;

        // se o pixel atual nao tem a mesma cor dos outros, retorna
        if(this_pixel != target_tone) return;
        
        colorPixel(&img->pixel[y][x], replacement_tone);
    
        floodFill(img, x,  y+1,     target_tone,   replacement_tone); // proximo pixel acima
        floodFill(img, x,  y-1,     target_tone,   replacement_tone); // proximo pixel abaixo
        floodFill(img, x+1,    y,   target_tone,   replacement_tone); // proximo pixel a direita
        floodFill(img, x-1,    y,   target_tone,   replacement_tone); // proximo pixel a esquerda
    }

    else return;

    return;
}